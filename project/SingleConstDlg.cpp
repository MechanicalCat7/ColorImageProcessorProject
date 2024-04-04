// SingleConstDlg.cpp: 구현 파일
//

#include "pch.h"
#include "Color Image Processor.h"
#include "afxdialogex.h"
#include "SingleConstDlg.h"


// CSingleConstDlg 대화 상자

IMPLEMENT_DYNAMIC(CSingleConstDlg, CDialog)

CSingleConstDlg::CSingleConstDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_SINGLE_CONST, pParent)
	, value(0)
{

}

CSingleConstDlg::CSingleConstDlg(CWnd* pParent, CString title, CString textDesc, double minVal, double maxVal)
	: CDialog(IDD_SINGLE_CONST, pParent)
	, value(0)
{
	m_title = title;
	m_textDesc = textDesc;
	m_minVal = minVal;
	m_maxVal = maxVal;
}

CSingleConstDlg::~CSingleConstDlg()
{
}

void CSingleConstDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TEXT_DESC, m_textDescCtl);
	DDX_Text(pDX, IDC_EDIT, value);
	DDV_MinMaxDouble(pDX, value, m_minVal, m_maxVal);
}


BEGIN_MESSAGE_MAP(CSingleConstDlg, CDialog)
END_MESSAGE_MAP()


// CSingleConstDlg 메시지 처리기


BOOL CSingleConstDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetWindowTextW(m_title);
	m_textDescCtl.SetWindowTextW(m_textDesc);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
