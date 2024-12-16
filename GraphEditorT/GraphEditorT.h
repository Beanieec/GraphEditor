
// GraphEditorT.h: основной файл заголовка для приложения GraphEditorT
//
#pragma once
#include "CMatrixDialog.h"

#ifndef __AFXWIN_H__
	#error "включить pch.h до включения этого файла в PCH"
#endif

#include "resource.h"       // основные символы


// CGraphEditorTApp:
// Сведения о реализации этого класса: GraphEditorT.cpp
//

class CGraphEditorTApp : public CWinApp
{
public:
	CGraphEditorTApp() noexcept;


// Переопределение
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	CMatrixDialog* matrixDlg;
	bool matrixExist = false;
// Реализация
	afx_msg void OnAppAbout();
	afx_msg void OnViewMatrix();
	afx_msg void OnUpdateViewMatrix(CCmdUI* pCmdUI);
	DECLARE_MESSAGE_MAP()
	afx_msg void OnHelp();
};

extern CGraphEditorTApp theApp;
