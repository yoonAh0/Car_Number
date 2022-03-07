// CFFT.cpp: 구현 파일
//

#include "stdafx.h"
#include "Hw.h"
#include "CFFT.h"
#include "fft.h"
#include "afxdialogex.h"


// CFFT 대화 상자

IMPLEMENT_DYNAMIC(CFFT, CDialogEx)

CFFT::CFFT(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FFT, pParent)
{
}

CFFT::~CFFT()
{
}

void CFFT::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC1, m_View1);
	DDX_Control(pDX, IDC_STATIC2, m_View2);
	DDX_Control(pDX, IDC_STATIC_FFT, m_ViewFFT);
}


BEGIN_MESSAGE_MAP(CFFT, CDialogEx)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


// CFFT 메시지 처리기


void FillBitmapInfo8(BITMAPINFO* bmi, int width, int height, int bpp, int origin)

{
	assert(bmi&&width >= 0 && height >= 0 && (bpp == 8 || bpp == 24 || bpp == 32));
	BITMAPINFOHEADER *bmih = &(bmi->bmiHeader);
	memset(bmih, 0, sizeof(*bmih));
	bmih->biSize = sizeof(BITMAPINFOHEADER);
	bmih->biWidth = width;
	bmih->biHeight = origin ? abs(height) : -abs(height);
	bmih->biPlanes = 1;
	bmih->biBitCount = (unsigned short)bpp;
	bmih->biCompression = BI_RGB;
	if (bpp == 8) {
		RGBQUAD *palette = bmi->bmiColors;
		for (int i = 0; i < 256; i++) {
			palette[i].rgbBlue = palette[i].rgbGreen = palette[i].rgbRed = (BYTE)i;
			palette[i].rgbReserved = 0;
		}
	}
}

void DisplayImage8(CDC* pDC, CRect rect, Mat& srcimg)
{
	Mat img;
	int step = ((int)(rect.Width() / 4)) * 4; // 4byte 단위조정해야 영상이 기울어지지 않는다.

	resize(srcimg, img, Size(step, rect.Height()));

	uchar buffer[sizeof(BITMAPINFOHEADER) * 1024];
	BITMAPINFO* bmi = (BITMAPINFO*)buffer;

	int bmp_w = img.cols;
	int bmp_h = img.rows;
	int depth = img.depth();
	int channels = img.channels();
	int bpp = 8 * channels;

	FillBitmapInfo8(bmi, bmp_w, bmp_h, bpp, 0);

	int from_x = MIN(0, bmp_w - 1);
	int from_y = MIN(0, bmp_h - 1);
	int sw = MAX(MIN(bmp_w - from_x, rect.Width()), 0);
	int sh = MAX(MIN(bmp_h - from_y, rect.Height()), 0);

	SetDIBitsToDevice(pDC->m_hDC, rect.left, rect.top, sw, sh, from_x, from_y, 0, sh, img.data + from_y * img.step, bmi, 0);

	img.release();
}

BOOL CFFT::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_count = 0;
	m_image = imread("D:/Users/USER/Downloads/images/halftone2.png", 0);
	FFT(m_image, dft_coef, dft_img);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CFFT::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
					   // 그리기 메시지에 대해서는 CDialogEx::OnPaint()을(를) 호출하지 마십시오.
	CDC* pDC;
	CRect rect;
	if (m_image.data != NULL) {
		pDC = m_View1.GetDC();	//picCtrl 크기 받아오기
		m_View1.GetClientRect(rect);
		DisplayImage8(pDC, rect, m_image);  // 카메라에서 읽어들인 영상을 화면에 그리기

		if (m_count == 0) {

			pDC = m_ViewFFT.GetDC();	//picCtrl 크기 받아오기
			m_ViewFFT.GetClientRect(rect);
			DisplayImage8(pDC, rect, dft_img);  // 카메라에서 읽어들인 영상을 화면에 그리기

			m_image.copyTo(m_result);
			pDC = m_View2.GetDC();		//picCtrl 크기 받아오기
			m_View2.GetClientRect(rect);
			DisplayImage8(pDC, rect, m_result);	 // 카메라에서 읽어들인 영상을 화면에 그리기
			ReleaseDC(pDC);
		}
		else {
			pDC = m_ViewFFT.GetDC();		//picCtrl 크기 받아오기
			m_ViewFFT.GetClientRect(rect);
			DisplayImage8(pDC, rect, dft);	 // 카메라에서 읽어들인 영상을 화면에 그리기

			pDC = m_View2.GetDC();		//picCtrl 크기 받아오기
			m_View2.GetClientRect(rect);
			DisplayImage8(pDC, rect, m_result);	 // 카메라에서 읽어들인 영상을 화면에 그리기
			ReleaseDC(pDC);
		}

	}
}


void CFFT::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CRect rt;
	((CStatic*)GetDlgItem(IDC_STATIC_FFT))->GetWindowRect(&rt);
	ScreenToClient(&rt);
	if (rt.PtInRect(point)) {
		if (m_count == 0) {
			Point center((point.x - rt.left) * 2, (point.y - rt.top) * 2);
			m_filter = get_Filter(dft_coef.size(), 5, center);
			multiply(dft_coef, m_filter, filtered_mat);
			log_mag(filtered_mat, dft);
			m_result = IFFT(filtered_mat, m_result.size());
		}
		else {
			Point center((point.x - rt.left) * 2, (point.y - rt.top) * 2);
			bitwise_and(m_filter, get_Filter(dft_coef.size(), 5, center), m_filter);
			multiply(dft_coef, m_filter, filtered_mat);
			log_mag(filtered_mat, dft);
			m_result = IFFT(filtered_mat, m_result.size());
		}
	}
	m_count++;
	Invalidate();
	CDialogEx::OnLButtonDown(nFlags, point);
}
