#pragma once


// CReplaceDlg �_�C�A���O

class CReplaceDlg : public CDialog
{
	DECLARE_DYNAMIC(CReplaceDlg)

public:
	CReplaceDlg(CWnd* pParent = NULL);   // �W���R���X�g���N�^
	virtual ~CReplaceDlg();

// �_�C�A���O �f�[�^
	enum { IDD = IDD_REPLACE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedReplace();
	CString m_old;
	CString m_new;
protected:
	virtual void OnOK();
	virtual void PostNcDestroy();
	virtual void OnCancel();
public:
	virtual BOOL OnInitDialog();
};
