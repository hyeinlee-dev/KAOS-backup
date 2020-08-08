
// TelescopeControlDlg.h : header file
//

#pragma once
#include "afxwin.h"
#include "Socket.h"


// CTelescopeControlDlg dialog
class CTelescopeControlDlg : public CDialogEx
{
// Construction
public:
	CTelescopeControlDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TELESCOPECONTROL_DIALOG };
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
public:
	afx_msg void OnClickedButtonConnectTelescope();
	afx_msg void OnClickedButtonDisconnectTelescope();
	CButton m_btnConnectTelescope;
	CButton m_btnDisconnectTelescope;
	CListBox m_listTCPLog;
	void AddTCPLog(CString sLog);
	void SaveLogFile(CString sPackageLog);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
//	CString m_sCurrentRA;
//	CString m_sCurrentDec;
//	CString m_sCurrentHA;
//	CString m_sSetTargetRA;
//	CString m_sSetTargetDec;
//	afx_msg void OnClickedButtonSetTarget();
//	CString m_sTargetRA;
//	CString m_sTargetDec;
//	CEdit m_sTargetHA;
//	afx_msg void OnKillfocusEditSetTargetRA();
//	afx_msg void OnSetfocusEditSetTargetRA();
	afx_msg void OnSetfocusEditSetTargetDec();
	afx_msg void OnKillfocusEditSetTargetDec();
	void SaveTCSInfomation();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	afx_msg LRESULT OnGetMessageFromDlg(WPARAM wParam, LPARAM lParam);
//	afx_msg LRESULT OnGetTelecopeMessage(WPARAM wParam, LPARAM lParam);
//	afx_msg LRESULT OnGetClientStatus(WPARAM wParam, LPARAM lParam);
public:
//	CButton m_btnSetTarget;
//	CString m_sDifferenceRA;
//	CString m_sDifferenceDec;
//	void CalculateDifferenceRA();
//	void CalculateDifferenceDec();
//	CListBox m_listWeatherLog;
//	void GetWeatherInfomation();
	HWND h_TELNETdlg;
private:
	Socket *pSocket;
public:
	CString m_sServerAddress;
	int m_nPortNumber;
	void SendCommandToServer(CString sCommand);
protected:
	afx_msg LRESULT OnGetClientStatus(WPARAM wParam, LPARAM lParam);
public:
	void AssortRecivedData(CString sServerData);
//	CString m_sCurrentAlt;
//	CString m_sCurrentAz;
//	CString m_sCurrentDomeAz;
//	CButton m_btnFindHome;
//	CButton m_btnZenith;
//	CButton m_btnStow;
//	CButton m_btnSlew;
//	CButton m_btnSetObject;
//	CButton m_btnStartTracking;
//	CButton m_btnRoofOpen;
//	CButton m_btnRoofClose;
//	CButton m_btnDomeAuto;
//	CButton m_btnAllProcessStop;
//	CString m_sDifferenceHA;
//	CString m_sDifferenceAlt;
//	CString m_sDifferenceAz;
//	CStatic m_picDisplayPosition;
//	void DisplayTelescopePosition();
//	CButton m_btnDomeHome;
//	void CalTelescopePosition();
//	CString m_sTargetName;
//	afx_msg void OnClickedButtonSetTargetInfo();
//	CString m_sSetObjectRA;
//	CString m_sSetObjectDec;
//	afx_msg void OnKillfocusEditGetTargetName();
//	afx_msg void OnSetfocusEditSetTargetInfo();
	void KillTimerAll();
	void SetTimerAll();
};
