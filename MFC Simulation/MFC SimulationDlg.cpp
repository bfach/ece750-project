
// MFC SimulationDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MFC Simulation.h"
#include "MFC SimulationDlg.h"
#include "afxdialogex.h"
#include "World.h"
#include "HandoverSignalStrength.h"
#include "HandoverLastMinute.h"

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
	DDX_Control(pDX, IDC_SS_HOME, m_ctlSignalHome);
	DDX_Control(pDX, IDC_SS_CELLTOWER, m_ctlSignalCell);
	DDX_Control(pDX, IDC_SS_WORK, m_ctlSignalWork);
	DDX_Control(pDX, IDC_PAUSE, m_ctlPause);
	DDX_Control(pDX, IDC_CONN_HOME, m_ctlHomeArrow);
	DDX_Control(pDX, IDC_CONN_CELL, m_ctlCellArrow);
	DDX_Control(pDX, IDC_CONN_WORK, m_ctlWorkArrow);
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

	m_ctlPause.SetCheck( BST_CHECKED );

	CImage i;
	i.Load( L"C:\\Users\\Shawn\\Desktop\\Simulation\\ece750-project\\MFC Simulation\\res\\arrow_down.png" );

	//i.LoadFromResource( AfxGetInstanceHandle(), IDB_GREENARROW );
	m_ctlHomeArrow.SetBitmap( i );
	m_ctlCellArrow.SetBitmap( i );
	m_ctlWorkArrow.SetBitmap( i );

	CButton* pButton = (CButton*)GetDlgItem(IDC_ALG_LASTMINUTE);
	if ( pButton ) pButton->SetCheck(true);

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
	ATLTRACE( L"%s: Starting\n", __FUNCTIONW__ );

	if ( 0 == pParam ) return -1;
	CMFCSimulationDlg *pDlg = static_cast<CMFCSimulationDlg*> (pParam);
	pDlg->threadProc();

	ATLTRACE( L"%s: Ending\n", __FUNCTIONW__ );
	return 0;
}

void CMFCSimulationDlg::threadProc()
{
	m_bPauseSimulation = false;

	while ( WAIT_TIMEOUT == ::WaitForSingleObject( m_hThreadShouldExit, 100 ) )
	{
		//ATLTRACE( L"%s: Tick %d\n", __FUNCTIONW__, m_nTickCount );
		if ( !m_bPauseSimulation ) 
		{
			m_nTickCount++;
			if ( m_nTickCount > m_nMaxTicks )
			{
				// Fake pressing the button to stop the simulation
				::SetEvent( m_hThreadShouldExit );
				::PostMessage( m_hWnd, WM_COMMAND, MAKELONG(IDC_BUTTON1, BN_CLICKED), (LPARAM)NULL);
				break;
			}
			else
			{
				m_ctlSlider.SetPos( m_nTickCount );
			}
		}

		int nProvider = m_pWorld->nProviderConnectedTo;
		m_pWorld->Tick(m_nTickCount);
		m_ctlSimulation.Invalidate();

		CString strTemp;

		strTemp.Format( L"%s: %d", m_pWorld->wirelessProviders[0].name, m_pWorld->wirelessProviders[0].signal );
		m_ctlSignalCell.SetWindowText( strTemp );

		strTemp.Format( L"%s: %d", m_pWorld->wirelessProviders[1].name, m_pWorld->wirelessProviders[1].signal );
		m_ctlSignalHome.SetWindowText( strTemp );

		strTemp.Format( L"%s: %d", m_pWorld->wirelessProviders[2].name, m_pWorld->wirelessProviders[2].signal );
		m_ctlSignalWork.SetWindowText( strTemp );

		// If the provider changed, do something
		if ( m_pWorld->nProviderConnectedTo != nProvider )
		{
			int n = m_pWorld->nProviderConnectedTo;
			m_ctlHomeArrow.ShowWindow( n == 1 ? SW_SHOW : SW_HIDE );
			m_ctlWorkArrow.ShowWindow( n == 2 ? SW_SHOW : SW_HIDE );
			m_ctlCellArrow.ShowWindow( n == 0 ? SW_SHOW : SW_HIDE );
		}
	}

	ATLTRACE( L"%s: Signalling\n", __FUNCTIONW__ );
	::SetEvent( m_hThreadHasExited );

	ATLTRACE( L"%s: Exiting\n", __FUNCTIONW__ );
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
		ATLTRACE( L"%s: Waiting for thread to exit\n", __FUNCTIONW__ );
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
		CHandoverAlgorithm *pAlg = 0;
		if (BST_CHECKED == ((CButton*)GetDlgItem(IDC_ALG_LASTMINUTE))->GetCheck()) pAlg = new CHandoverLastMinute;
		else if (BST_CHECKED == ((CButton*)GetDlgItem(IDC_ALG_BESTSIGNAL))->GetCheck()) pAlg = new CHandoverSignalStrength;

		m_pWorld = new CWorld(pAlg);

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
