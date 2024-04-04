// DualConstDlg.cpp: 구현 파일
//

#include "pch.h"
#include "Color Image Processor.h"
#include "afxdialogex.h"
#include "DualConstDlg.h"


// CDualConstDlg 대화 상자

IMPLEMENT_DYNAMIC(CDualConstDlg, CDialog)

CDualConstDlg::CDualConstDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DUAL_CONST, pParent)
	, value1(0)
	, value2(0)
{

}

CDualConstDlg::CDualConstDlg(CWnd* pParent, CString title, CString textDesc, CString textValue1, CString textValue2, double minVal, double maxVal)
	: CDialog(IDD_DUAL_CONST, pParent)
	, value1(0)
	, value2(0)
{
	m_title = title;
	m_textDesc = textDesc;
	m_textValue1 = textValue1;
	m_textValue2 = textValue2;
	m_minVal = minVal;
	m_maxVal = maxVal;
}

CDualConstDlg::~CDualConstDlg()
{
}

void CDualConstDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TEXT_DESC, m_textDescCtl);
	DDX_Control(pDX, IDC_TEXT_VALUE1, m_textValue1Ctl);
	DDX_Control(pDX, IDC_TEXT_VALUE2, m_textValue2Ctl);
	DDX_Text(pDX, IDC_EDIT1, value1);
	DDV_MinMaxDouble(pDX, value1, m_minVal, m_maxVal);
	DDX_Text(pDX, IDC_EDIT2, value2);
	DDV_MinMaxDouble(pDX, value2, m_minVal, m_maxVal);
}


BEGIN_MESSAGE_MAP(CDualConstDlg, CDialog)
END_MESSAGE_MAP()


// CDualConstDlg 메시지 처리기


BOOL CDualConstDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetWindowTextW(m_title);
	m_textDescCtl.SetWindowTextW(m_textDesc);
	m_textValue1Ctl.SetWindowTextW(m_textValue1);
	m_textValue2Ctl.SetWindowTextW(m_textValue2);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}
