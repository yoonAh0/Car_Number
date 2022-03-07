#pragma once
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;

// CRGBimage 대화 상자

class CRGBimage : public CDialogEx
{
	DECLARE_DYNAMIC(CRGBimage)

public:
	CRGBimage(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CRGBimage();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_RGBIMAGE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonStart();
	afx_msg void OnBnClickedButtonStop();
	Mat m_Image;
	VideoCapture m_Capture;
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	CStatic m_View;
	CWinThread* m_pThread;
	afx_msg void OnBnClickedRadioOriginal();
	afx_msg void OnBnClickedRadioRed();
	afx_msg void OnBnClickedRadioGreen();
	afx_msg void OnBnClickedRadioBlue();
	Mat m_bgr[3];
	int m_Color;
	CSliderCtrl m_ctrlSilder;
	int m_value;
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	int m_Height, m_Width;
};