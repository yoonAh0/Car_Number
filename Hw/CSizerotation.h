#pragma once
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;

// CSizerotation 대화 상자

class CSizerotation : public CDialogEx
{
	DECLARE_DYNAMIC(CSizerotation)

public:
	CSizerotation(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CSizerotation();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_SIZEROTATION };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	VideoCapture m_Capture;
	CWinThread* m_pThread7;
	Mat m_image;
	CStatic m_View1;
	CStatic m_View2;
	CSliderCtrl m_sliderSize;
	CSliderCtrl m_sliderRota;
	CEdit m_editSize;
	CEdit m_editRota;
	float m_fPosSize;
	int m_iPosRota;
	afx_msg void OnBnClickedButtonStart();
	afx_msg void OnBnClickedButtonStop();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
};
