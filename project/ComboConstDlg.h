#pragma once
#include "afxdialogex.h"


// CComboConstDlg 대화 상자

class CComboConstDlg : public CDialog
{
	DECLARE_DYNAMIC(CComboConstDlg)

public:
	CComboConstDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	CComboConstDlg(CWnd* pParent, CString title, CString textCombo, CString textValue, double minVal, double maxVal, CStringArray& items);   // 추가 생성자
	virtual ~CComboConstDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_COMBO_CONST };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
private:
	// 콤보박스 설명 텍스트 컨트롤
	CStatic m_textComboCtl;
	// 입력값 설명 텍스트 컨트롤
	CStatic m_textValueCtl;
	// 콤보박스 컨트롤
	CComboBox m_comboCtl;

	// 다이얼로그 제목
	CString m_title = _T("제목");
	// 콤보박스 설명 텍스트
	CString m_textCombo = _T("콤보박스");
	// 입력값 설명 텍스트
	CString m_textValue = _T("입력값");
	// 최소값
	double m_minVal = -255;
	// 최대값
	double m_maxVal = 255;
	// 콤보박스 항목
	CStringArray m_items;

public:
	// 콤보박스 선택값
	int selection = -1;
	// 입력값
	double value = 0;
	virtual BOOL OnInitDialog();
	afx_msg void OnSelectChange();
};
