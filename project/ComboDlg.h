#pragma once
#include "afxdialogex.h"


// CComboDlg 대화 상자

class CComboDlg : public CDialog
{
	DECLARE_DYNAMIC(CComboDlg)

public:
	CComboDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	CComboDlg(CWnd* pParent, CString title, CString textCombo, CStringArray& items);   // 추가 생성자
	virtual ~CComboDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_COMBO };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
private:
	// 콤보박스 설명 텍스트 컨트롤
	CStatic m_textComboCtl;
	// 콤보박스 컨트롤
	CComboBox m_comboCtl;

	// 다이얼로그 제목
	CString m_title = _T("제목");
	// 콤보박스 설명 텍스트
	CString m_textCombo = _T("콤보박스");
	// 콤보박스 항목
	CStringArray m_items;

public:
	int selection = -1;
	virtual BOOL OnInitDialog();
	afx_msg void OnSelectChange();
};
