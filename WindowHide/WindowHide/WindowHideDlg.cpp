
// WindowHideDlg.cpp : implementation file
//

#include "stdafx.h"
#include "WindowHide.h"
#include "WindowHideDlg.h"
#include "afxdialogex.h"
#include <algorithm>
#include <tlhelp32.h>
#include <iostream>

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

vector<DWORD> g_vToHidePid;
vector<HWND> g_vToResumeHwnd;

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CWindowHideDlg dialog



CWindowHideDlg::CWindowHideDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_WINDOWHIDE_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CWindowHideDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_ALLPROCESS, m_listCtrlAllProcess);
	DDX_Control(pDX, IDC_LIST_TOHIDEPROCESS, m_listCtrlToHideProcess);
}

BEGIN_MESSAGE_MAP(CWindowHideDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_REFRESH, &CWindowHideDlg::OnBnClickedButtonRefresh)
	ON_MESSAGE(WM_HOTKEY, OnHotKey)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_ALLPROCESS, &CWindowHideDlg::OnLvnItemchangedListAllprocess)
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CWindowHideDlg::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_REMOVE, &CWindowHideDlg::OnBnClickedButtonRemove)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_TOHIDEPROCESS, &CWindowHideDlg::OnLvnItemchangedListTohideprocess)
	ON_BN_CLICKED(IDC_BUTTON3, &CWindowHideDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON_SHOW, &CWindowHideDlg::OnBnClickedButtonShow)
END_MESSAGE_MAP()


// CWindowHideDlg message handlers

BOOL CWindowHideDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	//ModifyStyleEx(WS_EX_APPWINDOW, WS_EX_TOOLWINDOW);

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	RegisterHotKeys();
	m_listCtrlAllProcess.SetColumnHeader(L"Name,200;PID, 140");
	m_listCtrlAllProcess.SetCheckboxeStyle(RC_CHKBOX_NONE); // Enable checkboxes

	m_listCtrlToHideProcess.SetColumnHeader(L"Name,200");
	m_listCtrlToHideProcess.SetCheckboxeStyle(RC_CHKBOX_NONE); // Enable checkboxes

	OnBnClickedButtonRefresh();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CWindowHideDlg::OnDestroy()
{
	UnregisterHotKeys();
	return CDialogEx::OnDestroy();
}


void CWindowHideDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CWindowHideDlg::OnPaint()
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
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CWindowHideDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

LONG CWindowHideDlg::OnHotKey(WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
	case 0x0001:
	{
		OnBnClickedButton3();
	}
	break;
	case 0x0002:
	{
		OnBnClickedButtonShow();
	}
	break;

	}

	return 0;
}

void CWindowHideDlg::HideWindow(CString ProcessName)
{

	PROCESSENTRY32 psEntry;
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	psEntry.dwSize = sizeof(PROCESSENTRY32);
	BOOL bRet = Process32First(hSnapshot, &psEntry);
	while (bRet)
	{
		wstring procName = psEntry.szExeFile;
		if (procName == ProcessName.GetString())
		{
			g_vToHidePid.push_back(psEntry.th32ProcessID);
		}
		bRet = Process32Next(hSnapshot, &psEntry);
	}
}

bool HideApp(HWND hWnd)
{
	if (0 == hWnd)
		return false;

	// use ShowWindowAsync instead of SHowWindow. ShowWindowAsync can show windows asyncly.
	// An application can use this function to avoid becoming nonresponsive 
	// while waiting for a nonresponsive application to finish processing a show-window event.
	// But I have to remove the IsWindowVisible check, because of its Async.

	bool result = (::ShowWindowAsync/*ShowWindow*/(hWnd, SW_HIDE) == TRUE);

	return result;
}

bool ShowApp(HWND hWnd)
{
	::ShowWindowAsync(hWnd, SW_SHOW);
	return true;
}

void CWindowHideDlg::OnBnClickedButtonRefresh()
{
	// TODO: Add your control notification handler code here
	m_listCtrlAllProcess.DeleteAllItems();

	PROCESSENTRY32 psEntry;
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	psEntry.dwSize = sizeof(PROCESSENTRY32);
	BOOL bRet = Process32First(hSnapshot, &psEntry);
	while (bRet)
	{
		wstring procName = psEntry.szExeFile;
		//LOGFMTD("[ ######### ]  %s ", ws2s(procName).c_str());

		//if (ProcIsExplorer(procName) || !IsProcessInWhiteList(procName))     // explorer进程或者非白名单列表进程
		{
			unsigned int pid = psEntry.th32ProcessID;
			unsigned int parentId = psEntry.th32ParentProcessID;
			//if (IsLoginUserProc(pid))		// 是否是本登录帐户下的程序
			{
			/*	_ProcessInfo info;
				info.imagName = psEntry.szExeFile;
				info.pid = pid;
				info.parentPid = parentId;
				info.isNeedBoost = false;
				info.isRunAsAdmin = CQueryProcessInfo::instance().IsRunAsAdmin(pid);
				info.isInBlackList = IsProcessInBlackList(procName);
				CQueryProcessInfo::instance().GetProcImagPath(pid, info.filePath);
				CQueryProcessInfo::instance().GetProcCommandLine(pid, info.param);
				CQueryProcessInfo::instance().GetModuleResourceInfo(CQueryProcessInfo::Resource_CompanyName, info.filePath, info.appCompany);
				CQueryProcessInfo::instance().GetModuleResourceInfo(CQueryProcessInfo::Resource_FileDescription, info.filePath, info.description);

				// 如果是查询进程信息，我们才获取这两个数据，提高效率
				if (isQuery)
				{
					info.cpuUsed = GetProcessCpuUse(procName, pid);
					info.memoryUsed = CQueryProcessInfo::instance().GetProcMemory(pid);
					info.GpuUsed = GetProcessGpuUse(procName, pid);
					info.gpuMemroyUsed = CQueryProcessInfo::instance().GetProcGpuMemory(pid);
					//info.memoryUsed = 5000000000;
				}*/

				//vList.push_back(info);
				const int IDX = m_listCtrlAllProcess.InsertItem(m_listCtrlAllProcess.GetItemCount(), _T(""));
				m_listCtrlAllProcess.SetItemText(IDX, 0, procName.c_str());
				m_listCtrlAllProcess.SetItemText(IDX, 1, pid);
				//m_listCtrlAllProcess.SetItemData(IDX, procName.c_str());
			}
		}
		bRet = Process32Next(hSnapshot, &psEntry);
	}

	CloseHandle(hSnapshot);
}


void CWindowHideDlg::OnLvnItemchangedListAllprocess(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}


void CWindowHideDlg::OnBnClickedButtonAdd()
{
	// TODO: Add your control notification handler code here
	for (int i = 0; i < m_listCtrlAllProcess.GetItemCount(); i++)
	{
		if (m_listCtrlAllProcess.ExamItemStates(i, RC_ITEM_SELECTED))
		{
			//checked
			auto name = m_listCtrlAllProcess.GetItemText(i, 0);

			const int IDX = m_listCtrlToHideProcess.InsertItem(m_listCtrlToHideProcess.GetItemCount(), _T(""));
			m_listCtrlToHideProcess.SetItemText(IDX, 0, name);
		}
	}

}

void CWindowHideDlg::OnBnClickedButtonRemove()
{
	// TODO: Add your control notification handler code here
	for (int i = 0; i < m_listCtrlToHideProcess.GetItemCount(); i++)
	{
		if (m_listCtrlToHideProcess.ExamItemStates(i, RC_ITEM_SELECTED))
		{
			m_listCtrlToHideProcess.DeleteItem(i);
		}
	}
}

void CWindowHideDlg::OnLvnItemchangedListTohideprocess(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}

void CWindowHideDlg::OnBnClickedButton3()
{
	g_vToHidePid.clear();

	// TODO: Add your control notification handler code here
	for (int i = 0; i < m_listCtrlToHideProcess.GetItemCount(); i++)
	{
		auto name = m_listCtrlToHideProcess.GetItemText(i, 0);
		HideWindow(name);
	}


	// This method is easy to hang up, we try to start a thread to handle this
	// thus, we can stop it when timeout
	HANDLE hHandle = (HANDLE)_beginthreadex(NULL, 0, GameDesktopWndHndThreadProc, (void *) this, 0, NULL);

	DWORD dwRes = WaitForSingleObject(hHandle, INFINITE); // timeout = 7s

	if (dwRes == WAIT_TIMEOUT)
	{
		TerminateThread(hHandle, 1);
	}
}


void CWindowHideDlg::OnBnClickedButtonShow()
{
	// TODO: Add your control notification handler code here
	for (unsigned int i = 0; i < g_vToResumeHwnd.size(); i++)
	{
		if (ShowApp(g_vToResumeHwnd[i]))
		{
			//success&record

		}
	}
}

void CWindowHideDlg::RegisterHotKeys()
{
	int nTmpHotKeyID = 0x0001;
	BOOL isKeyRegistered = RegisterHotKey(GetSafeHwnd(), nTmpHotKeyID, MOD_ALT, VK_G);
	if (isKeyRegistered != FALSE)
		m_mapHotKeys[L"HIDE"] = nTmpHotKeyID;

	nTmpHotKeyID = 0x0002;
	isKeyRegistered = RegisterHotKey(GetSafeHwnd(), nTmpHotKeyID, MOD_ALT, Vk_H);
	if (isKeyRegistered != FALSE)
		m_mapHotKeys[L"SHOW"] = nTmpHotKeyID;
}

void CWindowHideDlg::UnregisterHotKeys()
{
	for each(auto e in m_mapHotKeys)
	{
		UnregisterHotKey(GetSafeHwnd(), e.second);
	}
}

bool IsToHideWindow(HWND hWnd)
{
	DWORD procesId = 0;
	GetWindowThreadProcessId(hWnd, &procesId);

	for each(auto e in g_vToHidePid)
	{
		if (e == procesId)
		{
			return true;
		}
	}

	return false;
}

BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lparam)
{
	if (IsWindow(hwnd) && IsWindowVisible(hwnd))
	{
		std::vector<HWND>* p = (std::vector<HWND>*)lparam;
		bool isGame = IsToHideWindow(hwnd);

		//if (!isValidApp)
		{
			//bool isMin = IsIconic(hwnd) > 0 ? true : false;
			if (isGame)
			{
				p->push_back(hwnd);
			}
		}
	}

	return true;
}


unsigned int CWindowHideDlg::GameDesktopWndHndThreadProc(void * pData)
{
	try
	{
		CWindowHideDlg* pDesktop = (CWindowHideDlg*)pData;
		if (NULL == pDesktop)
		{
			return 1;
		}

		std::vector<HWND> vHwnds;
		vHwnds.clear();
		g_vToResumeHwnd.clear();
		::EnumWindows(EnumWindowsProc, ((LPARAM)&vHwnds));

		for (unsigned int i = 0; i < vHwnds.size(); i++)
		{
			if (HideApp(vHwnds[i]))
			{
				//success&record
				g_vToResumeHwnd.push_back(vHwnds[i]);
			}
		}
	}
	catch (...)
	{
		return false;
	}

	return true;
}
