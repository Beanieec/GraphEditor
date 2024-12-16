
#pragma once
#include "resource.h"  
#include <afxdialogex.h>
#include "CCustomMatrix.h"

//#include "GraphEditorT.h"

class CGraphEditorTApp;

class CMatrixDialog : public CDialogEx
{
public:
	CMatrixDialog(CWnd* pParent = nullptr) {};   // ����������� �����������
	virtual ~CMatrixDialog() {};

	// ������ ����������� ����
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MATRIX_DIALOG};
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // ��������� DDX/DDV

	// ����������
protected:
	DECLARE_MESSAGE_MAP()
public:
	void OnClose();
	CGraphEditorTApp* pGraphApp;
	CCustomMatrix matrixControl;
	BOOL CreateModeless(CWnd* pParent = nullptr, CGraphEditorTApp* pApp = nullptr);
	void PostNcDestroy();
};

