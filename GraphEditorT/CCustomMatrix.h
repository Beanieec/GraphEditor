#pragma once
#include <afxwin.h>
#include "GraphEditorTDoc.h"

class CCustomMatrix : public CWnd
{
    DECLARE_DYNAMIC(CCustomMatrix)
public:
    CCustomMatrix();
    virtual ~CCustomMatrix() {}
    void OnDestroy();
    CDocument* GetDocument();
private:
    bool RegisterClass();
protected:
    afx_msg void OnPaint();
    DECLARE_MESSAGE_MAP()
};

