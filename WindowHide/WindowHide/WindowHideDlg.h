
// WindowHideDlg.h : header file
//

#pragma once
#include "ReportCtrl.h"

#include <iostream>
#include <vector>
#include <map>
using namespace std;

#define VK_1 49
#define VK_2 50
#define VK_3 51
#define VK_4 52
#define VK_5 53
#define VK_6 54


#define VK_D 68
#define VK_G 71
#define Vk_H 72

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
	virtual void OnDestroy();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg LONG OnHotKey(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
private:
	CReportCtrl m_listCtrlAllProcess;
	CReportCtrl m_listCtrlToHideProcess;
	std::map<wstring, int> m_mapHotKeys;

	void HideWindow(CString ProcessName);

public:
	afx_msg void OnBnClickedButtonRefresh();
	afx_msg void OnLvnItemchangedListAllprocess(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedButtonRemove();
	afx_msg void OnLvnItemchangedListTohideprocess(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButtonShow();
	void RegisterHotKeys();
	void UnregisterHotKeys();
	// game desktop handling thread
	static unsigned int _stdcall GameDesktopWndHndThreadProc(void* pData);
};
