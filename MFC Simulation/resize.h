// resize.cpp : header file
//
// Written by Torben B. Haagh (TBHaagh@hotmail.com)

#ifndef AFX_RESIZE_H__INCLUDED_
#define AFX_RESIZE_H__INCLUDED_

#include <afxtempl.h>

#define IDC_SIZEICON 0x7FFF

/////////////////////////////////////////////////////////////////////////////
// CRealRect

typedef float REAL;

struct REALRECT
{
	REAL left, top, right, bottom;
};

class CRealRect: public REALRECT
{
public:
	CRealRect();
	CRealRect(const RECT& rc);
	CRealRect(const REALRECT& rrc);
	CRealRect(REAL l, REAL t, REAL r, REAL b);

	CRealRect & operator = (const RECT& rc);
	CRealRect & operator = (const REALRECT& rrc);
	CRealRect & operator += (const REALRECT& rrc);

	CRealRect operator + (const REALRECT& rrc);
	CRealRect operator & (const REALRECT& rrc);

	bool operator == (const REALRECT& rrc);
	bool operator != (const REALRECT& rrc);

	operator CRect();

	BOOL IsNormalized();
	REAL Width();
	REAL Height();
};

/////////////////////////////////////////////////////////////////////////////
// CResize

#define SZ_RESIZE(x)	CRealRect(0,0,x,x) // resize 
#define SZ_REPOS(x)		CRealRect(x,x,x,x) // reposition 
#define SZ_HORRESIZE(x)	CRealRect(0,0,x,0) // horizontal resize
#define SZ_HORREPOS(x)	CRealRect(x,0,x,0) // horizontal reposition
#define SZ_VERRESIZE(x)	CRealRect(0,0,0,x) // vertical resize
#define SZ_VERREPOS(x)	CRealRect(0,x,0,x) // vertical reposition

struct SIZING
{
	UINT id;
	REALRECT rrc;
};

class CResize
{
public:
	CResize(CWnd* pwnd, const UINT nFlags = 0);

	void SetResize(const UINT nID, REAL left, REAL top, REAL right, REAL bottom);
	void SetResize(const UINT nID, const REALRECT& rrcSizing);
	void SetResize(SIZING arr[]);

	void SetMinSize(CSize& sz); // set the minimum size explicitly (initial size is the default)
	void SetMaxSize(CSize& sz); // set the maximum (no maximum as default)

	void SavePlacement(LPCTSTR pszSection); // save window placement to registry
	void LoadPlacement(LPCTSTR pszSection); // load saved window placement from registry

protected: // methods
	void SetResize(CWnd* pwnd, const CRealRect& rrcSizing, CRect rcWindow);

	void Init(); // called from OnInitDialog or OnInitialUpdate
	void Size(); // called from OnSize
	void GetMinMaxInfo(MINMAXINFO* pMMI); // called from OnGetMinMaxInfo

protected: // flags
	enum EFlags
	{
		SZ_NOSIZEICON			= 0x01, // do not add size icon
		SZ_NOHORISONTAL			= 0x02,	// no horizontal resizing
		SZ_NOVERTICAL			= 0x04,	// no vertical resizing
		SZ_NOMINSIZE			= 0x08,	// do not require a minimum size
		SZ_NOCLIPCHILDREN		= 0x10,	// do not set clip children style
		SZ_NOTRANSPARENTGROUP	= 0x20,	// do not set transparent style for group boxes
	};
	BOOL HasFlag(EFlags eFlag);

public: // helper classes
	class CItem
	{
	public:
		CItem(CWnd* pwnd, const CRealRect& rrcSizing, CRect& rcWindow, BOOL bDeleteWnd);
		~CItem();

		CWnd*		m_pwnd;			// window
		CRealRect	m_rrcSizing;	// sizing option
		CRealRect	m_rrcWindow;	// last control size
		BOOL		m_bDeleteWnd;	// delete the window
	};

	typedef CArray<CItem*, CItem*> CItemArray;

private: // helper methods
	BOOL Defer(HDWP, CItem*, int dx, int dy);

private: // helper data
	CWnd*		m_pwnd;			// the associative relation to the window to be resized
	UINT		m_nFlags;		// flags passed from constructor
	CItemArray	m_arrItems;		// array of controls
	CRect		m_rcWindow;		// last dialog size
	CSize		m_szMin; 		// smallest size allowed
	CSize		m_szMax;		// largest size allowed
	CStatic		m_scSizeIcon;	// size icon window
	CBitmap		m_bmSizeIcon;	// size icon bitmap
};

void AFXAPI DestructElements(CResize::CItem** pp, int n);

#include "resize.inl"

/////////////////////////////////////////////////////////////////////////////
// CResizeDialog

class CResizeDialog : public CDialog, public CResize
{
public:
	CResizeDialog(const UINT nID, CWnd* pParent = 0, const UINT nFlags = 0);

protected: // message map functions
	//{{AFX_MSG(CResizeDialog)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO *lpMMI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CResizePropertyPage

class CResizePropertyPage : public CPropertyPage, public CResize
{
	DECLARE_DYNCREATE(CResizePropertyPage)
public:
	CResizePropertyPage(const UINT nTemplate = 0, const UINT nCaption = 0, const UINT nFlags = 0);

protected: // message map functions
	//{{AFX_MSG(CResizePropertyPage)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO *lpMMI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CResizePropertySheet

class CResizePropertySheet : public CPropertySheet, public CResize
{
public:
	CResizePropertySheet(const UINT nCaption, CWnd* pParent = 0, const UINT nSelectPage = 0, const UINT nFlags = 0);
	CResizePropertySheet(LPCTSTR pszCaption, CWnd* pParent = 0, const UINT nSelectPage = 0, const UINT nFlags = 0);

	friend CResizePropertyPage;

	DWORD m_dwAddStyle, m_dwRemStyle;

protected: // message map functions
	//{{AFX_MSG(CResizePropertySheet)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO *lpMMI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
// CResizeFormView

class CResizeFormView : public CFormView, public CResize
{
	DECLARE_DYNCREATE(CResizeFormView)
public:
	CResizeFormView(const UINT nID = 0, const UINT nFlags = 0);

protected: // message map functions
	//{{AFX_MSG(CResizeFormView)
	virtual void OnInitialUpdate();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO *lpMMI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif // AFX_RESIZE_H__INCLUDED_
