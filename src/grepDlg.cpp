// grepDlg.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "uniq.h"
#include "grepDlg.h"
#include "uniqDoc.h"
#include "uniqview.h"
#include ".\grepdlg.h"


// CGrepDlg �_�C�A���O

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


// CGrepDlg ���b�Z�[�W �n���h��
BOOL CGrepDlg::OnInitDialog()
{

	SetIcon(AfxGetApp()->LoadIcon(IDI_GREP), TRUE);			// �傫���A�C�R���̐ݒ�
	SetIcon(AfxGetApp()->LoadIcon(IDI_GREP), FALSE);		// �������A�C�R���̐ݒ�

	return TRUE;
}



void CGrepDlg::OnBnClickedGrep()
{
	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	UpdateData(TRUE);
	CuniqGetView()->Grep( m_grep );
	UpdateData(FALSE);
}



void CGrepDlg::OnBnClickedGrepv()
{
	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
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
