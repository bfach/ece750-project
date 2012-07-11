
// MFC SimulationDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MFC Simulation.h"
#include "MFC SimulationDlg.h"
#include "afxdialogex.h"
#include "World.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


BEGIN_MESSAGE_MAP(CSimulatorView, CStatic)
	ON_WM_PAINT()
END_MESSAGE_MAP()

void CSimulatorView::OnPaint()
{
	CPaintDC pdc( &m_pParent->m_ctlSimulation ); // device context for painting
	CMemDC mdc( pdc, &m_pParent->m_ctlSimulation  );

	CRect r;
	this->GetWindowRect( &r );

	mdc.GetDC().FillSolidRect( 1,1, r.Width()-2, r.Height()-2, RGB(200,200,200) );

	// Are we currently running the simulation?
	if ( m_pParent->m_hThreadHasExited && WAIT_OBJECT_0 != ::WaitForSingleObject( m_pParent->m_hThreadHasExited, 0 ) )
	{
		// get a DC for the simulation drawing area
//		CPaintDC dc( &m_pParent->m_ctlSimulation ); // device context for painting
//		dc.SetBkMode( OPAQUE );
//		dc.SetBkColor( RGB(255,255,255) );


		if ( m_pParent->m_pWorld ) m_pParent->m_pWorld->Draw( mdc.GetDC() );
	}

}


// CMFCSimulationDlg dialog

CMFCSimulationDlg::CMFCSimulationDlg(CWnd* pParent /*=NULL*/)
	: CResizeDialog(CMFCSimulationDlg::IDD, pParent)
	, m_ctlSimulation( this )
	, m_pWorld( 0 )
	, m_pThread(0)
	, m_bPauseSimulation( false )
	, m_nMaxTicks( 300 )
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	m_hThreadShouldExit = ::CreateEvent( NULL, TRUE, TRUE, NULL );
	m_hThreadHasExited = ::CreateEvent( NULL, TRUE, TRUE, NULL );
}

void CMFCSimulationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TOWER1, m_ctlTower1);
	DDX_Control(pDX, IDC_TOWER2, m_ctlTower2);
	DDX_Control(pDX, IDC_TOWER3, m_ctlTower3);
	DDX_Control(pDX, IDC_OUTPUT, m_ctlOutput);
	DDX_Control(pDX, IDC_PICTURE, m_ctlSimulation);
	DDX_Control(pDX, IDC_BUTTON1, m_ctlStartSimulation);
	DDX_Control(pDX, IDCANCEL, m_ctlClose);
	DDX_Control(pDX, IDC_SLIDER1, m_ctlSlider);
}

BEGIN_MESSAGE_MAP(CMFCSimulationDlg, CResizeDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CMFCSimulationDlg::OnStartSimulation)
	ON_WM_HSCROLL()
END_MESSAGE_MAP()


// CMFCSimulationDlg message handlers

BOOL CMFCSimulationDlg::OnInitDialog()
{
	__super::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// Set up the automatic resizing of our controls
	SetResize( IDC_TOWER1, SZ_HORRESIZE(0.33) + SZ_HORREPOS(0) );
	SetResize( IDC_TOWER2, SZ_HORRESIZE(0.33) + SZ_HORREPOS(0.333) );
	SetResize( IDC_TOWER3, SZ_HORRESIZE(0.33) + SZ_HORREPOS(0.666) );

	SetResize( IDC_OUTPUT, SZ_HORRESIZE(1) );

	m_ctlOutput.SetWindowTextW(L"");
	m_ctlTower1.SetWindowTextW(L"");
	m_ctlTower2.SetWindowTextW(L"");
	m_ctlTower3.SetWindowTextW(L"");

	m_ctlSlider.SetRange( 0, m_nMaxTicks );

	return TRUE;  // return TRUE  unless you set the focus to a control
}


// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMFCSimulationDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}


// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMFCSimulationDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


UINT afxThreadProc( LPVOID pParam )
{
	if ( 0 == pParam ) return -1;
	CMFCSimulationDlg *pDlg = static_cast<CMFCSimulationDlg*> (pParam);
	pDlg->threadProc();
	return 0;
}

void CMFCSimulationDlg::threadProc()
{
	m_bPauseSimulation = false;

	while ( WAIT_TIMEOUT == ::WaitForSingleObject( m_hThreadShouldExit, 100 ) )
	{
		if ( !m_bPauseSimulation ) 
		{
			m_nTickCount++;
			if ( m_nTickCount > m_nMaxTicks )
			{
				// Fake pressing the button to stop the simulation
				::PostMessage( m_hWnd, WM_COMMAND, MAKELONG(IDC_BUTTON1, BN_CLICKED), (LPARAM)NULL);
				//OnStartSimulation(); // This is probably bad
				::SetEvent( m_hThreadShouldExit );
			}
			else
				m_ctlSlider.SetPos( m_nTickCount );
		}
		m_pWorld->Tick(m_nTickCount);
		m_ctlSimulation.Invalidate();
	}

	::SetEvent( m_hThreadHasExited );
}

void CMFCSimulationDlg::OnStartSimulation()
{
	// Unpause
	if ( m_bPauseSimulation )
	{
		m_ctlStartSimulation.SetWindowText( L"Stop Simulation" );
		m_bPauseSimulation = false;
		return;
	}

	m_ctlStartSimulation.EnableWindow(FALSE);

	if ( 0 != m_pWorld )
	{
		// End the simulation
		//
		::SetEvent( m_hThreadShouldExit );
		DWORD dwRes = ::WaitForSingleObject( m_hThreadHasExited, 5000 );
		ATLASSERT( WAIT_TIMEOUT != dwRes );

		delete m_pWorld;
		m_pWorld = 0;

		m_ctlStartSimulation.SetWindowText( L"Start Simulation" );
		m_ctlClose.EnableWindow( TRUE );
		m_ctlSlider.EnableWindow( FALSE );
	}
	else
	{
		// Start the simulation
		//
		m_ctlClose.EnableWindow( FALSE );
		m_ctlSlider.EnableWindow( TRUE );

		// Create the world
		m_pWorld = new CWorld;

		// Kick off the thread
		::ResetEvent( m_hThreadShouldExit );
		::ResetEvent( m_hThreadHasExited );
		m_nTickCount = 0;
		m_pThread = AfxBeginThread( afxThreadProc, (LPVOID)this );

		m_ctlSimulation.Invalidate();
		m_ctlStartSimulation.SetWindowText( L"Stop Simulation" );

		this->addToOutput( L"hello world");
		this->addToOutput( L"1234");
	}

	m_ctlStartSimulation.EnableWindow(TRUE);
}

void CMFCSimulationDlg::addToOutput(CString str)
{
	CString newStr;
	m_ctlOutput.GetWindowTextW(newStr);
	newStr += str;
	m_ctlOutput.SetWindowTextW(newStr);
}

void CMFCSimulationDlg::OnHScroll( UINT nSBCode, UINT nPos, CScrollBar* pScrollBar )
{
//	CSliderCtrl *pSlider = (CSliderCtrl*)pScrollBar;
//	int nPos = pSlider->GetPos();
	if ( false == m_bPauseSimulation )
	{
		m_ctlStartSimulation.SetWindowText( L"Resume Simulation" );
		m_bPauseSimulation = true;
	}

	if ( SB_THUMBPOSITION == nSBCode || SB_THUMBTRACK == nSBCode )
	{
		m_nTickCount = nPos;
	}
}
