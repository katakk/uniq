
#pragma once


// CMyDocTemplate �R�}���h �^�[�Q�b�g

class CMyDocTemplate : public CSingleDocTemplate
{
	DECLARE_DYNAMIC(CMyDocTemplate)
public:
	CMyDocTemplate(UINT nIDResource, CRuntimeClass* pDocClass,
		CRuntimeClass* pFrameClass, CRuntimeClass* pViewClass);
	virtual Confidence MatchDocType(LPCTSTR lpszPathName, CDocument*& rpDocMatch);
	virtual BOOL GetDocString(CString& rString, enum DocStringIndex index) const; 

};


