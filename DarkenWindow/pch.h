#pragma once

//#define ISOLATION_AWARE_ENABLED 1
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <windowsx.h>
#pragma comment(lib, "msimg32.lib")
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")
#include <commctrl.h>
#pragma comment(lib, "comctl32.lib")
#include <vsstyle.h>
#include <vssym32.h>
#include <uxtheme.h>
#pragma comment(lib, "uxtheme.lib")
#include <dwmapi.h>
#pragma comment(lib, "dwmapi.lib")
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

#include <tchar.h>
#include <crtdbg.h>
#include <strsafe.h>
#include <locale.h>

#include <algorithm>
#include <string>
#include <memory>
#include <vector>
#include <list>
#include <map>

#import <msxml3.dll>

#include "AviUtl/aviutl_plugin_sdk/filter.h"
#include "AviUtl/aviutl_exedit_sdk/aviutl.hpp"
#include "AviUtl/aviutl_exedit_sdk/exedit.hpp"
#include "Common/Tracer.h"
#include "Common/WinUtility.h"
#include "Common/FileUpdateChecker.h"
#include "Common/MSXML.h"
#include "Common/Hook.h"
#include "Common/AviUtlInternal.h"
#include "Detours.4.0.1/detours.h"
#pragma comment(lib, "Detours.4.0.1/detours.lib")

//---------------------------------------------------------------------
#if 0
#pragma comment(linker,"\"/manifestdependency:type='win32' \
name='Microsoft.Windows.Common-Controls' version='6.0.0.0' \
processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
