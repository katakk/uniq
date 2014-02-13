// uniqDoc.cpp : CuniqDoc �N���X�̎���
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


// CuniqDoc �R���X�g���N�V����/�f�X�g���N�V����

CuniqDoc::CuniqDoc()
{
	// TODO: ���̈ʒu�ɂP�x�����Ă΂��\�z�p�̃R�[�h��ǉ����Ă��������B
}

CuniqDoc::~CuniqDoc()
{
}

BOOL CuniqDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	reinterpret_cast<CEditView*>(m_viewList.GetHead())->SetWindowText(NULL);

	// TODO: ���̈ʒu�ɍď�����������ǉ����Ă��������B
	// (SDI �h�L�������g�͂��̃h�L�������g���ė��p���܂��B)
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


// CuniqDoc �V���A����

void CuniqDoc::Serialize(CArchive& ar)
{
	// CEditView �́A���ׂẴV���A��������������G�f�B�b�g �R���g���[�����܂�ł��܂��B
	reinterpret_cast<CuniqView*>(m_viewList.GetHead())->Serialize(ar);

	ar.GetFile()->GetStatus(m_filestatus);
	if(((CuniqApp*)AfxGetApp())->m_bmp.GetSafeHwnd())
		((CuniqApp*)AfxGetApp())->m_bmp.Invalidate();


}


// CuniqDoc �f�f

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

// CuniqDoc �R�}���h


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
		ID_INDICATOR_LASTREAD, _T("�ēǍ����s"));
	return FALSE;
}
