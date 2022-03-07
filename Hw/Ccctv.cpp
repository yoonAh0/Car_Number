// Ccctv.cpp: 구현 파일
//

#include "stdafx.h"
#include "Hw.h"
#include "Ccctv.h"
#include "afxdialogex.h"


// Ccctv 대화 상자

IMPLEMENT_DYNAMIC(Ccctv, CDialog)

Ccctv::Ccctv(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_CCTV, pParent)
{

}

Ccctv::~Ccctv()
{
}

void Ccctv::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC1, m_View1);
	DDX_Control(pDX, IDC_STATIC2, m_View2);
	DDX_Control(pDX, IDC_STATIC3, m_View3);
	DDX_Control(pDX, IDC_STATIC4, m_View4);
}


BEGIN_MESSAGE_MAP(Ccctv, CDialog)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON_START, &Ccctv::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &Ccctv::OnBnClickedButtonStop)
	ON_BN_CLICKED(IDC_BUTTON_BG, &Ccctv::OnBnClickedButtonBg)
END_MESSAGE_MAP()


// Ccctv 메시지 처리기

BOOL m_bThreadFlag1;
UINT ThreadImageCaptureFunc1(LPVOID param)
{
	Ccctv *pDlg = (Ccctv *)param;
	while (m_bThreadFlag1) {
		pDlg->m_Capture >> pDlg->m_Cam;  // 영상 획득
		pDlg->Invalidate(FALSE);
	}
	return 0;
}
void FillBitmapInfo1(BITMAPINFO* bmi, int width, int height, int bpp, int origin)
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
void DisplayImage1(CDC* pDC, CRect rect, Mat& srcimg)
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

	FillBitmapInfo1(bmi, bmp_w, bmp_h, bpp, 0);

	int from_x = MIN(0, bmp_w - 1);
	int from_y = MIN(0, bmp_h - 1);
	int sw = MAX(MIN(bmp_w - from_x, rect.Width()), 0);
	int sh = MAX(MIN(bmp_h - from_y, rect.Height()), 0);

	SetDIBitsToDevice(pDC->m_hDC, rect.left, rect.top, sw, sh, from_x, from_y, 0, sh, img.data + from_y * img.step, bmi, 0);

	img.release();
}
BOOL Ccctv::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_Capture.open(0);   // 카메라 연결
	if (!m_Capture.isOpened()) AfxMessageBox(_T("There is no camera captured!"));
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void Ccctv::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
					   // 그리기 메시지에 대해서는 CDialog::OnPaint()을(를) 호출하지 마십시오.
	CRect rect;
	CDC* pDC;
	pDC = m_View2.GetDC();
	m_View2.GetClientRect(rect);

	if (m_Cam.data != NULL) {
		DisplayImage1(pDC, rect, m_Cam);  // 카메라에서 읽어들인 영상을 화면에 그리기
		if (m_Back.data != NULL) {
			Mat grayCam, grayBack, diffThre, diffColor;
			cvtColor(m_Cam, grayCam, COLOR_BGR2GRAY);	
			cvtColor(m_Back, grayBack, COLOR_BGR2GRAY);
			absdiff(grayBack, grayCam, m_Diff);
			threshold(m_Diff, diffThre, 15, 255, THRESH_BINARY);
			pDC = m_View3.GetDC();
			m_View3.GetClientRect(rect);
			DisplayImage1(pDC, rect, diffThre);

			cvtColor(diffThre, diffColor, COLOR_GRAY2BGR);
			bitwise_and(m_Cam, diffColor, diffColor);
			pDC = m_View4.GetDC();
			m_View4.GetClientRect(rect);
			DisplayImage1(pDC, rect, diffColor);
		}
		ReleaseDC(pDC);
	}
	
}


void Ccctv::OnBnClickedButtonStart()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_bThreadFlag1 = TRUE;
	m_pThread1 = AfxBeginThread(ThreadImageCaptureFunc1, this);
}


void Ccctv::OnBnClickedButtonStop()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_bThreadFlag1 = FALSE;
}


void Ccctv::OnBnClickedButtonBg()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (m_Cam.data != NULL) {
		CRect rect;
		CDC* pDC;
		pDC = m_View1.GetDC();
		m_View1.GetClientRect(rect);
		DisplayImage1(pDC, rect, m_Cam);  // 카메라에서 읽어들인 영상을 화면에 그리기
		ReleaseDC(pDC);
	}
	m_Cam.copyTo(m_Back);
}
