// MainFrm.cpp : CMainFrame クラスの実装
//

#include "stdafx.h"
#include "uniq.h"

#include "MainFrm.h"
#include "uniqDoc.h"
#include "uniqView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_TIMER()
//	ON_WM_SYSKEYDOWN()
	ON_COMMAND(ID_RELOADTIMER, OnReloadTimer)
	ON_UPDATE_COMMAND_UI(ID_RELOADTIMER, OnUpdateReloadTimer)
//	ON_COMMAND(ID_VIEW_TOOLBARMAIN, OnViewToolbar)
//	ON_COMMAND(ID_VIEW_TOOLBARSORT, OnViewToolbarSort)
//	ON_COMMAND(ID_VIEW_TOOLBARCOLS, OnViewToolbarCols)
//	ON_COMMAND(ID_VIEW_TOOLBARTOOL, OnViewToolbarTool)
//	ON_UPDATE_COMMAND_UI(ID_VIEW_TOOLBARMAIN, OnUpdateViewToolbar)
//	ON_UPDATE_COMMAND_UI(ID_VIEW_TOOLBARSORT, OnUpdateViewToolbarSort)
//	ON_UPDATE_COMMAND_UI(ID_VIEW_TOOLBARCOLS, OnUpdateViewToolbarCols)
//	ON_UPDATE_COMMAND_UI(ID_VIEW_TOOLBARTOOL, OnUpdateViewToolbarTool)
ON_WM_SIZE()
ON_WM_MOVE()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // ステータス ライン インジケータ
ID_INDICATOR_LASTREAD,
//ID_INDICATOR_KANA,
	//ID_INDICATOR_OVR,
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	//ID_INDICATOR_SCRL,
};


// CMainFrame コンストラクション/デストラクション

CMainFrame::CMainFrame()
{
	m_ReloadTimer = FALSE;
}

CMainFrame::~CMainFrame()
{
}

void CMainFrame::DockControlBar(CToolBar *bef, CToolBar *cur)
{
	CRect rc;
	CSize sz;
	RecalcLayout();

	if(bef) {
		bef->GetWindowRect(rc);
		sz = bef->CalcFixedLayout(FALSE,TRUE);
		rc.MoveToX(sz.cx);
		cur->EnableDocking(CBRS_ALIGN_ANY);
		CFrameWnd::DockControlBar(cur, (UINT)0, rc);
	}else{
		cur->EnableDocking(CBRS_ALIGN_ANY);
		CFrameWnd::DockControlBar(cur);
	}
	RecalcLayout();
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("ツール バーの作成に失敗しました。\n");
		return -1;      // 作成できませんでした。
	}
/*

	if (!m_wndToolBarSort.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBarSort.LoadToolBar(IDR_SORT))
	{
		TRACE0("ツール バーの作成に失敗しました。\n");
		return -1;      // 作成できませんでした。
	}

	if (!m_wndToolBarCols.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBarCols.LoadToolBar(IDR_COLS))
	{
		TRACE0("ツール バーの作成に失敗しました。\n");
		return -1;      // 作成できませんでした。
	}


	if (!m_wndToolBarTool.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBarTool.LoadToolBar(IDR_TOOL))
	{
		TRACE0("ツール バーの作成に失敗しました。\n");
		return -1;      // 作成できませんでした。
	}
*/
	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("ステータス バーの作成に失敗しました。\n");
		return -1;      // 作成できませんでした。
	}


	// TODO: ツール バーをドッキング可能にしない場合は、これらの 3 行を削除してください。
	EnableDocking(CBRS_ALIGN_ANY);

	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(NULL, &m_wndToolBar);
//	DockControlBar(&m_wndToolBar, &m_wndToolBarSort);
//	DockControlBar(&m_wndToolBarSort, &m_wndToolBarCols);
//	DockControlBar(&m_wndToolBarCols, &m_wndToolBarTool);

	return 0;
}
/*
void CMainFrame::OnUpdateViewToolbar(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_wndToolBar.IsVisible());
}
void CMainFrame::OnUpdateViewToolbarSort(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_wndToolBarSort.IsVisible());
}
void CMainFrame::OnUpdateViewToolbarCols(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_wndToolBarCols.IsVisible());
}
void CMainFrame::OnUpdateViewToolbarTool(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_wndToolBarTool.IsVisible());
}

void CMainFrame::OnViewToolbar()
{
	ShowControlBar( &m_wndToolBar, !m_wndToolBar.IsVisible(), FALSE );
	RecalcLayout();
}
void CMainFrame::OnViewToolbarSort()
{
	ShowControlBar( &m_wndToolBarSort, !m_wndToolBarSort.IsVisible(), FALSE ); 
	RecalcLayout();
}
void CMainFrame::OnViewToolbarCols()
{
	ShowControlBar( &m_wndToolBarCols, !m_wndToolBarCols.IsVisible(), FALSE ); 
	RecalcLayout();
}
void CMainFrame::OnViewToolbarTool()
{
	ShowControlBar( &m_wndToolBarTool, !m_wndToolBarTool.IsVisible(), FALSE ); 
	RecalcLayout();
}
*/

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	cs.dwExStyle |= WS_EX_ACCEPTFILES;	//ファイルのドロップを受け付ける
	// TODO: この位置で CREATESTRUCT cs を修正して Window クラスまたはスタイルを
	//  修正してください。
	cs.cx = 640;
	cs.cy = cs.cx * 750 / 1000;      // 高さ
	return TRUE;
}


// CMainFrame 診断

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG


// CMainFrame メッセージ ハンドラ
void CMainFrame::OnUpdateFrameTitle(BOOL bAddToTitle)
{
    CDocument* pDocument = GetActiveDocument();
    if (bAddToTitle && pDocument != NULL){
        CString title = ((CuniqDoc *)pDocument)->GetTitle();
		//if(! pDocument->GetPathName().IsEmpty()) {
		//	title = pDocument->GetPathName();
		//	pDocument->GetFile()->
		//}
        title += " + ";
        title += m_strTitle;
        SetWindowText(title);
    }else SetWindowText("");
} 

void CMainFrame::OnTimer(UINT_PTR nIDEvent)
{
	static int called = 0;
	if( nIDEvent == ID_RELOADTIMER )
	{
		// アップデート検知か10分ごとに再読込
		called ++;
		if(CuniqGetDoc()->IsUpdate() || called > 600) {
			called = 0;
			CuniqGetView()->OnReloadDocument();
		}
	}

}

void CMainFrame::OnReloadTimer()
{
	m_ReloadTimer = ! m_ReloadTimer;

	if(m_ReloadTimer)
		SetTimer(ID_RELOADTIMER, 1000, NULL); 
	else
		KillTimer(ID_RELOADTIMER); 
}

void CMainFrame::OnUpdateReloadTimer(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(m_ReloadTimer);
}


int CMainFrame::StatusBarSetPaneText(int ID, LPCTSTR str)
{
	if(! GetSafeHwnd() ) return 0;

	int n=m_wndStatusBar.CommandToIndex(ID);
	if(n>=0){
		m_wndStatusBar.SetPaneText(n,str);
	}
	return 0;
}




void CMainFrame::OnSize(UINT nType, int cx, int cy)
{
	CFrameWnd::OnSize(nType, cx, cy);

	// TODO : ここにメッセージ ハンドラ コードを追加します。
	((CuniqApp*)AfxGetApp())->m_bmp.SetLocate();
}

void CMainFrame::OnMove(int x, int y)
{
	CFrameWnd::OnMove(x, y);

	// TODO : ここにメッセージ ハンドラ コードを追加します。
	((CuniqApp*)AfxGetApp())->m_bmp.SetLocate();
}
