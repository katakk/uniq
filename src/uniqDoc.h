// uniqDoc.h : CuniqDoc �N���X�̃C���^�[�t�F�C�X
//


#pragma once

class CuniqDoc : public CDocument
{
protected: // �V���A��������̂ݍ쐬���܂��B
	CuniqDoc();
	DECLARE_DYNCREATE(CuniqDoc)

// ����
public:

// ����
public:
	void OnUpdateReloadDocument(CCmdUI* pCmdUI);
	BOOL IsUpdate();
		CFileStatus m_filestatus;



// �I�[�o�[���C�h
	public:
//		virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
		virtual CFile* GetFile(LPCTSTR lpszFileName, UINT nOpenFlags,
			CFileException* pError);
	virtual const CString& GetTitle();
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// ����
public:
	virtual ~CuniqDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// �������ꂽ�A���b�Z�[�W���蓖�Ċ֐�
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
};


