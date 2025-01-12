#pragma once
#include "afxdialogex.h"
#include <vector>

using namespace std;

// CTrailingDlg 대화 상자

class CTrailingDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CTrailingDlg)

public:
	CTrailingDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CTrailingDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CTrailingDlg };
#endif
private:



public:
	// 클릭 원 반지름 및 외접원 테두리 두께 값 반환
	int GetClickRadius() const { return m_clickRadius; }
	int GetCircleBorderWidth() const { return m_circleBorderWidth; }

	// 클릭된 좌표를 업데이트하는 함수
	void UpdateClickPoints(const vector<CPoint>& points);
	void InitUI();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	// 클릭 원 반지름 및 외접원 테두리 두께 값
	int m_clickRadius;
	int m_circleBorderWidth;
	CString m_ClickedPoint1;
	CString m_ClickedPoint2;
	CString m_ClickedPoint3;
	afx_msg void OnBorderBnClickedButton();
	afx_msg void OnRadiusBnClickedButton();
};
