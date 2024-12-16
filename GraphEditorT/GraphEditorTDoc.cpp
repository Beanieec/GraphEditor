
// GraphEditorTDoc.cpp: реализация класса CGraphEditorTDoc 
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS можно определить в обработчиках фильтров просмотра реализации проекта ATL, эскизов
// и поиска; позволяет совместно использовать код документа в данным проекте.
#ifndef SHARED_HANDLERS
#include "GraphEditorT.h"
#endif
#include <nlohmann/json.hpp>
#include "GraphEditorTDoc.h"

#include <propkey.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CGraphEditorTDoc

IMPLEMENT_DYNCREATE(CGraphEditorTDoc, CDocument)

BEGIN_MESSAGE_MAP(CGraphEditorTDoc, CDocument)
END_MESSAGE_MAP()


// Создание или уничтожение CGraphEditorTDoc

CGraphEditorTDoc::CGraphEditorTDoc() noexcept
{
	// TODO: добавьте код для одноразового вызова конструктора

}

CGraphEditorTDoc::~CGraphEditorTDoc()
{
}

BOOL CGraphEditorTDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: добавьте код повторной инициализации
	// (Документы SDI будут повторно использовать этот документ)

	return TRUE;
}

std::string CStringToString(const CString& cstr) {
	int utf8Length = WideCharToMultiByte(CP_UTF8, 0, cstr, -1, nullptr, 0, nullptr, nullptr);
	if (utf8Length <= 0) return "";

	std::string utf8String(utf8Length - 1, '\0');
	WideCharToMultiByte(CP_UTF8, 0, cstr, -1, &utf8String[0], utf8Length, nullptr, nullptr);

	return utf8String;
}

CString StringToCString(const std::string& str) {
	int utf16Length = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, nullptr, 0);
	if (utf16Length <= 0) return CString();

	CString cstr;
	MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, cstr.GetBuffer(utf16Length), utf16Length);
	cstr.ReleaseBuffer();

	return cstr;
}

void to_json(nlohmann::json& j, const Lineer& l) {
	j = nlohmann::json{
		{"x0", l.x0}, {"y0", l.y0},
		{"x1", l.x1}, {"y1", l.y1},
		{"side", l.side},
		{"number", l.number},
		{"name", CStringToString(l.name)},
		{"high0", l.high0}, {"high1", l.high1}
	};
}

void from_json(const nlohmann::json& j, Lineer& l) {
	j.at("x0").get_to(l.x0);
	j.at("y0").get_to(l.y0);
	j.at("x1").get_to(l.x1);
	j.at("y1").get_to(l.y1);
	j.at("side").get_to(l.side);
	j.at("number").get_to(l.number);
	l.name = StringToCString(j.at("name").get<std::string>());
	j.at("high0").get_to(l.high0);
	j.at("high1").get_to(l.high1);
}


void to_json(nlohmann::json& j, const Higher& h) {
	j = nlohmann::json{
		{"x", h.x}, {"y", h.y},
		{"size", h.size},
		{"number", h.number},
		{"name", CStringToString(h.name)}
	};
}

void from_json(const nlohmann::json& j, Higher& h) {
	j.at("x").get_to(h.x);
	j.at("y").get_to(h.y);
	j.at("size").get_to(h.size);
	j.at("number").get_to(h.number);
	h.name = StringToCString(j.at("name").get<std::string>());
}


// Сериализация CGraphEditorTDoc

void CGraphEditorTDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring()) {

		std::vector<Higher> vertices(m_higher.GetSize());
		for (int i = 0; i < m_higher.GetSize(); ++i) {
			vertices[i] = m_higher[i];
		}

		std::vector<Lineer> edges(m_lineer.GetSize());
		for (int i = 0; i < m_lineer.GetSize(); ++i) {
			edges[i] = m_lineer[i];
		}


		nlohmann::json j;
		j["Higher"] = vertices;
		j["Lineer"] = edges;


		std::string jsonString = j.dump();
		ar.Write(jsonString.c_str(), jsonString.size());
	}
	else {

		ULONGLONG fileSize = ar.GetFile()->GetLength();
		char* buffer = new char[static_cast<size_t>(fileSize) + 1];
		ar.Read(buffer, static_cast<UINT>(fileSize));
		buffer[fileSize] = '\0';


		nlohmann::json j = nlohmann::json::parse(buffer);
		delete[] buffer;

		std::vector<Higher> vertices = j["Higher"].get<std::vector<Higher>>();
		std::vector<Lineer> edges = j["Lineer"].get<std::vector<Lineer>>();

		m_higher.RemoveAll();
		m_higher.SetSize(static_cast<INT_PTR>(vertices.size()));
		for (size_t i = 0; i < vertices.size(); ++i) {
			m_higher[i] = vertices[i];
		}

		m_lineer.RemoveAll();
		m_lineer.SetSize(static_cast<INT_PTR>(edges.size()));
		for (size_t i = 0; i < edges.size(); ++i) {
			m_lineer[i] = edges[i];
		}
	}
}

#ifdef SHARED_HANDLERS

// Поддержка для эскизов
void CGraphEditorTDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Измените этот код для отображения данных документа
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Поддержка обработчиков поиска
void CGraphEditorTDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Задание содержимого поиска из данных документа.
	// Части содержимого должны разделяться точкой с запятой ";"

	// Например:  strSearchContent = _T("точка;прямоугольник;круг;объект ole;");
	SetSearchContent(strSearchContent);
}

void CGraphEditorTDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// Диагностика CGraphEditorTDoc

#ifdef _DEBUG
void CGraphEditorTDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CGraphEditorTDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// Команды CGraphEditorTDoc


void CGraphEditorTDoc::DeleteContents()
{
	m_higher.RemoveAll();
	m_lineer.RemoveAll();

	CDocument::DeleteContents();
}
