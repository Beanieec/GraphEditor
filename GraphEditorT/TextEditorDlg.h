#pragma once
#include "afxdialogex.h"


// Диалоговое окно TextEditorDlg

class TextEditorDlg : public CDialog
{
	DECLARE_DYNAMIC(TextEditorDlg)

public:
	TextEditorDlg(CWnd* pParent = nullptr);   // стандартный конструктор
	virtual ~TextEditorDlg();
	CString etext = L" ";
	CString GetText();


// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TextEditor };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangeEdittext();
};
