#pragma once
#include "afxcmn.h"


// CFindDlg ダイアログ

class CFindDlg : public CDialog
{
	DECLARE_DYNCREATE(CFindDlg)

public:
	CFindDlg(CWnd* pParent = NULL);   // 標準コンストラクタ
	virtual ~CFindDlg();
// オーバーライド

// ダイアログ データ
	enum { IDD = IDD_FIND };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

	virtual void OnOK();
	virtual void OnCancel();
	virtual void PostNcDestroy();
public:
	afx_msg void OnBnClickedReplace();
	CString m_find;
	CListCtrl m_list;
	afx_msg void OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedClear();
	afx_msg void OnBnClickedSplit();
};
