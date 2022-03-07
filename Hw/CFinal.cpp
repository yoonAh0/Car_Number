// CFinal.cpp: 구현 파일
//

#include "stdafx.h"
#include "Hw.h"
#include "CFinal.h"
#include "afxdialogex.h"
#include "f_preprocess.h"
#include "f_candidate.h"
#include "f_draw_rota.h"
#include "f_SVM.h"
#include "f_classify_objects.h"

BOOL m_bThread;

// CFinal 대화 상자

IMPLEMENT_DYNAMIC(CFinal, CDialogEx)

CFinal::CFinal(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FINAL, pParent)
	, m_editResult(_T(""))
{
	m_K1 = 15;
	m_K2 = 15;
	plate_no = -1;
	m_bThread = FALSE;
}

CFinal::~CFinal()
{
}

void CFinal::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_VIDEO, m_View1);
	DDX_Control(pDX, IDC_STATIC1, m_View2);
	DDX_Control(pDX, IDC_STATIC2, m_View3);
	DDX_Text(pDX, IDC_EDIT1, m_editResult);
}


BEGIN_MESSAGE_MAP(CFinal, CDialogEx)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON_PLAY, &CFinal::OnBnClickedButtonPlay)
END_MESSAGE_MAP()

//함수
void CFinal::ThreadFunc()
{
	while (true)
	{
		if (!m_bThread) break;
		m_Capture >> m_image;  // 영상 획득

		//원본 영상
		if (m_image.data != NULL)
		{
			CRect rect;
			CDC* pDC;
			pDC = m_View1.GetDC();
			m_View1.GetClientRect(rect);
			DisplayImage(pDC, rect, m_image);  //원본 영상 picturecontrol에 띄우기
			ReleaseDC(pDC);
		}

		//svm
		Mat morph = preprocessing(m_image);		//전처리
		vector<RotatedRect> candidates;
		find_candidates(morph, candidates);		//후보 영역 검출
		m_candidate_img = make_candidates(m_image, candidates);		//후보 영상 생성

		plate_no = classify_plates(m_svm, m_candidate_img);		//SVM 분류

		CString result;
		if (plate_no >= 0) {
			//knn
			vector <Rect> obejct_rects, sorted_rects;
			vector <Mat> numbers;							// 숫자 객체 

			preprocessing_plate(m_candidate_img[plate_no], m_plate_img);	// 번호판 영상 전처리
			cvtColor(m_plate_img, m_color_plate, COLOR_GRAY2BGR);

			find_objects(~m_plate_img, obejct_rects);		// 숫자 및 문자 검출  

			sort_rects(obejct_rects, sorted_rects);			// 검출객체 정렬(x 좌표기준)

			for (size_t i = 0; i < sorted_rects.size(); i++)
			{
				numbers.push_back(m_plate_img(sorted_rects[i]));	// 정렬된 숫자 영상
				rectangle(m_color_plate, sorted_rects[i], Scalar(0, 0, 255), 1); // 사각형 그리기
			}

			if (numbers.size() == 7) {
				result = classify_numbers(numbers, m_knn, m_K1, m_K2);		// kNN 분류 수행
				SetDlgItemTextW(IDC_EDIT1, result);
			}
			else {
				result = _T("숫자(문자) 객체를 정확히 검출하지 못했습니다.");
				SetDlgItemTextW(IDC_EDIT1, result);
			}

			if (m_candidate_img[plate_no].data != NULL)
			{
				CRect rect;
				CDC* pDC;
				pDC = m_View2.GetDC();
				m_View2.GetClientRect(rect);
				DisplayImage(pDC, rect, m_candidate_img[plate_no]);  //차량 영상 번호 영역 picturecontrol에 띄우기
				ReleaseDC(pDC);
			}
			if (m_color_plate.data != NULL)
			{
				CRect rect;
				CDC* pDC;
				pDC = m_View3.GetDC();
				m_View3.GetClientRect(rect);
				DisplayImage(pDC, rect, m_color_plate);  //번호판 영상 picturecontrol에 띄우기
				ReleaseDC(pDC);
			}
		}
		else {
			result = _T("번호판을 검출하지 못하였습니다. ");
			SetDlgItemTextW(IDC_EDIT1, result);
		}
		Sleep(10);
	}
}

void CFinal::FillBitmapInfo(BITMAPINFO* bmi, int width, int height, int bpp, int origin)
{
	assert(bmi&&width >= 0 && height >= 0 && (bpp == 8 || bpp == 24 || bpp == 32));
	BITMAPINFOHEADER *bmih = &(bmi->bmiHeader);
	memset(bmih, 0, sizeof(*bmih));
	bmih->biSize = sizeof(BITMAPINFOHEADER);
	bmih->biWidth = width;
	bmih->biHeight = origin ? abs(height) : -abs(height);
	bmih->biPlanes = 1;
	bmih->biBitCount = (unsigned short)bpp;
	bmih->biCompression = BI_RGB;
	if (bpp == 8) {
		RGBQUAD *palette = bmi->bmiColors;
		for (int i = 0; i < 256; i++) {
			palette[i].rgbBlue = palette[i].rgbGreen = palette[i].rgbRed = (BYTE)i;
			palette[i].rgbReserved = 0;
		}
	}
}

void CFinal::DisplayImage(CDC* pDC, CRect rect, Mat& srcimg)
{
	Mat img;
	int step = ((int)(rect.Width() / 4)) * 4; // 4byte 단위조정해야 영상이 기울어지지 않는다.

	resize(srcimg, img, Size(step, rect.Height()));

	uchar buffer[sizeof(BITMAPINFOHEADER) * 1024];
	BITMAPINFO* bmi = (BITMAPINFO*)buffer;

	int bmp_w = img.cols;
	int bmp_h = img.rows;
	int depth = img.depth();
	int channels = img.channels();
	int bpp = 8 * channels;

	FillBitmapInfo(bmi, bmp_w, bmp_h, bpp, 0);

	int from_x = MIN(0, bmp_w - 1);
	int from_y = MIN(0, bmp_h - 1);
	int sw = MAX(MIN(bmp_w - from_x, rect.Width()), 0);
	int sh = MAX(MIN(bmp_h - from_y, rect.Height()), 0);

	SetDIBitsToDevice(pDC->m_hDC, rect.left, rect.top, sw, sh, from_x, from_y, 0, sh, img.data + from_y * img.step, bmi, 0);

	img.release();
}


// CFinal 메시지 처리기

BOOL CFinal::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_Capture.open(1);   // 카메라 연결
	if (!m_Capture.isOpened()) AfxMessageBox(_T("카메라 없눈뎅?"));

	//knn
	m_knn[0] = kNN_train("D:/Users/USER/Downloads/images/train/train_numbers.png", m_K1, 10, 20);
	m_knn[1] = kNN_train("D:/Users/USER/Downloads/images/train/train_texts2.png", m_K2, 25, 20);

	// 	학습된 데이터 로드
	m_svm = ml::StatModel::load<ml::SVM>("D:/Users/USER/Downloads/images/train/SVMtrain.xml");


	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CFinal::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
					   // 그리기 메시지에 대해서는 CDialogEx::OnPaint()을(를) 호출하지 마십시오.
	
}


void CFinal::OnBnClickedButtonPlay()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_bThread = TRUE;
	m_pThread = new std::thread(&CFinal::ThreadFunc, this);
}
