// CTrailingDlg.cpp: 구현 파일
//

#include "pch.h"
#include "MFCDev.h"
#include "afxdialogex.h"
#include "CTrailingDlg.h"
#include "MFCDevDlg.h"
#include "CLeadingDlg.h"


// CTrailingDlg 대화 상자

IMPLEMENT_DYNAMIC(CTrailingDlg, CDialogEx)

CTrailingDlg::CTrailingDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CTrailingDlg, pParent)
	, m_clickRadius(10)
	, m_circleBorderWidth(2)
{

}

CTrailingDlg::~CTrailingDlg()
{
}

void CTrailingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT5, m_clickRadius);
	DDX_Text(pDX, IDC_EDIT4, m_circleBorderWidth);
	DDX_Text(pDX, IDC_EDIT1, m_ClickedPoint1);
	DDX_Text(pDX, IDC_EDIT2, m_ClickedPoint2);
	DDX_Text(pDX, IDC_EDIT3, m_ClickedPoint3);
}


BEGIN_MESSAGE_MAP(CTrailingDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CTrailingDlg::OnBorderBnClickedButton)
	ON_BN_CLICKED(IDC_BUTTON2, &CTrailingDlg::OnRadiusBnClickedButton)
END_MESSAGE_MAP()


// CTrailingDlg 메시지 처리기


BOOL CTrailingDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	ModifyStyle(WS_CAPTION, 0);
	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

// 클릭된 좌표 업데이트 함수
void CTrailingDlg::UpdateClickPoints(const vector<CPoint>& points) {
	for (size_t i = 0; i < points.size(); ++i) {
		CString str;
		str.Format(_T("X=%d, Y=%d"), points[i].x, points[i].y);
		if (i == 0) {
			m_ClickedPoint1 = str;
	    }
		else if (i == 1) {
			m_ClickedPoint2 = str;
		}
		else if (i == 2) {
			m_ClickedPoint3 = str;
		}

	}
	UpdateData(FALSE);
}

//반지름적용
void CTrailingDlg::OnBorderBnClickedButton()
{
	UpdateData(TRUE);

	 
	//// 메인 다이얼로그 가져오기
	CWnd* pMainWnd = AfxGetMainWnd();
	if (pMainWnd && pMainWnd->IsKindOf(RUNTIME_CLASS(CMFCDevDlg))) {
		CMFCDevDlg* pMainDialog = static_cast<CMFCDevDlg*>(pMainWnd);

	//	// CLeftTopDialog 가져오기
		CLeadingDlg* pcLeadingDlg = pMainDialog->getCLeadingDlg();
		if (pcLeadingDlg) {
			pcLeadingDlg->SetClickRadius(m_clickRadius);       // 클릭 원 반지름 설정
		}
	}

}

//두께적용
void CTrailingDlg::OnRadiusBnClickedButton()
{
	UpdateData(TRUE);
	//// 메인 다이얼로그 가져오기
	CWnd* pMainWnd = AfxGetMainWnd();
	if (pMainWnd && pMainWnd->IsKindOf(RUNTIME_CLASS(CMFCDevDlg))) {
		CMFCDevDlg* pMainDialog = static_cast<CMFCDevDlg*>(pMainWnd);

		//	// CLeftTopDialog 가져오기
		CLeadingDlg* pcLeadingDlg = pMainDialog->getCLeadingDlg();
		if (pcLeadingDlg) {
			pcLeadingDlg->SetCircleBorderWidth(m_circleBorderWidth); // 정원 테두리 두께 설정
		}
	}
}


void CTrailingDlg::InitUI()
{

	CString str;
	str.Format(_T("X=%d, Y=%d"), 0, 0);
	m_ClickedPoint1 = str;
	m_ClickedPoint2 = str;
	m_ClickedPoint3 = str;
	m_clickRadius = 10;
	m_circleBorderWidth = 2;
	UpdateData(FALSE);
}