// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__0BCBE130_3293_11D5_8353_0050040E4B91__INCLUDED_)
#define AFX_STDAFX_H__0BCBE130_3293_11D5_8353_0050040E4B91__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afx.h>
#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

// TODO: reference additional headers your program requires here

#if _MFC_VER < 0x0800
// calculate number of elements in a fixed-size array
#define _countof(arr) (sizeof(arr) / sizeof(arr[0]))
#endif

#if _MFC_VER > 0x0600
inline CString Tokenize(const CString& Str, LPCTSTR pszTokens, int& iStart)
{
	return(Str.Tokenize(pszTokens, iStart));	// added in NET2005
}
#else
CString Tokenize(const CString& Str, LPCTSTR pszTokens, int& iStart);
#endif

#if _MFC_VER < 0x0800
int _tfopen_s(FILE ** pfile, const TCHAR *file, const TCHAR *mode);
#endif

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__0BCBE130_3293_11D5_8353_0050040E4B91__INCLUDED_)
