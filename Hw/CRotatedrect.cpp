// CRotatedrect.cpp: 구현 파일
//

#include "stdafx.h"
#include "Hw.h"
#include "CRotatedrect.h"
#include "afxdialogex.h"

//#ifdef _DEBUG
//#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
//#endif


// CRotatedrect 대화 상자

IMPLEMENT_DYNAMIC(CRotatedrect, CDialogEx)

CRotatedrect::CRotatedrect(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_ROTATEDRECT, pParent)
{
	m_angle = 0;
}

CRotatedrect::~CRotatedrect()
{
}

void CRotatedrect::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CRotatedrect, CDialogEx)
	ON_BN_CLICKED(IDOK, &CRotatedrect::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CRotatedrect::OnBnClickedCancel)
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CRotatedrect 메시지 처리기


void CRotatedrect::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//CDialogEx::OnOK();
}


void CRotatedrect::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//CDialogEx::OnCancel();
}


BOOL CRotatedrect::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_View.Attach(GetDlgItem(IDC_STATIC_PIC)->m_hWnd);
	SetTimer(1, 20, NULL);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CRotatedrect::OnPaint()
{
	CDC* pDC2;
	CRect rect2;
	CPaintDC dc(this); // device context for painting
					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
					  //  그리기 메시지에 대해서는 CDialogEx::OnPaint()을(를) 호출하지 마십시오.
	if (m_image1.data != NULL) {
		pDC2 = m_View.GetDC();	//picCtrl 크기 받아오기
		m_View.GetClientRect(rect2);
		DisplayImage(pDC2, rect2, m_image1);  // 카메라에서 읽어들인 영상을 화면에 그리기
		ReleaseDC(pDC2);
	}	
}


void CRotatedrect::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CRect rect1;
	CDC* pDC1;
	m_image1 = Mat(300, 500, CV_8UC1, Scalar(500));
	Rect bound_rect1(0, 0, 0, 0);
	Rect bound_rect2;
	RotatedRect rot_rect;

	for (int i = 0; i < m_arrPo.GetSize(); i++)
	{
		Point4 point = m_arrPo.GetAt(i);	//도형 위치 가져오기

		Point2i center((point.x22 / 2) + 70, (point.y22 / 2) + 50);	//중심점
		Point2f pts[4];	//꼭짓점 저장 배열
		Size2i size((point.x22 - point.x11), (point.y22 - point.y11));	//크기
		rot_rect = RotatedRect(center, size, m_angle);	//회전 사각형

		bound_rect2 = rot_rect.boundingRect();	//회전 사각형 영역
		bound_rect1 = bound_rect1 | bound_rect2;
		circle(m_image1, rot_rect.center, 1, Scalar(0), 2);	//회전 사각형 중심점 그리기

		rot_rect.points(pts);	//꼭짓점 받아오기

		for (int j = 0; j < 4; j++) {
			line(m_image1, pts[j], pts[(j + 1) % 4], Scalar(0), 2);	//꼭짓점 이어 그리기
		}
	}
	rectangle(m_image1, bound_rect1, Scalar(0), 1);	//회전 사각형 영역 그리기
	m_angle++;

	if (m_image1.data != NULL) {
		pDC1 = m_View.GetDC();	//picCtrl 크기 받아오기
		m_View.GetClientRect(rect1);
		DisplayImage(pDC1, rect1, m_image1);  // 카메라에서 읽어들인 영상을 화면에 그리기
		ReleaseDC(pDC1);
	}
	CDialogEx::OnTimer(nIDEvent);
}


void CRotatedrect::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	m_ptRB = point;
	point44.x11 = m_ptLT.x;
	point44.y11 = m_ptLT.y;
	point44.x22 = m_ptRB.x;
	point44.y22 = m_ptRB.y;
	m_arrPo.Add(point44);
	CDialogEx::OnLButtonUp(nFlags, point);
}


void CRotatedrect::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	m_ptLT = point;
	CDialogEx::OnLButtonDown(nFlags, point);
}


void CRotatedrect::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (nFlags & MK_LBUTTON) {
		m_ptRB = point;
	}
	CDialogEx::OnMouseMove(nFlags, point);
}


void CRotatedrect::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	m_View.Detach();
}

void CRotatedrect::FillBitmapInfo(BITMAPINFO* bmi, int width, int height, int bpp, int origin)

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

void CRotatedrect::DisplayImage(CDC* pDC, CRect rect, Mat& srcimg) 
{
	Mat img;
	int step = ((int)(rect.Width() / 4)) * 4 ; // 4byte 단위조정해야 영상이 기울어지지 않는다.

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
