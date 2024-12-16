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
	bool ctrl = false;
	bool lshift = false;
	bool draging = false;
	CPoint mouseNow;
	CPoint lastMousePos;
	CPoint fieldOffset = CPoint(0, 0);
	int mode_app = 1;
	bool drawInProgress = false;
	bool dragInProgress = false;
	int draghigher = -1;
	int draglineer = -1;
	Lineer lineInProgress;
	Higher highInProgress;

	CGraphEditorTApp* app;

	///TOOLBAR///
	bool m_ButtonMoveEnabled = false;
	bool m_ButtonHigherEnabled = true;
	bool m_ButtonLineerEnabled = true;
	bool m_ButtonClearEnabled = true;
	bool m_ButtonTextEnabled = true;

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


	void IndexingOjects();

	int findHigher(CPoint point);
	int findLineer(CPoint point);
	afx_msg void OnButtonfirst();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnButtonhigher();
	afx_msg void OnButtonlineer();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnButtonclear();
	afx_msg void OnUpdateButtonclear(CCmdUI* pCmdUI);
	afx_msg void OnUpdateButtonfirst(CCmdUI* pCmdUI);
	afx_msg void OnUpdateButtonhigher(CCmdUI* pCmdUI);
	afx_msg void OnUpdateButtonlineer(CCmdUI* pCmdUI);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnButtonText();
	afx_msg void OnUpdateButtonText(CCmdUI* pCmdUI);
};

#ifndef _DEBUG  // версия отладки в GraphEditorTView.cpp
inline CGraphEditorTDoc* CGraphEditorTView::GetDocument() const
   { return reinterpret_cast<CGraphEditorTDoc*>(m_pDocument); }
#endif

