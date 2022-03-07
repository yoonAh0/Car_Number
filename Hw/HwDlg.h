
// HwDlg.h: 헤더 파일
//

#pragma once
#include <opencv2/opencv.hpp>
#include "CParabola.h"
#include "CRotatedrect.h"
#include "CRGBimage.h"
#include "Ccctv.h"
#include "CCircyl.h"
#include "CLinesearch.h"
#include "CSizerotation.h"
#include "CFFT.h"
#include "CFinal.h"
using namespace std;
using namespace cv;



// CHwDlg 대화 상자
class CHwDlg : public CDialogEx
{
// 생성입니다.
public:
	CHwDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_HW_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;
	CRotatedrect *pRot;
	CParabola *pPara;
	CRGBimage *pImg;
	Ccctv *pCCTV;
	CCircyl *pCir;
	CLinesearch *pLine;
	CSizerotation *pSiRo;
	CFFT *pFFT;
	CFinal *pFin;


	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CStatic m_View;
//	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
//	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
//	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
//	afx_msg void OnTimer(UINT_PTR nIDEvent);
//	CTabCtrl m_tabTest;
	afx_msg void OnTcnSelchangeTabTest(NMHDR *pNMHDR, LRESULT *pResult);
	CTabCtrl m_tabTest;
	afx_msg void OnDestroy();
};
