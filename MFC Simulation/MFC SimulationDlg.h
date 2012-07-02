
// MFC SimulationDlg.h : header file
//

#pragma once
#include "afxwin.h"
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
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnStartSimulation();
	afx_msg void OnEnChangeEdit1();

public: 
	void addToOutput(CString str);

private:
	CStatic m_ctlTower1;
	CStatic m_ctlTower2;
	CStatic m_ctlTower3;
	CStatic m_ctlOutput;
	CSimulatorView m_ctlSimulation;

	bool m_bRunning;
	CWorld * m_pWorld;
};
