#include "pch.h"
#include "CCustomMatrix.h"
#include <vector>
#include <string>



IMPLEMENT_DYNAMIC(CCustomMatrix, CWnd)
CCustomMatrix::CCustomMatrix() {
	this->RegisterClass();
}

void CCustomMatrix::OnDestroy()
{
	CWnd::OnDestroy();
	
}

CDocument* CCustomMatrix::GetDocument()
{

    CWnd* pParent = GetParent();
    if (!pParent)
        return nullptr;

    CView* pView = DYNAMIC_DOWNCAST(CView, pParent);
    if (pView)
        return pView->GetDocument();

    CFrameWnd* pFrame = GetParentFrame();
    if (pFrame)
    {
        pView = pFrame->GetActiveView();
        if (pView)
            return pView->GetDocument();
    }
    AfxMessageBox(L"Документ не найден");
    return nullptr;
}

bool CCustomMatrix::RegisterClass()
{
	WNDCLASS wndcls;
	HINSTANCE hInst = AfxGetInstanceHandle();

	if (!(::GetClassInfo(hInst, L"CCustomMatrix", &wndcls)))
	{
		wndcls.style = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;

		wndcls.lpfnWndProc = ::DefWindowProc;
		wndcls.cbClsExtra = wndcls.cbWndExtra = 0;
		wndcls.hInstance = hInst;
		wndcls.hIcon = NULL;
		wndcls.hCursor = AfxGetApp()->LoadStandardCursor(IDC_ARROW);
		wndcls.hbrBackground = (HBRUSH)(COLOR_3DFACE + 1);
		wndcls.lpszMenuName = NULL;
		wndcls.lpszClassName = L"CCustomMatrix";

		if (!AfxRegisterClass(&wndcls))
		{
			AfxThrowResourceException();
			return FALSE;
		}
	}
	return TRUE;
}

void CCustomMatrix::OnPaint()
{
    CPaintDC dc(this);
    CDocument* pDoc = GetDocument();
    CGraphEditorTDoc* pMyDoc = static_cast<CGraphEditorTDoc*>(pDoc);  
    CRect rect;
    GetClientRect(&rect);
    CBrush* brush;
    CPen* pen;

    int vertexRadius = 20;
    int matrixSize = pMyDoc->m_higher.GetSize();

    std::vector<std::vector<int>> adjMatrix(matrixSize, std::vector<int>(matrixSize, 0));

    for (int i = 0; i < pMyDoc->m_lineer.GetSize(); i++) {
        int start = pMyDoc->m_lineer[i].high0;
        int end = pMyDoc->m_lineer[i].high1;
        bool orient = pMyDoc->m_lineer[i].side;
        if (orient) {
            adjMatrix[start][end] = 1;
        }
        else
        {
            adjMatrix[start][end] = 1;
            adjMatrix[end][start] = 1;
        }
    }
    dc.Rectangle(rect);
    
    int startZ = 28;
    int startM = 35;
    int cellSize = 30;
    CBrush bbrush1, bbrush2, bbrush3;
    bbrush1.CreateSolidBrush(RGB(100, 100, 100));
    bbrush2.CreateSolidBrush(RGB(90, 90, 90));
    bbrush3.CreateSolidBrush(RGB(130, 130, 130));
    
    pen = (CPen*)dc.SelectStockObject(NULL_PEN);
    brush = dc.SelectObject(&bbrush1);
    for (int i = 60; i < rect.bottom; i += 60) {
        dc.Rectangle(rect.right, rect.top + i, rect.left, rect.top + i + 30);
    }
    brush = dc.SelectObject(&bbrush2);
    for (int i = 30; i < rect.bottom; i += 60) {
        dc.Rectangle(rect.right, rect.top + i, rect.left, rect.top + i + 30);
    }
    pen = dc.SelectObject(pen);
    brush = dc.SelectObject(&bbrush3);
    dc.Rectangle(rect.right, rect.top, rect.left, rect.top + 30);
    dc.Rectangle(rect.left, rect.top + 29, rect.left + 30, rect.bottom);

    for (int i = 0; i < matrixSize; i++) {
        std::wstring strNumber = std::to_wstring(i);
        dc.SetBkColor(RGB(130, 130, 130));
        dc.TextOutW(startZ + 7 + i * cellSize, startZ - 20, strNumber.c_str(), strNumber.length());
        dc.TextOutW(startZ - 20, startZ + 7 + i * cellSize, strNumber.c_str(), strNumber.length());
    }

    for (int i = 0; i < matrixSize; i++) {
        for (int j = 0; j < matrixSize; j++) {
            CString value = (adjMatrix[i][j] == 1) ? _T("1") : _T("0");
            if (i % 2 == 0) {
                dc.SetBkColor(RGB(90, 90, 90));
            }
            else {
                dc.SetBkColor(RGB(100, 100, 100));
            }
            dc.TextOutW(startM + j * cellSize, startM + i * cellSize, value);
        }
    }
    
}

BEGIN_MESSAGE_MAP(CCustomMatrix, CWnd)
    ON_WM_PAINT()
END_MESSAGE_MAP()