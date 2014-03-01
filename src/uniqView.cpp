// uniqView.cpp : CuniqView クラスの実装
//

#include "stdafx.h"
#include "uniq.h"
#include "MainFrm.h"

#include "uniqDoc.h"
#include "uniqView.h"
#include "findDlg.h"
#include "replaceDlg.h"
#include "grepDlg.h"
#include "afxtempl.h"

#include "SortableStringArray.h"
#include ".\uniqview.h"
#include <afxtempl.h>
//#include <afximpl.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CuniqView

IMPLEMENT_DYNCREATE(CuniqView, CEditView)

BEGIN_MESSAGE_MAP(CuniqView, CEditView)
	//{{AFX_MSG_MAP(CDropView)
	ON_WM_DROPFILES()
	//	ON_WM_NCLBUTTONDOWN()
	ON_WM_MOUSEWHEEL()
	ON_WM_LBUTTONDOWN()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	//	ON_WM_SYSKEYDOWN()
	ON_WM_KEYDOWN()
	ON_WM_CREATE()

	ON_COMMAND(ID_TRIMLINE, OnTrimLine)
	ON_COMMAND(ID_TRIMSPACE, OnTrimSpace)
	ON_COMMAND(ID_RETURN, OnReturn)
	ON_COMMAND(ID_NORETURN, OnNoReturn)

	ON_COMMAND(ID_GOTOP, OnGoTop)
	ON_COMMAND(ID_GOBOTTOM, OnGoBottom)
	ON_COMMAND(ID_RELOAD, OnReloadDocument)
	ON_COMMAND(ID_UNIQ, OnUniq)
	ON_COMMAND(ID_REVERSE, OnReverse)
	ON_COMMAND(ID_SORT, OnSort)
	ON_COMMAND(ID_REVERSESORT, OnReverseSort)
	ON_COMMAND(ID_SORTL, OnSortLogical)
	ON_COMMAND(ID_REVERSESORTL, OnReverseSortLogical)
	ON_COMMAND(ID_SELECTALL, OnSelectall)
	ON_COMMAND(ID_GREP, OnGrep)
	ON_COMMAND(ID_GREPV, OnGrepV)
	ON_COMMAND(ID_FIND, OnFind)
	ON_COMMAND(ID_REPLACE, OnReplace)
	ON_COMMAND(ID_UNIQR, OnUniqCols)
	ON_COMMAND(ID_PARSE, OnParseLine)

	ON_CONTROL_REFLECT(EN_HSCROLL, OnUpdate)
	ON_CONTROL_REFLECT(EN_VSCROLL, OnUpdate)
	ON_CONTROL_REFLECT(EN_SETFOCUS, OnUpdate)
	ON_CONTROL_REFLECT(EN_CHANGE, OnUpdate)
	ON_CONTROL_REFLECT(EN_UPDATE, OnUpdate)
	//}}AFX_MSG_MAP
	// 標準印刷コマンド
	ON_COMMAND(ID_FILE_PRINT, CEditView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CEditView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CEditView::OnFilePrintPreview)
	ON_UPDATE_COMMAND_UI(ID_REPLACE, OnUpdateReplace)
	ON_UPDATE_COMMAND_UI(ID_GREP, OnUpdateGrep)
	ON_UPDATE_COMMAND_UI(ID_GREPV, OnUpdateGrep)
	ON_UPDATE_COMMAND_UI(ID_FIND, OnUpdateFind)
END_MESSAGE_MAP()

// CuniqView コンストラクション/デストラクション
void CuniqView::OnDropFiles(HDROP hDropInfo) 
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください

	//ドロップされたファイルの個数を取得
	UINT uiCount=DragQueryFile(hDropInfo,~0lu,NULL,0);

	for(UINT i=0; i<uiCount; i++){
		//ファイル名の長さを取得
		UINT uiLen=DragQueryFile(hDropInfo,i,NULL,0);

		//ファイル名を取得
		CString name;
		DragQueryFile(hDropInfo,i,name.GetBuffer(uiLen+1),uiLen+1);
		name.ReleaseBuffer();

		//name(ファイル名)を使って何らかの処理を行う
		AfxGetApp()->OpenDocumentFile(name);
	}

	//	CView::OnDropFiles(hDropInfo);	//必要ないと思われるので呼ばない
}




CuniqView::CuniqView()
{
	m_FindDlg=NULL;
	m_ReplaceDlg=NULL;
	m_GrepDlg=NULL;
}

CuniqView::~CuniqView()
{	
	delete m_FindDlg;
	delete m_ReplaceDlg;
	delete m_GrepDlg;
}


void CuniqView::OnUpdate()
{
	if(((CuniqApp*)AfxGetApp())->m_bmp.GetSafeHwnd()) {
		//SetFocus();
		((CuniqApp*)AfxGetApp())->m_bmp.Invalidate();
		//((CuniqApp*)AfxGetApp())->m_bmp.UpdateWindow();
		if(!((CuniqApp*)AfxGetApp())->m_bmp.IsWindowVisible()) GetCaretLine();
	}
}
/*
void CuniqView::OnDraw( CDC* pDC )
{
ASSERT_VALID(this);

PaintWindowlessControls(pDC);
#ifdef _DEBUG
if (pDC->IsPrinting())
TRACE(traceAppMsg, 0, "Warning: CFormView does not support printing.\n");
#endif

UNUSED(pDC);     // unused in release build

CClientDC cdc(this);
CPoint p1(80,0);
CPoint p2(80,500);

cdc.MoveTo(p2);
cdc.LineTo(p1);
}
*/

// doc not const
void ParseLines(CStringArray &sa, CString& doc)
{
	int i = 0, s = 0;
	int l = doc.GetLength();
	LPTSTR buff = doc.GetBuffer(0);

	for(; i < l + 1;)
	{
		if( buff[i] == _T('\r') )
		{
			buff[i] = _T('\0');
			sa.Add(&buff[s]);

			if( buff[i + 1] == _T('\n') )
				i++;

			s = i + 1;
		}
		else if( buff[i] == _T('\n') )
		{
			buff[i] = _T('\0');
			sa.Add(&buff[s]);

			s = i + 1;
		}

		i++;
	}

	sa.Add(&buff[s]);
	doc = _T("");
}

void CuniqView::ParseLines(CStringArray &sa)
{
	CString temp;
	GetEditCtrl().GetWindowText(temp);
	::ParseLines(sa, temp);
}

void CuniqView::StoreStringArray(CStringArray& sa)
{
	int s, e;
	INT_PTR i;
	CString newtext;

	GetEditCtrl().GetSel(s,e);

	for(i=0;i<sa.GetCount();i++){
		newtext += sa[i];
		if( i != sa.GetCount() -1 ) newtext += _T("\r\n");
	}
	GetEditCtrl().SetWindowText(newtext);
	GetEditCtrl().SetSel(s,s);// un select
}

////

void CuniqView::OnFind()
{
	if(m_FindDlg == NULL || m_FindDlg->GetSafeHwnd() == NULL) {
		delete m_FindDlg;
		m_FindDlg = new CFindDlg(this);
		GetSelText(m_FindDlg->m_find);
		m_FindDlg->UpdateData(FALSE);
	}	
	m_FindDlg->ShowWindow(m_FindDlg->IsWindowVisible() ? SW_HIDE : SW_SHOW);
}

void CuniqView::OnUpdateFind(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_FindDlg && m_FindDlg->GetSafeHwnd() && m_FindDlg->IsWindowVisible());
}

void CuniqView::OnReplace()
{
	if(m_ReplaceDlg == NULL || m_ReplaceDlg->GetSafeHwnd() == NULL) {
		delete m_ReplaceDlg;
		m_ReplaceDlg = new CReplaceDlg(this);
		GetSelText(m_ReplaceDlg->m_old);
		GetSelText(m_ReplaceDlg->m_new);
		m_ReplaceDlg->UpdateData(FALSE);
	}
	m_ReplaceDlg->ShowWindow(m_ReplaceDlg->IsWindowVisible() ? SW_HIDE : SW_SHOW);
}
void CuniqView::OnUpdateReplace(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_ReplaceDlg && m_ReplaceDlg->GetSafeHwnd()  && m_ReplaceDlg->IsWindowVisible());
}

void CuniqView::OnGrep()
{
	CString sel;
	GetSelText(sel);
	if(!sel.IsEmpty())
	{
		Grep(sel);
		return;
	}

	if(m_GrepDlg == NULL || m_GrepDlg->GetSafeHwnd() == NULL) {
		delete m_GrepDlg;
		m_GrepDlg = new CGrepDlg(this);
		GetSelText(m_GrepDlg->m_grep);
		m_GrepDlg->UpdateData(FALSE);
		m_GrepDlg->SetDefID(IDC_GREP);
	}
	m_GrepDlg->ShowWindow(m_GrepDlg->IsWindowVisible() ? SW_HIDE : SW_SHOW);
}
void CuniqView::OnGrepV()
{
	CString sel;
	GetSelText(sel);
	if(!sel.IsEmpty())
	{
		GrepV(sel);
		return;
	}

	if(m_GrepDlg == NULL || m_GrepDlg->GetSafeHwnd() == NULL) {
		delete m_GrepDlg;
		m_GrepDlg = new CGrepDlg(this);
		GetSelText(m_GrepDlg->m_grep);
		m_GrepDlg->UpdateData(FALSE);
		m_GrepDlg->SetDefID(IDC_GREPV);
	}
	m_GrepDlg->ShowWindow(m_GrepDlg->IsWindowVisible() ? SW_HIDE : SW_SHOW);
}

void CuniqView::OnUpdateGrep(CCmdUI *pCmdUI)
{
	pCmdUI->SetCheck(m_GrepDlg && m_GrepDlg->GetSafeHwnd()  && m_GrepDlg->IsWindowVisible());
}





void CuniqView::OnUniqCols()
{
	int i;
	int iSubString = 0;

	CString temp;
	CStringArray sa;
	CStringArray entry;
	CString rString;
	CString buff;

	TCHAR chSep = _T(' ');

	ParseLines(sa);
	for(i=0;i<sa.GetCount();i++) {
		iSubString = 0;
		buff.Empty();
		temp.Empty();
		while(AfxExtractSubString(rString, sa[i], iSubString++, chSep))
		{
			if(rString.IsEmpty()) continue;
			if(temp != rString)
			{
				buff += rString;
				if(rString != _T(" ") )
					buff += _T(" ");
				temp = rString;
			}
		}
		if( buff.Right(1) == _T(" ") )
			buff = buff.Left(buff.GetLength() - 1);
		entry.Add(buff);
	}

	StoreStringArray(entry);
}



void CuniqView::OnParseLine()
{
	int i;
	int iSubString = 0;

	CStringArray sa;
	CStringArray entry;

	CString rString;
	CString buff;
	CString dst;
	CString format;

	TCHAR chSep = _T(' ');

	CArray<int, int> col;

	ParseLines(sa);
	for(i=0;i<sa.GetCount();i++) {
		iSubString = 0;
		while(AfxExtractSubString(rString, sa[i], iSubString++, chSep))
		{
			col.Add(0);
			if(rString.IsEmpty()) continue;
			col[iSubString - 1] = max( col[iSubString - 1], rString.GetLength() + 1);
		}
	}

	for(i=0;i<sa.GetCount();i++) {
		iSubString = 0;
		buff.Empty();
		while(AfxExtractSubString(rString, sa[i], iSubString++, chSep))
		{
			if(rString.IsEmpty()) continue;

			format.Format(_T("%%-%ds"), col[iSubString - 1] );
			dst.Format(format, rString);
			buff += dst;
		}
		entry.Add(buff);
	}

	StoreStringArray(entry);
}

void CuniqView::SaveHistory(LPCTSTR file, LPCTSTR save)
{
	CString str; 
	str = AfxGetApp()->m_pszAppName;
	str += file;//;
	CFile setting(str, CFile::modeWrite | CFile::modeCreate | CFile::modeNoTruncate );

	setting.SeekToEnd();
	setting.Write(save, _tcslen(save) );
	setting.Write(_T("\r\n"), 2 );

}

void CuniqView::Grep(LPCTSTR grep)
{
	SaveHistory(_T(".grep.txt"), grep);


	INT_PTR i, j;
	BOOL find;
	CString cg(grep);
	CStringArray sa;
	CStringArray sagrep;
	CStringArray entry;

	::ParseLines(sagrep, cg);
	ParseLines(sa);

	for(i=0;i<sa.GetCount();i++) {
		find = FALSE;
		for(j=0;j<sagrep.GetCount();j++) {
			if(sagrep[j].IsEmpty()) continue;
			if(sa[i].Find(sagrep[j]) != -1) {
				find = TRUE;
				break;
			}
		}

		if(find == TRUE) { /* !!! */
			entry.Add(sa[i]);
		}
	}

	StoreStringArray(entry);
}

void CuniqView::GrepV(LPCTSTR grep)
{

	SaveHistory(_T(".grep.txt"), grep);


	INT_PTR i, j;
	BOOL find;
	CString cg(grep);
	CStringArray sa;
	CStringArray sagrep;
	CStringArray entry;

	::ParseLines(sagrep, cg);
	ParseLines(sa);

	for(i=0;i<sa.GetCount();i++) {
		find = FALSE;
		for(j=0;j<sagrep.GetCount();j++) {
			if(sagrep[j].IsEmpty()) continue;
			if(sa[i].Find(sagrep[j]) != -1) {
				find = TRUE;
				break;
			}
		}

		if(find == FALSE) { /* !!! */
			entry.Add(sa[i]);
		}
	}

	StoreStringArray(entry);
}

void CuniqView::FindStart(LPCTSTR _find)
{
	INT_PTR i, j;
	CStringArray fa;
	CStringArray sa;
	CStringArray entry;

//	if(strFind.IsEmpty()) return;

	::ParseLines(fa, CString(_find));
	ParseLines(sa);

	if(m_FindDlg == NULL || m_FindDlg->GetSafeHwnd() == NULL) {
		delete m_FindDlg;
		m_FindDlg = new CFindDlg(this);
	}	

	LV_ITEM item;
	int f;

	item.iItem = 0;
	item.mask = LVIF_TEXT;

	for(i=0;i<sa.GetCount();i++) {
			if (sa[i].IsEmpty()) continue;
		for(j=0;j<fa.GetCount();j++) {
			if (fa[j].IsEmpty()) continue;
			f = 0;
			while( (f = sa[i].Find( fa[j], f )) != -1)
			{
				item.iItem = m_FindDlg->m_list.GetItemCount();
				//	++;
				CString str;
				str.Format(_T("%5d"), i);
				item.pszText = str.GetBuffer(0);
				item.iSubItem = 0;
				item.iItem = m_FindDlg->m_list.InsertItem(&item);

				str.Format(_T("%5d"), f);
				item.pszText = str.GetBuffer(0);
				item.iSubItem = 1;
				m_FindDlg->m_list.SetItem(&item);

				item.pszText = fa[j].GetBuffer(0);
				item.iSubItem = 2;
				m_FindDlg->m_list.SetItem(&item);

				item.pszText = sa[i].GetBuffer(0);
				item.iSubItem = 3;
				m_FindDlg->m_list.SetItem(&item);

				f += fa[j].GetLength();
			}
		}
	}

	//StoreStringArray(entry);
}

void CuniqView::Replace(LPCTSTR pszOld, LPCTSTR pszNew )
{
	CString temp;
	int s, e;

	GetEditCtrl().GetSel(s,e);

	GetEditCtrl().GetWindowText(temp);
	temp.Replace( pszOld, pszNew );
	GetEditCtrl().SetWindowText(temp);

	GetEditCtrl().SetSel(s,s);// select off
}

////

void CuniqView::OnSelectall()
{
	GetEditCtrl().SetSel(0,-1,0);
}

void CuniqView::OnTrimLine()
{
	INT_PTR i;
	CStringArray satemp;
	CStringArray sa;

	ParseLines(sa);

	for(i=0;i<sa.GetCount();i++){
		if(!sa[i].IsEmpty()){
			satemp.Add(sa[i]);
		}
	}

	StoreStringArray(satemp);
}

void CuniqView::OnReturn()
{
	INT_PTR i;
	CStringArray sa;
	CStringArray satemp;

	int s, e;
	GetEditCtrl().GetSel(s,e);
	s -= GetEditCtrl().LineIndex(GetEditCtrl().LineFromChar(s));
	if( s <= 0 ) return;

	ParseLines(sa);

	for(i=0;i<sa.GetCount();i++){
		CString rString;
		LPCTSTR p = sa[i];
		int size = sa[i].GetLength();

		/*
		非漢字              0x8140-0x84BE
		第一水準            0x889F-0x9872
		第二水準            0x989F-0x9FFC, 0xE040-0xEAA4
		NEC 特殊文字 	    0x8740-0x879C
		NEC選定IBM拡張文字 	0xED40-0xEEFC
		IBM選定IBM拡張文字 	0xFA40-0xFC4B
		*/
#define SJIS(c) (((((unsigned char)c)>=0x81)&&(((unsigned char)c)<=0x9f))||((((unsigned char)c)>=0xe0)&&(((unsigned char)c)<=0xfc)))

		if(size == 0)
			satemp.Add(_T(""));
		else
			while( size > 0 )
			{
				int nSpl = s;
				int nLen = size > nSpl ? nSpl : size;

				if (nLen == nSpl)
				{
					while(((unsigned)p[nLen-1])&0x80) {
						if(nLen == 0) break; 
						nLen--;
					}
					nLen += ((nSpl - nLen) / 2 * 2); //二の倍数になるよ
				}
				// cation if nSpl is 1 then nLen is 0;
				if(nLen==0) {
					nLen = 2;
				}

				memcpy(rString.GetBufferSetLength(nLen), p, nLen*sizeof(TCHAR));
				satemp.Add(rString);
				p += nLen;
				size -= nLen;
			}
	}


	StoreStringArray(satemp);
}

void CuniqView::OnNoReturn()
{
	INT_PTR i;
	CString newtext;
	CStringArray sa;

	ParseLines(sa);

	for(i=0;i<sa.GetCount();i++){
		newtext += sa[i];
		newtext += _T(" ");
	}
	if( newtext.Right(1) == _T(" ") )
		newtext = newtext.Left(newtext.GetLength() - 1);
	GetEditCtrl().SetWindowText(newtext);
}

void CuniqView::OnTrimSpace()
{
	INT_PTR i;
	CStringArray satemp;
	CStringArray sa;

	ParseLines(sa);

	for(i=0;i<sa.GetCount();i++){
		for(LPCTSTR p = sa[i];*p!=_T('\0');p++)
		{
			if(*p==_T(' ')) continue;
			if(*p==_T('\t')) continue;
			break;
		}
		satemp.Add(p);
	}

	StoreStringArray(satemp);
}

void CuniqView::OnUniq()
{
	INT_PTR i;
	CStringArray sauniq;
	CString temp;
	CStringArray satemp;

	ParseLines(sauniq);

	//uniq
	if( sauniq.GetCount() > 0 )
	{
		temp = sauniq[0];
		satemp.Add(sauniq[0]);

		for(i=1;i<sauniq.GetCount();i++){
			if(temp != sauniq[i]){
				temp = sauniq[i];
				satemp.Add(sauniq[i]);
			}
		}

		StoreStringArray(satemp);
	}
}

void CuniqView::OnSort()
{
	CSortableStringArray sa;

	ParseLines(sa);
	sa.Sort();

	StoreStringArray(sa);
}


void CuniqView::OnReverseSort()
{
	CSortableStringArray sa;
	ParseLines(sa);
	sa.Sort(CSortableStringArray::ReverseSort);
	StoreStringArray(sa);
}

void CuniqView::OnSortLogical()
{
	CSortableStringArray sa;
	ParseLines(sa);
	sa.Sort(CSortableStringArray::Logical);
	StoreStringArray(sa);
}

void CuniqView::OnReverseSortLogical()
{
	CSortableStringArray sa;
	ParseLines(sa);
	sa.Sort(CSortableStringArray::ReverseLogical);
	StoreStringArray(sa);
}

void CuniqView::OnReverse()
{
	INT_PTR i;
	CStringArray sa;
	CStringArray reverse;
	ParseLines(sa);
	for(i=sa.GetCount() -1;i>=0;i--){
		reverse.Add(sa[i]);
	}
	StoreStringArray(reverse);
}


////
BOOL CuniqView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: この位置で CREATESTRUCT cs を修正して Window クラスまたはスタイルを
	//  修正してください。

	BOOL bPreCreated = CEditView::PreCreateWindow(cs);
	cs.dwExStyle |= WS_EX_ACCEPTFILES;	//ファイルのドロップを受け付ける

	//	cs.style &= ~(ES_AUTOHSCROLL|WS_HSCROLL);	// ワード ラップを使用可能にします

	return bPreCreated;
}


// CuniqView 印刷

BOOL CuniqView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// デフォルトの CEditView の印刷準備
	return CEditView::OnPreparePrinting(pInfo);
}

void CuniqView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	// デフォルトの CEditView で印刷を開始します。
	CEditView::OnBeginPrinting(pDC, pInfo);
}

void CuniqView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	// デフォルトの CEditView の印刷後の後処理
	CEditView::OnEndPrinting(pDC, pInfo);
}


// CuniqView 診断

#ifdef _DEBUG
void CuniqView::AssertValid() const
{
	CEditView::AssertValid();
}

void CuniqView::Dump(CDumpContext& dc) const
{
	CEditView::Dump(dc);
}

CuniqDoc* CuniqView::GetDocument() const // デバッグ以外のバージョンはインラインです。
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CuniqDoc)));
	return (CuniqDoc*)m_pDocument;
}
#endif //_DEBUG

void CuniqView::OnReloadDocument()
{
	int s, e;

	GetEditCtrl().GetSel(s,e);

	GetDocument()->OnOpenDocument(GetDocument()->GetPathName());

	GetEditCtrl().SetSel(s,e);
}

// CuniqView メッセージ ハンドラ
void CuniqView::OnLButtonDown(UINT nFlags, CPoint point)
{

	CEditView::OnLButtonDown(nFlags, point);

	OnUpdate();
}

BOOL CuniqView::OnMouseWheel(UINT nFlags, short zDelta, CPoint point)
{
	BOOL result = CEditView::OnMouseWheel(nFlags, zDelta, point);
	//OnUpdate();
	return result;
}

void CuniqView::OnNcLButtonDown(UINT nFlags, CPoint point)
{
	CEditView::OnNcLButtonDown(nFlags, point);
	OnUpdate();
}


void CuniqView::OnGoTop()
{
	GetEditCtrl().SetSel(0,0);
}

void CuniqView::OnGoBottom()
{
	CString str;
	GetEditCtrl().GetWindowText(str);
	GetEditCtrl().SetSel(str.GetLength(),str.GetLength());
}

void CuniqView::OnVScroll( UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	CEditView::OnVScroll(nSBCode, nPos, pScrollBar);
	OnUpdate();
}

void CuniqView::OnHScroll( UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	CEditView::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CuniqView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CEditView::OnKeyDown(nChar ,nRepCnt, nFlags);
	if(nChar == VK_DOWN || nChar == VK_UP || nChar == VK_LEFT || nChar == VK_RIGHT )
		OnUpdate();
}

///

int CuniqView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CEditView::OnCreate(lpCreateStruct) == -1)
		return -1;

	/* ドロップターゲットへの登録 */
	//	if (!m_dropTarget.Register(this))
	//	return -1;

	return 0;
}


//DROPEFFECT CuniqView::OnDragEnter(COleDataObject* pDataObject, 
//								  DWORD dwKeyState, CPoint point) 
//{
//	OnUpdate();
//	return CEditView::OnDragEnter(pDataObject, dwKeyState, point);
//
//}
//DROPEFFECT CuniqView::OnDragOver(COleDataObject* pDataObject, 
//								 DWORD dwKeyState, CPoint point) 
//{
//	OnUpdate();
//	return CEditView::OnDragOver(pDataObject, dwKeyState, point);
//}


//BOOL CuniqView::OnDrop(COleDataObject* pDataObject,
//					   DROPEFFECT dropEffect, CPoint point) 
//{
//	OnUpdate();
//	return CEditView::OnDrop(pDataObject, dropEffect, point);
//}


///


void CuniqView::SetCaretLine(int l)
{
	int c = GetEditCtrl().LineIndex(l);
	GetEditCtrl().SetSel(c,c);
}

INT_PTR CuniqView::GetCaretLine()
{
	int s, e;
	GetEditCtrl().GetSel(s, e);
	int n = GetEditCtrl().CharFromPos(GetEditCtrl().GetCaretPos());
	if(n == -1 ) return -1;
	int nLineIndex = HIWORD(n);
	int nCharIndex = LOWORD(n);
	nCharIndex = CharFromPos(GetEditCtrl().GetCaretPos());

	CString str;
	str.Format(_T("Line=%d, Char=%d   (%d bytes+a)"),
		nLineIndex,
		nCharIndex - (GetEditCtrl().LineIndex(nLineIndex) ),
		//	nCharIndex - (GetEditCtrl().LineIndex(nLineIndex) % 0x10000 ),
		nCharIndex
		);
	((CMainFrame*)AfxGetMainWnd())->StatusBarSetPaneText(ID_SEPARATOR, str);
	return nLineIndex;
}

INT_PTR CuniqView::GetCaretView()
{
	CRect rect;
	GetClientRect(rect);

	return rect.Height() / 15;
}


////////////////////


/*
1. 現在の選択文字列を取得する関数

現在選択されている文字列を取得するケースは結構あるのですが、
CEditView のメンバ関数 GetSelectedText では、
最初の改行文字までの文字列しか取得できません。
MFC のソースを眺めてみるとこの関数は検索・置換関係でのみ
使われており、検索・置換ダイアログでは改行入力が出来ない
のでこういう仕様なのでしょうか？あまり汎用的な
使用を前提として書かれているわけではなさそうです。
そこで、独自に選択文字列を取得する関数を書いておきます。
ちなみに、CuniqView は CEditView からの派生クラスです。
*/


// 選択テキストを取得
BOOL CuniqView::GetSelText(CString& s) const
{
	int ss, se;
	GetEditCtrl().GetSel(ss, se);
	int nLen = se - ss;
	if (nLen == 0) return FALSE;
	LPCTSTR pText = LockBuffer();
	LPTSTR pBuf = s.GetBufferSetLength(nLen+1);
	lstrcpyn(pBuf, pText+ss, nLen+1);
	s.ReleaseBuffer();
	UnlockBuffer();
	return TRUE;
}



BOOL CuniqView::GetMidText(CString& s, int nStart, int nLen) const
{
	if (nLen <= 0) return FALSE;
	LPCTSTR pText = LockBuffer();
	LPTSTR pBuf = s.GetBufferSetLength(nLen+1);
	lstrcpyn(pBuf, pText+nStart, nLen+1);
	s.ReleaseBuffer();
	UnlockBuffer();
	return TRUE;
}

//e.g. http://homepage3.nifty.com/aokura/tips/edit1.html
// 指定位置に最も近い文字のインデックスを得る
int CuniqView::CharFromPos(CPoint pt) const
{
	CEdit& edit = GetEditCtrl();
	int start = edit.GetFirstVisibleLine();

	CDC* pDC = edit.GetDC();
	CFont* pOldFont = pDC->SelectObject(edit.GetFont());
	CRect r;
	edit.GetRect(&r);
	CSize size = pDC->GetTextExtent(_T("Z"), 1);
	int dy = (pt.y - r.top) / size.cy;
	int nChar = edit.LineIndex(start + dy);
	if (nChar == -1)
		nChar = GetWindowTextLength();
	else
	{
		int nLen = edit.LineLength(nChar);
		CPoint p1, p2, p3;
		int j, d1, d2;
		BOOL flag = FALSE;
		for (j=0; j<nLen; j++)
		{
			if (j==0)
			{
				p2 = PosFromChar(nChar+j);
				p3 = PosFromChar(nChar+j+1);
				d1 = p2.x - r.left;
				d2 = p3.x - (p3.x - p2.x) / 2;
			}
			else
			{
				p1 = PosFromChar(nChar+j-1);
				p2 = PosFromChar(nChar+j);
				p3 = PosFromChar(nChar+j+1);
				d1 = p2.x - (p2.x - p1.x) / 2;
				d2 = p3.x - (p3.x - p2.x) / 2;
			}
			if (pt.x >= d1 && pt.x < d2)
			{
				nChar += j;
				flag = TRUE;
				break;
			}
		}
		if (!flag) nChar += nLen;
	}
	pDC->SelectObject(pOldFont);
	edit.ReleaseDC(pDC);

#ifdef _MBCS
	LPCTSTR lpszText = LockBuffer();
	ASSERT(lpszText != NULL);
	if (_ismbstrail((const unsigned char *)lpszText, 
		(const unsigned char *)(lpszText+nChar)))
		nChar--;
	UnlockBuffer();
#endif

	return nChar;
}

// 文字インデックスから位置を得る
CPoint CuniqView::PosFromChar(int nChar) const
{
	CEdit& edit = GetEditCtrl();
	CPoint pt;
	pt = edit.PosFromChar(nChar);
	if (pt.x != -1 || pt.y != -1) return pt;

	TRACE(_T("%ld(%ld,%ld)-"), nChar, pt.x, pt.y);

	if (nChar <= 0)
	{
		CRect r;
		edit.GetRect(&r);
		pt.x = r.left;
		pt.y = r.top;
	}
	else
	{
		CDC* pDC = edit.GetDC();
		CFont* pOldFont = pDC->SelectObject(edit.GetFont());
		TEXTMETRIC tm;
		pDC->GetTextMetrics(&tm);

		int nLine = edit.LineFromChar(nChar-1);
		int nStart = edit.LineIndex(nLine);
		int nLen = edit.LineLength(nChar-1);
		if (nChar == nStart + nLen)
		{
			LPTSTR pBuf = new TCHAR [nLen+1];
			edit.GetLine(nLine, pBuf, nLen);
			pBuf[nLen] = _T('\0');
			int nTab = tm.tmAveCharWidth * m_nTabStops / 4;
			CSize size = pDC->GetTabbedTextExtent(
				&(pBuf[nLen-1]),1,1,&nTab);
			delete [] pBuf;
			pt = edit.PosFromChar(nChar-1);
			pt.x += size.cx;
		}
		else
		{
			pt = edit.PosFromChar(nStart);
			pt.y += tm.tmHeight;
		}

		pDC->SelectObject(pOldFont);
		edit.ReleaseDC(pDC);
	}

	TRACE(_T("(%ld,%ld)\n"), pt.x, pt.y);

	return pt;
}

// 可視行の範囲を検索する
void CuniqView::VisibleSel( int& s, int& e)
{
	CRect editRect;
	CEdit& edit = GetEditCtrl();

	GetClientRect(editRect);
	edit.GetRect(&editRect);
	e = -1;
	s = edit.GetFirstVisibleLine();
	for(int cnt = s; cnt <= edit.GetLineCount(); cnt++)
	{
		e    = edit.LineIndex(cnt);
		CPoint chaPt = edit.PosFromChar(e);

		//文字の先頭が表示範囲より外の場合
		if(chaPt.y >= editRect.bottom)
		{
			break;
		}        
	}

}


void ReOpen(CArchive& ar, UINT nLen, LPVOID & hText, LPTSTR & lpszText)
{
	BOOL isUnicode = FALSE;
	LPSTR target = (LPSTR)lpszText;
	for(UINT i = 0 ; i < nLen; i++)
	{
		if(target[i * sizeof(TCHAR)] == 0x00 )
		{
			isUnicode = TRUE;
			break;
		}
	}

#if UNICODE
	if(! isUnicode){

		// ascii to unicode

		nLen = ::MultiByteToWideChar(CP_UTF8, 0, target, -1, NULL, 0);
		hText2  = LocalAlloc(LMEM_MOVEABLE, static_cast<UINT>(
			::ATL::AtlMultiplyThrow(static_cast<UINT>(nLen+1),static_cast<UINT>(sizeof(WCHAR)))
			));
		lpszText = (LPTSTR)LocalLock(hText2);
		::MultiByteToWideChar(CP_UTF8, 0, target, -1, lpszText, nLen);
		LocalUnlock(hText);
		LocalFree(hText);
		hText = hText2;

	}
#else
	{
		// ascii to unicode
		/*
		nLen = ::MultiByteToWideChar(CP_UTF8, 0, target, -1, NULL, 0);
		hText2  = LocalAlloc(LMEM_MOVEABLE, static_cast<UINT>(
		::ATL::AtlMultiplyThrow(static_cast<UINT>(nLen+1),static_cast<UINT>(sizeof(WCHAR)))
		));
		lpszText = (LPTSTR)LocalLock(hText2);
		::MultiByteToWideChar(CP_UTF8, 0, target, -1, (LPWSTR)(lpszText), nLen);
		LocalUnlock(hText);
		LocalFree(hText);
		hText = hText2;
		*/
		if(isUnicode){

			// utf8 to mbcs
			LPWSTR target = (LPWSTR)lpszText;

			nLen = ::WideCharToMultiByte(CP_UTF8,
				0, target, -1, NULL, 0, NULL, NULL);
			LPVOID hText2 = LocalAlloc(LMEM_MOVEABLE, (nLen+1)*sizeof(WCHAR));
			if (hText2 == NULL)
				AfxThrowMemoryException();

			lpszText = (LPTSTR)LocalLock(hText2);
			::WideCharToMultiByte(CP_UTF8,
				0, target, -1, lpszText, nLen, NULL, NULL);
			LocalUnlock(hText);
			LocalFree(hText);
			hText = hText2;

		}
	}
#endif
}

//#include "C:\Program Files\Microsoft Visual Studio .NET 2003\Vc7\atlmfc\src\mfc\afximpl.h"

struct AUX_DATA
{
	// system metrics
	int cxVScroll, cyHScroll;
	int cxIcon, cyIcon;

	int cxBorder2, cyBorder2;

	// device metrics for screen
	int cxPixelsPerInch, cyPixelsPerInch;

	// convenient system color
	HBRUSH hbrWindowFrame;
	HBRUSH hbrBtnFace;

	// color values of system colors used for CToolBar
	COLORREF clrBtnFace, clrBtnShadow, clrBtnHilite;
	COLORREF clrBtnText, clrWindowFrame;

	// standard cursors
	HCURSOR hcurWait;
	HCURSOR hcurArrow;
	HCURSOR hcurHelp;       // cursor used in Shift+F1 help

	// special GDI objects allocated on demand
	HFONT   hStatusFont;
	HFONT   hToolTipsFont;
	HBITMAP hbmMenuDot;

	// other system information
	BOOL    bWin95;         // TRUE if Windows 95 (not NT)

	// Implementation
	AUX_DATA();
	~AUX_DATA();
	void UpdateSysColors();
	void UpdateSysMetrics();
};


extern AFX_DATA AUX_DATA afxData;


void CuniqView::ReadFromArchive(CArchive& ar, UINT nLen)

{
	ASSERT_VALID(this);

	LPVOID hText = LocalAlloc(LMEM_MOVEABLE, (nLen+1)*sizeof(TCHAR));
	if (hText == NULL)
		AfxThrowMemoryException();

	LPTSTR lpszText = (LPTSTR)LocalLock(hText);
	ASSERT(lpszText != NULL);
	if (ar.Read(lpszText, nLen*sizeof(TCHAR)) != nLen*sizeof(TCHAR))
	{
		LocalUnlock(hText);
		LocalFree(hText);
		AfxThrowArchiveException(CArchiveException::endOfFile);
	}
	// Replace the editing edit buffer with the newly loaded data
	lpszText[nLen] = '\0';



	ReOpen(ar, nLen, hText, lpszText);

	if (afxData.bWin95)
	{
		// set the text with SetWindowText, then free
		BOOL bResult = ::SetWindowText(m_hWnd, lpszText);
		LocalUnlock(hText);
		LocalFree(hText);

		// make sure that SetWindowText was successful
		if (!bResult || ::GetWindowTextLength(m_hWnd) < (int)nLen)
			AfxThrowMemoryException();

		// remove old shadow buffer
		delete[] m_pShadowBuffer;
		m_pShadowBuffer = NULL;
		m_nShadowSize = 0;

		ASSERT_VALID(this);
		return;
	}

	LocalUnlock(hText);
	HLOCAL hOldText = GetEditCtrl().GetHandle();
	ASSERT(hOldText != NULL);
	LocalFree(hOldText);
	GetEditCtrl().SetHandle((HLOCAL)hText);
	Invalidate();
	ASSERT_VALID(this);
}

void CuniqView::Serialize(CArchive& ar)
{
	ASSERT_VALID(this);
	if (ar.IsStoring())
	{	// storing code
		WriteToArchive(ar);
	}
	else
	{	// loading code
		CFile* pFile = ar.GetFile();
		ASSERT(pFile->GetPosition() == 0);
		ULONGLONG nFileSize = pFile->GetLength();
		if (nFileSize/sizeof(TCHAR) > GetEditCtrl().GetLimitText())
		{
			// Larger than edit control limit. Call SetLimitText() to set your own max size.
			// Refer to documentation for EM_LIMITTEXT for max sizes for your target OS.
			AfxMessageBox(AFX_IDP_FILE_TOO_LARGE);
			AfxThrowUserException();
		}
		// ReadFromArchive takes the number of characters as argument
		ReadFromArchive(ar, (UINT)nFileSize/sizeof(TCHAR));

		CStringArray sa;
		ParseLines(sa);
		StoreStringArray(sa);
	}
}


