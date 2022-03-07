#pragma once
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;

// CLinesearch 대화 상자

class CLinesearch : public CDialogEx
{
	DECLARE_DYNAMIC(CLinesearch)

public:
	CLinesearch(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CLinesearch();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LINESEARCH };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	VideoCapture m_Capture;
	Mat m_Cam, m_Cap;
	Mat m_thre, m_canny;
	CWinThread* m_pThread2;
	afx_msg void OnBnClickedButtonStart();
	afx_msg void OnBnClickedButtonLine();
	afx_msg void OnBnClickedButtonStop();
	CStatic m_View1;
	CStatic m_View2;
	int m_cnt, m_cntLine, m_check;
	afx_msg void OnBnClickedButtonCapture();
};
