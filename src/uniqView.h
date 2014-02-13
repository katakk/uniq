// uniqView.h : CuniqView クラスのインターフェイス
//


#pragma once

void ParseLines(CStringArray &sa, CString& doc);

class CuniqView : public CEditView
{
protected: // シリアル化からのみ作成します。
	CuniqView();
	DECLARE_DYNCREATE(CuniqView)
protected:
	//{{AFX_MSG(CuniqView)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnNcLButtonDown(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint point);
	afx_msg void OnDropFiles(HDROP hDropInfo);
	//}}AFX_MSG
	// 属性
public:
	CuniqDoc* GetDocument() const;

	// 操作
public:
	void ParseLines(CStringArray &sa);
	void StoreStringArray(CStringArray& sa);
	INT_PTR GetCaretLine();
	INT_PTR GetCaretView();
	void SetCaretLine(int l);
	void SaveHistory(LPCTSTR file, LPCTSTR save);
	void Grep(LPCTSTR grep);
	void GrepV(LPCTSTR grep);
	void FindStart(LPCTSTR find);
	void Replace( LPCTSTR pszOld, LPCTSTR pszNew );

	COleDropTarget m_dropTarget;
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct) ;
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnParseLine();
	afx_msg void OnTrimSpace();
	afx_msg void OnTrimLine();
	afx_msg void OnUniqCols();
	afx_msg void OnUniq();
	afx_msg void OnReverse();
	afx_msg void OnSort();
	afx_msg void OnReverseSort();
	afx_msg void OnSortLogical();
	afx_msg void OnReverseSortLogical();
	afx_msg void OnSelectall();
	afx_msg void OnReturn();
	afx_msg void OnNoReturn();

	afx_msg void OnGoTop();
	afx_msg void OnGoBottom();

	afx_msg void OnFind();
	afx_msg void OnReplace();
	afx_msg void OnGrep();
	afx_msg void OnGrepV();


//	afx_msg DROPEFFECT OnDragEnterAndDragOver(COleDataObject* pDataObject, 
//		DWORD dwKeyState, CPoint point);
//	afx_msg DROPEFFECT OnDragOver(COleDataObject* pDataObject, 
//		DWORD dwKeyState, CPoint point);
//	afx_msg DROPEFFECT OnDragEnter(COleDataObject* pDataObject, 
//		DWORD dwKeyState, CPoint point);
//	afx_msg BOOL OnDrop(COleDataObject* pDataObject,
//		DROPEFFECT dropEffect, CPoint point) ;


	// オーバーライド
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

	// 実装
public:
	virtual ~CuniqView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	void OnUpdate();
	void OnReloadDocument();

	BOOL GetSelText(CString& s) const;
	BOOL GetMidText(CString& s, int nStart, int nLen) const;
	int CharFromPos(CPoint pt) const;

	CPoint PosFromChar(int nChar) const;
	void VisibleSel( int& s, int& e);


	class CFindDlg * m_FindDlg;
	class CReplaceDlg * m_ReplaceDlg;
	class CGrepDlg * m_GrepDlg;



	// 生成された、メッセージ割り当て関数
protected:
	DECLARE_MESSAGE_MAP()
public:
	void ReadFromArchive(CArchive& ar, UINT nLen);
	virtual void Serialize(CArchive& ar);
	afx_msg void OnUpdateReplace(CCmdUI *pCmdUI);
	afx_msg void OnUpdateGrep(CCmdUI *pCmdUI);
	afx_msg void OnUpdateFind(CCmdUI *pCmdUI);
};

#ifndef _DEBUG  // uniqView.cpp のデバッグ バージョン
inline CuniqDoc* CuniqView::GetDocument() const
{ return reinterpret_cast<CuniqDoc*>(m_pDocument); }
#endif


