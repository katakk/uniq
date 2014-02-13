// uniqDoc.h : CuniqDoc クラスのインターフェイス
//


#pragma once

class CuniqDoc : public CDocument
{
protected: // シリアル化からのみ作成します。
	CuniqDoc();
	DECLARE_DYNCREATE(CuniqDoc)

// 属性
public:

// 操作
public:
	void OnUpdateReloadDocument(CCmdUI* pCmdUI);
	BOOL IsUpdate();
		CFileStatus m_filestatus;



// オーバーライド
	public:
//		virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
		virtual CFile* GetFile(LPCTSTR lpszFileName, UINT nOpenFlags,
			CFileException* pError);
	virtual const CString& GetTitle();
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// 実装
public:
	virtual ~CuniqDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成された、メッセージ割り当て関数
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
};


