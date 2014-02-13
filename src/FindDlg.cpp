// FindDlg.cpp : �����t�@�C��
//

#include "stdafx.h"
#include "uniq.h"
#include "FindDlg.h"
#include "SortableStringArray.h"


// CFindDlg �_�C�A���O

IMPLEMENT_DYNCREATE(CFindDlg, CDialog)

CFindDlg::CFindDlg(CWnd* pParent /*=NULL*/)
: CDialog(CFindDlg::IDD, pParent)
, m_find(_T(""))
{
	Create(IDD,pParent);
}

CFindDlg::~CFindDlg()
{
}

void CFindDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_find);
	DDX_Control(pDX, IDC_LIST1, m_list);
}

BOOL CFindDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	SetIcon(AfxGetApp()->LoadIcon(IDI_FIND), TRUE);		//   �����A�C�R���̐ݒ�
	SetIcon(AfxGetApp()->LoadIcon(IDI_FIND), FALSE);		// �������A�C�R���̐ݒ�

	m_list.ModifyStyle(0,LVS_REPORT| LVS_SINGLESEL |LVS_SHOWSELALWAYS);
	m_list.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	m_list.InsertColumn(0, _T("line"), LVCFMT_LEFT, 40, 2);
	m_list.InsertColumn(1, _T("col"),  LVCFMT_LEFT, 40, 2);
	m_list.InsertColumn(2, _T("find"), LVCFMT_LEFT, 50, -1);
	m_list.InsertColumn(3, _T("source"), LVCFMT_LEFT, 500, -1);

	return TRUE;  // �t�H�[�J�X���R���g���[���ɐݒ肵���ꍇ�������ATRUE ��Ԃ��܂��B
}

BEGIN_MESSAGE_MAP(CFindDlg, CDialog)

	ON_BN_CLICKED(IDC_REPLACE, OnBnClickedReplace)

	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, OnNMDblclkList1)
	ON_BN_CLICKED(ID_CLEAR, OnBnClickedClear)
	ON_BN_CLICKED(IDC_SPLIT, OnBnClickedSplit)
END_MESSAGE_MAP()


// CFindDlg ���b�Z�[�W �n���h��

void CFindDlg::OnOK()
{
	DestroyWindow();
}

void CFindDlg::OnCancel()
{
	DestroyWindow();
}

void CFindDlg::PostNcDestroy()
{
	//delete this;
}

void CFindDlg::OnBnClickedReplace()
{
	UpdateData(TRUE);
	CuniqGetView()->FindStart( m_find );
	UpdateData(FALSE);
}

void CFindDlg::OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO : �����ɃR���g���[���ʒm�n���h�� �R�[�h��ǉ����܂��B
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);

	CEdit& edit = CuniqGetView()->GetEditCtrl();
	int line, col;
	int s, e;
	TCHAR szText[0x100];
	m_list.GetItemText(phdr->iItem, 0, szText, 0x100);
	line = _tstoi(szText);
	m_list.GetItemText(phdr->iItem, 1, szText, 0x100);
	col = _tstoi(szText);
	m_list.GetItemText(phdr->iItem, 2, szText, 0x100);

	s = edit.LineIndex(line) + col;
	e = s + (int) _tcslen(szText);
	edit.SetSel(s, e );

	*pResult = 0;
}

void CFindDlg::OnBnClickedClear()
{
	m_list.DeleteAllItems();
}

void CFindDlg::OnBnClickedSplit()
{
	UpdateData(TRUE);
	m_find.Replace(" ", "\n");
	m_find.Replace(".", "\n");
//	m_find.Replace("/", "\n");
	m_find.Replace("(", "\n");
	m_find.Replace(")", "\n");
	m_find.Replace("]", "\n");
	m_find.Replace("[", "\n");
	m_find.Replace(",", "\n");
	m_find.Replace("\t", "\n");
	m_find.Replace("\"", "\n");
	CString temp;
	CSortableStringArray sa;
	::ParseLines(sa,m_find);
	sa.Sort();
	m_find = _T("");
	for(INT_PTR i = 0; i < sa.GetCount(); i++)
	{
		if( sa[i].IsEmpty() ) continue;
		if( sa[i] == temp ) continue;
		temp = sa[i];
		m_find += sa[i];
		m_find += _T("\r\n");
	}
	UpdateData(FALSE);
}
