// uniq.cpp : アプリケーションのクラス動作を定義します。
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
	// 標準のファイル基本ドキュメント コマンド
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// 標準の印刷セットアップ コマンド
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()


// CuniqApp コンストラクション

CuniqApp::CuniqApp()
{
	// TODO: この位置に構築用コードを追加してください。
	// ここに InitInstance 中の重要な初期化処理をすべて記述してください。
}


// 唯一の CuniqApp オブジェクトです。

CuniqApp theApp;

// CuniqApp 初期化

BOOL CuniqApp::InitInstance()
{
	m_font.CreatePointFont( 80, _T("ＭＳ ゴシック"));

	// アプリケーション　マニフェストが　visual スタイルを有効にするために、
	// ComCtl32.dll バージョン 6　以降の使用を指定する場合は、
	// Windows XP に　InitCommonControls() が必要です。さもなければ、ウィンドウ作成はすべて失敗します。
	InitCommonControls();

	CWinApp::InitInstance();

	// OLE ライブラリを初期化します。
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();
	// 標準初期化
	// これらの機能を使わずに、最終的な実行可能ファイルのサイズを縮小したい場合は、
	// 以下から、不要な初期化ルーチンを
	// 削除してください。
	// 設定が格納されているレジストリ キーを変更します。
	// TODO: この文字列を、会社名または組織名などの、
	// 適切な文字列に変更してください。
	SetRegistryKey(m_pszAppName);
	LoadStdProfileSettings(15);  // 標準の INI ファイルのオプションをロードします (MRU を含む)
	// アプリケーション用のドキュメント テンプレートを登録します。ドキュメント テンプレート
	//  はドキュメント、フレーム ウィンドウとビューを結合するために機能します。
	CMyDocTemplate* pDocTemplate;
	pDocTemplate = new CMyDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CuniqDoc),
		RUNTIME_CLASS(CMainFrame),       // メイン SDI フレーム ウィンドウ
		RUNTIME_CLASS(CuniqView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);
	// DDE、file open など標準のシェル コマンドのコマンドラインを解析します。
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);
	// コマンド ラインで指定されたディスパッチ コマンドです。アプリケーションが
	// /RegServer、/Register、/Unregserver または /Unregister で起動された場合、 False を返します。
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;
	// メイン ウィンドウが初期化されたので、表示と更新を行います。
	m_pMainWnd->SetFont(&m_font,TRUE);
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();

	// 接尾辞が存在する場合にのみ DragAcceptFiles を呼び出してください。
	//  SDI アプリケーションでは、ProcessShellCommand の直後にこの呼び出しが発生しなければなりません。

	m_bmp.Create();
	m_bmp.ShowWindow(SW_SHOW);
	m_bmp.SetLocate();

	return TRUE;
}


// アプリケーションのバージョン情報に使われる CAboutDlg ダイアログ

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

	// ダイアログ データ
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート

	// 実装
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
		ID_INDICATOR_LASTREAD, _T("読込失敗"));
	CuniqGetFrame()->StatusBarSetPaneText(
		ID_SEPARATOR, lpszFileName);

	return CuniqGetDoc();
//	AfxMessageBox(CString(lpszFileName) + _T(" : open fail"));
}


void CuniqApp::AddDocTemplate(CDocTemplate* pTemplate)	// 追加
{
	if (m_pDocManager == NULL)
		m_pDocManager = new CMyDocManager;	
			// クラス名を CDocManager から変更
	m_pDocManager->AddDocTemplate(pTemplate);
}

