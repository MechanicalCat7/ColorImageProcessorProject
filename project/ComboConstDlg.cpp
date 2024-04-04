// ComboConstDlg.cpp: 구현 파일
//

#include "pch.h"
#include "Color Image Processor.h"
#include "afxdialogex.h"
#include "ComboConstDlg.h"


// CComboConstDlg 대화 상자

IMPLEMENT_DYNAMIC(CComboConstDlg, CDialog)

CComboConstDlg::CComboConstDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_COMBO_CONST, pParent)
	, value(0)
{

}

CComboConstDlg::CComboConstDlg(CWnd* pParent, CString title, CString textCombo, CString textValue, double minVal, double maxVal, CStringArray& items)
	: CDialog(IDD_COMBO_CONST, pParent)
	, value(0)
{
	m_title = title;
	m_textCombo = textCombo;
	m_textValue = textValue;
	m_minVal = minVal;
	m_maxVal = maxVal;
	m_items.Copy(items);
}

CComboConstDlg::~CComboConstDlg()
{
}

void CComboConstDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TEXT_COMBO, m_textComboCtl);
	DDX_Control(pDX, IDC_TEXT_VALUE, m_textValueCtl);
	DDX_Control(pDX, IDC_COMBO, m_comboCtl);
	DDX_Text(pDX, IDC_EDIT, value);
	DDV_MinMaxDouble(pDX, value, m_minVal, m_maxVal);
}


BEGIN_MESSAGE_MAP(CComboConstDlg, CDialog)
	ON_CBN_SELCHANGE(IDC_COMBO, &CComboConstDlg::OnSelectChange)
END_MESSAGE_MAP()


// CComboConstDlg 메시지 처리기


BOOL CComboConstDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetWindowTextW(m_title);
	m_textComboCtl.SetWindowTextW(m_textCombo);
	m_textValueCtl.SetWindowTextW(m_textValue);

	for (int i = 0; i < m_items.GetCount(); ++i) {
		m_comboCtl.AddString(m_items[i]);
	}

	if (m_comboCtl.GetCount() > 0) {
		m_comboCtl.SetCurSel(0);
		selection = 0;
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CComboConstDlg::OnSelectChange()
{
	selection = m_comboCtl.GetCurSel();
}
