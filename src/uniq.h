// uniq.h : uniq アプリケーションのメイン ヘッダー ファイル
//
#pragma once

#ifndef __AFXWIN_H__
	#error このファイルを PCH に含める前に、'stdafx.h' を含めてください。
#endif

#include "resource.h"       // メイン シンボル
#include "BitmapWnd.h"

#include "MainFrm.h"

#include "uniqDoc.h"
#include "uniqView.h"

//class CBitmapWnd;
// CuniqApp:
// このクラスの実装については、uniq.cpp を参照してください。
//

class CuniqApp : public CWinApp
{
public:
	CuniqApp();



	afx_msg void OnUpdateBitmap(CCmdUI* pCmdUI);
	afx_msg void OnBitmap(void);
	CBitmapWnd m_bmp;


// オーバーライド
public:
	virtual BOOL InitInstance();
	CFont m_font;

// 実装
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()

public:
//	void ParseLines(CStringArray &sa);
//	void StoreStringArray(CStringArray& sa);
//	CEdit& GetEditCtrl() const;

protected:
public:
	CDocument* OpenDocumentFile(LPCTSTR lpszFileName);
	virtual void AddDocTemplate(CDocTemplate* pTemplate);
};

inline CMainFrame * CuniqGetFrame() { return (CMainFrame*)AfxGetMainWnd(); }
inline CuniqView * CuniqGetView() { return (CuniqView*)CuniqGetFrame()->GetActiveView(); }
inline CuniqDoc * CuniqGetDoc() { return (CuniqDoc*)CuniqGetFrame()->GetActiveDocument(); }



extern CuniqApp theApp;
