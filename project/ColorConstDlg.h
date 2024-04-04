#pragma once
#include "afxdialogex.h"


// CColorConstDlg 대화 상자

class CColorConstDlg : public CDialog
{
	DECLARE_DYNAMIC(CColorConstDlg)

public:
	CColorConstDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	CColorConstDlg(CWnd* pParent, CString title, CString textColor, CString textValue, double minVal, double maxVal);   // 추가 생성자
	virtual ~CColorConstDlg();
	virtual BOOL OnInitDialog();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_COLOR_CONST };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
private:
	// 색상 선택 설명 텍스트 컨트롤
	CStatic m_textColorCtl;
	// 입력값 설명 텍스트 컨트롤
	CStatic m_textValueCtl;

	// 다이얼로그 제목
	CString m_title = _T("제목");
	// 색상 선택 설명 텍스트
	CString m_textColor = _T("색상");
	// 입력값 설명 텍스트
	CString m_textValue = _T("입력값");
	// 최소값
	double m_minVal = -255;
	// 최대값
	double m_maxVal = 255;
public:
	// 색상 선택 컨트롤
	CMFCColorButton m_color;
	// 입력값
	double value;
};
