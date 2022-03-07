#pragma once
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;

// CCircyl 대화 상자

class CCircyl : public CDialogEx
{
	DECLARE_DYNAMIC(CCircyl)

public:
	CCircyl(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CCircyl();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CIRCYL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonStart();
	afx_msg void OnBnClickedButtonStop();
	Mat m_src1, m_src2;
	Mat m_dst1, m_dst2;
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	CStatic m_View;
//	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
