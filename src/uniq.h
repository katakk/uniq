// uniq.h : uniq �A�v���P�[�V�����̃��C�� �w�b�_�[ �t�@�C��
//
#pragma once

#ifndef __AFXWIN_H__
	#error ���̃t�@�C���� PCH �Ɋ܂߂�O�ɁA'stdafx.h' ���܂߂Ă��������B
#endif

#include "resource.h"       // ���C�� �V���{��
#include "BitmapWnd.h"

#include "MainFrm.h"

#include "uniqDoc.h"
#include "uniqView.h"

//class CBitmapWnd;
// CuniqApp:
// ���̃N���X�̎����ɂ��ẮAuniq.cpp ���Q�Ƃ��Ă��������B
//

class CuniqApp : public CWinApp
{
public:
	CuniqApp();



	afx_msg void OnUpdateBitmap(CCmdUI* pCmdUI);
	afx_msg void OnBitmap(void);
	CBitmapWnd m_bmp;


// �I�[�o�[���C�h
public:
	virtual BOOL InitInstance();
	CFont m_font;

// ����
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()

public:
//	void ParseLines(CStringArray &sa);
//	void StoreStringArray(CStringArray& sa);
//	CEdit& GetEditCtrl() const;

protected:
public:
	CDocument* OpenDocumentFile(LPCTSTR lpszFileName);
	virtual void AddDocTemplate(CDocTemplate* pTemplate);
};

inline CMainFrame * CuniqGetFrame() { return (CMainFrame*)AfxGetMainWnd(); }
inline CuniqView * CuniqGetView() { return (CuniqView*)CuniqGetFrame()->GetActiveView(); }
inline CuniqDoc * CuniqGetDoc() { return (CuniqDoc*)CuniqGetFrame()->GetActiveDocument(); }



extern CuniqApp theApp;
