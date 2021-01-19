
// WindowHideDlg.h : header file
//

#pragma once
#include "ReportCtrl.h"

#include <iostream>
#include <vector>
using namespace std;

// CWindowHideDlg dialog
class CWindowHideDlg : public CDialogEx
{
// Construction
public:
	CWindowHideDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_WINDOWHIDE_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
private:
	CReportCtrl m_listCtrlAllProcess;
	CReportCtrl m_listCtrlToHideProcess;

	void HideWindow(CString ProcessName);

public:
	afx_msg void OnBnClickedButtonRefresh();
	afx_msg void OnLvnItemchangedListAllprocess(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedButtonRemove();
	afx_msg void OnLvnItemchangedListTohideprocess(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButtonShow();

	// game desktop handling thread
	static unsigned int _stdcall GameDesktopWndHndThreadProc(void* pData);
};
