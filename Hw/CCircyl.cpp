// CCircyl.cpp: 구현 파일
//

#include "stdafx.h"
#include "Hw.h"
#include "CCircyl.h"
#include "afxdialogex.h"

// CCircyl 대화 상자

IMPLEMENT_DYNAMIC(CCircyl, CDialogEx)

CCircyl::CCircyl(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CIRCYL, pParent)
{

}

CCircyl::~CCircyl()
{
}

void CCircyl::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC1, m_View);
}


BEGIN_MESSAGE_MAP(CCircyl, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_START, &CCircyl::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CCircyl::OnBnClickedButtonStop)
	ON_WM_PAINT()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CCircyl 메시지 처리기
void FillBitmapInfo5(BITMAPINFO* bmi, int width, int height, int bpp, int origin)
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
void DisplayImage5(CDC* pDC, CRect rect, Mat& srcimg)
{
	Mat img;
	int step = ((int)(rect.Width() / 4)) * 4; // 4byte 단위조정
	resize(srcimg, img, Size(step, rect.Height()));

	uchar buffer[sizeof(BITMAPINFOHEADER) * 1024];
	BITMAPINFO* bmi = (BITMAPINFO*)buffer;

	int bmp_w = img.cols;
	int bmp_h = img.rows;
	int depth = img.depth();
	int channels = img.channels();
	int bpp = 8 * channels;

	FillBitmapInfo5(bmi, bmp_w, bmp_h, bpp, 0);

	int from_x = MIN(0, bmp_w - 1);
	int from_y = MIN(0, bmp_h - 1);
	int sw = MAX(MIN(bmp_w - from_x, rect.Width()), 0);
	int sh = MAX(MIN(bmp_h - from_y, rect.Height()), 0);

	SetDIBitsToDevice(pDC->m_hDC, rect.left, rect.top, sw, sh, from_x, from_y, 0, sh, img.data + from_y * img.step, bmi, 0);

	img.release();
}
BOOL m_bThreadFlag3 = FALSE;
CWinThread *m_Thread;
UINT ThreadImageCaptureFunc3(LPVOID param)
{
	CCircyl *pDlg = (CCircyl *)param;
	while (m_bThreadFlag3) {
		pDlg->Invalidate(FALSE);
		Sleep(30);

	}
	return 0;
}
BOOL CCircyl::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_src1 = imread("D:/Users/USER/Desktop/윤아영/기타/사진/배경사진/bts55.jpg");
	m_src2 = imread("D:/Users/USER/Desktop/윤아영/기타/사진/배경사진/bts22.jpg");
	m_src1.copyTo(m_dst1);
	m_src2.copyTo(m_dst2);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
void CCircyl::OnBnClickedButtonStart()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_bThreadFlag3 = TRUE;
	m_Thread = ::AfxBeginThread(ThreadImageCaptureFunc3, this);
}


void CCircyl::OnBnClickedButtonStop()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_bThreadFlag3 = FALSE;
}

void CCircyl::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
					  //  그리기 메시지에 대해서는 CDialogEx::OnPaint()을(를) 호출하지 마십시오.
	int width = 400, height = 277;
	Mat image(width, height, m_src1.type(), Scalar(255, 255, 255));
	Point2f center(width / 2 - 65, height / 3);
	Size size(60, 32);

	for (int i = 0; i < m_src1.cols - 1; i++) {

		for (int j = 0; j < m_src1.rows; j++) {
			m_src2.ptr<Vec3b>(j, i + 1)[0] = m_dst2.ptr<Vec3b>(j, i)[0];
			m_src2.ptr<Vec3b>(j, i + 1)[1] = m_dst2.ptr<Vec3b>(j, i)[1];
			m_src2.ptr<Vec3b>(j, i + 1)[2] = m_dst2.ptr<Vec3b>(j, i)[2];

			m_src1.ptr<Vec3b>(j, i + 1)[0] = m_dst1.ptr<Vec3b>(j, i)[0];
			m_src1.ptr<Vec3b>(j, i + 1)[1] = m_dst1.ptr<Vec3b>(j, i)[1];
			m_src1.ptr<Vec3b>(j, i + 1)[2] = m_dst1.ptr<Vec3b>(j, i)[2];
			if (i == 0) {
				m_src1.ptr<Vec3b>(j, i)[0] = m_src1.ptr<Vec3b>(j, m_src1.cols - 1)[0];
				m_src1.ptr<Vec3b>(j, i)[1] = m_src1.ptr<Vec3b>(j, m_src1.cols - 1)[1];
				m_src1.ptr<Vec3b>(j, i)[2] = m_src1.ptr<Vec3b>(j, m_src1.cols - 1)[2];

				m_src2.ptr<Vec3b>(j, i)[0] = m_src2.ptr<Vec3b>(j, m_src2.cols - 1)[0];
				m_src2.ptr<Vec3b>(j, i)[1] = m_src2.ptr<Vec3b>(j, m_src2.cols - 1)[1];
				m_src2.ptr<Vec3b>(j, i)[2] = m_src2.ptr<Vec3b>(j, m_src2.cols - 1)[2];
			}
		}
	}
	m_src1.copyTo(m_dst1);
	m_src2.copyTo(m_dst2);


	double rad = 3.14159 / 180;
	double angle = 180;

	for (int i = 0; i < m_src1.cols - 1; i++) {

		for (int j = 0; j < m_src1.rows; j++) {
			Point2d EllPoint2(-size.width*cos(angle * rad), size.height*sin(angle * rad));
			Point2d EllPoint(EllPoint2.x + center.x, EllPoint2.y + center.y);
			if (EllPoint.y >= 92) {
				image.at<Vec3b>(EllPoint.y + j, EllPoint.x)[0] = m_src1.at<Vec3b>(j, i)[0];
				image.at<Vec3b>(EllPoint.y + j, EllPoint.x)[1] = m_src1.at<Vec3b>(j, i)[1];
				image.at<Vec3b>(EllPoint.y + j, EllPoint.x)[2] = m_src1.at<Vec3b>(j, i)[2];
			}
			if (EllPoint.y < 92) {
				image.at<Vec3b>(EllPoint.y + j, EllPoint.x)[0] = m_src2.at<Vec3b>(j, i)[0];
				image.at<Vec3b>(EllPoint.y + j, EllPoint.x)[1] = m_src2.at<Vec3b>(j, i)[1];
				image.at<Vec3b>(EllPoint.y + j, EllPoint.x)[2] = m_src2.at<Vec3b>(j, i)[2];

			}
		}
		angle = (1 * 360 / (double)m_src1.cols) + angle;
	}

	CRect rect;
	CDC* pDC;
	pDC = m_View.GetDC();
	m_View.GetClientRect(rect);
	DisplayImage5(pDC, rect, image);

}
