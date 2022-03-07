
// HwDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "Hw.h"
#include "HwDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CHwDlg 대화 상자



CHwDlg::CHwDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_HW_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CHwDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_STATIC1, m_View);
	//DDX_Control(pDX, IDC_TAB_TEST, m_tabTest);
}

BEGIN_MESSAGE_MAP(CHwDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
//	ON_WM_LBUTTONDOWN()
//	ON_WM_LBUTTONUP()
//	ON_WM_MOUSEMOVE()
//	ON_WM_TIMER()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_TEST, &CHwDlg::OnTcnSelchangeTabTest)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CHwDlg 메시지 처리기

BOOL CHwDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	CRect rect;
	m_tabTest.Attach(GetDlgItem(IDC_TAB_TEST)->m_hWnd);
	m_tabTest.GetWindowRect(rect);	//탭 컨트롤 크기 가져오기
			
	m_tabTest.InsertItem(0, _T("RotaRect"));		//회전 사각형 탭 추가
	m_tabTest.InsertItem(1, _T("Parabola"));		//포물선 탭 추가
	m_tabTest.InsertItem(2, _T("Channel"));		//채널분리 탭 추가
	m_tabTest.InsertItem(3, _T("CCTV"));			//cctv 탭 추가
	m_tabTest.InsertItem(4, _T("CirCyl"));	//원통 회전 영상 탭 추가
	m_tabTest.InsertItem(5, _T("LineSear"));		//선 찾기 탭 추가
	m_tabTest.InsertItem(6, _T("Size/Rota"));	//크기 회전 탭 추가
	m_tabTest.InsertItem(7, _T("FFT"));				//FFT 필터 추가
	m_tabTest.InsertItem(8, _T("Final"));				//기말 프젝 추가

	m_tabTest.SetCurSel(0);		//1번째 탭 표시
	

	pRot = new CRotatedrect;	//회전 사각형 탭 추가
	pRot->Create(IDD_ROTATEDRECT, &m_tabTest);	//m_tabTest에 생성
	pRot->MoveWindow(0, 20, rect.Width() - 5, rect.Height() - 5);	//크기 조절
	pRot->ShowWindow(SW_SHOW);	//표시

	pPara = new CParabola;	//포물선 탭 추가
	pPara->Create(IDD_PARABOLA, &m_tabTest);	//m_tabTest에 생성
	pPara->MoveWindow(0, 20, rect.Width() - 5, rect.Height() - 5);	//크기 조절
	pPara->ShowWindow(SW_HIDE);	//표시

	pImg = new CRGBimage;	//채널분리 탭 추가
	pImg->Create(IDD_RGBIMAGE, &m_tabTest);	//m_tabTest에 생성
	pImg->MoveWindow(0, 20, rect.Width() - 5, rect.Height() - 5);	//크기 조절
	pImg->ShowWindow(SW_HIDE);	//표시

	pCCTV = new Ccctv;	//cctv 탭 추가
	pCCTV->Create(IDD_CCTV, &m_tabTest);	//m_tabTest에 생성
	pCCTV->MoveWindow(0, 20, rect.Width() - 5, rect.Height() - 5);	//크기 조절
	pCCTV->ShowWindow(SW_HIDE);	//표시

	pCir = new CCircyl;	//원통 회전 영상 탭 추가
	pCir->Create(IDD_CIRCYL, &m_tabTest);	//m_tabTest에 생성
	pCir->MoveWindow(0, 20, rect.Width() - 5, rect.Height() - 5);	//크기 조절
	pCir->ShowWindow(SW_HIDE);	//표시

	pLine = new CLinesearch;	//선 찾기 탭 추가
	pLine->Create(IDD_LINESEARCH, &m_tabTest);	//m_tabTest에 생성
	pLine->MoveWindow(0, 20, rect.Width() - 5, rect.Height() - 5);	//크기 조절
	pLine->ShowWindow(SW_HIDE);	//표시

	pSiRo = new CSizerotation;	//크기 회전 탭 추가
	pSiRo->Create(IDD_DIALOG_SIZEROTATION, &m_tabTest);	//m_tabTest에 생성
	pSiRo->MoveWindow(0, 20, rect.Width() - 5, rect.Height() - 5);	//크기 조절
	pSiRo->ShowWindow(SW_HIDE);	//표시

	pFFT = new CFFT;	//크기 회전 탭 추가
	pFFT->Create(IDD_FFT, &m_tabTest);	//m_tabTest에 생성
	pFFT->MoveWindow(0, 20, rect.Width() - 5, rect.Height() - 5);	//크기 조절
	pFFT->ShowWindow(SW_HIDE);	//표시

	pFin = new CFinal;	//크기 회전 탭 추가
	pFin->Create(IDD_FINAL, &m_tabTest);	//m_tabTest에 생성
	pFin->MoveWindow(0, 20, rect.Width() - 5, rect.Height() - 5);	//크기 조절
	pFin->ShowWindow(SW_HIDE);	//표시
	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CHwDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CHwDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CHwDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CHwDlg::OnTcnSelchangeTabTest(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (IDC_TAB_TEST == pNMHDR->idFrom)	//IDC_TAB_TEST인지 확인
	{
		int iSelect = m_tabTest.GetCurSel();

		switch (iSelect)
		{
		case 0: 
			pRot->ShowWindow(SW_SHOW);	//표시
			pPara->ShowWindow(SW_HIDE);	//숨김
			pImg->ShowWindow(SW_HIDE);	//숨김
			pCCTV->ShowWindow(SW_HIDE);	//숨김
			pCir->ShowWindow(SW_HIDE);	//숨김
			pLine->ShowWindow(SW_HIDE);	//숨김
			pSiRo->ShowWindow(SW_HIDE);	//숨김
			pFFT->ShowWindow(SW_HIDE);	//숨김
			pFin->ShowWindow(SW_HIDE); //숨김
			break;
		case 1:
			pRot->ShowWindow(SW_HIDE);	//숨김
			pPara->ShowWindow(SW_SHOW);	//표시
			pImg->ShowWindow(SW_HIDE);	//숨김
			pCCTV->ShowWindow(SW_HIDE);	//숨김
			pCir->ShowWindow(SW_HIDE);	//숨김
			pLine->ShowWindow(SW_HIDE);	//숨김
			pSiRo->ShowWindow(SW_HIDE);	//숨김
			pFFT->ShowWindow(SW_HIDE);	//숨김
			pFin->ShowWindow(SW_HIDE); //숨김
			break;
		case 2:
			pRot->ShowWindow(SW_HIDE);	//숨김
			pPara->ShowWindow(SW_HIDE);	//숨김
			pImg->ShowWindow(SW_SHOW);	//표시
			pCCTV->ShowWindow(SW_HIDE);	//숨김
			pCir->ShowWindow(SW_HIDE);	//숨김
			pLine->ShowWindow(SW_HIDE);	//숨김
			pSiRo->ShowWindow(SW_HIDE);	//숨김
			pFFT->ShowWindow(SW_HIDE);	//숨김
			pFin->ShowWindow(SW_HIDE); //숨김
			break;
		case 3:
			pRot->ShowWindow(SW_HIDE);	//숨김
			pPara->ShowWindow(SW_HIDE);	//숨김
			pImg->ShowWindow(SW_HIDE);	//숨김
			pCCTV->ShowWindow(SW_SHOW);	//표시
			pCir->ShowWindow(SW_HIDE);	//숨김
			pLine->ShowWindow(SW_HIDE);	//숨김
			pSiRo->ShowWindow(SW_HIDE);	//숨김
			pFFT->ShowWindow(SW_HIDE);	//숨김
			pFin->ShowWindow(SW_HIDE); //숨김
			break;
		case 4:
			pRot->ShowWindow(SW_HIDE);	//숨김
			pPara->ShowWindow(SW_HIDE);	//숨김
			pImg->ShowWindow(SW_HIDE);	//숨김
			pCCTV->ShowWindow(SW_HIDE);	//숨김
			pCir->ShowWindow(SW_SHOW);	//표시
			pLine->ShowWindow(SW_HIDE);	//숨김
			pSiRo->ShowWindow(SW_HIDE);	//숨김
			pFFT->ShowWindow(SW_HIDE);	//숨김
			pFin->ShowWindow(SW_HIDE); //숨김
			break;
		case 5:
			pRot->ShowWindow(SW_HIDE);	//숨김
			pPara->ShowWindow(SW_HIDE);	//숨김
			pImg->ShowWindow(SW_HIDE);	//숨김
			pCCTV->ShowWindow(SW_HIDE);	//숨김
			pCir->ShowWindow(SW_HIDE);	//숨김
			pLine->ShowWindow(SW_SHOW);	//표시
			pSiRo->ShowWindow(SW_HIDE);	//숨김
			pFFT->ShowWindow(SW_HIDE);	//숨김
			pFin->ShowWindow(SW_HIDE); //숨김
			break;
		case 6:
			pRot->ShowWindow(SW_HIDE);	//숨김
			pPara->ShowWindow(SW_HIDE);	//숨김
			pImg->ShowWindow(SW_HIDE);	//숨김
			pCCTV->ShowWindow(SW_HIDE);	//숨김
			pCir->ShowWindow(SW_HIDE);	//숨김
			pLine->ShowWindow(SW_HIDE);	//숨김
			pSiRo->ShowWindow(SW_SHOW);	//표시
			pFFT->ShowWindow(SW_HIDE);	//숨김
			pFin->ShowWindow(SW_HIDE); //숨김
			break;
		case 7:
			pRot->ShowWindow(SW_HIDE);	//숨김
			pPara->ShowWindow(SW_HIDE);	//숨김
			pImg->ShowWindow(SW_HIDE);	//숨김
			pCCTV->ShowWindow(SW_HIDE);	//숨김
			pCir->ShowWindow(SW_HIDE);	//숨김
			pLine->ShowWindow(SW_HIDE);	//숨김
			pSiRo->ShowWindow(SW_HIDE);	//숨김
			pFFT->ShowWindow(SW_SHOW);	//표시
			pFin->ShowWindow(SW_HIDE); //숨김
			break;
		case 8:
			pRot->ShowWindow(SW_HIDE);	//숨김
			pPara->ShowWindow(SW_HIDE);	//숨김
			pImg->ShowWindow(SW_HIDE);	//숨김
			pCCTV->ShowWindow(SW_HIDE);	//숨김
			pCir->ShowWindow(SW_HIDE);	//숨김
			pLine->ShowWindow(SW_HIDE);	//숨김
			pSiRo->ShowWindow(SW_HIDE);	//숨김
			pFFT->ShowWindow(SW_HIDE);	//숨김
			pFin->ShowWindow(SW_SHOW); //표시
			break;

		}
	}
	*pResult = 0;
}


void CHwDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	m_tabTest.Detach();
}
