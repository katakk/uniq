// MyDocManager.cpp : é¿ëïÉtÉ@ÉCÉã
//

#include "stdafx.h"
#include "uniq.h"
#include "MyDocManager.h"

//afximpl.h
#define _countof(array) (sizeof(array)/sizeof(array[0]))
BOOL AFXAPI AfxFullPath(LPTSTR lpszPathOut, LPCTSTR lpszFileIn);
BOOL AFXAPI AfxComparePath(LPCTSTR lpszPath1, LPCTSTR lpszPath2);
#ifndef _MAC
BOOL AFXAPI AfxResolveShortcut(CWnd* pWnd, LPCTSTR pszShortcutFile,LPTSTR pszPath, int cchPath);
#endif

// CMyDocManager

IMPLEMENT_DYNCREATE(CMyDocManager, CDocManager)

CMyDocManager::CMyDocManager()
{
}

CMyDocManager::~CMyDocManager()
{
}



// CMyDocManager êfíf

#ifdef _DEBUG
void CMyDocManager::AssertValid() const
{
	CDocManager::AssertValid();
}

void CMyDocManager::Dump(CDumpContext& dc) const
{
	CDocManager::Dump(dc);
}
#endif //_DEBUG



CDocument* CMyDocManager::OpenDocumentFile(LPCTSTR lpszFileName)
{
	// find the highest confidence
	POSITION pos = m_templateList.GetHeadPosition();
	CDocTemplate::Confidence bestMatch = CDocTemplate::noAttempt;
	CDocTemplate* pBestTemplate = NULL;
	CDocument* pOpenDocument = NULL;

	TCHAR szPath[_MAX_PATH];
	ASSERT(lstrlen(lpszFileName) < _countof(szPath));
	TCHAR szTemp[_MAX_PATH];
	if (lpszFileName[0] == '\"')
		++lpszFileName;
	lstrcpyn(szTemp, lpszFileName, _MAX_PATH);
	LPTSTR lpszLast = _tcsrchr(szTemp, '\"');
	if (lpszLast != NULL)
		*lpszLast = 0;
	
	if( AfxFullPath(szPath, szTemp) == FALSE )
	{
		ASSERT(FALSE);
		return NULL; // We won't open the file. MFC requires paths with
		             // length < _MAX_PATH
	}

	TCHAR szLinkName[_MAX_PATH];
	if (AfxResolveShortcut(AfxGetMainWnd(), szPath, szLinkName, _MAX_PATH))
		lstrcpy(szPath, szLinkName);

	while (pos != NULL)
	{
		CDocTemplate* pTemplate = (CDocTemplate*)m_templateList.GetNext(pos);
		ASSERT_KINDOF(CDocTemplate, pTemplate);

		CDocTemplate::Confidence match;
		ASSERT(pOpenDocument == NULL);
		match = pTemplate->MatchDocType(szPath, pOpenDocument);
		if (match > bestMatch)
		{
			bestMatch = match;
			pBestTemplate = pTemplate;
		}
		if (match == CDocTemplate::yesAlreadyOpen)
			break;      // stop here
	}

	if (pOpenDocument != NULL)
	{
		POSITION pos = pOpenDocument->GetFirstViewPosition();
		if (pos != NULL)
		{
			CView* pView = pOpenDocument->GetNextView(pos); // get first one
			ASSERT_VALID(pView);
			CFrameWnd* pFrame = pView->GetParentFrame();

			if (pFrame == NULL)
				TRACE(traceAppMsg, 0, "Error: Can not find a frame for document to activate.\n");
			else
			{
				pFrame->ActivateFrame();

				if (pFrame->GetParent() != NULL)
				{
					CFrameWnd* pAppFrame;
					if (pFrame != (pAppFrame = (CFrameWnd*)AfxGetApp()->m_pMainWnd))
					{
						ASSERT_KINDOF(CFrameWnd, pAppFrame);
						pAppFrame->ActivateFrame();
					}
				}
			}
		}
		else
			TRACE(traceAppMsg, 0, "Error: Can not find a view for document to activate.\n");

		return pOpenDocument;
	}

	if (pBestTemplate == NULL)
	{
		AfxMessageBox(AFX_IDP_FAILED_TO_OPEN_DOC);
		return NULL;
	}

	return pBestTemplate->OpenDocumentFile(szPath);
}