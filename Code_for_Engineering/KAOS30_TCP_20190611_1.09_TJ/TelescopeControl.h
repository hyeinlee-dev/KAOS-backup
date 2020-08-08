
// TelescopeControl.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CTelescopeControlApp:
// See TelescopeControl.cpp for the implementation of this class
//

class CTelescopeControlApp : public CWinApp
{
public:
	CTelescopeControlApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CTelescopeControlApp theApp;