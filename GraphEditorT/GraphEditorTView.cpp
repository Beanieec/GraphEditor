#include "pch.h"
#include <string>
#include "framework.h"
#ifndef SHARED_HANDLERS
#include "GraphEditorT.h"
#endif

#include "GraphEditorTDoc.h"
#include "GraphEditorTView.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


void CGraphEditorTView::DrawField(CDC& dc) {	
	CRect rect;
	GetClientRect(rect);
	CBrush* brush;
	CPen* pen;
	CPen LineBack, BoldLineBack;
	CBrush background;
	background.CreateSolidBrush(RGB(255, 255, 255));
	LineBack.CreatePen(PS_SOLID, 1, RGB(220, 223, 227));
	BoldLineBack.CreatePen(PS_SOLID, 2, RGB(220, 223, 227));
	brush = dc.SelectObject(&background);
	dc.Rectangle(rect.left + 2, rect.top + 2, rect.Width() - 1, rect.Height() - 1);
	pen = dc.SelectObject(&LineBack);

	for (int i = 0; i < max(rect.right, rect.bottom); i += backscale) {
		dc.MoveTo(rect.left + i, rect.top);
		dc.LineTo(rect.left + i, rect.bottom);
		dc.MoveTo(rect.left, rect.top + i);
		dc.LineTo(rect.right, rect.top + i);
	}
	dc.SelectObject(pen);
	LineBack.DeleteObject();
	pen = dc.SelectObject(&BoldLineBack);
	for (int i = 0; i < max(rect.right, rect.bottom); i += backscale * 5) {
		dc.MoveTo(rect.left + i, rect.top);
		dc.LineTo(rect.left + i, rect.bottom);
		dc.MoveTo(rect.left, rect.top + i);
		dc.LineTo(rect.right, rect.top + i);
	}
	dc.SelectObject(brush);
	dc.SelectObject(pen);
	background.DeleteObject();
	BoldLineBack.DeleteObject();
}

void CGraphEditorTView::DrawHighers(CDC& dc) {
	auto pDoc = GetDocument();
	CRect rect; 
	GetClientRect(rect);
	CString number;
	if (dragInProgress) {
		dc.Ellipse(highInProgress.x - highInProgress.size, highInProgress.y - highInProgress.size, highInProgress.x + highInProgress.size, highInProgress.y + highInProgress.size);
	}
	for (int i = 0; i < pDoc->m_higher.GetCount(); i++) {
		auto h = pDoc->m_higher.GetAt(i);
		h.number = i;
		std::wstring strNumber = std::to_wstring(h.number);
		dc.Ellipse(h.x - h.size, h.y - h.size, h.x + h.size, h.y + h.size);
		if (strNumber.size() > 1)
			dc.TextOut(h.x - 9, h.y - 8, strNumber.c_str(), strNumber.length());
		else
			dc.TextOut(h.x - 4, h.y - 8, strNumber.c_str(), strNumber.length());
	}
	
}

void CGraphEditorTView::DrawLineers(CDC& dc) {
	auto pDoc = GetDocument();
	CPen* pen;
	CPen linepen;
	linepen.CreatePen(PS_SOLID, 2, RGB(0, 0, 0));

	pen = dc.SelectObject(&linepen);
	for (int i = 0; i < pDoc->m_lineer.GetCount(); i++) {
		auto l = pDoc->m_lineer.GetAt(i);
		dc.MoveTo(l.x0, l.y0);
		dc.LineTo(l.x1, l.y1);
	}
	if (drawInProgress) {
		dc.MoveTo(lineInProgress.x0, lineInProgress.y0);
		dc.LineTo(lineInProgress.x1, lineInProgress.y1);
	}
	dc.SelectObject(pen);
	linepen.DeleteObject();
}

int CGraphEditorTView::findHigher(CPoint point) {
	auto pDoc = GetDocument();
	for (int i = 0; i < pDoc->m_higher.GetCount(); i++) {
		
		if (pow(point.x - pDoc->m_higher[i].x, 2) + pow(point.y - pDoc->m_higher[i].y, 2) <= pow(pDoc->m_higher[i].size, 2)) {
			return i;
		}
	}
	return -1;
}
// CGraphEditorTView

IMPLEMENT_DYNCREATE(CGraphEditorTView, CView)

BEGIN_MESSAGE_MAP(CGraphEditorTView, CView)
	ON_WM_MOUSEWHEEL()
	ON_COMMAND(ID_BUTTONFIRST, &CGraphEditorTView::OnButtonfirst)
	ON_WM_LBUTTONDOWN()
	ON_COMMAND(ID_BUTTONHIGHER, &CGraphEditorTView::OnButtonhigher)
	ON_COMMAND(ID_BUTTONLINEER, &CGraphEditorTView::OnButtonlineer)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_ERASEBKGND()
	ON_COMMAND(ID_BUTTONCLEAR, &CGraphEditorTView::OnButtonclear)
END_MESSAGE_MAP()

CGraphEditorTView::CGraphEditorTView() noexcept
{
}

CGraphEditorTView::~CGraphEditorTView()
{
}

BOOL CGraphEditorTView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: изменить класс Window или стили посредством изменения CREATESTRUCT cs
	return CView::PreCreateWindow(cs);
}

void CGraphEditorTView::DDraw(CDC& dc) {
	CRect rect;
	GetClientRect(rect);

	//////////////////////////buffer////////////////////////////
	CBitmap* pOldBitmap;
	CDC Mem;
	CBitmap membt;
	Mem.CreateCompatibleDC(&dc);
	membt.CreateCompatibleBitmap(&dc, rect.Width(), rect.Height());
	pOldBitmap = Mem.SelectObject(&membt);
	////////////////////////////////////////////////////////////

	DrawField(Mem);
	DrawLineers(Mem);
	DrawHighers(Mem);
	
	dc.BitBlt(rect.left, rect.top, rect.Width(), rect.Height(), &Mem, 0, 0, SRCCOPY);
	Mem.SelectObject(pOldBitmap);
	membt.DeleteObject();
}

void CGraphEditorTView::OnDraw(CDC* pDC)
{
	CGraphEditorTDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	DDraw(*pDC);
}

BOOL CGraphEditorTView::OnEraseBkgnd(CDC* pDC)
{
	DDraw(*pDC);
	return TRUE;
}

#ifdef _DEBUG
void CGraphEditorTView::AssertValid() const
{
	CView::AssertValid();
}

void CGraphEditorTView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CGraphEditorTDoc* CGraphEditorTView::GetDocument() const
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGraphEditorTDoc)));
	return (CGraphEditorTDoc*)m_pDocument;
}
#endif //_DEBUG

BOOL CGraphEditorTView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	if (zDelta > 0 && backscale < 50) {
		backscale++;
		Invalidate();
	}
	if (zDelta < 0 && backscale > 5) {
		backscale--;
		Invalidate();
	}

	return CView::OnMouseWheel(nFlags, zDelta, pt);
}

///////////////////////////////////TOOLBAR/////////////////////////////////////////////

void CGraphEditorTView::OnButtonfirst()
{
	mode_app = 1;
}

void CGraphEditorTView::OnButtonhigher()
{	
	mode_app = 2;
}


void CGraphEditorTView::OnButtonlineer()
{
	mode_app = 3;

}

void CGraphEditorTView::OnButtonclear()
{
	mode_app = 4;
}
///////////////////////////////////////////////////////////////////////////////////////

void CGraphEditorTView::OnLButtonDown(UINT nFlags, CPoint point)
{
	auto pDoc = GetDocument();
	/*CClientDC dc(this);*/
	switch (mode_app) {
	case 1:
	{
		if (!dragInProgress) {
			auto figure = findHigher(point);
			if (figure > -1) {
				draghigher = figure;
				/*highInProgress.x = pDoc->m_higher[figure].x;
				highInProgress.y = pDoc->m_higher[figure].y;*/
				dragInProgress = true;
			}
		}
		break;
	}
	case 2:
	{
		Higher h;
		h.x = point.x;
		h.y = point.y;
		h.size = 20;

		pDoc->m_higher.Add(h);
		InvalidateRect(NULL);
		break;
	}
	case 3:
	{
		if (!drawInProgress) {
			auto figure = findHigher(point);
			if (figure > -1) {
				lineInProgress.x0 = pDoc->m_higher[figure].x;
				lineInProgress.y0 = pDoc->m_higher[figure].y;
				drawInProgress = true;
			}

		}
		break;
	}
	case 4:
	{
		auto figure = findHigher(point);
		if (figure > -1) {
			pDoc->m_higher.RemoveAt(figure);
		}
		InvalidateRect(NULL);
		break;
	}
	}

	CView::OnLButtonDown(nFlags, point);
}

void CGraphEditorTView::OnMouseMove(UINT nFlags, CPoint point)
{
	auto pDoc = GetDocument();
	if (dragInProgress) {
		highInProgress.x = point.x;
		highInProgress.y = point.y;
		highInProgress.size = 20;

		Higher h = highInProgress;
		pDoc->m_higher.SetAt(draghigher, h);
		
		InvalidateRect(NULL);
	}

	if (drawInProgress) {
		lineInProgress.x1 = point.x;
		lineInProgress.y1 = point.y;
		InvalidateRect(NULL);
	}

	CView::OnMouseMove(nFlags, point);
}

void CGraphEditorTView::OnLButtonUp(UINT nFlags, CPoint point)
{
	auto pDoc = GetDocument();

	if (dragInProgress) {
		
		dragInProgress = false;
		draghigher = -1;
		InvalidateRect(NULL);
	}

	if (drawInProgress) {
		auto figure = findHigher(point);
		if (figure > -1) {
			lineInProgress.x1 = pDoc->m_higher[figure].x;
			lineInProgress.y1 = pDoc->m_higher[figure].y;
			pDoc->m_lineer.Add(lineInProgress);
			InvalidateRect(NULL);
			drawInProgress = false;
		}
		else {
			drawInProgress = false;
			InvalidateRect(NULL);
		}
	}	

	CView::OnLButtonUp(nFlags, point);
}




