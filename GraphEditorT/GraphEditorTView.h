#pragma once


class CGraphEditorTView : public CView
{
protected:
	CGraphEditorTView() noexcept;
	DECLARE_DYNCREATE(CGraphEditorTView)
public:
	CGraphEditorTDoc* GetDocument() const;
public:
	virtual ~CGraphEditorTView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);



private:
	int backscale = 20;
	int mode_app = 2;
	bool drawInProgress = false;
	bool dragInProgress = false;
	int draghigher = -1;
	Lineer lineInProgress;
	Higher highInProgress;
	///TOOLBAR///
	bool tb_move = false;
	bool tb_higher = true;
	bool tb_lineer = false;
	/////////////
public:
	virtual void OnDraw(CDC* pDC);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

	void DDraw(CDC& dc);
	void DrawField(CDC& dc);
	void DrawHighers(CDC& dc);
	void DrawLineers(CDC& dc);

	int findHigher(CPoint point);
	afx_msg void OnButtonfirst();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnButtonhigher();
	afx_msg void OnButtonlineer();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnButtonclear();
};

#ifndef _DEBUG  // версия отладки в GraphEditorTView.cpp
inline CGraphEditorTDoc* CGraphEditorTView::GetDocument() const
   { return reinterpret_cast<CGraphEditorTDoc*>(m_pDocument); }
#endif

