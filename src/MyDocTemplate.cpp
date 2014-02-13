// MyDocTemplate.cpp : 実装ファイル
//

#include "stdafx.h"
#include "uniq.h"
#include "MyDocTemplate.h"


// CMyDocTemplate

IMPLEMENT_DYNAMIC(CMyDocTemplate, CSingleDocTemplate)

CMyDocTemplate::CMyDocTemplate(UINT nIDResource, CRuntimeClass* pDocClass,
							   CRuntimeClass* pFrameClass, CRuntimeClass* pViewClass)
:CSingleDocTemplate(nIDResource, pDocClass, pFrameClass, pViewClass)
{

}

// CMyDocTemplate メッセージ ハンドラ
BOOL CMyDocTemplate::GetDocString(CString& rString, enum DocStringIndex index) const
{
    if (index == CDocTemplate::docName){
		rString = CTime::GetCurrentTime().Format( "%Y%m%d-%H%M%S");
        return TRUE;
	}else{
		return CSingleDocTemplate::GetDocString(rString, index);
	}
}

CMyDocTemplate::Confidence CMyDocTemplate::MatchDocType(LPCTSTR lpszPathName, CDocument*& rpDocMatch)
{
    Confidence type = CSingleDocTemplate::MatchDocType(lpszPathName, rpDocMatch);
    if (type == yesAlreadyOpen){
        rpDocMatch = NULL;
        return yesAttemptNative;
    }else return type;
} 