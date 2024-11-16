
// GraphEditorT.h: основной файл заголовка для приложения GraphEditorT
//
#pragma once

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

// Реализация
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CGraphEditorTApp theApp;
