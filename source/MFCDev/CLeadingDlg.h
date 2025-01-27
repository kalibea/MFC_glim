#pragma once
#include "afxdialogex.h"
#include<vector>

using namespace std;

class CTrailingDlg;

// CLeadingDlg 대화 상자

class CLeadingDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CLeadingDlg)

public:
	CLeadingDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CLeadingDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CLeadingDlg };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

private:
	vector<CPoint> m_clickedPoints;// 클릭된  원
	int            m_draggedPointIndex;// 드래그 중인 지점의 인덱스
	bool           m_isDragging;// 드래그 중인지 여부   

	int m_iclickRadius;          // 클릭 원 반지름
	int m_icircleBorderWidth; //정원의 두께

	// 백 버퍼
	CBitmap        m_bitmap;
	CSize          m_clientSize;

	void DrawToBuffer();
	void DrawCircle(HDC hdc, CPoint center, int radius);
	void DrawCircumcircle(HDC hdc);

	void UpdateDraggingState(CPoint point);
	bool IsPointInsideCircle(CPoint center, CPoint point, int radius);

public:
	CTrailingDlg* GeTrailingDlg();
	void SetClickRadius(int radius);
	void SetCircleBorderWidth(int borderWidth);
	void InitCircleAndCircumcircle();
	void RandomCircles();

public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};
