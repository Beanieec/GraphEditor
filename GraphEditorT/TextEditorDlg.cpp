// TextEditorDlg.cpp: файл реализации
//

#include "pch.h"
#include "GraphEditorT.h"
#include "afxdialogex.h"
#include "TextEditorDlg.h"


// Диалоговое окно TextEditorDlg

IMPLEMENT_DYNAMIC(TextEditorDlg, CDialog)

TextEditorDlg::TextEditorDlg(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_TextEditor, pParent)
{

}


TextEditorDlg::~TextEditorDlg()
{
}

void TextEditorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDITTEXT, etext);
}


BEGIN_MESSAGE_MAP(TextEditorDlg, CDialog)
	ON_EN_CHANGE(IDC_EDITTEXT, &TextEditorDlg::OnEnChangeEdittext)
END_MESSAGE_MAP()

CString TextEditorDlg::GetText() {

	return etext;
}
// Обработчики сообщений TextEditorDlg


void TextEditorDlg::OnEnChangeEdittext()
{
	// TODO:  Если это элемент управления RICHEDIT, то элемент управления не будет
	// send this notification unless you override the CDialog::OnInitDialog()
	// функция и вызов CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Добавьте код элемента управления
}
