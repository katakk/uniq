#pragma once


// CMyDocManager �h�L�������g

class CMyDocManager : public CDocManager
{
	DECLARE_DYNCREATE(CMyDocManager)

public:
	CMyDocManager();
	virtual ~CMyDocManager();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	CDocument* OpenDocumentFile(LPCTSTR lpszFileName);


};
