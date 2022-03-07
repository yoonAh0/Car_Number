#pragma once
#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;

// CFinal 대화 상자

class CFinal : public CDialogEx
{
	DECLARE_DYNAMIC(CFinal)

public:
	CFinal(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CFinal();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FINAL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnBnClickedButtonPlay();

	Mat m_image;

	CStatic m_View1;	//원본 영상
	CStatic m_View2;	//차량 영상 번호 영역
	CStatic m_View3;	//번호판 영상
	CString m_editResult;	//분류 결과

	int m_K1, m_K2;
	Ptr<ml::SVM> m_svm;
	Ptr<ml::KNearest> m_knn[2];
	Mat  m_plate_img, m_color_plate;	// 컬러 번호판 영상 
	vector<Mat> m_candidate_img;
	int plate_no;

	VideoCapture m_Capture;
	thread* m_pThread;

	void FillBitmapInfo(BITMAPINFO* bmi, int width, int height, int bpp, int origin);
	void DisplayImage(CDC* pDC, CRect rect, Mat& srcimg);
	void ThreadFunc();	//Thread 함수
};
