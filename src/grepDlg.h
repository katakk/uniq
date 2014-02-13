#pragma once
#include "afxwin.h"


// CGrepDlg �_�C�A���O

class CGrepDlg : public CDialog
{
	DECLARE_DYNAMIC(CGrepDlg)

public:
	CGrepDlg(CWnd* pParent = NULL);   // �W���R���X�g���N�^
	virtual ~CGrepDlg();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_GREPDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedGrep();
	afx_msg void OnBnClickedGrepv();
	CString m_grep;

protected:
	virtual void PostNcDestroy();
	virtual void OnOK();
	virtual void OnCancel();
public:

	CButton m_grepv;
	CButton m_grepb;
	afx_msg void OnBnClickedHistory();
};
