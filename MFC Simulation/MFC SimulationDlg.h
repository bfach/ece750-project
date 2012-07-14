
// MFC SimulationDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include "afxcmn.h"
class CWorld;
class CMFCSimulationDlg;

class CSimulatorView : public CStatic
{
public:
	CSimulatorView( CMFCSimulationDlg *pParent ) { m_pParent = pParent; }
	virtual ~CSimulatorView() {}

	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();

	CMFCSimulationDlg * m_pParent;
};

// CMFCSimulationDlg dialog
class CMFCSimulationDlg : public CResizeDialog
{
	friend CSimulatorView;

// Construction
public:
	CMFCSimulationDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	enum { IDD = IDD_MFCSIMULATION_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnHScroll(UINT, UINT, CScrollBar*); 

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnStartSimulation();
	afx_msg void OnEnChangeEdit1();

public: 
	void addToOutput(CString str);
	void threadProc();

private:
	CStatic m_ctlTower1;
	CStatic m_ctlTower2;
	CStatic m_ctlTower3;
	CStatic m_ctlOutput;
	CSimulatorView m_ctlSimulation;

	HANDLE m_hThreadShouldExit;
	HANDLE m_hThreadHasExited;
	int m_nMaxTicks;
	CWorld * m_pWorld;
	CWinThread *m_pThread;
	CButton m_ctlStartSimulation;
	CButton m_ctlClose;
	CSliderCtrl m_ctlSlider;
	int m_nTickCount;
	bool m_bPauseSimulation;
public:
	CStatic m_ctlSignalHome;
	CStatic m_ctlSignalCell;
	CStatic m_ctlSignalWork;
	CButton m_ctlPause;
	CStatic m_ctlHomeArrow;
	CStatic m_ctlCellArrow;
	CStatic m_ctlWorkArrow;
};
