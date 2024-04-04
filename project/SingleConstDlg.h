#pragma once
#include "afxdialogex.h"


// CSingleConstDlg 대화 상자

class CSingleConstDlg : public CDialog
{
	DECLARE_DYNAMIC(CSingleConstDlg)

public:
	CSingleConstDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	CSingleConstDlg(CWnd* pParent, CString title, CString textDesc, double minVal, double maxVal);   // 추가 생성자
	virtual ~CSingleConstDlg();

	virtual BOOL OnInitDialog();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SINGLE_CONST };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
private:
	// 다이얼로그 설명 텍스트 컨트롤
	CStatic m_textDescCtl;

	// 제목
	CString m_title = _T("제목");
	// 설명
	CString m_textDesc = _T("설명");
	// 최소값
	double m_minVal = -255;
	// 최대값
	double m_maxVal = 255;
public:
	// 입력값
	double value;
};
