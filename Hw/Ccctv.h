#pragma once
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;
// Ccctv 대화 상자

class Ccctv : public CDialog
{
	DECLARE_DYNAMIC(Ccctv)

public:
	Ccctv(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~Ccctv();


// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CCTV };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	Mat m_Cam, m_Back, m_Diff;
	VideoCapture m_Capture;
	afx_msg void OnPaint();
	CWinThread* m_pThread1;
	afx_msg void OnBnClickedButtonStart();
	afx_msg void OnBnClickedButtonStop();
	afx_msg void OnBnClickedButtonBg();
	CStatic m_View1;
	CStatic m_View2;
	CStatic m_View3;
	CStatic m_View4;
};
