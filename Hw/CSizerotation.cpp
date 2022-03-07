// CSizerotation.cpp: 구현 파일
//

#include "stdafx.h"
#include "Hw.h"
#include "CSizerotation.h"
#include "afxdialogex.h"


// CSizerotation 대화 상자

IMPLEMENT_DYNAMIC(CSizerotation, CDialogEx)

CSizerotation::CSizerotation(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_SIZEROTATION, pParent)
{

}

CSizerotation::~CSizerotation()
{
}

void CSizerotation::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC1, m_View1);
	DDX_Control(pDX, IDC_STATIC2, m_View2);
	DDX_Control(pDX, IDC_SLIDER_SIZE, m_sliderSize);
	DDX_Control(pDX, IDC_SLIDER_ROTA, m_sliderRota);
	//DDX_Control(pDX, IDC_EDIT_SIZE, m_editSize);
	//DDX_Control(pDX, IDC_EDIT_ROTA, m_editRota);
}


BEGIN_MESSAGE_MAP(CSizerotation, CDialogEx)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON_START, &CSizerotation::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CSizerotation::OnBnClickedButtonStop)
	ON_WM_HSCROLL()
END_MESSAGE_MAP()


// CSizerotation 메시지 처리기

//uchar bilinear_value(Mat img, double x, double y) {
//	if (x >= img.cols - 2) x--;
//	if (y >= img.rows - 2) y--;
//
//	Point pt((int)x, (int)y);
//	int A = img.at<uchar>(pt);
//	int B = img.at<uchar>(pt + Point(0, 1));
//	int C = img.at<uchar>(pt + Point(1, 0));
//	int D = img.at<uchar>(pt + Point(1, 1));
//
//	double alpha = y - pt.y;
//	double beta = x - pt.x;
//	int M1 = A + (int)cvRound(alpha *(B - A));
//	int M2 = C + (int)cvRound(alpha *(D - C));
//	int P = M1 + (int)cvRound(beta * (M2 - M1));
//	return saturate_cast<uchar>(P);
//}
//
//void affine_transform(Mat img, Mat& dst, Mat map, Size size) {
//	dst = Mat(img.size(), img.type(), Scalar(0));
//	Rect rect(Point(0, 0), img.size());
//
//	Mat inv_map;
//	invertAffineTransform(map, inv_map);
//
//	for (int i = 0; i < dst.rows; i++) {
//		for (int j = 0; j < dst.cols; j++) {
//			Point3d ji(j, i, 1);
//			Mat xy = inv_map * (Mat)ji;
//			Point2d pt = (Point2d)xy;
//
//			if (rect.contains(pt)) {
//				dst.at<uchar>(i, j) = bilinear_value(img, pt.x, pt.y);
//			}
//		}
//	}
//}
//
//Mat getAffineMap(Point2d center, double dgree, double fx = 1, double fy = 1, Point2d translate = Point(0, 0))
//{
//	Mat rot_map = Mat::eye(3, 3, CV_64F);
//	Mat center_trans = Mat::eye(3, 3, CV_64F);
//	Mat org_trans = Mat::eye(3, 3, CV_64F);
//	Mat scale_map = Mat::eye(3, 3, CV_64F);
//	Mat trans_map = Mat::eye(3, 3, CV_64F);
//
//	double radian = dgree / 180 * CV_PI;
//	rot_map.at<double>(0, 0) = cos(radian);
//	rot_map.at<double>(0, 1) = sin(radian);
//	rot_map.at<double>(1, 0) = -sin(radian);
//	rot_map.at<double>(1, 1) = cos(radian);
//
//	center_trans.at<double>(0, 2) = center.x;
//	center_trans.at<double>(1, 2) = center.y;
//	org_trans.at<double>(0, 2) = -center.x;
//	org_trans.at<double>(1, 2) = -center.y;
//
//	scale_map.at<double>(0, 2) = fx;
//	scale_map.at<double>(1, 2) = fy;
//	trans_map.at<double>(0, 2) = translate.x;
//	trans_map.at<double>(1, 2) = translate.y;
//
//	Mat ret_map = center_trans * rot_map * trans_map * scale_map * org_trans;
//	ret_map.resize(2);
//	return ret_map;
//}

BOOL m_bThreadFlag7;
UINT ThreadImageCaptureFunc7(LPVOID param)
{
	CSizerotation *pDlg = (CSizerotation *)param;
	while (m_bThreadFlag7) {
		pDlg->m_Capture >> pDlg->m_image;  // 영상 획득
		pDlg->Invalidate(FALSE);
	}
	return 0;
}
void FillBitmapInfo7(BITMAPINFO* bmi, int width, int height, int bpp, int origin)

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

void DisplayImage7(CDC* pDC, CRect rect, Mat& srcimg)
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

	FillBitmapInfo7(bmi, bmp_w, bmp_h, bpp, 0);

	int from_x = MIN(0, bmp_w - 1);
	int from_y = MIN(0, bmp_h - 1);
	int sw = MAX(MIN(bmp_w - from_x, rect.Width()), 0);
	int sh = MAX(MIN(bmp_h - from_y, rect.Height()), 0);

	SetDIBitsToDevice(pDC->m_hDC, rect.left, rect.top, sw, sh, from_x, from_y, 0, sh, img.data + from_y * img.step, bmi, 0);

	img.release();
}

BOOL CSizerotation::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_Capture.open(0);   // 카메라 연결
	if (!m_Capture.isOpened()) AfxMessageBox(_T("There is no camera captured!"));

	m_sliderSize.SetRange(5, 20);
	m_sliderSize.SetPos(10);
	//크기 에디트 박스
	m_fPosSize = m_sliderSize.GetPos() / 10.f;
	//CString sPos1;
	//sPos1.Format(_T("%.2f"), m_fPosSize);
	//m_editSize.SetWindowText(sPos1);

	//회전 슬라이더
	m_sliderRota.SetRange(0, 360);
	m_sliderRota.SetPos(0);
	//회전 에디트 박스
	m_iPosRota = m_sliderRota.GetPos();
	//CString sPos2;
	//sPos2.Format(_T("%d"), m_iPosRota);
	//m_editRota.SetWindowText(sPos2);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CSizerotation::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
					   // 그리기 메시지에 대해서는 CDialogEx::OnPaint()을(를) 호출하지 마십시오.
	CDC* pDC;
	CRect rect;
	Mat dst1, rot_map;
	Point center = m_image.size() / 2;
	Size size = m_image.size();
	if (m_image.data != NULL) {
		pDC = m_View1.GetDC();	//picCtrl 크기 받아오기
		m_View1.GetClientRect(rect);
		DisplayImage7(pDC, rect, m_image);  // 카메라에서 읽어들인 영상을 화면에 그리기

		//회전 
		//rotation(m_image, dst1, m_iPosRota, center);
		//크기
		//scaling_bilinear(m_image, dst1, Size(m_image.cols * m_fPosSize, m_image.rows * m_fPosSize));

		rot_map = getRotationMatrix2D(center, m_iPosRota, m_fPosSize);
		warpAffine(m_image, dst1, rot_map, size);

		pDC = m_View2.GetDC();	//picCtrl 크기 받아오기
		m_View2.GetClientRect(rect);
		DisplayImage7(pDC, rect, dst1);  // 카메라에서 읽어들인 영상을 화면에 그리기
		ReleaseDC(pDC);
	}
}


void CSizerotation::OnBnClickedButtonStart()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_bThreadFlag7 = TRUE;
	m_pThread7 = AfxBeginThread(ThreadImageCaptureFunc7, this);
}


void CSizerotation::OnBnClickedButtonStop()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_bThreadFlag7 = FALSE;
}


void CSizerotation::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (IDC_SLIDER_ROTA == pScrollBar->GetDlgCtrlID()) {
		m_iPosRota = m_sliderRota.GetPos();

		//CString sPos1;
		//sPos1.Format(_T("%d"), m_iPosRota);
		//m_editRota.SetWindowText(sPos1);
	}
	if (IDC_SLIDER_SIZE == pScrollBar->GetDlgCtrlID()) {
		m_fPosSize = m_sliderSize.GetPos() / 10.f;

		//CString sPos2;
		//sPos2.Format(_T("%.2f"), m_fPosSize);
		//m_editSize.SetWindowText(sPos2);
	}

	CDC* pDC;
	CRect rect;
	Mat dst1, rot_map;
	Point center = m_image.size() / 2;
	Size size = m_image.size();
	if (m_image.data != NULL) {
		pDC = m_View1.GetDC();	//picCtrl 크기 받아오기
		m_View1.GetClientRect(rect);
		DisplayImage7(pDC, rect, m_image);  // 카메라에서 읽어들인 영상을 화면에 그리기

		//회전 
		//rotation(m_image, dst1, m_iPosRota, center);
		//크기
		//scaling_bilinear(m_image, dst1, Size(m_image.cols * m_fPosSize, m_image.rows * m_fPosSize));

		rot_map = getRotationMatrix2D(center, m_iPosRota, m_fPosSize);
		warpAffine(m_image, dst1, rot_map, size);

		pDC = m_View2.GetDC();	//picCtrl 크기 받아오기
		m_View2.GetClientRect(rect);
		DisplayImage7(pDC, rect, dst1);  // 카메라에서 읽어들인 영상을 화면에 그리기
		ReleaseDC(pDC);
	}
	CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);
}
