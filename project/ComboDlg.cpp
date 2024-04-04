// ComboDlg.cpp: 구현 파일
//

#include "pch.h"
#include "Color Image Processor.h"
#include "afxdialogex.h"
#include "ComboDlg.h"


// CComboDlg 대화 상자

IMPLEMENT_DYNAMIC(CComboDlg, CDialog)

CComboDlg::CComboDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_COMBO, pParent)
{

}

CComboDlg::CComboDlg(CWnd* pParent, CString title, CString textCombo, CStringArray& items)
	: CDialog(IDD_COMBO, pParent)
{
	m_title = title;
	m_textCombo = textCombo;
	m_items.Copy(items);
}

CComboDlg::~CComboDlg()
{
}

void CComboDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TEXT_COMBO, m_textComboCtl);
	DDX_Control(pDX, IDC_COMBO, m_comboCtl);
}


BEGIN_MESSAGE_MAP(CComboDlg, CDialog)
	ON_CBN_SELCHANGE(IDC_COMBO, &CComboDlg::OnSelectChange)
END_MESSAGE_MAP()


// CComboDlg 메시지 처리기


BOOL CComboDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetWindowTextW(m_title);
	m_textComboCtl.SetWindowTextW(m_textCombo);

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


void CComboDlg::OnSelectChange()
{
	selection = m_comboCtl.GetCurSel();
}
