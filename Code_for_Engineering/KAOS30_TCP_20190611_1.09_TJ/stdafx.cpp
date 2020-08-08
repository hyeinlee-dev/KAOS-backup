
// stdafx.cpp : source file that includes just the standard includes
// TelescopeControl.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

CLIB_ASCOM_TCS *pLibAscomTCS;
CLIB_TCP *pLibTCP;

struct TELESCOPE g_Telescope;
struct DOME g_Dome;
struct OBSERVATORY g_Observatory;