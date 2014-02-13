// ReplaceDlg.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "uniq.h"
#include "ReplaceDlg.h"
#include "uniqDoc.h"
#include "uniqview.h"
#include ".\replacedlg.h"


// CReplaceDlg �_�C�A���O

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


// CReplaceDlg ���b�Z�[�W �n���h��

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

	// TODO :  �����ɏ�������ǉ����Ă�������
	SetIcon(AfxGetApp()->LoadIcon(IDI_REPLACE), TRUE);		// �������A�C�R���̐ݒ�
	SetIcon(AfxGetApp()->LoadIcon(IDI_REPLACE), FALSE);		// �������A�C�R���̐ݒ�

	return TRUE;  // return TRUE unless you set the focus to a control
	// ��O : OCX �v���p�e�B �y�[�W�͕K�� FALSE ��Ԃ��܂��B
}
