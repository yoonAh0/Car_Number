#pragma once
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;

// CParabola 대화 상자

class CParabola : public CDialogEx
{
	DECLARE_DYNAMIC(CParabola)

public:
	CParabola(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CParabola();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PARABOLA };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	CStatic m_View;
	void dusflq(Point2f pt1, Point2f pt2, Point2f pt3);
	void DisplayImage(CDC* pDC, CRect rect, Mat& srcimg);
	void FillBitmapInfo(BITMAPINFO* bmi, int width, int height, int bpp, int origin);
	Mat m_image1;
	int m_cnt;
	double m_a, m_b, m_c;
	Point2f m_pt1, m_pt2, m_pt3;
	Point2f m_pts[3];
	//CArray<Point2f, &Point2f> m_arrPos;
};
