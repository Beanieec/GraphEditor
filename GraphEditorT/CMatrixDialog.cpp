#include "pch.h"
#include "CMatrixDialog.h"
#include "GraphEditorT.h"


void CMatrixDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CUSTOM_MATRIX, matrixControl);
}


BEGIN_MESSAGE_MAP(CMatrixDialog, CDialogEx)
	ON_WM_CLOSE()
END_MESSAGE_MAP()

BOOL CMatrixDialog::CreateModeless(CWnd* pParent, CGraphEditorTApp* pApp)
{
	pGraphApp = pApp;
	return CDialogEx::Create(IDD_MATRIX_DIALOG, pParent);
}
void CMatrixDialog::PostNcDestroy()
{
	pGraphApp->matrixExist = false;
	pGraphApp->matrixDlg = nullptr;
	delete this;  // Удаляем объект при закрытии
}
void CMatrixDialog::OnClose()
{
	DestroyWindow(); // Уничтожает окно и вызывает PostNcDestroy()
}