#include "pch.h"
#include <string>
#include <vector>
#include <algorithm>
#include "framework.h"
#ifndef SHARED_HANDLERS
#include "GraphEditorT.h"
#include "TextEditorDlg.h"
#endif


#include "GraphEditorTView.h"

#define PI 3.14

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


void CGraphEditorTView::DrawField(CDC& dc) {	
	CRect rect;
	GetClientRect(rect);
	CBrush* brush;
	CPen* pen;
	CPen LineBack, BoldLineBack, BoldCant;
	CBrush background;
	background.CreateSolidBrush(RGB(255, 255, 255));
	LineBack.CreatePen(PS_SOLID, 1, RGB(220, 223, 227));
	BoldLineBack.CreatePen(PS_SOLID, 2, RGB(220, 223, 227));
	brush = dc.SelectObject(&background);
	dc.Rectangle(rect.left, rect.top, rect.Width(), rect.Height());
	pen = dc.SelectObject(&LineBack);

	int TopLogical = rect.top - fieldOffset.y;
	int LeftLogical = rect.left - fieldOffset.x;
	int RightLogical = rect.right - fieldOffset.x;
	int BottomLogical = rect.bottom - fieldOffset.y;

	int startX = (LeftLogical / backscale) * backscale;
	int startY = (TopLogical / backscale) * backscale;

	for (int x = startX; x <= RightLogical; x += backscale)
	{
		CPoint lineStart(x + fieldOffset.x, rect.top);
		CPoint lineEnd(x + fieldOffset.x, rect.bottom);

		if ((x / backscale) % 5 == 0)
			pen = dc.SelectObject(&BoldLineBack);
		else
			pen = dc.SelectObject(&LineBack);

		dc.MoveTo(lineStart);
		dc.LineTo(lineEnd);
	}
	for (int y = startY; y <= BottomLogical; y += backscale)
	{
		CPoint lineStart(rect.left, y + fieldOffset.y);
		CPoint lineEnd(rect.right, y + fieldOffset.y);

		if ((y / backscale) % 5 == 0)
			pen = dc.SelectObject(&BoldLineBack);
		else
			pen = dc.SelectObject(&LineBack);

		dc.MoveTo(lineStart);
		dc.LineTo(lineEnd);
	}
	
	///////////////////////
	CString debugInfo;
	debugInfo.Format(_T("Смещение поля: (%d, %d)"), fieldOffset.x, fieldOffset.y);
	dc.TextOut(5, rect.bottom - 20, debugInfo);
	//////////////////////

	dc.SelectObject(brush);
	dc.SelectObject(pen);
	LineBack.DeleteObject();
	background.DeleteObject();
	BoldLineBack.DeleteObject();
}

void CGraphEditorTView::DrawHighers(CDC& dc) {
	auto pDoc = GetDocument();
	CRect rect;
	CPen* pen;
	CPen linepen;
	linepen.CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
	GetClientRect(rect);
	pen = dc.SelectObject(&linepen);
	if (dragInProgress) {
		dc.Ellipse(highInProgress.x - highInProgress.size + fieldOffset.x, highInProgress.y - highInProgress.size + fieldOffset.y, highInProgress.x + highInProgress.size + fieldOffset.x, highInProgress.y + highInProgress.size + fieldOffset.y);
	}
	for (int i = 0; i < pDoc->m_higher.GetCount(); i++) {
		auto h = pDoc->m_higher.GetAt(i);

		std::wstring strNumber = std::to_wstring(h.number);
		dc.Ellipse(h.x - h.size + fieldOffset.x, h.y - h.size + fieldOffset.y, h.x + h.size + fieldOffset.x, h.y + h.size + fieldOffset.y);

		CSize textSize = dc.GetTextExtent(h.name);
		dc.TextOutW(h.x + fieldOffset.x - textSize.cx / 2, h.y - h.size - 17 + fieldOffset.y, h.name);

		if (strNumber.size() > 1)
			dc.TextOut(h.x - 9 + fieldOffset.x, h.y - 8 + fieldOffset.y, strNumber.c_str(), strNumber.length());
		else
			dc.TextOut(h.x - 4 + fieldOffset.x, h.y - 8 + fieldOffset.y, strNumber.c_str(), strNumber.length());
	}
	dc.SelectObject(pen);
	linepen.DeleteObject();
}

void CGraphEditorTView::DrawLineers(CDC& dc) {
	auto pDoc = GetDocument();
	CPen* pen;
	CBrush* brush;
	CPen linepen;
	linepen.CreatePen(PS_SOLID, 2, RGB(0, 0, 0));

	pen = dc.SelectObject(&linepen);
	for (int i = 0; i < pDoc->m_lineer.GetCount(); i++) {
		auto l = pDoc->m_lineer.GetAt(i);
		if (l.x0 == l.x1 && l.y0 == l.y1) {
			brush = (CBrush*) dc.SelectStockObject(NULL_BRUSH);
			dc.Ellipse(l.x0 - 40 + fieldOffset.x, l.y0 - 40 + fieldOffset.y, l.x1 + fieldOffset.x, l.y1 + fieldOffset.y);
			dc.SelectObject(brush);
		}
		else {
			if (l.side) {
				double angle = atan2(l.y1 - l.y0, l.x1 - l.x0);

				double arrowSize = 20.0;
				double shiftLength = 20.0;

				double dx = l.x1 - l.x0;
				double dy = l.y1 - l.y0;
				double length = sqrt(dx * dx + dy * dy);
				dx /= length;
				dy /= length;

				double arrowBaseX = l.x1 - shiftLength * dx;
				double arrowBaseY = l.y1 - shiftLength * dy;

				double arrowAngle1 = angle + PI / 6;
				double arrowAngle2 = angle - PI / 6;

				CPoint arrowEnd1(
					arrowBaseX + fieldOffset.x - arrowSize * cos(arrowAngle1),
					arrowBaseY + fieldOffset.y - arrowSize * sin(arrowAngle1)
				);
				CPoint arrowEnd2(
					arrowBaseX + fieldOffset.x - arrowSize * cos(arrowAngle2),
					arrowBaseY + fieldOffset.y - arrowSize * sin(arrowAngle2)
				);

				dc.MoveTo(arrowBaseX + fieldOffset.x, arrowBaseY + fieldOffset.y);
				dc.LineTo(arrowEnd1.x, arrowEnd1.y);
				dc.MoveTo(arrowBaseX + fieldOffset.x, arrowBaseY + fieldOffset.y);
				dc.LineTo(arrowEnd2.x, arrowEnd2.y);
			}
			dc.MoveTo(l.x0 + fieldOffset.x, l.y0 + fieldOffset.y);
			dc.LineTo(l.x1 + fieldOffset.x, l.y1 + fieldOffset.y);

			int cX = (l.x0 + fieldOffset.x + l.x1 + fieldOffset.x) / 2;
			int cY = (l.y0 + fieldOffset.y + l.y1 + fieldOffset.y) / 2;
			dc.TextOutW(cX, cY, l.name);
		}
	}
	if (drawInProgress) {			
		dc.MoveTo(lineInProgress.x0 + fieldOffset.x, lineInProgress.y0 + fieldOffset.y);
		dc.LineTo(lineInProgress.x1 + fieldOffset.x, lineInProgress.y1 + fieldOffset.y);		
	}
	dc.SelectObject(pen);
	linepen.DeleteObject();
}

int CGraphEditorTView::findHigher(CPoint point) {
	auto pDoc = GetDocument();
	for (int i = 0; i < pDoc->m_higher.GetCount(); i++) {
		
		if (pow(point.x - pDoc->m_higher[i].x - fieldOffset.x, 2) + pow(point.y - pDoc->m_higher[i].y - fieldOffset.y, 2) <= pow(pDoc->m_higher[i].size, 2)) {
			return i;
		}
	}
	return -1;
}

int CGraphEditorTView::findLineer(CPoint point) {
	auto pDoc = GetDocument();
	const double edgeThickness = 20.0;

	for (int i = 0; i < pDoc->m_lineer.GetCount(); i++) {

		double start_x = pDoc->m_higher[pDoc->m_lineer[i].high0].x + fieldOffset.x;
		double start_y = pDoc->m_higher[pDoc->m_lineer[i].high0].y + fieldOffset.y;
		double end_x = pDoc->m_higher[pDoc->m_lineer[i].high1].x + fieldOffset.x;
		double end_y = pDoc->m_higher[pDoc->m_lineer[i].high1].y + fieldOffset.y;

		double lengthSquared = (end_x - start_x) * (end_x - start_x) + (end_y - start_y) * (end_y - start_y);

		double t = ((point.x - start_x) * (end_x - start_x) + (point.y - start_y) * (end_y - start_y)) / lengthSquared;
		double a = min(1.0, t);
		t = max(0.0, a);

		double closestX = start_x + t * (end_x - start_x);
		double closestY = start_y + t * (end_y - start_y);

		double distanceSquared = pow(point.x - closestX, 2) + pow(point.y - closestY, 2);

		if (distanceSquared <= pow(edgeThickness / 2.0, 2)) {
			return i;
		}
	}
	return -1;
}

void CGraphEditorTView::IndexingOjects() {
	auto pDoc = GetDocument();

	for (int i = 0; i < pDoc->m_lineer.GetCount(); i++) {
		pDoc->m_lineer[i].number = i;
	}

	for (int i = 0; i < pDoc->m_higher.GetCount(); i++) {
		pDoc->m_higher[i].number = i;
	}
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
	ON_UPDATE_COMMAND_UI(ID_BUTTONCLEAR, &CGraphEditorTView::OnUpdateButtonclear)
	ON_UPDATE_COMMAND_UI(ID_BUTTONFIRST, &CGraphEditorTView::OnUpdateButtonfirst)
	ON_UPDATE_COMMAND_UI(ID_BUTTONHIGHER, &CGraphEditorTView::OnUpdateButtonhigher)
	ON_UPDATE_COMMAND_UI(ID_BUTTONLINEER, &CGraphEditorTView::OnUpdateButtonlineer)
	ON_COMMAND(ID_BUTTONTEXT, &CGraphEditorTView::OnButtonText)
	ON_UPDATE_COMMAND_UI(ID_BUTTONTEXT, &CGraphEditorTView::OnUpdateButtonText)
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	
END_MESSAGE_MAP()

CGraphEditorTView::CGraphEditorTView() noexcept
{
	app = (CGraphEditorTApp *)AfxGetApp();	
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
	mouseNow = pt;
	return CView::OnMouseWheel(nFlags, zDelta, pt);
}

///////////////////////////////////TOOLBAR/////////////////////////////////////////////

void CGraphEditorTView::OnButtonfirst()
{
	mode_app = 1;

	m_ButtonMoveEnabled = false;
	m_ButtonHigherEnabled = true;
	m_ButtonLineerEnabled = true;
	m_ButtonClearEnabled = true;
	m_ButtonTextEnabled = true;

	GetDocument()->UpdateAllViews(nullptr);
}
void CGraphEditorTView::OnUpdateButtonfirst(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_ButtonMoveEnabled);
}

void CGraphEditorTView::OnButtonhigher()
{	

	mode_app = 2;

	m_ButtonMoveEnabled = true;
	m_ButtonHigherEnabled = false;
	m_ButtonLineerEnabled = true;
	m_ButtonClearEnabled = true;
	m_ButtonTextEnabled = true;

	GetDocument()->UpdateAllViews(nullptr);
}
void CGraphEditorTView::OnUpdateButtonhigher(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_ButtonHigherEnabled);
}

void CGraphEditorTView::OnButtonlineer()
{
	mode_app = 3;

	m_ButtonMoveEnabled = true;
	m_ButtonHigherEnabled = true;
	m_ButtonLineerEnabled = false;
	m_ButtonClearEnabled = true;
	m_ButtonTextEnabled = true;

	GetDocument()->UpdateAllViews(nullptr);
}
void CGraphEditorTView::OnUpdateButtonlineer(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_ButtonLineerEnabled);
}

void CGraphEditorTView::OnButtonclear()
{

	mode_app = 4;
	m_ButtonMoveEnabled = true;
	m_ButtonHigherEnabled = true;
	m_ButtonLineerEnabled = true;
	m_ButtonClearEnabled = false;
	m_ButtonTextEnabled = true;

	GetDocument()->UpdateAllViews(nullptr);
}
void CGraphEditorTView::OnUpdateButtonclear(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_ButtonClearEnabled);
}

void CGraphEditorTView::OnButtonText()
{
	mode_app = 5;
	m_ButtonMoveEnabled = true;
	m_ButtonHigherEnabled = true;
	m_ButtonLineerEnabled = true;
	m_ButtonClearEnabled = true;
	m_ButtonTextEnabled = false;

	GetDocument()->UpdateAllViews(nullptr);
}
void CGraphEditorTView::OnUpdateButtonText(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_ButtonTextEnabled);
}
///////////////////////////////////////////////////////////////////////////////////////

void CGraphEditorTView::OnLButtonDown(UINT nFlags, CPoint point)
{
	auto pDoc = GetDocument();
	if (lshift) {

		lastMousePos = point;
		draging = true;
	}else{
		switch (mode_app) {
		case 1:
		{
			if (!dragInProgress) {
				auto figure = findHigher(point);
				if (figure > -1) {
					draghigher = figure;
					dragInProgress = true;
				}
			}
			break;
		}
		case 2:
		{
			Higher h;
			h.x = point.x - fieldOffset.x;
			h.y = point.y - fieldOffset.y;
			//h.size = backscale;////////////////////
			h.size = 20;

			pDoc->m_higher.Add(h);

			IndexingOjects();
			if(app->matrixDlg)
				app->matrixDlg->matrixControl.Invalidate();
				
			InvalidateRect(NULL);
			pDoc->SetModifiedFlag();
			break;
		}
		case 3:
		{
			if (!drawInProgress) {
				auto figure = findHigher(point);
				if (figure > -1) {
					lineInProgress.x0 = pDoc->m_higher[figure].x;
					lineInProgress.y0 = pDoc->m_higher[figure].y;
					lineInProgress.high0 = figure;
					drawInProgress = true;
				}
			}
			break;
		}
		case 4:
		{
			auto figure = findHigher(point);
			if (figure > -1) {
				for (int i = pDoc->m_lineer.GetCount() - 1; i >= 0; --i) {

					if (pDoc->m_lineer[i].high0 == figure || pDoc->m_lineer[i].high1 == figure) {

						pDoc->m_lineer.RemoveAt(i);
					}
				}

				pDoc->m_higher.RemoveAt(figure);

				for (int i = 0; i < pDoc->m_lineer.GetCount(); i++) {
					if (pDoc->m_lineer[i].high0 > figure) {
						pDoc->m_lineer[i].high0--;
					}
					if (pDoc->m_lineer[i].high1 > figure) {
						pDoc->m_lineer[i].high1--;
					}
				}
				pDoc->SetModifiedFlag();
			}
			if (app->matrixDlg)
				app->matrixDlg->matrixControl.Invalidate();
			IndexingOjects();
			InvalidateRect(NULL);
			break;
		}
		case 5:
		{
			auto figure = findHigher(point);
			if (figure > -1) {
				TextEditorDlg dlg;
				dlg.etext = pDoc->m_higher[figure].name;
				if (dlg.DoModal() == IDOK) {
					pDoc->m_higher[figure].name = dlg.GetText();
					pDoc->SetModifiedFlag();
					Invalidate();
				}
			}
			auto liine = findLineer(point);
			if (liine > -1) {
				TextEditorDlg dlg;
				dlg.etext = pDoc->m_lineer[liine].name;
				if (dlg.DoModal() == IDOK) {
					pDoc->m_lineer[liine].name = dlg.GetText();
					pDoc->SetModifiedFlag();
					Invalidate(); 
				}
			}			
		}
		}
	}

	CView::OnLButtonDown(nFlags, point);
}

void CGraphEditorTView::OnMouseMove(UINT nFlags, CPoint point)
{
	auto pDoc = GetDocument();
	if (draging) {
		CPoint delta = point - lastMousePos;
		fieldOffset += delta;
		lastMousePos = point;

		Invalidate();
	}
	if (dragInProgress) {
		highInProgress.x = point.x - fieldOffset.x;
		highInProgress.y = point.y - fieldOffset.y;
		highInProgress.size = pDoc->m_higher[draghigher].size;
		highInProgress.number = pDoc->m_higher[draghigher].number;
		highInProgress.name = pDoc->m_higher[draghigher].name;

		for (int i = 0; i < pDoc->m_lineer.GetCount(); i++) {
			if (pDoc->m_lineer[i].high0 == draghigher) {
				pDoc->m_lineer[i].x0 = highInProgress.x;
				pDoc->m_lineer[i].y0 = highInProgress.y;
			}
			if (pDoc->m_lineer[i].high1 == draghigher) {
				pDoc->m_lineer[i].x1 = highInProgress.x;
				pDoc->m_lineer[i].y1 = highInProgress.y;
			}
		}

		Higher h = highInProgress;
		pDoc->m_higher.SetAt(draghigher, h);
		pDoc->SetModifiedFlag();
		InvalidateRect(NULL);
	}
	if (drawInProgress) {
		lineInProgress.x1 = point.x - fieldOffset.x;
		lineInProgress.y1 = point.y - fieldOffset.y;
		InvalidateRect(NULL);
	}

	CView::OnMouseMove(nFlags, point);
}

void CGraphEditorTView::OnLButtonUp(UINT nFlags, CPoint point)
{
	auto pDoc = GetDocument();
	if (draging) {
		lshift = false;
		draging = false;
	}
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
			lineInProgress.high1 = figure;
			if(ctrl)
				lineInProgress.side = false;
			else
				lineInProgress.side = true;
			
			pDoc->m_lineer.Add(lineInProgress);
			IndexingOjects();

			if (app->matrixDlg)
				app->matrixDlg->matrixControl.Invalidate();

			pDoc->SetModifiedFlag();
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

void CGraphEditorTView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == VK_SHIFT)
	{
		
		lshift = true;	
	}
	if (nChar == VK_CONTROL) {
		ctrl = true;
	}

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CGraphEditorTView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (nChar == VK_SHIFT)
	{
		lshift = false;
		draging = false;
	}
	if (nChar == VK_CONTROL) {
		ctrl = false;
	}

	CView::OnKeyUp(nChar, nRepCnt, nFlags);
}