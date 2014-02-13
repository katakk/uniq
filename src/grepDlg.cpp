// grepDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "uniq.h"
#include "grepDlg.h"
#include "uniqDoc.h"
#include "uniqview.h"
#include ".\grepdlg.h"


// CGrepDlg ダイアログ

IMPLEMENT_DYNAMIC(CGrepDlg, CDialog)
CGrepDlg::CGrepDlg(CWnd* pParent /*=NULL*/)
: CDialog(CGrepDlg::IDD, pParent)
, m_grep(_T(""))
{
	Create(IDD,pParent);
}


CGrepDlg::~CGrepDlg()
{
}

void CGrepDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_grep);
	DDX_Control(pDX, IDC_GREPV, m_grepv);
	DDX_Control(pDX, IDC_GREP, m_grepb);
}

void CGrepDlg::PostNcDestroy()
{
	//delete this;
}

void CGrepDlg::OnOK()
{
	DestroyWindow();
}

void CGrepDlg::OnCancel()
{
	DestroyWindow();
}

BEGIN_MESSAGE_MAP(CGrepDlg, CDialog)
	ON_BN_CLICKED(IDC_GREP, OnBnClickedGrep)
	ON_BN_CLICKED(IDC_GREPV, OnBnClickedGrepv)

	ON_BN_CLICKED(IDC_HISTORY, OnBnClickedHistory)
END_MESSAGE_MAP()


// CGrepDlg メッセージ ハンドラ
BOOL CGrepDlg::OnInitDialog()
{

	SetIcon(AfxGetApp()->LoadIcon(IDI_GREP), TRUE);			// 大きいアイコンの設定
	SetIcon(AfxGetApp()->LoadIcon(IDI_GREP), FALSE);		// 小さいアイコンの設定

	return TRUE;
}



void CGrepDlg::OnBnClickedGrep()
{
	// TODO : ここにコントロール通知ハンドラ コードを追加します。
	UpdateData(TRUE);
	CuniqGetView()->Grep( m_grep );
	UpdateData(FALSE);
}



void CGrepDlg::OnBnClickedGrepv()
{
	// TODO : ここにコントロール通知ハンドラ コードを追加します。
	UpdateData(TRUE);
	CuniqGetView()->GrepV( m_grep );
	UpdateData(FALSE);
}





void CGrepDlg::OnBnClickedHistory()
{
	CString str; 
	str = AfxGetApp()->m_pszAppName;
	str += _T(".grep.txt");
	ShellExecute(NULL, "open", str, NULL, NULL, SW_SHOWNORMAL);
}
