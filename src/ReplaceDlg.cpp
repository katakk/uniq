// ReplaceDlg.cpp : 実装ファイル
//

#include "stdafx.h"
#include "uniq.h"
#include "ReplaceDlg.h"
#include "uniqDoc.h"
#include "uniqview.h"
#include ".\replacedlg.h"


// CReplaceDlg ダイアログ

IMPLEMENT_DYNAMIC(CReplaceDlg, CDialog)
CReplaceDlg::CReplaceDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CReplaceDlg::IDD, pParent)
	, m_old(_T(""))
	, m_new(_T(""))
{
	Create(IDD,pParent);
}

CReplaceDlg::~CReplaceDlg()
{
}

void CReplaceDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_old);
	DDX_Text(pDX, IDC_EDIT2, m_new);
}


BEGIN_MESSAGE_MAP(CReplaceDlg, CDialog)
	ON_BN_CLICKED(IDC_REPLACE, OnBnClickedReplace)
END_MESSAGE_MAP()


// CReplaceDlg メッセージ ハンドラ

void CReplaceDlg::OnBnClickedReplace()
{
	UpdateData(TRUE);
	CuniqGetView()->Replace( m_old, m_new );
	UpdateData(FALSE);
}

void CReplaceDlg::OnOK()
{
	DestroyWindow();
}

void CReplaceDlg::PostNcDestroy()
{
	//delete this;
}

void CReplaceDlg::OnCancel()
{
	DestroyWindow();
}

BOOL CReplaceDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO :  ここに初期化を追加してください
	SetIcon(AfxGetApp()->LoadIcon(IDI_REPLACE), TRUE);		// 小さいアイコンの設定
	SetIcon(AfxGetApp()->LoadIcon(IDI_REPLACE), FALSE);		// 小さいアイコンの設定

	return TRUE;  // return TRUE unless you set the focus to a control
	// 例外 : OCX プロパティ ページは必ず FALSE を返します。
}
