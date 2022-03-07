// CRGBimage.cpp: 구현 파일
//

#include "stdafx.h"
#include "Hw.h"
#include "CRGBimage.h"
#include "afxdialogex.h"
#define ORIGIN 0
#define RED 1
#define GREEN 2
#define BLUE 3

// CRGBimage 대화 상자

IMPLEMENT_DYNAMIC(CRGBimage, CDialogEx)

CRGBimage::CRGBimage(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_RGBIMAGE, pParent)
{
	m_Color = 0;
}

CRGBimage::~CRGBimage()
{
}

void CRGBimage::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_PIC, m_View);
	DDX_Control(pDX, IDC_SLIDER1, m_ctrlSilder);
}


BEGIN_MESSAGE_MAP(CRGBimage, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_START, &CRGBimage::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CRGBimage::OnBnClickedButtonStop)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_RADIO_ORIGINAL, &CRGBimage::OnBnClickedRadioOriginal)
	ON_BN_CLICKED(IDC_RADIO_RED, &CRGBimage::OnBnClickedRadioRed)
	ON_BN_CLICKED(IDC_RADIO_GREEN, &CRGBimage::OnBnClickedRadioGreen)
	ON_BN_CLICKED(IDC_RADIO_BLUE, &CRGBimage::OnBnClickedRadioBlue)
	ON_WM_HSCROLL()
END_MESSAGE_MAP()

BOOL m_bThreadFlag;
UINT ThreadImageCaptureFunc(LPVOID param)
{
	CRGBimage *pDlg = (CRGBimage *)param;
	while (m_bThreadFlag) {
		pDlg->m_Capture >> pDlg->m_Image;  // 영상 획득
		pDlg->Invalidate(FALSE);
	}
	return 0;
}
void FillBitmapInfo(BITMAPINFO* bmi, int width, int height, int bpp, int origin)
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
void DisplayImage(CDC* pDC, CRect rect, Mat& srcimg)
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

	FillBitmapInfo(bmi, bmp_w, bmp_h, bpp, 0);

	int from_x = MIN(0, bmp_w - 1);
	int from_y = MIN(0, bmp_h - 1);
	int sw = MAX(MIN(bmp_w - from_x, rect.Width()), 0);
	int sh = MAX(MIN(bmp_h - from_y, rect.Height()), 0);

	SetDIBitsToDevice(pDC->m_hDC, rect.left, rect.top, sw, sh, from_x, from_y, 0, sh, img.data + from_y * img.step, bmi, 0);

	img.release();
}

// CRGBimage 메시지 처리기
BOOL CRGBimage::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_Capture.open(0);   // 카메라 연결
	if (!m_Capture.isOpened()) AfxMessageBox(_T("There is no camera captured!"));

	m_ctrlSilder.SetRange(0, 255);
	m_ctrlSilder.SetPos(128);

	CStatic *staticSize = (CStatic*)GetDlgItem(IDC_STATIC_PIC);
	CRect rect;
	staticSize->GetClientRect(rect);
	m_Width = rect.Width();
	m_Height = rect.Height();
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CRGBimage::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
					   // 그리기 메시지에 대해서는 CDialogEx::OnPaint()을(를) 호출하지 마십시오.
	//m_Image = Mat(m_Width, m_Height, CV_8UC3);
	split(m_Image, m_bgr);	//bgr로 채널 분리
	if (m_Image.data != NULL) {
		Mat tmp;
		int add_value = m_value - 128;
		CRect rect;
		CDC* pDC;
		pDC = m_View.GetDC();
		m_View.GetClientRect(rect);
		if (m_Color == ORIGIN) {
			Mat output;
			output = m_Image.clone();
			for (int y = 0; y < 480; y++) {
				for (int x = 0; x < 1919; x++) {
					if (output.at<uchar>(y, x) + add_value > 255)
						output.at<uchar>(y, x) = saturate_cast<uchar>(output.at<uchar>(y, x) + 255);
					else
						output.at<uchar>(y, x) = saturate_cast<uchar>(output.at<uchar>(y, x) + add_value);
				}
			}
			tmp = output;
		}
		if (m_Color == RED) {
			tmp = m_bgr[2] + add_value;
		}
		if (m_Color == GREEN) {
			tmp = m_bgr[1] + add_value;
		}
		if (m_Color == BLUE) {
			tmp = m_bgr[0] + add_value;
		}
		DisplayImage(pDC, rect, tmp);  // 카메라에서 읽어들인 영상을 화면에 그리기
		ReleaseDC(pDC);
	}

}
void CRGBimage::OnBnClickedButtonStart()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_bThreadFlag = TRUE;
	m_pThread = AfxBeginThread(ThreadImageCaptureFunc, this);
}
void CRGBimage::OnBnClickedButtonStop()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_bThreadFlag = FALSE;
}
void CRGBimage::OnBnClickedRadioOriginal()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_Color = ORIGIN;
}
void CRGBimage::OnBnClickedRadioRed()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_Color = RED;
	
}
void CRGBimage::OnBnClickedRadioGreen()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_Color = GREEN;
	
}
void CRGBimage::OnBnClickedRadioBlue()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_Color = BLUE;
}

void CRGBimage::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (IDC_SLIDER1 == pScrollBar->GetDlgCtrlID())
	{
		m_value = m_ctrlSilder.GetPos();
	}
	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}
