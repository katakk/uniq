// MainFrm.h : CMainFrame クラスのインターフェイス
//

#pragma once


class CMainFrame : public CFrameWnd
{

protected: // シリアル化からのみ作成します。
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// 属性
public:

	BOOL m_ReloadTimer;
	void OnUpdateReloadTimer(CCmdUI* pCmdUI);
	void OnReloadTimer(void);

// 操作
public:
	void AutoReload(BOOL enable);
	afx_msg void OnTimer(UINT_PTR nIDEvent);

// オーバーライド
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnUpdateFrameTitle(BOOL bAddToTitle); 
// 実装
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // コントロール バー用メンバ
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;
//	CToolBar    m_wndToolBarTool;
//	CToolBar    m_wndToolBarSort;
//	CToolBar    m_wndToolBarCols;
	//CToolBar    m_wndToolBar;

//	 生成された、メッセージ割り当て関数
protected:
	void DockControlBar(CToolBar *bef, CToolBar *cur);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//afx_msg void OnUpdateViewToolbar(CCmdUI *pCmdUI);
	//afx_msg void OnUpdateViewToolbarTool(CCmdUI *pCmdUI);
	//afx_msg void OnUpdateViewToolbarSort(CCmdUI *pCmdUI);
	//afx_msg void OnUpdateViewToolbarCols(CCmdUI *pCmdUI);
	//afx_msg void OnViewToolbar();
	//afx_msg void OnViewToolbarTool();
	//afx_msg void OnViewToolbarSort();
	//afx_msg void OnViewToolbarCols();

	DECLARE_MESSAGE_MAP()
public:
	int StatusBarSetPaneText(int ID, LPCTSTR str);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMove(int x, int y);
};


