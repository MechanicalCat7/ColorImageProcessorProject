// ColorConstDlg.cpp: 구현 파일
//

#include "pch.h"
#include "Color Image Processor.h"
#include "afxdialogex.h"
#include "ColorConstDlg.h"


// CColorConstDlg 대화 상자

IMPLEMENT_DYNAMIC(CColorConstDlg, CDialog)

CColorConstDlg::CColorConstDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_COLOR_CONST, pParent)
	, value(0)
{

}

CColorConstDlg::CColorConstDlg(CWnd* pParent, CString title, CString textColor, CString textValue, double minVal, double maxVal)
	: CDialog(IDD_COLOR_CONST, pParent)
	, value(0)
{
	m_title = title;
	m_textColor = textColor;
	m_textValue = textValue;
	m_minVal = minVal;
	m_maxVal = maxVal;
}

CColorConstDlg::~CColorConstDlg()
{
}

void CColorConstDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TEXT_COLOR, m_textColorCtl);
	DDX_Control(pDX, IDC_TEXT_VALUE, m_textValueCtl);
	DDX_Control(pDX, IDC_COLOR, m_color);
	DDX_Text(pDX, IDC_EDIT, value);
	DDV_MinMaxDouble(pDX, value, m_minVal, m_maxVal);
}


BEGIN_MESSAGE_MAP(CColorConstDlg, CDialog)
END_MESSAGE_MAP()


// CColorConstDlg 메시지 처리기


BOOL CColorConstDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetWindowTextW(m_title);
	m_textColorCtl.SetWindowTextW(m_textColor);
	m_textValueCtl.SetWindowTextW(m_textValue);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
