// resize.cpp : implementation file
//
// Written by Torben B. Haagh (TBHaagh@hotmail.com)

#include "stdafx.h"
#include "resize.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#pragma warning(disable: 4355) // 'this' : used in base member initializer list

#ifndef OBM_SIZE
#define OBM_SIZE 32766
#endif

#define ENTRY_WINDOWPLACEMENT _T("WindowPlacement")

/////////////////////////////////////////////////////////////////////////////
// CResize

CResize::CResize(CWnd* pwnd, const UINT nFlags) : 
	m_pwnd(pwnd),
	m_nFlags(nFlags),
	m_szMin(0,0),
	m_szMax(0,0),
	m_rcWindow(0,0,0,0)
{

}

void CResize::SetResize(const UINT nID, const REALRECT& rrcSizing)
{
	CItem* pItem = 0;
	// search for an item with the given id
	for (int i=m_arrItems.GetSize(); i--; )
	{
		if (m_arrItems[i]->m_pwnd->GetDlgCtrlID() == (int) nID) pItem = m_arrItems[i];
	}
	// if we didn't find it then create one
	if (pItem == 0)
	{
		HWND hwnd = ::GetDlgItem(m_pwnd->m_hWnd, nID);
		CWnd* pwnd = m_pwnd->FromHandlePermanent(hwnd);
		BOOL bPermanentWnd = (pwnd != 0);
		if (!bPermanentWnd)
		{
			// there is no permanent window, create one
			pwnd = new CWnd; // CItem handles the delete
			pwnd->Attach(hwnd);
		}

		CRect rcWindow;
		pwnd->GetWindowRect(rcWindow);
		m_pwnd->ScreenToClient(rcWindow);

		m_arrItems.Add(pItem = new CItem(pwnd, rrcSizing, rcWindow, !bPermanentWnd));

		if (!HasFlag(SZ_NOTRANSPARENTGROUP))
		{
			// if the control is a group box, then make it transparent
			TCHAR pszClassName[8];
			::GetClassName(hwnd, pszClassName, 8);
			DWORD dwStyle = pwnd->GetStyle();
			if (_tcsicmp(pszClassName, _T("Button")) == 0 && (dwStyle & BS_GROUPBOX) && !(dwStyle & WS_TABSTOP))
			{
				pwnd->ModifyStyleEx(0, WS_EX_TRANSPARENT, SWP_NOMOVE|SWP_NOSIZE|SWP_NOZORDER|SWP_NOACTIVATE);
			}
		}
	}
	else
	{
		// the item already exists, just add the new sizing option
		pItem->m_rrcSizing += rrcSizing;
	}
	// we should only allow sizing within the rect {0,0,1,1}
	ASSERT((CRealRect(0,0,1,1) & pItem->m_rrcSizing) == pItem->m_rrcSizing);
}

void CResize::Init()
{
	// get the dialog size
	m_pwnd->GetWindowRect(m_rcWindow);
	if (!HasFlag(SZ_NOMINSIZE)) 
	{
		// set minimum size to current window size
		m_szMin.cx = m_rcWindow.Width(); 
		m_szMin.cy = m_rcWindow.Height(); 
	}
	// set the clipchildren style to prevent flickering
	if (!HasFlag(SZ_NOCLIPCHILDREN)) 
	{
		m_pwnd->ModifyStyle(0, WS_CLIPCHILDREN);
	}
	// add the size icon
	if (!HasFlag(SZ_NOSIZEICON))
	{
		m_bmSizeIcon.LoadOEMBitmap(OBM_SIZE);
		m_scSizeIcon.Create(0, WS_CHILD | WS_VISIBLE | SS_BITMAP, CRect(0,0,16,16), m_pwnd, IDC_SIZEICON);
		m_scSizeIcon.SetBitmap(m_bmSizeIcon);

		CRect rcIcon, rcClient;
		m_pwnd->GetClientRect(rcClient);
		m_scSizeIcon.GetClientRect(rcIcon);
		m_scSizeIcon.SetWindowPos(&CWnd::wndTop, rcClient.right-rcIcon.Width(), rcClient.bottom-rcIcon.Height(), 0, 0, SWP_NOSIZE);

		SetResize(IDC_SIZEICON, SZ_REPOS(1));
	}
}

void CResize::Size()
{
	if (m_arrItems.GetSize() > 0)
	{
		CRect rcWindow;
		m_pwnd->GetWindowRect(rcWindow);

		int dx = rcWindow.Width() - m_rcWindow.Width();
		int dy = rcWindow.Height() - m_rcWindow.Height();

		BOOL bDefered = TRUE;
		HDWP hdwp = BeginDeferWindowPos(m_arrItems.GetSize());
		for (int i=0; bDefered && i<m_arrItems.GetSize(); i++)
		{
			bDefered = Defer(hdwp, m_arrItems[i], dx, dy);
		}
		if (bDefered) EndDeferWindowPos(hdwp);

		m_rcWindow = rcWindow;
	}
}

BOOL CResize::Defer(HDWP hdwp, CItem* pItem, int dx, int dy)
{
	CRealRect rrcItem = pItem->m_rrcWindow;
	rrcItem.left += dx * pItem->m_rrcSizing.left;
	rrcItem.top += dy * pItem->m_rrcSizing.top;
	rrcItem.right += dx * pItem->m_rrcSizing.right;
	rrcItem.bottom += dy * pItem->m_rrcSizing.bottom;
	
	if (rrcItem != pItem->m_rrcWindow)
	{
		pItem->m_rrcWindow = rrcItem;
	
		HWND hwnd = pItem->m_pwnd->m_hWnd;
		if (::IsWindow(hwnd))
		{
			int x = (int) rrcItem.left, y = (int) rrcItem.top;
			int cx = (int) rrcItem.Width(), cy = (int) rrcItem.Height();
			if (DeferWindowPos(hdwp, hwnd, 0, x, y, cx, cy, SWP_NOZORDER) ==  0)
			{
				TRACE("DeferWindowPos failed for ID %i\n", GetDlgCtrlID(hwnd));
				return FALSE;
			}
		}
	}
	return TRUE;
}

void CResize::GetMinMaxInfo(MINMAXINFO* pMMI)
{
	if (!m_rcWindow.IsRectNull())
	{
		if (HasFlag(SZ_NOHORISONTAL))
		{
			pMMI->ptMaxTrackSize.x = pMMI->ptMaxSize.x = m_rcWindow.Width();
		}
		if (HasFlag(SZ_NOVERTICAL))
		{
			pMMI->ptMaxTrackSize.y = pMMI->ptMaxSize.y = m_rcWindow.Height();
		}
	}
	if (m_szMin.cx != 0) pMMI->ptMinTrackSize.x = m_szMin.cx;
	if (m_szMin.cy != 0) pMMI->ptMinTrackSize.y = m_szMin.cy;
	if (m_szMax.cx != 0) pMMI->ptMaxTrackSize.x = m_szMax.cx;
	if (m_szMax.cy != 0) pMMI->ptMaxTrackSize.y = m_szMax.cy;
}

void CResize::LoadPlacement(LPCTSTR pszSection)
{
	UINT nBytes = 0;
	BYTE* pBytes = 0;
	AfxGetApp()->GetProfileBinary(pszSection, ENTRY_WINDOWPLACEMENT, &pBytes, &nBytes);
	if (nBytes == sizeof(WINDOWPLACEMENT))
	{
		m_pwnd->SetWindowPlacement((WINDOWPLACEMENT*) pBytes);
	}
	if (pBytes && nBytes) delete[] pBytes;

}

void CResize::SavePlacement(LPCTSTR pszSection)
{
	WINDOWPLACEMENT wp;
	m_pwnd->GetWindowPlacement(&wp);
	AfxGetApp()->WriteProfileBinary(pszSection, ENTRY_WINDOWPLACEMENT, (BYTE*) &wp, sizeof(wp));
}

void AFXAPI DestructElements(CResize::CItem** pp, int n) 
{ 
	while (n--) delete *pp++; 
}

/////////////////////////////////////////////////////////////////////////////
// CResizeDialog

CResizeDialog::CResizeDialog(const UINT nID, CWnd* pParent, const UINT nFlags) : 
	CDialog(nID, pParent),
	CResize(this, nFlags)
{

}

BEGIN_MESSAGE_MAP(CResizeDialog, CDialog)
	//{{AFX_MSG_MAP(CResizeDialog)
	ON_WM_SIZE()
	ON_WM_GETMINMAXINFO()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CResizeDialog::OnInitDialog() 
{
	ASSERT(GetStyle() & WS_THICKFRAME); // Needs resizable frame

	CDialog::OnInitDialog();
	CResize::Init();

	return TRUE;
}

void CResizeDialog::OnGetMinMaxInfo(MINMAXINFO* pMMI)
{
	CResize::GetMinMaxInfo(pMMI);
}

void CResizeDialog::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	CResize::Size();
}

/////////////////////////////////////////////////////////////////////////////
// CResizePropertyPage

IMPLEMENT_DYNCREATE(CResizePropertyPage, CPropertyPage)

CResizePropertyPage::CResizePropertyPage(const UINT nTemplate, const UINT nCaption, const UINT nFlags) : 
	CPropertyPage(nTemplate, nCaption),
	CResize(this, nFlags | SZ_NOSIZEICON)
{

}

BEGIN_MESSAGE_MAP(CResizePropertyPage, CPropertyPage)
	//{{AFX_MSG_MAP(CResizePropertyPage)
	ON_WM_SIZE()
	ON_WM_GETMINMAXINFO()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CResizePropertyPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	CResize::Init();

	return TRUE;
}

void CResizePropertyPage::OnGetMinMaxInfo(MINMAXINFO* pMMI)
{
	CResize::GetMinMaxInfo(pMMI);
}

void CResizePropertyPage::OnSize(UINT nType, int cx, int cy)
{
	CPropertyPage::OnSize(nType, cx, cy);
	CResize::Size();
}

/////////////////////////////////////////////////////////////////////////////
// CResizePropertySheet

CResizePropertySheet::CResizePropertySheet(const UINT nCaption, CWnd* pParent, const UINT nSelectPage, const UINT nFlags) :
	CPropertySheet(nCaption, pParent, nSelectPage),
	CResize(this, nFlags)
{
	m_dwRemStyle = m_dwAddStyle = 0;
}

CResizePropertySheet::CResizePropertySheet(LPCTSTR pszCaption, CWnd* pParent, const UINT nSelectPage, const UINT nFlags) :
	CPropertySheet(pszCaption, pParent, nSelectPage),
	CResize(this, nFlags)
{
	m_dwRemStyle = m_dwAddStyle = 0;
}

BEGIN_MESSAGE_MAP(CResizePropertySheet, CPropertySheet)
	//{{AFX_MSG_MAP(CResizePropertySheet)
	ON_WM_SIZE()
	ON_WM_GETMINMAXINFO()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CResizePropertySheet::OnInitDialog() 
{
	// add user set style and resizable frame
	ModifyStyle(m_dwRemStyle, m_dwAddStyle | WS_THICKFRAME);

	CPropertySheet::OnInitDialog();

	if (!HasFlag(SZ_NOSIZEICON))
	{
		// the size icon is too close to the buttons, so inflate the sheet
		CRect rc;
		GetWindowRect(rc);
		rc.InflateRect(1,1);
		MoveWindow(rc);
	}

	// add sizing entries to the system menu
	if (CMenu* pSysMenu = GetSystemMenu(FALSE))
	{
		CString szMaximize, szMinimize, szSize, szRestore;
		// try to get the strings from the topmost window
		CWnd* pwndTop = 0;
		for (pwndTop = this; pwndTop->GetParent(); pwndTop = pwndTop->GetParent());
		if (CMenu* pTopSysMenu = pwndTop->GetSystemMenu(FALSE))
		{
			pTopSysMenu->GetMenuString(SC_MAXIMIZE, szMaximize, MF_BYCOMMAND);
			pTopSysMenu->GetMenuString(SC_MINIMIZE, szMinimize, MF_BYCOMMAND);
			pTopSysMenu->GetMenuString(SC_SIZE, szSize, MF_BYCOMMAND);
			pTopSysMenu->GetMenuString(SC_RESTORE, szRestore, MF_BYCOMMAND);
		}
		// if we din't get the strings then set them to the English defaults
		if (szMaximize.IsEmpty()) szMaximize = _T("Ma&ximize");
		if (szMinimize.IsEmpty()) szMinimize = _T("Mi&nimize");
		if (szSize.IsEmpty()) szSize = _T("&Size");
		if (szRestore.IsEmpty()) szRestore = _T("&Restore");

		pSysMenu->InsertMenu(1, MF_BYPOSITION | MF_SEPARATOR, 0, (LPCTSTR) 0);
		pSysMenu->InsertMenu(1, MF_BYPOSITION | MF_STRING, SC_MAXIMIZE, szMaximize);
		pSysMenu->InsertMenu(1, MF_BYPOSITION | MF_STRING, SC_MINIMIZE, szMinimize);
		pSysMenu->InsertMenu(1, MF_BYPOSITION | MF_STRING, SC_SIZE, szSize);
		pSysMenu->InsertMenu(0, MF_BYPOSITION | MF_STRING, SC_RESTORE, szRestore);
	}

	CResize::Init();

	// set sheet sizings
	SetResize(IDOK, SZ_REPOS(1));
	SetResize(IDCANCEL, SZ_REPOS(1));
	SetResize(IDHELP, SZ_REPOS(1));
	SetResize(ID_APPLY_NOW, SZ_REPOS(1));
	SetResize(AFX_IDC_TAB_CONTROL, SZ_RESIZE(1));

	// set page sizings
	CRect rcPage;
	GetActivePage()->GetWindowRect(rcPage);
	ScreenToClient(rcPage);
	for (int i=0; i<GetPageCount(); i++)
	{
		SetResize(GetPage(i), SZ_RESIZE(1), rcPage);
	}
	return TRUE;
}

void CResizePropertySheet::OnGetMinMaxInfo(MINMAXINFO* pMMI)
{
	CResize::GetMinMaxInfo(pMMI);
}

void CResizePropertySheet::OnSize(UINT nType, int cx, int cy)
{
	CPropertySheet::OnSize(nType, cx, cy);
	CResize::Size();
}

/////////////////////////////////////////////////////////////////////////////
// CResizeFormView

IMPLEMENT_DYNCREATE(CResizeFormView, CFormView)

CResizeFormView::CResizeFormView(const UINT nID, const UINT nFlags) : 
	CFormView(nID),
	CResize(this, nFlags | SZ_NOSIZEICON)
{

}

BEGIN_MESSAGE_MAP(CResizeFormView, CFormView)
	//{{AFX_MSG_MAP(CResizeFormView)
	ON_WM_SIZE()
	ON_WM_GETMINMAXINFO()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CResizeFormView::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();

	ResizeParentToFit(FALSE);

	SetScrollSizes(MM_TEXT, CSize(0,0)); // disable the scroll bars

	CResize::Init();
}

void CResizeFormView::OnGetMinMaxInfo(MINMAXINFO* pMMI)
{
	// Notice that WM_GETMINMAXINFO must be handled in the parent frame
	CResize::GetMinMaxInfo(pMMI);
}

void CResizeFormView::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);
	CResize::Size();
}
