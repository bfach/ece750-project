
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
	CPaintDC dc( &m_pParent->m_ctlSimulation ); // device context for painting
	CRect r;
	this->GetWindowRect( &r );

	dc.FillSolidRect( 1,1, r.Width()-2, r.Height()-2, RGB(200,200,200) );

	// Are we currently running the simulation?
	if ( m_pParent->m_bRunning )
	{
		// get a DC for the simulation drawing area
//		CPaintDC dc( &m_pParent->m_ctlSimulation ); // device context for painting
//		dc.SetBkMode( OPAQUE );
//		dc.SetBkColor( RGB(255,255,255) );


		if ( m_pParent->m_pWorld ) m_pParent->m_pWorld->Draw( dc );
	}

}


// CMFCSimulationDlg dialog

CMFCSimulationDlg::CMFCSimulationDlg(CWnd* pParent /*=NULL*/)
	: CResizeDialog(CMFCSimulationDlg::IDD, pParent)
	, m_ctlSimulation( this )
	, m_pWorld( 0 )
	, m_bRunning( false )
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCSimulationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TOWER1, m_ctlTower1);
	DDX_Control(pDX, IDC_TOWER2, m_ctlTower2);
	DDX_Control(pDX, IDC_TOWER3, m_ctlTower3);
	DDX_Control(pDX, IDC_OUTPUT, m_ctlOutput);
	DDX_Control(pDX, IDC_PICTURE, m_ctlSimulation);
}

BEGIN_MESSAGE_MAP(CMFCSimulationDlg, CResizeDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CMFCSimulationDlg::OnStartSimulation)
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


void CMFCSimulationDlg::OnStartSimulation()
{
	// Create the world
	m_pWorld = new CWorld;

	m_bRunning = true;

	m_ctlSimulation.Invalidate();
}
