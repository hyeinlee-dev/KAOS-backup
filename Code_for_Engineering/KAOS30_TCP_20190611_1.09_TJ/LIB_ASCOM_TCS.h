#pragma once

/* Modification Log

DATE			NAME			CONTENTS
2017.08.09		Tae-Geun Ji		First version of C++ library for ASCOM TCS Platform was created.

*/

#include "CChooser.h"
#include "CTelescopeV3.h"

class CLIB_ASCOM_TCS : public CWnd
{
	DECLARE_DYNAMIC(CLIB_ASCOM_TCS)

public:
	CLIB_ASCOM_TCS();
	virtual ~CLIB_ASCOM_TCS();

protected:
	DECLARE_MESSAGE_MAP()

public:

	CString g_sTelescopeName;

	//========== Initial Functions for ASCOM Instruments
	bool LibAscomConnectTelescopeDriver(void);

	//========== Control Functions for ASCOM Instruments
	bool LibAscomLinkTelescope(void);
	bool LibAscomUnlinkTelescope(void);
	double LibAscomGetTelescopeRA(void);
	double LibAscomGetTelescopeDec(void);
	void LibAscomSetTelescopePosition(void);
	void LibAscomSlewTelescope(void);
	void LibAscomTrackingTelescope(bool bValue);
};

DWORD ThreadSlewTelesopeProcess(CLIB_ASCOM_TCS* pTCS);