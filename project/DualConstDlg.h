#pragma once
#include "afxdialogex.h"


// CDualConstDlg 대화 상자

class CDualConstDlg : public CDialog
{
	DECLARE_DYNAMIC(CDualConstDlg)

public:
	CDualConstDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	CDualConstDlg(CWnd* pParent, CString title, CString textDesc, CString textValue1, CString textValue2, double minVal, double maxVal);   // 추가 생성자
	virtual ~CDualConstDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DUAL_CONST };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
private:
	// 다이얼로그 설명 텍스트 컨트롤
	CStatic m_textDescCtl;
	// 입력값 텍스트1 컨트롤
	CStatic m_textValue1Ctl;
	// 입력값 텍스트2 컨트롤
	CStatic m_textValue2Ctl;

	// 다이얼로그 제목
	CString m_title = _T("제목");
	// 다이얼로그 설명 텍스트
	CString m_textDesc = _T("설명");
	// 입력값 텍스트1
	CString m_textValue1 = _T("값1");
	// 입력값 텍스트2
	CString m_textValue2 = _T("값2");
	// 최소값
	double m_minVal = -255;
	// 최대값
	double m_maxVal = 255;

public:
	// 입력값1
	double value1;
	// 입력값2
	double value2;
	virtual BOOL OnInitDialog();
};
