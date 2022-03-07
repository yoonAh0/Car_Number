// CLinesearch.cpp: 구현 파일
//

#include "stdafx.h"
#include "Hw.h"
#include "CLinesearch.h"
#include "afxdialogex.h"


// CLinesearch 대화 상자

IMPLEMENT_DYNAMIC(CLinesearch, CDialogEx)

CLinesearch::CLinesearch(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_LINESEARCH, pParent)
{
	m_cnt = 0;
	m_check = 0;
}

CLinesearch::~CLinesearch()
{
}

void CLinesearch::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC1, m_View1);
	DDX_Control(pDX, IDC_STATIC2, m_View2);
}


BEGIN_MESSAGE_MAP(CLinesearch, CDialogEx)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON_START, &CLinesearch::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_LINE, &CLinesearch::OnBnClickedButtonLine)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CLinesearch::OnBnClickedButtonStop)
	ON_BN_CLICKED(IDC_BUTTON_CAPTURE, &CLinesearch::OnBnClickedButtonCapture)
END_MESSAGE_MAP()


// CLinesearch 메시지 처리기


BOOL CLinesearch::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_Capture.open(0);   // 카메라 연결
	if (!m_Capture.isOpened()) AfxMessageBox(_T("There is no camera captured!"));
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

BOOL m_bThreadFlag2;
UINT ThreadImageCaptureFunc2(LPVOID param)
{
	CLinesearch *pDlg = (CLinesearch *)param;
	while (m_bThreadFlag2) {
		pDlg->m_Capture >> pDlg->m_Cam;  // 영상 획득
		pDlg->Invalidate(FALSE);
	}
	return 0;
}
void FillBitmapInfo2(BITMAPINFO* bmi, int width, int height, int bpp, int origin)
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
void DisplayImage2(CDC* pDC, CRect rect, Mat& srcimg)
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

	FillBitmapInfo2(bmi, bmp_w, bmp_h, bpp, 0);

	int from_x = MIN(0, bmp_w - 1);
	int from_y = MIN(0, bmp_h - 1);
	int sw = MAX(MIN(bmp_w - from_x, rect.Width()), 0);
	int sh = MAX(MIN(bmp_h - from_y, rect.Height()), 0);

	SetDIBitsToDevice(pDC->m_hDC, rect.left, rect.top, sw, sh, from_x, from_y, 0, sh, img.data + from_y * img.step, bmi, 0);

	img.release();
}
void CLinesearch::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
					   // 그리기 메시지에 대해서는 CDialogEx::OnPaint()을(를) 호출하지 마십시오.
	CDC* pDC;
	CRect rect;
	threshold(m_Cap, m_thre, 111, 255, THRESH_BINARY);
	Canny(m_thre, m_canny, 50, 200, 3);
	if (m_Cap.data != NULL) {
		pDC = m_View1.GetDC();	//picCtrl 크기 받아오기
		m_View1.GetClientRect(rect);
		DisplayImage2(pDC, rect, m_Cam);  // 카메라에서 읽어들인 영상을 화면에 그리기
		pDC = m_View2.GetDC();	//picCtrl 크기 받아오기
		m_View2.GetClientRect(rect);
		DisplayImage2(pDC, rect, m_canny);  // 카메라에서 읽어들인 영상을 화면에 그리기
		ReleaseDC(pDC);
	}
}


void CLinesearch::OnBnClickedButtonStart()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_bThreadFlag2 = TRUE;
	m_pThread2 = AfxBeginThread(ThreadImageCaptureFunc2, this);
}


void CLinesearch::OnBnClickedButtonLine()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	CString str;
	m_check++;
	for (int x = 0; x < m_canny.cols; x++) {
		if (m_canny.at<uchar>(m_canny.rows / 2, x) == 255) {
			m_cnt++;
		}
	}
	m_cntLine = m_cnt / 2;
	if (m_check == 1) {
		SetDlgItemInt(IDC_EDIT_LINE, m_cntLine);
	}

	Point p1, p2;
	for (int i = 0; i < m_canny.cols; i++) {
		if (m_canny.at<uchar>((m_canny.rows / 2) - 5, i) == 255) {
			p1 = Point((m_canny.rows / 2) - 5, i);
			break;
		}
	}
	for (int i = 0; i < m_canny.cols; i++) {
		if (m_canny.at<uchar>((m_canny.rows / 2) + 5, i) == 255) {
			p2 = Point((m_canny.rows / 2) + 5, i);
			break;
		}
	}

	float d1, d2;
	d1 = p1.x - p2.x;
	d2 = p1.y - p2.y;

	float angle;
	angle = atan2(d2, d1) * 180 / CV_PI;

	if (angle > 0) {
		angle = 90 + (180 - angle);
	}
	else if (angle < 0) {
		angle = 90 - (angle + 180);
	}

	SetDlgItemInt(IDC_EDIT_ANGLE, (int)angle);

	m_bThreadFlag2 = FALSE;
}


void CLinesearch::OnBnClickedButtonStop()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//m_bThreadFlag2 = FALSE;
}


void CLinesearch::OnBnClickedButtonCapture()
{
	m_Cam.copyTo(m_Cap);
}
