// BitmapWnd.cpp : 実装ファイル
//

#include "stdafx.h"
#include "uniq.h"
#include "BitmapWnd.h"
#include ".\bitmapwnd.h"


// CBitmapWnd

IMPLEMENT_DYNAMIC(CBitmapWnd, CWnd)
CBitmapWnd::CBitmapWnd()
{
}

CBitmapWnd::~CBitmapWnd()
{
}


BEGIN_MESSAGE_MAP(CBitmapWnd, CWnd)
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MBUTTONDOWN()
	ON_WM_MBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_PAINT()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CBitmapWnd メッセージ ハンドラ


void CBitmapWnd::EnterMouseCapture(UINT nFlags, CPoint point)
{
	if( nFlags & MK_CONTROL ) return;

	ExitMouseCapture(nFlags, point);

	m_bRSelecting = TRUE;
	SetCapture();
	SetCaret(point);

}

void CBitmapWnd::ExitMouseCapture(UINT nFlags, CPoint point)
{
	if( m_bMoving == TRUE ) {
		// ドラッグ中だった場合
		m_bMoving = FALSE;
		ReleaseCapture();	
	}
	if( m_bRSelecting == TRUE ) {
		// ドラッグ中だった場合
		m_bRSelecting = FALSE;
		ReleaseCapture();	
	}
}


void CBitmapWnd::OnMButtonDown(UINT nFlags, CPoint point)
{
	EnterMouseCapture(nFlags, point);
	CWnd::OnMButtonDown(nFlags, point);
}


void CBitmapWnd::OnRButtonDown(UINT nFlags, CPoint point)
{
	EnterMouseCapture(nFlags, point);
	CWnd::OnRButtonDown(nFlags, point);
}

void CBitmapWnd::OnLButtonDown(UINT nFlags, CPoint point)
{

	if( nFlags & MK_CONTROL )
	{
		m_bMoving = TRUE;
		SetCapture();
		m_pointOld = point;
	}
	else {
		EnterMouseCapture(nFlags, point);
	}
	CWnd::OnLButtonDown(nFlags, point);
}


void CBitmapWnd::OnRButtonUp(UINT nFlags, CPoint point)
{
	ExitMouseCapture(nFlags, point);
	CWnd::OnRButtonUp(nFlags, point);
}

void CBitmapWnd::OnLButtonUp(UINT nFlags, CPoint point)
{
	ExitMouseCapture(nFlags, point);
	CWnd::OnLButtonUp(nFlags, point);
}
void CBitmapWnd::OnMButtonUp(UINT nFlags, CPoint point)
{
	ExitMouseCapture(nFlags, point);
	CWnd::OnMButtonUp(nFlags, point);
}

void CBitmapWnd::SetCaret(CPoint point)
{
	int i , m;
	if( GetHeight(i, m) )
	{
		CuniqGetView()->SetFocus();
		CuniqGetView()->SetCaretLine(i + point.y);

		Invalidate();
		CuniqGetView()->SetFocus();
	}
}

void CBitmapWnd::OnMouseMove(UINT nFlags, CPoint point)
{
	if( m_bMoving == TRUE ) {
		CRect rect;
		GetWindowRect(&rect);
		rect.left += (point.x - m_pointOld.x);
		rect.right += (point.x - m_pointOld.x);
		rect.top += (point.y - m_pointOld.y);
		rect.bottom += (point.y - m_pointOld.y);
		SetWindowPos(NULL, rect.left, rect.top,
			rect.right - rect.left, rect.bottom - rect.top,
			SWP_SHOWWINDOW|SWP_NOZORDER);
	}
	if( m_bRSelecting == TRUE ) {
		SetCaret(point);
	}
	CWnd::OnMouseMove(nFlags, point);
}

void CBitmapWnd::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);
}

BOOL CBitmapWnd::GetHeight(INT_PTR & start, INT_PTR & end)
{
	CRect rect;
	GetClientRect(rect);

	start = CuniqGetView()->GetCaretLine();
	if(start == -1) return FALSE;
	//	start /= 16;
	//	start *= 16;

	start -= rect.Height() / 3;
	if( start < 0 ) start = 0;

	end = start + rect.Height() - 1;
	return TRUE;
}

void CBitmapWnd::OnPaint()
{
	INT_PTR m;
	int i, j, x, y;

	CRect rect2;
	CRect rect;

	CDC memDC, *masterdc, *dc;
	CBitmap memBmp, *memOldBmp;
	PAINTSTRUCT     ps;



	masterdc = BeginPaint( &ps );
	dc = &memDC;
	GetClientRect(&rect);

	//仮想デバイスコンテキスト作成
	memDC.CreateCompatibleDC(masterdc);
	memBmp.CreateCompatibleBitmap(GetDC(),rect.Width(),rect.Height());
	memOldBmp = memDC.SelectObject(&memBmp);

	do {

		GetClientRect(rect);
		GetClientRect(rect2);
		CBrush br;
		br.CreateSolidBrush(RGB(225,225,225));
		CBrush br2;
		br2.CreateSolidBrush(RGB(235,235,235));
		CBrush br3;
		br3.CreateSolidBrush(RGB(255,255,255));
		dc->FillRect(rect, &br);

		x = 0;
		y = 0;

		///
		CStringArray sa;

		CuniqGetView()->ParseLines(sa);


		static	INT_PTR sm = 0;
		static	int si = 0;
		if(! GetHeight(i, m) ) { i = si; m = sm; }
		else { si = i; sm = m; }

		if( m > sa.GetCount() ) m = sa.GetCount();
		static INT_PTR sn;
		static INT_PTR snv;
		INT_PTR n =  CuniqGetView()->GetCaretLine();
		INT_PTR nv = CuniqGetView()->GetCaretView();

		if(n == -1) { n = sn; nv = snv; }
		else  { sn = n; snv = nv; }

		rect2.top = (LONG)(n - i - nv) ;
		rect2.bottom = (LONG)(n  - i + nv);
		dc->FillRect(rect2, &br2);


		rect2.top = (LONG)(n - i - 1);
		rect2.bottom = (LONG)(n  - i + 1);
		dc->FillRect(rect2, &br3);


		for(  ; i < m ; i++ )
		{
			int q = 0;
			int qq = 0;
			x = 1;
			y += 1;
			for( j = 0 ; j < sa[i].GetLength() ; j++ )
			{
				x += 1;
				if( sa[i].GetAt(j) == _T('\"') )
					qq = !qq;
				if( sa[i].GetAt(j) == _T('\'') )
					q = !q;

				if( sa[i].GetAt(j) == _T(' ') );
				else if( sa[i].GetAt(j) == _T('\t') ) {
					x += 7;
					x /= 8;
					x *= 8;
				}
				else if( sa[i].GetAt(j) >= _T('0') && sa[i].GetAt(j) <= _T('9') )
					dc->SetPixel(x,y, (q || qq)? RGB(200,120,120): RGB(0,0,255));		
				else if( 
					sa[i].GetAt(j) == _T('/') || sa[i].GetAt(j) == _T('\\') ||
					sa[i].GetAt(j) == _T('*') || sa[i].GetAt(j) == _T('+') ||
					sa[i].GetAt(j) == _T('(') || sa[i].GetAt(j) == _T(')') ||
					sa[i].GetAt(j) == _T('!')  ) {
						dc->SetPixel(x,y, (q || qq)? RGB(200,120,120): RGB(150,220,150));
					}
				else if( 
					sa[i].GetAt(j) == _T('=')) {
						dc->SetPixel(x,y, (q || qq)? RGB(200,120,120): RGB(220,220,150));
					}
				else if( 
					sa[i].GetAt(j) == _T('.') || sa[i].GetAt(j) == _T('-') ||
					sa[i].GetAt(j) == _T('_') ||
					sa[i].GetAt(j) == _T(',') ) {
						dc->SetPixel(x,y, (q || qq)? RGB(200,120,120): RGB(200,200,200));
					}
				else if( sa[i].GetAt(j) >= _T('a') && sa[i].GetAt(j) <= _T('z') )
					dc->SetPixel(x,y, (q || qq)? RGB(200,0,0): RGB(128,128,128));
				else if( sa[i].GetAt(j) >= _T('A') && sa[i].GetAt(j) <= _T('Z') )
					dc->SetPixel(x,y, (q || qq)? RGB(200,0,0): RGB(64,64,64));
				else 
					dc->SetPixel(x,y, (q || qq)? RGB(200,0,0): RGB(0,0,0));
			}

		}

	} while(0);

	//描画 仮想デバイス廃棄
	masterdc->BitBlt(0,0,rect.Width(),rect.Height(),dc,0,0,SRCCOPY);
	memDC.SelectObject(memOldBmp);
	memBmp.DeleteObject();

	EndPaint( &ps );
	//CWnd::OnPaint();
}

void CBitmapWnd::SetLocate()
{
	CRect rect;
	int w, h;

	if(CuniqGetFrame() == NULL) return;
	if(CuniqGetView() == NULL) return;

	if(CuniqGetFrame()->GetSafeHwnd() == NULL) return;
	if(CuniqGetView()->GetSafeHwnd() == NULL) return;
	if(GetSafeHwnd() == NULL) return;

	CuniqGetView()->GetWindowRect(rect);
	w = rect.Width() > 600 ? 600 : rect.Width();
	h = rect.Height();
	w /= 4;

	rect.left = rect.right - w - 4*1 - 20;
	rect.right = rect.left + w ;
	rect.top += 4*2;
	rect.bottom = h + rect.top - 4*3 - 20;

	MoveWindow(rect);
}

void CBitmapWnd::Create()
{
	DestroyWindow();

	CWnd::CreateEx(WS_EX_WINDOWEDGE | WS_EX_TOOLWINDOW,
		AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW, AfxGetApp()->LoadCursor(IDC_ARROW)),
		_T(""), 
		WS_POPUP | WS_BORDER | WS_THICKFRAME,
		CRect(0,0,0,0),
		CuniqGetView(),
		0 );
	SetLocate();
}

