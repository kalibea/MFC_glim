// CLeadingDlg.cpp: 구현 파일
//

#include "pch.h"
#include "MFCDev.h"
#include "afxdialogex.h"
#include "CLeadingDlg.h"
#include "MFCDevDlg.h"
#include "CTrailingDlg.h"
#include <random>
#include <thread>
#include <chrono>

using namespace std;

// CLeadingDlg 대화 상자

IMPLEMENT_DYNAMIC(CLeadingDlg, CDialogEx)

class CTrailingDlg;

CLeadingDlg::CLeadingDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CLeadingDlg, pParent),
    m_draggedPointIndex(-1),
    m_isDragging(false)
{

}

CLeadingDlg::~CLeadingDlg()
{
}

void CLeadingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CLeadingDlg, CDialogEx)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()


// CLeadingDlg 메시지 처리기


BOOL CLeadingDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	ModifyStyle(WS_CAPTION, 0);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}





void CLeadingDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
    CDialogEx::OnLButtonDown(nFlags, point);

    static CTrailingDlg* trailingDlg = GeTrailingDlg();

	if (m_clickedPoints.size() < 3) {
		// 클릭된 점 추가
		m_clickedPoints.push_back(point);
		HWND hwnd = AfxGetMainWnd()->m_hWnd;

		// 클릭 지점을 UI에 업데이트
		if (trailingDlg) {
			trailingDlg->UpdateClickPoints(m_clickedPoints);
		}
	}
	else {
		// 기존 점 중 하나를 선택해 드래그 상태로 전환
		UpdateDraggingState(point);
	}

	Invalidate(); // 화면 갱신
}


void CLeadingDlg::OnLButtonUp(UINT nFlags, CPoint point){

	m_isDragging = false;
	m_draggedPointIndex = -1;
}




void CLeadingDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialogEx::OnMouseMove(nFlags, point);

	static CTrailingDlg* trailingDlg = GeTrailingDlg();

	if (m_isDragging && m_draggedPointIndex != -1) {
		// 드래그 중인 점의 위치 업데이트
		m_clickedPoints[m_draggedPointIndex] = point;

		// 클릭 지점을 UI에 업데이트
		if (trailingDlg) {
			trailingDlg->UpdateClickPoints(m_clickedPoints);
		}

		Invalidate(); // 화면 갱신
	}
}

// 드래그 상태 업데이트
void CLeadingDlg::UpdateDraggingState(CPoint point) {

    static CTrailingDlg* trailingDlg = GeTrailingDlg();

    int radius = trailingDlg ? trailingDlg->GetClickRadius() : 20;

    for (size_t i = 0; i < m_clickedPoints.size(); ++i) {
        if (IsPointInsideCircle(m_clickedPoints[i], point, radius)) {
            m_draggedPointIndex = static_cast<int>(i);
            m_isDragging = true;
            break;
        }
    }
}

// 클릭한 점이 원 안에 있는지 확인
bool CLeadingDlg::IsPointInsideCircle(CPoint center, CPoint point, int radius) {
    int dx = center.x - point.x;
    int dy = center.y - point.y;
    return (dx * dx + dy * dy) <= (radius * radius);
}

// 화면 그리기
void CLeadingDlg::OnPaint() {
    CPaintDC dc(this);

    CRect rect;
    GetClientRect(&rect);

    // 백 버퍼 생성
    if (m_bitmap.GetSafeHandle() == nullptr || rect.Size() != m_clientSize) {
        m_clientSize = rect.Size();

        CClientDC clientDC(this);
        CDC memoryDC;
        memoryDC.CreateCompatibleDC(&clientDC);

        m_bitmap.DeleteObject();
        m_bitmap.CreateCompatibleBitmap(&clientDC, m_clientSize.cx, m_clientSize.cy);
        CBitmap* pOldBitmap = memoryDC.SelectObject(&m_bitmap);
        memoryDC.FillSolidRect(&rect, RGB(255, 255, 255)); // 배경 흰색

        memoryDC.SelectObject(pOldBitmap);
    }

    // 화면을 백 버퍼에 그리기
    DrawToBuffer();

    // 백 버퍼를 화면에 복사
    CDC memoryDC;
    memoryDC.CreateCompatibleDC(&dc);
    CBitmap* pOldBitmap = memoryDC.SelectObject(&m_bitmap);

    dc.BitBlt(0, 0, m_clientSize.cx, m_clientSize.cy, &memoryDC, 0, 0, SRCCOPY);

    memoryDC.SelectObject(pOldBitmap);
}

// 백 버퍼에 그림 그리기
void CLeadingDlg::DrawToBuffer() {
    CDC memoryDC;
    CClientDC clientDC(this);
    memoryDC.CreateCompatibleDC(&clientDC);

    CBitmap* pOldBitmap = memoryDC.SelectObject(&m_bitmap);
    memoryDC.FillSolidRect(0, 0, m_clientSize.cx, m_clientSize.cy, RGB(233, 233, 233));

    HDC hdc = memoryDC.GetSafeHdc();

    static CTrailingDlg* trailingDlg = GeTrailingDlg();
    // 클릭된 점을 원으로 그리기
    int radius = trailingDlg ? trailingDlg->GetClickRadius() : 10;
    for (const auto& point : m_clickedPoints) {
        DrawCircle(hdc, point, radius);
    }

    // 외접원 그리기
    if (m_clickedPoints.size() == 3) {
        DrawCircumcircle(hdc);
    }

    memoryDC.SelectObject(pOldBitmap);
}

// 원 그리기 함수
void CLeadingDlg::DrawCircle(HDC hdc, CPoint center, int radius) {
    HBRUSH hBrush = CreateSolidBrush(RGB(0, 0, 0)); // 검은색 브러시
    HBRUSH hOldBrush = (HBRUSH)SelectObject(hdc, hBrush);
    Ellipse(hdc, center.x - radius, center.y - radius, center.x + radius, center.y + radius);

    SelectObject(hdc, hOldBrush);
    DeleteObject(hBrush);
}

// 외접원 그리기
void CLeadingDlg::DrawCircumcircle(HDC hdc) {
    if (m_clickedPoints.size() != 3) return;

    static CTrailingDlg* trailingDlg = GeTrailingDlg();

    // 외접원의 중심과 반지름 계산
    const CPoint& p1 = m_clickedPoints[0];
    const CPoint& p2 = m_clickedPoints[1];
    const CPoint& p3 = m_clickedPoints[2];

    int d = 2 * ((p1.x * (p2.y - p3.y)) + (p2.x * (p3.y - p1.y)) + (p3.x * (p1.y - p2.y)));
    if (d == 0) return;

    int ux = ((p1.x * p1.x + p1.y * p1.y) * (p2.y - p3.y) +
        (p2.x * p2.x + p2.y * p2.y) * (p3.y - p1.y) +
        (p3.x * p3.x + p3.y * p3.y) * (p1.y - p2.y)) /
        d;
    int uy = ((p1.x * p1.x + p1.y * p1.y) * (p3.x - p2.x) +
        (p2.x * p2.x + p2.y * p2.y) * (p1.x - p3.x) +
        (p3.x * p3.x + p3.y * p3.y) * (p2.x - p1.x)) /
        d;

    int radius = static_cast<int>(sqrt((ux - p1.x) * (ux - p1.x) + (uy - p1.y) * (uy - p1.y)));

    int borderWidth = trailingDlg ? trailingDlg->GetCircleBorderWidth() : 2;
    HPEN hPen = CreatePen(PS_SOLID, borderWidth, RGB(0, 0, 255));
    HGDIOBJ oldPen = SelectObject(hdc, hPen);

    Arc(hdc, ux - radius, uy - radius, ux + radius, uy + radius, 0, 0, 0, 0);

    SelectObject(hdc, oldPen);
    DeleteObject(hPen);
}


CTrailingDlg* CLeadingDlg::GeTrailingDlg() {
	CWnd* pMainWnd = AfxGetMainWnd(); // 메인 다이얼로그 핸들 얻기
	if (pMainWnd && pMainWnd->IsKindOf(RUNTIME_CLASS(CMFCDevDlg))) {
		CMFCDevDlg* pMainDialog = static_cast<CMFCDevDlg*>(pMainWnd);
		return pMainDialog->getCTrailingDlg(); 
	}
	return nullptr;
}

void CLeadingDlg::SetClickRadius(int radius) {
    m_iclickRadius = radius;
    Invalidate();
}

void CLeadingDlg::SetCircleBorderWidth(int borderWidth) {
    m_icircleBorderWidth = borderWidth;
    Invalidate();
}

void CLeadingDlg::InitCircleAndCircumcircle() {

    m_clickedPoints.clear(); // 클릭된 점 초기화
    Invalidate(); 
}

void CLeadingDlg::RandomCircles() {

    thread([this]() {
        for (int i = 0; i < 10; ++i) {
            this_thread::sleep_for(chrono::milliseconds(500)); // 0.5초 대기,초당 2회
            for (auto& pt : m_clickedPoints) {
                pt.x = rand() % 500 + 50; // X 좌표 랜덤 이동
                pt.y = rand() % 500 + 50; // Y 좌표 랜덤 이동
            }
            Invalidate(); 
        }
        }).detach(); // 별도 스레드로 실행
}