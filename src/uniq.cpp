// uniq.cpp : �A�v���P�[�V�����̃N���X������`���܂��B
//

#include "stdafx.h"
#include "BitmapWnd.h"
#include "uniq.h"
#include "MainFrm.h"

#include "uniqDoc.h"
#include "uniqView.h"
#include "uniq.h"
#include "SortableStringArray.h"
#include "MyDocTemplate.h"
#include "grepDlg.h"
#include ".\uniq.h"
#include "MyDocManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CuniqApp

BEGIN_MESSAGE_MAP(CuniqApp, CWinApp)
	ON_UPDATE_COMMAND_UI(ID_BITMAP, OnUpdateBitmap)
	ON_COMMAND(ID_BITMAP, OnBitmap)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	// �W���̃t�@�C����{�h�L�������g �R�}���h
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// �W���̈���Z�b�g�A�b�v �R�}���h
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()


// CuniqApp �R���X�g���N�V����

CuniqApp::CuniqApp()
{
	// TODO: ���̈ʒu�ɍ\�z�p�R�[�h��ǉ����Ă��������B
	// ������ InitInstance ���̏d�v�ȏ��������������ׂċL�q���Ă��������B
}


// �B��� CuniqApp �I�u�W�F�N�g�ł��B

CuniqApp theApp;

// CuniqApp ������

BOOL CuniqApp::InitInstance()
{
	m_font.CreatePointFont( 80, _T("�l�r �S�V�b�N"));

	// �A�v���P�[�V�����@�}�j�t�F�X�g���@visual �X�^�C����L���ɂ��邽�߂ɁA
	// ComCtl32.dll �o�[�W���� 6�@�ȍ~�̎g�p���w�肷��ꍇ�́A
	// Windows XP �Ɂ@InitCommonControls() ���K�v�ł��B�����Ȃ���΁A�E�B���h�E�쐬�͂��ׂĎ��s���܂��B
	InitCommonControls();

	CWinApp::InitInstance();

	// OLE ���C�u���������������܂��B
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();
	// �W��������
	// �����̋@�\���g�킸�ɁA�ŏI�I�Ȏ��s�\�t�@�C���̃T�C�Y���k���������ꍇ�́A
	// �ȉ�����A�s�v�ȏ��������[�`����
	// �폜���Ă��������B
	// �ݒ肪�i�[����Ă��郌�W�X�g�� �L�[��ύX���܂��B
	// TODO: ���̕�������A��Ж��܂��͑g�D���Ȃǂ́A
	// �K�؂ȕ�����ɕύX���Ă��������B
	SetRegistryKey(m_pszAppName);
	LoadStdProfileSettings(15);  // �W���� INI �t�@�C���̃I�v�V���������[�h���܂� (MRU ���܂�)
	// �A�v���P�[�V�����p�̃h�L�������g �e���v���[�g��o�^���܂��B�h�L�������g �e���v���[�g
	//  �̓h�L�������g�A�t���[�� �E�B���h�E�ƃr���[���������邽�߂ɋ@�\���܂��B
	CMyDocTemplate* pDocTemplate;
	pDocTemplate = new CMyDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CuniqDoc),
		RUNTIME_CLASS(CMainFrame),       // ���C�� SDI �t���[�� �E�B���h�E
		RUNTIME_CLASS(CuniqView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);
	// DDE�Afile open �ȂǕW���̃V�F�� �R�}���h�̃R�}���h���C������͂��܂��B
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);
	// �R�}���h ���C���Ŏw�肳�ꂽ�f�B�X�p�b�` �R�}���h�ł��B�A�v���P�[�V������
	// /RegServer�A/Register�A/Unregserver �܂��� /Unregister �ŋN�����ꂽ�ꍇ�A False ��Ԃ��܂��B
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
	// ���C�� �E�B���h�E�����������ꂽ�̂ŁA�\���ƍX�V���s���܂��B
	m_pMainWnd->SetFont(&m_font,TRUE);
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();

	// �ڔ��������݂���ꍇ�ɂ̂� DragAcceptFiles ���Ăяo���Ă��������B
	//  SDI �A�v���P�[�V�����ł́AProcessShellCommand �̒���ɂ��̌Ăяo�����������Ȃ���΂Ȃ�܂���B

	m_bmp.Create();
	m_bmp.ShowWindow(SW_SHOW);
	m_bmp.SetLocate();

	return TRUE;
}


// �A�v���P�[�V�����̃o�[�W�������Ɏg���� CAboutDlg �_�C�A���O

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

	// �_�C�A���O �f�[�^
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g

	// ����
protected:
	DECLARE_MESSAGE_MAP()
public:

	afx_msg void OnBnClickedOk();
	CString m_version;
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
, m_version(_T(""))
{
	m_version.Format("%s\r\n%s\r\n", __DATE__, __TIME__ );
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_version);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)

	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()

void CuniqApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

void CuniqApp::OnUpdateBitmap(CCmdUI* pCmdUI)
{
	if(((CuniqApp*)AfxGetApp())->m_bmp.GetSafeHwnd())
		pCmdUI->SetCheck(m_bmp.IsWindowVisible() ? TRUE : FALSE );

}

void CuniqApp::OnBitmap(void)
{
	m_bmp.ShowWindow(m_bmp.IsWindowVisible() ? SW_HIDE: SW_SHOW);
	m_bmp.SetLocate();
}

void CAboutDlg::OnBnClickedOk()
{
	OnOK();
}

CDocument* CuniqApp::OpenDocumentFile(LPCTSTR lpszFileName)
{
//	CFile f;
//	CFileFind ff;
	CFileStatus fs;

//	if (! ff.FindFile(lpszFileName))
//		goto EXIT;
	if (! CFile::GetStatus(lpszFileName, fs))
		goto EXIT;
		
	if (fs.m_attribute & (CFile::volume | CFile::directory) )
		goto EXIT;

	ASSERT(m_pDocManager != NULL);
	return m_pDocManager->OpenDocumentFile(lpszFileName);
EXIT:
	CuniqGetFrame()->StatusBarSetPaneText(
		ID_INDICATOR_LASTREAD, _T("�Ǎ����s"));
	CuniqGetFrame()->StatusBarSetPaneText(
		ID_SEPARATOR, lpszFileName);

	return CuniqGetDoc();
//	AfxMessageBox(CString(lpszFileName) + _T(" : open fail"));
}


void CuniqApp::AddDocTemplate(CDocTemplate* pTemplate)	// �ǉ�
{
	if (m_pDocManager == NULL)
		m_pDocManager = new CMyDocManager;	
			// �N���X���� CDocManager ����ύX
	m_pDocManager->AddDocTemplate(pTemplate);
}

