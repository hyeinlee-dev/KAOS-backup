/* Modification Log

DATE			NAME			CONTENTS
2017.08.09		Tae-Geun Ji		First version of C++ library for ASCOM TCS Platform was created.

*/

#include "stdafx.h"
#include "LIB_ASCOM_TCS.h"

CTelescopeV3 pTelescope;

IMPLEMENT_DYNAMIC(CLIB_ASCOM_TCS, CWnd)

CLIB_ASCOM_TCS::CLIB_ASCOM_TCS()
{

}

CLIB_ASCOM_TCS::~CLIB_ASCOM_TCS()
{
}

BEGIN_MESSAGE_MAP(CLIB_ASCOM_TCS, CWnd)
END_MESSAGE_MAP()

//============================================================================================================
//=================================== Initial Functions for ASCOM Telescope ==================================
//============================================================================================================

//===== Get ASCOM Driver(.dll) from Focuser
bool CLIB_ASCOM_TCS::LibAscomConnectTelescopeDriver(void)
{
	CChooser pChoose;
	COleException err;

	if (!pChoose.CreateDispatch(L"ASCOM.Utilities.Chooser", &err))
	{
		CString errMsg;
		errMsg.Format(L"ASCOM driver not found in your computer.");
		AfxMessageBox(errMsg);
		return false;
	}

	pChoose.put_DeviceType(L"Telescope");
	g_sTelescopeName = pChoose.Choose(g_sTelescopeName);
	g_Telescope.sModel = g_sTelescopeName;

	return true;
}

bool CLIB_ASCOM_TCS::LibAscomLinkTelescope(void)
{
	if (pTelescope != NULL) {

		CString errMsg;
		errMsg.Format(L"Telescope is already connected.");
		AfxMessageBox(errMsg);
		return true;
	}

	COleException err;
	if (!pTelescope.CreateDispatch(g_sTelescopeName, &err)) {

		CString errMsg;
		errMsg.Format(L"Telescope is empty. Please select Telescope.");
		AfxMessageBox(errMsg);
		return false;
	}

	pTelescope.put_Connected(TRUE);
	pTelescope.get_Connected();

	return true;
}

bool CLIB_ASCOM_TCS::LibAscomUnlinkTelescope(void)
{
	if (pTelescope != NULL && pTelescope.get_Connected()) {

		pTelescope.put_Connected(FALSE);
		pTelescope = NULL;

		return false;
	}
	return true;
}

double CLIB_ASCOM_TCS::LibAscomGetTelescopeRA(void) {

	double dRA;
	dRA = pTelescope.get_RightAscension();

	return dRA;
}

double CLIB_ASCOM_TCS::LibAscomGetTelescopeDec(void) {

	double dDEC;
	dDEC = pTelescope.get_Declination();

	return dDEC;
}

void CLIB_ASCOM_TCS::LibAscomSetTelescopePosition(void) {

	pTelescope.put_TargetRightAscension(g_Telescope.dSetTargetRA);
	pTelescope.put_TargetDeclination(g_Telescope.dSetTargetDec);
}

void CLIB_ASCOM_TCS::LibAscomSlewTelescope(void) {

	::PostMessage(g_Telescope.hTCPdlg, WM_TELESCOPE, ON_TELESCOPE_MOVE, 0);

	DWORD dwThreadID;

	g_Telescope.m_hThreadSlewTelescope = NULL;
	if (g_Telescope.m_hThreadSlewTelescope != NULL)
		CloseHandle(g_Telescope.m_hThreadSlewTelescope);

	g_Telescope.m_hThreadSlewTelescope = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadSlewTelesopeProcess, this, 0, &dwThreadID);

	if (!g_Telescope.m_hThreadSlewTelescope) {

		AfxMessageBox(L"It can not creat thread for telescope moving!");
		return;
	}
}

void CLIB_ASCOM_TCS::LibAscomTrackingTelescope(bool bValue) {

	pTelescope.put_Tracking(bValue);
}

//===== Thread for Telescope Moving
DWORD ThreadSlewTelesopeProcess(CLIB_ASCOM_TCS* pTCS) {

	pTelescope.SlewToTarget();

	::PostMessage(g_Telescope.hTCPdlg, WM_TELESCOPE, OFF_TELESCOPE_MOVE, 0); // add 20170807 Tae-Geun
	g_Telescope.m_hThreadSlewTelescope = NULL;

	return TRUE;
}