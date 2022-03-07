// CParabola.cpp: 구현 파일
//

#include "stdafx.h"
#include "Hw.h"
#include "CParabola.h"
#include "afxdialogex.h"


// CParabola 대화 상자

IMPLEMENT_DYNAMIC(CParabola, CDialogEx)

CParabola::CParabola(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PARABOLA, pParent)
{
	m_cnt = 0;
}

CParabola::~CParabola()
{
}

void CParabola::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CParabola, CDialogEx)
	ON_BN_CLICKED(IDOK, &CParabola::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CParabola::OnBnClickedCancel)
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()
//	ON_WM_TIMER()
END_MESSAGE_MAP()


// CParabola 메시지 처리기


void CParabola::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//CDialogEx::OnOK();
}


void CParabola::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//CDialogEx::OnCancel();
}


BOOL CParabola::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_View.Attach(GetDlgItem(IDC_STATIC_PIC1)->m_hWnd);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CParabola::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
					   // 그리기 메시지에 대해서는 CDialogEx::OnPaint()을(를) 호출하지 마십시오.
	CDC* pDC2;
	CRect rect2;
	//if (m_image1.data != NULL) {
	//	pDC2 = m_View.GetDC();	//picCtrl 크기 받아오기
	//	m_View.GetClientRect(rect2);
	//	DisplayImage(pDC2, rect2, m_image1);  // 카메라에서 읽어들인 영상을 화면에 그리기
	//	ReleaseDC(pDC2); 
	//}
}


void CParabola::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CRect rect1;
	CDC* pDC1;
	//SetCursorPos(point.x, point.y);
	m_image1 = Mat(350, 530, CV_8UC3, Scalar(255, 255, 255));	// B G R
	Point2f center(point.x, point.y);
	circle(m_image1, center, 10, Scalar(20), 1);

	if (m_cnt == 0) {
		m_pt1 = Point2f(point.x, point.y);
		m_pts[0] = center;
	}
	else if (m_cnt == 1) {
		m_pt2 = Point2f(point.x, point.y);
		m_pts[1] = center;
		for (int i = 0; i < 2; i++) {
			circle(m_image1, m_pts[i], 10, Scalar(20), 1);
		}
	}
	else if (m_cnt == 2) {
		m_pt3 = Point2f(point.x, point.y);
		m_pts[2] = center;

		dusflq(m_pt1, m_pt2, m_pt3);

		Point ps(20, 480); // 시작 지점

		int x, y, xx, yy;

		for (int x = 0; x < 530; x++) {
			y = (int)(x * x * m_a + x * m_b + m_c);
			//xx = ps.x + x;
			//yy = ps.y - y;

			// 출력 범위 제한
			//xx = max(min(500 - 1, xx), 0);
			//yy = max(min(500 - 1, yy), 0);

			Point2f pt(x, y);
			circle(m_image1, pt, 1, Scalar(0,0,255), 1);
			//m_image1.at<Vec3b>(y, x) = Vec3b(0, 0, 255);
		}
		m_cnt = -1;
		for (int i = 0; i < 3; i++) {
			circle(m_image1, m_pts[i], 10, Scalar(20), 1);
		}
	}
	m_cnt++;

	if (m_image1.data != NULL) {
		pDC1 = m_View.GetDC();	//picCtrl 크기 받아오기
		m_View.GetClientRect(rect1);
		DisplayImage(pDC1, rect1, m_image1);  // 카메라에서 읽어들인 영상을 화면에 그리기
		ReleaseDC(pDC1);
	}
	CDialogEx::OnLButtonDown(nFlags, point);
}

void CParabola::dusflq(Point2f pt1, Point2f pt2, Point2f pt3) {

	Mat abc;
	Mat A = (Mat_<float>(3, 3) << pow(pt1.x, 2), pt1.x, 1, pow(pt2.x, 2), pt2.x, 1, pow(pt3.x, 2), pt3.x, 1);
	Mat B = (Mat_<float>(3, 1) << pt1.y, pt2.y, pt3.y);

	solve(A, B, abc);
	//cout << "Coefficients:\n " << abc << endl;

	m_a = abc.at<float>(0);
	m_b = abc.at<float>(1);
	m_c = abc.at<float>(2);
}

void CParabola::FillBitmapInfo(BITMAPINFO* bmi, int width, int height, int bpp, int origin)

{
	assert(bmi && width >= 0 && height >= 0 && (bpp == 8 || bpp == 24 || bpp == 32));
	BITMAPINFOHEADER* bmih = &(bmi->bmiHeader);
	memset(bmih, 0, sizeof(*bmih));
	bmih->biSize = sizeof(BITMAPINFOHEADER);
	bmih->biWidth = width;
	bmih->biHeight = origin ? abs(height) : -abs(height);
	bmih->biPlanes = 1;
	bmih->biBitCount = (unsigned short)bpp;
	bmih->biCompression = BI_RGB;
	if (bpp == 8) {
		RGBQUAD* palette = bmi->bmiColors;
		for (int i = 0; i < 256; i++) {
			palette[i].rgbBlue = palette[i].rgbGreen = palette[i].rgbRed = (BYTE)i;
			palette[i].rgbReserved = 0;
		}
	}
}

void CParabola::DisplayImage(CDC* pDC, CRect rect, Mat& srcimg)
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
