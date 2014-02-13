#pragma once

// CBitmapWnd
class CBitmapWnd : public CWnd
{
	DECLARE_DYNAMIC(CBitmapWnd)

public:
	CBitmapWnd();
	virtual ~CBitmapWnd();
	void EnterMouseCapture(UINT nFlags, CPoint point);
	void ExitMouseCapture(UINT nFlags, CPoint point);
	void SetCaret(CPoint point);

	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);

protected:
	CPoint m_pointOld;
	BOOL m_bMoving;
	BOOL m_bRSelecting;
	DECLARE_MESSAGE_MAP()

public:
	void Create();
	void SetLocate();
	BOOL GetHeight(INT_PTR & start, INT_PTR & end);
};


