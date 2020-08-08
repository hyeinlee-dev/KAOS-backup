
// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // Exclude rarely-used stuff from Windows headers
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // some CString constructors will be explicit

// turns off MFC's hiding of some common and often safely ignored warning messages
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions


#include <afxdisp.h>        // MFC Automation classes



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC support for Internet Explorer 4 Common Controls
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // MFC support for ribbons and control bars

#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define WM_CLIENT WM_APP+1

#include <winsock2.h>
#include <Ws2tcpip.h>

#include "LIB_ASCOM_TCS.h"
#include "LIB_TCP.h"

extern CLIB_ASCOM_TCS *pLibAscomTCS;
extern CLIB_TCP *pLibTCP;

#define MAX_BUFF	2048

#define CONTROL_ASCOM_MODE	0
#define CONTROL_REAL_MODE	1

#define TIMER_CHECK_TELESCOPE_POSITION	0
#define TIME_CHECK_TCS_POSITION			1
#define TIMER_CHECK_DIFFERENCE_POSITION 2
#define TIMER_CHECK_WEATHER				3 // add 20170921 by Tae-Geun
#define TIMER_DISPLAY_POSITION			4
#define TIME_MILLISECOND 200

struct TELESCOPE {

	int nControlMode;
	bool bLink;
	bool bDriver;

	double dCurrentRA, dCurrentDec, dCurrentHA;
	double dSetTargetRA, dSetTargetDec;

	CString sModel;
	CString sCurrentRA, sCurrentDec, sCurrentHA;
	CString sDifferenceRA, sDifferenceDec;
	CString sSetTargetRA, sSetTargetDec;
	CString sName;
	CString sJD;

	HANDLE m_hThreadSlewTelescope;
	HWND hTCPdlg;

	TELESCOPE() {

		bLink = false;
		bDriver = false;
		sName = _T("TAKAHASHI CCA-250");
	}
};
extern struct TELESCOPE g_Telescope;

struct DOME {

	int nControlMode;
	bool bLink;
};
extern struct DOME g_Dome;

struct OBSERVATORY {

	CString sName = _T("McDonald Observatory");
};
extern struct OBSERVATORY g_Observatory;

// Define ID for communication for Telescope
#define WM_TELESCOPE WM_USER + 2000
#define ON_TELESCOPE_MOVE 0
#define OFF_TELESCOPE_MOVE 1

// Define ID for communication between Packages
#define WM_MAIN		WM_USER + 7000
#define WM_TCP		WM_USER + 7001
#define WM_DAP		WM_USER + 7002
#define WM_AFP		WM_USER + 7003

#define ID_MAIN	1
#define ID_TCP	2
#define ID_DAP	3
#define ID_AFP	4

#define EXIT_DIALOG	0

//color
#define CR_CYAN			RGB(0, 255, 255)
#define CR_YELLOW		RGB(255, 255, 0)
#define CR_BLUE			RGB(0, 0, 255)
#define	CR_RED			RGB(255, 0, 0)
#define	CR_PURPLE		RGB(128, 0, 128)
#define	CR_WHITE		RGB(255, 255, 255)
#define CR_BLACK		RGB(0, 0, 0)
#define CR_ORANGE		RGB(255, 102, 0)
#define CR_WEAKORANGE	RGB(255, 180, 51)
#define CR_GREEN		RGB(0, 255, 0)
#define CR_DARKGREEN	RGB(0, 255, 100)
#define CR_DARKBLUE		RGB(0, 0, 102)
#define CR_PINK			RGB(255, 0, 255)

#define _USE_MATH_DEFINES // add 20171228 by Tae-Geun

// add 20170810 by Tae-Geun : Define ID for Information between TCP-DAP
#define UPDATE_INFOMATION 0

// Main title of KAOS30
#define MAIN_PROGRAM _T("KAOS30 V1.09")

#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif


