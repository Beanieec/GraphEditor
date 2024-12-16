
// GraphEditorTDoc.h: интерфейс класса CGraphEditorTDoc 
//


#pragma once
#include "Higher.h"
#include "Lineer.h"

class CGraphEditorTDoc : public CDocument
{
protected: // создать только из сериализации
	CGraphEditorTDoc() noexcept;
	DECLARE_DYNCREATE(CGraphEditorTDoc)

// Атрибуты
public:

// Операции
public:

// Переопределение
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Реализация
public:
	virtual ~CGraphEditorTDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Созданные функции схемы сообщений
protected:
	DECLARE_MESSAGE_MAP()

public:
	CArray<Higher> m_higher;
	CArray<Lineer> m_lineer;

#ifdef SHARED_HANDLERS
	// Вспомогательная функция, задающая содержимое поиска для обработчика поиска
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
	virtual void DeleteContents();
};
