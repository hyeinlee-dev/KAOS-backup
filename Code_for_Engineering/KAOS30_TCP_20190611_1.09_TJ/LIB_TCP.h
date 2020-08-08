#pragma once

//=======================================================================================
//================= Telescope / Dome Simulation & Control Library  v1.00 ================
//=======================================================================================

/* Modification Log

DATE			NAME			CONTENTS
2017.08.07		TJ				First version of library was created.

*/

class CLIB_TCP : public CWnd
{
	DECLARE_DYNAMIC(CLIB_TCP)

public:
	CLIB_TCP();

	virtual ~CLIB_TCP();

protected:
	DECLARE_MESSAGE_MAP()

public:

	//========== Control Functions for Camera
	void TCPLinkTelescope(void);
	void TCPUnlinkTelescope(void);
	void TCPGetTelescopePosition(void);
	void TCPSetTelescopePosition(void);
	void TCPSlewTelescope(void);
	void TCPTrackingTelescope(bool bValue);
};