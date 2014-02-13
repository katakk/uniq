// uniqDoc.cpp : CuniqDoc クラスの実装
//

#include "stdafx.h"
#include "uniq.h"

#include "MainFrm.h"
#include "uniqDoc.h"
#include "uniqView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CuniqDoc

IMPLEMENT_DYNCREATE(CuniqDoc, CDocument)

BEGIN_MESSAGE_MAP(CuniqDoc, CDocument)
	ON_UPDATE_COMMAND_UI(ID_RELOAD, OnUpdateReloadDocument)
END_MESSAGE_MAP()


// CuniqDoc コンストラクション/デストラクション

CuniqDoc::CuniqDoc()
{
	// TODO: この位置に１度だけ呼ばれる構築用のコードを追加してください。
}

CuniqDoc::~CuniqDoc()
{
}

BOOL CuniqDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	reinterpret_cast<CEditView*>(m_viewList.GetHead())->SetWindowText(NULL);

	// TODO: この位置に再初期化処理を追加してください。
	// (SDI ドキュメントはこのドキュメントを再利用します。)
	reinterpret_cast<CEditView*>(m_viewList.GetHead())->SetFont(&((CuniqApp*)AfxGetApp())->m_font,TRUE);

	return TRUE;
}


#include <afxpriv.h>
#include ".\uniqdoc.h"

CFile* CuniqDoc::GetFile(LPCTSTR lpszFileName, UINT nOpenFlags,
		CFileException* pError)
{
	CMirrorFile* pFile = new CMirrorFile;
	ASSERT(pFile != NULL);
	if (!pFile->Open(lpszFileName, (nOpenFlags & ~0x70) | CFile::shareDenyNone, pError))
	{
		delete pFile;
		pFile = NULL;
	}

	return pFile;
}


// CuniqDoc シリアル化

void CuniqDoc::Serialize(CArchive& ar)
{
	// CEditView は、すべてのシリアル化を処理するエディット コントロールを含んでいます。
	reinterpret_cast<CuniqView*>(m_viewList.GetHead())->Serialize(ar);

	ar.GetFile()->GetStatus(m_filestatus);
	if(((CuniqApp*)AfxGetApp())->m_bmp.GetSafeHwnd())
		((CuniqApp*)AfxGetApp())->m_bmp.Invalidate();


}


// CuniqDoc 診断

#ifdef _DEBUG
void CuniqDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CuniqDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

// CuniqDoc コマンド


void CuniqDoc::OnUpdateReloadDocument(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(!GetPathName().IsEmpty());
}


BOOL CuniqDoc::IsUpdate()
{
	CFileFind ff;
	CFileStatus fs;

	if(GetPathName().IsEmpty())
		return FALSE;

	if (! ff.FindFile(GetPathName()))
		return FALSE;
		
	if (! CFile::GetStatus(GetPathName(), fs))
		return FALSE;

	if(
		m_filestatus.m_size != fs.m_size ||
		m_filestatus.m_atime != fs.m_atime ||
		m_filestatus.m_ctime != fs.m_ctime ||
		m_filestatus.m_mtime != fs.m_mtime)
		return TRUE;
	else
		return FALSE;
}

const CString& CuniqDoc::GetTitle()
{
	if(! m_strPathName.IsEmpty() )
	{
		CString s;
		CFileStatus sf;

		CFile::GetStatus(m_strPathName, sf);
		CTimeSpan ts = sf.m_mtime - sf.m_ctime;
		
		if ( sf.m_size > 10000 )
			s.Format( _T("%I64u Kbytes"), sf.m_size / 1000 );
		else
			s.Format( _T("%I64u bytes"), sf.m_size );
		m_strTitle.Format( _T("%s [%s %I64d hours [%s]"),
			m_strPathName, s,
			ts.GetTotalHours(),
		//	sf.m_ctime.Format( "%Y/%m/%d-%H:%M:%S"),
			sf.m_mtime.Format( "%Y/%m/%d-%H:%M:%S") );

	}
	
	return m_strTitle;
}

BOOL CuniqDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	BOOL b = FALSE;
	CFile f;
//	CFileFind ff;
	CFileStatus fs;

//	if (! ff.FindFile(lpszPathName))
//		goto EXIT;
		
	if (! CFile::GetStatus(lpszPathName, fs))
		goto EXIT;

	if (fs.m_attribute & (CFile::volume | CFile::directory) )
		goto EXIT;

	if(! f.Open(lpszPathName, CFile::modeRead | CFile::typeBinary 
		| CFile::shareDenyNone | CFile::osSequentialScan ))
		goto EXIT;
	f.Close();

	b = CDocument::OnOpenDocument(lpszPathName);
	((CMainFrame*)AfxGetMainWnd())->StatusBarSetPaneText(
		ID_INDICATOR_LASTREAD, CTime::GetCurrentTime().Format(_T("%H:%M:%S")));
	return b;

EXIT:
	CuniqGetFrame()->StatusBarSetPaneText(
		ID_INDICATOR_LASTREAD, _T("再読込失敗"));
	return FALSE;
}
