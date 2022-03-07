#pragma once
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;

// CFFT 대화 상자

class CFFT : public CDialogEx
{
	DECLARE_DYNAMIC(CFFT)

public:
	CFFT(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CFFT();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FFT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	Mat m_image, m_fft, m_result;
	Mat dft_coef, dft_img, filtered_mat, dft, m_filter;
	CStatic m_View1;
	CStatic m_View2;
	CStatic m_ViewFFT;
	int m_count;
};
