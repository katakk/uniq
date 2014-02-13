#pragma once


// CReplaceDlg ダイアログ

class CReplaceDlg : public CDialog
{
	DECLARE_DYNAMIC(CReplaceDlg)

public:
	CReplaceDlg(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CReplaceDlg();

// ダイアログ データ
	enum { IDD = IDD_REPLACE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

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
