
// TelescopeControlDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TelescopeControl.h"
#include "TelescopeControlDlg.h"
#include "afxdialogex.h"
#include "math.h"

#include <afxinet.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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


// CTelescopeControlDlg dialog



CTelescopeControlDlg::CTelescopeControlDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_TELESCOPECONTROL_DIALOG, pParent)
	, m_sServerAddress(_T("luna.as.utexas.edu"))
	, m_nPortNumber(6666)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CTelescopeControlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_CONNECT_TELESCOPE, m_btnConnectTelescope);
	DDX_Control(pDX, IDC_BUTTON_DISCONNECT_TELESCOPE, m_btnDisconnectTelescope);
	DDX_Control(pDX, IDC_LIST_TCP_LOG, m_listTCPLog);
	DDX_Text(pDX, IDC_EDIT_SERVER_ADDRESS, m_sServerAddress);
	DDX_Text(pDX, IDC_EDIT_PORT_NUMBER, m_nPortNumber);
}

BEGIN_MESSAGE_MAP(CTelescopeControlDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_CONNECT_TELESCOPE, &CTelescopeControlDlg::OnClickedButtonConnectTelescope)
	ON_BN_CLICKED(IDC_BUTTON_DISCONNECT_TELESCOPE, &CTelescopeControlDlg::OnClickedButtonDisconnectTelescope)
	ON_WM_TIMER()
	ON_EN_SETFOCUS(IDC_EDIT_SET_TARGET_DEC, &CTelescopeControlDlg::OnSetfocusEditSetTargetDec)
	ON_EN_KILLFOCUS(IDC_EDIT_SET_TARGET_DEC, &CTelescopeControlDlg::OnKillfocusEditSetTargetDec)
	ON_MESSAGE(WM_TCP, &CTelescopeControlDlg::OnGetMessageFromDlg)
	ON_MESSAGE(WM_CLIENT, &CTelescopeControlDlg::OnGetClientStatus)
END_MESSAGE_MAP()


// CTelescopeControlDlg message handlers

BOOL CTelescopeControlDlg::OnInitDialog()
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
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	
	pLibAscomTCS = new CLIB_ASCOM_TCS;
	pLibTCP = new CLIB_TCP;

	//g_Telescope.hTCPdlg = GetSafeHwnd();
	g_Telescope.nControlMode = CONTROL_ASCOM_MODE;

	m_btnConnectTelescope.EnableWindow(TRUE);
	m_btnDisconnectTelescope.EnableWindow(FALSE);

	h_TELNETdlg = GetSafeHwnd();
	m_btnDisconnectTelescope.EnableWindow(FALSE);

	AddTCPLog(_T("KAOS30 Telescope Control Package v1.09"));

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CTelescopeControlDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else if (nID == SC_CLOSE) {

		if (IDYES == this->MessageBox(L"Are you sure you want to exit the KAOS30 TCP?", L"Message", MB_YESNO)) {

			delete pLibTCP;

			CWnd* pWnd = FindWindow(NULL, MAIN_PROGRAM);

			if (pWnd != NULL)
				pWnd->SendMessage(WM_MAIN, (WPARAM)ID_TCP, (LPARAM)EXIT_DIALOG);
		}
		else
			return;
	}
	CDialogEx::OnSysCommand(nID, lParam);
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CTelescopeControlDlg::OnPaint()
{
	if (IsIconic()) {
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
	else {
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CTelescopeControlDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BOOL CTelescopeControlDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE) return TRUE;

	return CDialogEx::PreTranslateMessage(pMsg);
}

void CTelescopeControlDlg::OnClickedButtonConnectTelescope()
{
	// TODO: Add your control notification handler code her

	m_btnConnectTelescope.EnableWindow(FALSE);

	UpdateData(TRUE);

	WSADATA wsaData;
	int nRet = WSAStartup(0x202, &wsaData);

	char *cError;
	pSocket = new Socket;

	CStringA ansiServerAddress(m_sServerAddress);

	if (cError = pSocket->Create(AF_INET, SOCK_STREAM, IPPROTO_IP)) {

		AfxMessageBox((CString)cError, MB_OK | MB_ICONHAND);

		return;
	}
	if (cError = pSocket->Select(h_TELNETdlg, WM_CLIENT, FD_READ | FD_WRITE | FD_CONNECT | FD_CLOSE)) {

		AfxMessageBox((CString)cError, MB_OK | MB_ICONHAND);
		return;
	}
	if (cError = pSocket->Connect(ansiServerAddress, (WORD)m_nPortNumber)) {

		AfxMessageBox((CString)cError, MB_OK | MB_ICONHAND);
		return;
	}

	CString sHostName;
	CString sIP;
	int nHostPort;

	nHostPort = pSocket->getport();
	sHostName = pSocket->gethostname();
	sIP = pSocket->getaddr();

	CString sLog;

	sLog = "Host name = ";
	sLog = sLog + sHostName;
	m_listTCPLog.AddString(sLog);

	CString sPortNumber;
	sPortNumber.Format(_T("%d"), nHostPort);
	sLog = "Port Number = ";
	sLog = sLog + sPortNumber;

	sLog = "IP address = ";
	sLog = sLog + sIP;
	m_listTCPLog.AddString(sLog);

	m_listTCPLog.AddString(_T("Connection is in progress..."));

/*
	pLibTCP->TCPLinkTelescope();
	if (g_Telescope.bLink) {

		UpdateData(FALSE);
		AddTCPLog(_T("Telescope connected."));

		SetTimer(TIMER_CHECK_TELESCOPE_POSITION, TIME_MILLISECOND, NULL);

		m_btnConnectTelescope.EnableWindow(FALSE);
		m_btnDisconnectTelescope.EnableWindow(TRUE);
	}
	else
		AddTCPLog(_T("Telescope is not connected."));
*/
}


void CTelescopeControlDlg::OnClickedButtonDisconnectTelescope()
{
	// TODO: Add your control notification handler code here
	KillTimer(TIME_CHECK_TCS_POSITION);
	KillTimer(TIMER_DISPLAY_POSITION);
	KillTimer(TIMER_CHECK_TELESCOPE_POSITION);

	m_listTCPLog.AddString(_T("Waiting for disconnection..."));
/*
	char *cMessage;
	char *cError;

	cMessage = "quit";

	if (cError = pSocket->Send(cMessage)) {

		AfxMessageBox((CString)cError, MB_OK | MB_ICONHAND);
	}
*/
	m_listTCPLog.AddString(_T("Disconnected."));

	WSACleanup();

	if (pSocket) {

		delete pSocket;
		pSocket = NULL;
	}

	m_btnConnectTelescope.EnableWindow(TRUE);
	m_btnDisconnectTelescope.EnableWindow(FALSE);

	UpdateData(false);
	g_Telescope.bLink = true;
}

void CTelescopeControlDlg::SendCommandToServer(CString sCommand)
{
	char *cCommand;
	char *cError;

	wchar_t* wchar_str;
	int char_str_len;

	wchar_str = sCommand.GetBuffer(sCommand.GetLength());
	char_str_len = WideCharToMultiByte(CP_ACP, 0, wchar_str, -1, NULL, 0, NULL, NULL);
	cCommand = new char[char_str_len];
	WideCharToMultiByte(CP_ACP, 0, wchar_str, -1, cCommand, char_str_len, 0, 0);

	if (cError = pSocket->Send(cCommand)) {

		AfxMessageBox((CString)cError, MB_OK | MB_ICONHAND);
	}

	delete[] cCommand;
}

//void CTelescopeControlDlg::OnClickedButtonSetTarget()
//{
//	// TODO: Add your control notification handler code here
//	SetTimer(TIMER_CHECK_DIFFERENCE_POSITION, TIME_MILLISECOND, NULL);
//
//	m_btnSetObject.EnableWindow(FALSE);
//
//	UpdateData(TRUE);
//
//
//	SendCommandToServer(_T("echo RA: Dec: | fio -ts 100 Tel"));
//
///*
//	g_Telescope.sSetTargetRA = m_sSetTargetRA;
//	g_Telescope.sSetTargetDec = m_sSetTargetDec;
//
//	if (m_sSetTargetRA.GetLength() == 8)
//		m_sTargetRA = m_sSetTargetRA + _T(".0");
//	else
//		m_sTargetRA = m_sSetTargetRA;
//
//	if (m_sSetTargetDec.GetLength() == 8)
//		m_sTargetDec = m_sSetTargetDec + _T(".0");
//	else
//		m_sTargetDec = m_sSetTargetDec;
//
//	if (m_sSetTargetRA == "" && m_sSetTargetDec == "") {
//
//		AfxMessageBox(_T("Please input RA, DEC of the target."));
//		return;
//	}
//
//	pLibTCP->TCPSetTelescopePosition();
//	pLibTCP->TCPTrackingTelescope(true);
//	pLibTCP->TCPSlewTelescope();
//*/
//	UpdateData(FALSE);
//}

void CTelescopeControlDlg::AddTCPLog(CString sLog)
{

	if (sLog.GetLength() <= 0)
		return;

	if (m_listTCPLog.GetCount() >= 50)
		m_listTCPLog.ResetContent();

	SYSTEMTIME tWindowsTime;
	GetLocalTime(&tWindowsTime);

	CString sTime;
	sTime.Format(_T("[LT %02d:%02d:%02d ]  "), tWindowsTime.wHour, tWindowsTime.wMinute, tWindowsTime.wSecond);

	m_listTCPLog.AddString(sTime + sLog);

	CString sText;
	CSize pSize;
	int nX = 0;
	CDC* pDC = m_listTCPLog.GetDC();
	for (int i = 0; i < m_listTCPLog.GetCount(); i++)
	{
		m_listTCPLog.GetText(i, sText);
		pSize = pDC->GetTextExtent(sText);

		if (pSize.cx > nX)
			nX = pSize.cx;
	}
	m_listTCPLog.ReleaseDC(pDC);
	m_listTCPLog.SetHorizontalExtent(nX);
	m_listTCPLog.SendMessage(WM_VSCROLL, SB_BOTTOM);
}


void CTelescopeControlDlg::SaveLogFile(CString sPackageLog)
{
/*
	AddTCPLog(sPackageLog);

	CFileFind find;

	CString sFileDirectory, sFilePath;
	sFileDirectory.Empty();
	sFilePath.Empty();

	sFileDirectory.Format(_T("C:\\KAOS30\\CFG"));

	SYSTEMTIME tSysTime;
	GetLocalTime(&tSysTime);
	sFilePath.Format(_T("C:\\KAOS30\\CFG\\%d%02d%02d.log"), tSysTime.wYear, tSysTime.wMonth, tSysTime.wDay); // 20170417 modified by Tae-Geun Ji

	if (!find.FindFile((LPCTSTR)sFileDirectory))
		CreateDirectory((LPCTSTR)sFileDirectory, NULL);

	CStdioFile fileLog;
	if (!fileLog.Open(sFilePath, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite))
		return;
	// 20170417 modified by Tae-Geun Ji
	sFilePath.Format(_T("[%04d/%02d/%02d] [%02d:%02d:%02d]   "), tSysTime.wYear, tSysTime.wMonth, tSysTime.wDay, tSysTime.wHour, tSysTime.wMinute, tSysTime.wSecond);

	fileLog.SeekToEnd();
	fileLog.WriteString(sFilePath + sPackageLog);
	fileLog.Write("\r\n", 2);
	fileLog.Close();
*/
}


void CTelescopeControlDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default

	switch (nIDEvent) {
	case TIME_CHECK_TCS_POSITION:
	{
		SendCommandToServer(_T("./e_getcoord > /home/talon/KAOS_server/TCSInfo.txt"));
	}
	break;
	}

	CDialogEx::OnTimer(nIDEvent);
}

void CTelescopeControlDlg::SetTimerAll()
{
	SetTimer(TIME_CHECK_TCS_POSITION, TIME_MILLISECOND, NULL);
}

void CTelescopeControlDlg::KillTimerAll()
{
	KillTimer(TIME_CHECK_TCS_POSITION);
}

void CTelescopeControlDlg::OnSetfocusEditSetTargetDec()
{
	// TODO: Add your control notification handler code here
	KillTimerAll();
}

void CTelescopeControlDlg::OnKillfocusEditSetTargetDec()
{
	// TODO: Add your control notification handler code here
	SetTimerAll();
}


void CTelescopeControlDlg::SaveTCSInfomation()
{
	UpdateData(true);

	CFileFind find;

	CString sFileDirectory, sFilePath;

	sFileDirectory.Format(_T("C:\\KAOS30\\CFG"));
	sFilePath.Format(_T("C:\\KAOS30\\CFG\\TCP.txt")); // 20170417 modified by Tae-Geun Ji

	CString sSection, sKey, sValue;

	//Telescope Information
	sSection = _T("TCS Information");

	sKey = _T("TIME-OBS");
	sValue = "";
	WritePrivateProfileString(sSection, sKey, sValue, sFilePath);

	sKey = _T("DATE-OBS");
	sValue = "";
	WritePrivateProfileString(sSection, sKey, sValue, sFilePath);

	sKey = _T("RA");
	sValue = "";
	WritePrivateProfileString(sSection, sKey, sValue, sFilePath);

	sKey = _T("DEC");
	sValue = "";
	WritePrivateProfileString(sSection, sKey, sValue, sFilePath);

	sKey = _T("EQUINOX");
	sValue = "";
	WritePrivateProfileString(sSection, sKey, sValue, sFilePath);

	sKey = _T("ZENITH");
	sValue = "";
	WritePrivateProfileString(sSection, sKey, sValue, sFilePath);

	sKey = _T("AZIMUTH");
	sValue = "";
	WritePrivateProfileString(sSection, sKey, sValue, sFilePath);

	sKey = _T("AIRMASS");
	sValue = "";
	WritePrivateProfileString(sSection, sKey, sValue, sFilePath);

	sKey = _T("LST");
	sValue = "";
	WritePrivateProfileString(sSection, sKey, sValue, sFilePath);

	sKey = _T("JD");
	sValue = "";
	WritePrivateProfileString(sSection, sKey, sValue, sFilePath);

	sKey = _T("TEL_RA");
	sValue = "";
	WritePrivateProfileString(sSection, sKey, sValue, sFilePath);

	sKey = _T("TEL_DEC");
	sValue = "";
	WritePrivateProfileString(sSection, sKey, sValue, sFilePath);

	sKey = _T("TELESCOP");
	sValue = "";
	WritePrivateProfileString(sSection, sKey, sValue, sFilePath);

	sKey = _T("OBSERVAT");
	sValue = "";
	WritePrivateProfileString(sSection, sKey, sValue, sFilePath);
}


afx_msg LRESULT CTelescopeControlDlg::OnGetMessageFromDlg(WPARAM wParam, LPARAM lParam)
{
	if (!g_Telescope.bLink)
		return 0;

	switch ((int)wParam) {

	case UPDATE_INFOMATION:

		if (lParam)
			SaveTCSInfomation();

		break;
	}
	return 0;
}

afx_msg LRESULT CTelescopeControlDlg::OnGetClientStatus(WPARAM wParam, LPARAM lParam)
{
	CString sError;
	char *cBuffer;
	int nLen = 0;
	CString sBuffer;

	switch (LOWORD(lParam)) {

		case FD_CONNECT:
		{
			if (HIWORD(lParam)) {        //error
	
				WSASetLastError(HIWORD(lParam));
	
				sError = pSocket->GetError();
				AfxMessageBox(_T("Check the server open."), MB_OK | MB_ICONHAND);
				m_listTCPLog.AddString(_T("Connection fail."));
				m_btnConnectTelescope.EnableWindow(TRUE);

				return 0;
			}

			g_Telescope.bLink = true;
			m_listTCPLog.AddString(_T("Connected."));

			SetTimer(TIME_CHECK_TCS_POSITION, TIME_MILLISECOND, NULL);

			SetTimer(TIMER_CHECK_TELESCOPE_POSITION, 2000, NULL);
			SetTimer(TIMER_DISPLAY_POSITION, 250, NULL);
			m_btnDisconnectTelescope.EnableWindow(TRUE);
		}
		break;
	
		case FD_WRITE:
		{

		}
		break;
	
		case FD_READ:
		{
			cBuffer = pSocket->Read();
			nLen = pSocket->getlastread();

			sBuffer = cBuffer;
			AssortRecivedData(sBuffer);
		}
		break;

		case FD_CLOSE:
		{
			pSocket->GetError();

			m_listTCPLog.AddString(_T("Disconnected."));

			if (HIWORD(lParam))
				WSASetLastError(HIWORD(lParam));

			WSACleanup();

			if (pSocket) {

				delete pSocket;
				pSocket = NULL;
			}

			m_btnConnectTelescope.EnableWindow(TRUE);
		}
		break;
	
	}

	return 0;
}


void CTelescopeControlDlg::AssortRecivedData(CString sServerData)
{
	int nIndex = 0;
	int nDifferenceRA = -1;
	int nDifferenceDec = -1;

	if (sServerData.Mid(3, 4) == "Date") {

		nIndex = sServerData.Find(_T("JD"));
		g_Telescope.sJD = sServerData.Mid(nIndex + 5, 13);

		nIndex = sServerData.Find(_T("RA"));
		m_sCurrentRA = sServerData.Mid(nIndex + 5, 11);

		nIndex = sServerData.Find(_T("Dec"));
		m_sCurrentDec = sServerData.Mid(nIndex + 6, 9);
		
		nIndex = sServerData.Find(_T("HA"));
		m_sCurrentHA = sServerData.Mid(nIndex + 5, 11);

		nIndex = sServerData.Find(_T("Alt"));
		m_sCurrentAlt = sServerData.Mid(nIndex + 6, 9);

		nIndex = sServerData.Find(_T("Az"));
		m_sCurrentAz = sServerData.Mid(nIndex + 5, 9);

		nIndex = sServerData.Find(_T("Dome"));
		m_sCurrentDomeAz = sServerData.Mid(nIndex + 14, 6);

		nIndex = sServerData.Find(_T("Diff"));
		if (nIndex == -1) {

			m_sDifferenceRA = "";
			m_sDifferenceDec = "";
			m_sDifferenceHA = "";
			m_sDifferenceAlt = "";
			m_sDifferenceAz = "";
		}
		else {

			nIndex = sServerData.Find(_T("dRA"));
			m_sDifferenceRA = sServerData.Mid(nIndex + 6, 11);

			nIndex = sServerData.Find(_T("dDec"));
			m_sDifferenceDec = sServerData.Mid(nIndex + 7, 9);

			nIndex = sServerData.Find(_T("dHA"));
			m_sDifferenceHA = sServerData.Mid(nIndex + 6, 11);

			nDifferenceRA = m_sDifferenceRA.Find(_T("0:00:00.0"));
			nDifferenceDec = m_sDifferenceDec.Find(_T("0:00:00"));

			if (nDifferenceRA < 0 && nDifferenceDec < 0) {

				m_sTargetRA = "";
				m_sTargetDec = "";
			}
			else {

				double dCurrentRASecond = _wtof(m_sDifferenceRA.Right(4));
				double dDifferenceDecSecond = _wtof(m_sDifferenceDec.Right(2));

				if (dCurrentRASecond == 0.0 && dDifferenceDecSecond == 0) {

					m_sTargetRA = m_sCurrentRA;
					m_sTargetDec = m_sCurrentDec;
				}
				else {

					m_sTargetRA = m_sTargetRA;
					m_sTargetDec = m_sTargetDec;
				}
			}
		}
	}
	
	UpdateData(false);
}