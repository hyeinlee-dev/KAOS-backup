//=======================================================================================
//================= Telescope / Dome Simulation & Control Library  v1.00 ================
//=======================================================================================

/* Modification Log

DATE			NAME			CONTENTS
2017.08.07		TJ				First version of library was created.
2017.08.09		TJ				Added function for ASCOM Library.
*/

#include "stdafx.h"

IMPLEMENT_DYNAMIC(CLIB_TCP, CWnd)

CLIB_TCP::CLIB_TCP()
{

}

CLIB_TCP::~CLIB_TCP()
{

}

BEGIN_MESSAGE_MAP(CLIB_TCP, CWnd)
END_MESSAGE_MAP()

//===============================================================================
//====================== Control Functions for Telescope ========================
//===============================================================================

void CLIB_TCP::TCPLinkTelescope(void) {

	switch (g_Telescope.nControlMode) {

	case CONTROL_ASCOM_MODE:
	{
		g_Telescope.bDriver = pLibAscomTCS->LibAscomConnectTelescopeDriver();

		if (g_Telescope.bDriver)
			g_Telescope.bLink = pLibAscomTCS->LibAscomLinkTelescope();
	}
	break;

	case CONTROL_REAL_MODE:
	{

	}
	break;
	
	}
}

void CLIB_TCP::TCPUnlinkTelescope(void) {

	switch (g_Telescope.nControlMode) {

	case CONTROL_ASCOM_MODE:

		g_Telescope.bLink = pLibAscomTCS->LibAscomUnlinkTelescope();
		break;

	case CONTROL_REAL_MODE:

		break;
	}
}

void CLIB_TCP::TCPGetTelescopePosition(void) {

	switch (g_Telescope.nControlMode) {

	case CONTROL_ASCOM_MODE:

		double dTotalSecond;
		int nHour, nMinute;
		double dSecond;

		g_Telescope.dCurrentRA = pLibAscomTCS->LibAscomGetTelescopeRA();
		g_Telescope.dCurrentDec = pLibAscomTCS->LibAscomGetTelescopeDec();

		dTotalSecond = g_Telescope.dCurrentRA * 3600.0;
		nHour = (int)(dTotalSecond / 3600.0);
		nMinute = ((int)dTotalSecond - (nHour * 3600)) / 60;
		dSecond = dTotalSecond - (double)(nHour * 3600) - (double)(nMinute * 60);

		g_Telescope.sCurrentRA.Format(_T("%02d:%02d:%04.1f"), nHour, nMinute, dSecond);

		double dTotalArcsecond;
		int nDegree, nArcminute;
		double dArcsecond;

		dTotalArcsecond = g_Telescope.dCurrentDec * 3600.0;
		nDegree = (int)(dTotalArcsecond / 3600.0);
		nArcminute = ((int)dTotalArcsecond - (nDegree * 3600)) / 60;
		dArcsecond = dTotalArcsecond - (double)(nDegree * 3600) - (double)(nArcminute * 60);

		if (g_Telescope.dCurrentDec < 0) {

			nDegree = - nDegree;
			nArcminute = -nArcminute;
			g_Telescope.sCurrentDec.Format(_T("-%02d:%02d:%04.1f"), nDegree, nArcminute, dArcsecond);
		}
		else
			g_Telescope.sCurrentDec.Format(_T("%02d:%02d:%04.1f"), nDegree, nArcminute, dArcsecond);

//		g_Telescope.sCurrentHA = "03:00:00";

		break;

	case CONTROL_REAL_MODE:

		break;
	}
}

void CLIB_TCP::TCPSetTelescopePosition(void) {

	switch (g_Telescope.nControlMode) {

	case CONTROL_ASCOM_MODE:

		double dHour, dMinute, dSecond;
		double dTotalHour;

		dHour = _wtof(g_Telescope.sSetTargetRA.Left(2));
		dMinute = _wtof(g_Telescope.sSetTargetRA.Mid(3,2));
		dSecond = _wtof(g_Telescope.sSetTargetRA.Right(2));
		dTotalHour = dHour + (dMinute / 60.0) + (dSecond / 3600.0);

		double dDegree, dArcminute, dArcsecond;
		double dTotalDegree;

		dDegree = _wtof(g_Telescope.sSetTargetDec.Left(2));
		dArcminute = _wtof(g_Telescope.sSetTargetDec.Mid(3, 2));
		dArcsecond = _wtof(g_Telescope.sSetTargetDec.Right(2));
		dTotalDegree = dDegree + (dArcminute / 60.0) + (dArcsecond / 3600.0);

		g_Telescope.dSetTargetRA = dTotalHour;
		g_Telescope.dSetTargetDec = dTotalDegree;

		pLibAscomTCS->LibAscomSetTelescopePosition();
		break;

	case CONTROL_REAL_MODE:

		break;
	}
}

void CLIB_TCP::TCPSlewTelescope(void) {

	switch (g_Telescope.nControlMode) {

	case CONTROL_ASCOM_MODE:

		pLibAscomTCS->LibAscomSlewTelescope();
		break;

	case CONTROL_REAL_MODE:

		break;
	}
}

void CLIB_TCP::TCPTrackingTelescope(bool bValue) {

	switch (g_Telescope.nControlMode) {

	case CONTROL_ASCOM_MODE:

		pLibAscomTCS->LibAscomTrackingTelescope(bValue);
		break;

	case CONTROL_REAL_MODE:

		break;
	}
}