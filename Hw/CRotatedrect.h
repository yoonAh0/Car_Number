#pragma once
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;

struct Point4 {
	int x11, y11, x22, y22;
	Point4() {};
	Point4(int x1, int y1, int x2, int y2) { x11 = x1, y11 = y1, x22 = x2, y22 = y2; }
};


// CRotatedrect 대화 상자

class CRotatedrect : public CDialogEx
{
	DECLARE_DYNAMIC(CRotatedrect)

public:
	CRotatedrect(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CRotatedrect();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ROTATEDRECT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	CPoint m_ptLT;
	CPoint m_ptRB;
	CArray <Point4, Point4&> m_arrPo;
	Mat m_image1, m_image2;
	int m_angle;
	Point4 point44;
	void FillBitmapInfo(BITMAPINFO* bmi, int width, int height, int bpp, int origin);
	void DisplayImage(CDC* pDC, CRect rect, Mat& srcimg);
	CStatic m_View;
	afx_msg void OnDestroy();
};
