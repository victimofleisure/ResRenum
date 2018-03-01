// stdafx.cpp : source file that includes just the standard includes
//	ResRenum.pch will be the pre-compiled header
//	stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

// TODO: reference any additional headers you need in STDAFX.H
// and not in this file

#if _MFC_VER <= 0x0600
CString Tokenize(const CString& Str, LPCTSTR pszTokens, int& iStart)
{
	// adapted from NET2008 CStringT::Tokenize
	ASSERT(iStart >= 0);
	if ((pszTokens == NULL) || (*pszTokens == (TCHAR)0)) {
		if (iStart < Str.GetLength()) {
			return(Str.Mid(iStart));
		}
	} else {
		LPCTSTR	pStr = Str;
		LPCTSTR pszPlace = pStr + iStart;
		LPCTSTR pszEnd = pStr + Str.GetLength();
		if (pszPlace < pszEnd) {
			int nIncluding = _tcsspn(pszPlace, pszTokens);
			if ((pszPlace + nIncluding) < pszEnd) {
				pszPlace += nIncluding;
				int nExcluding = _tcscspn(pszPlace, pszTokens);
				int iFrom = iStart + nIncluding;
				int nUntil = nExcluding;
				iStart = iFrom + nUntil + 1;
				return(Str.Mid(iFrom, nUntil));
			}
		}
	}
	// return empty string, done tokenizing
	iStart = -1;
	return(_T(""));
}
#endif

#if _MFC_VER <= 0x0600
#include <errno.h> 
int _tfopen_s(FILE ** pfile, const TCHAR *file, const TCHAR *mode)
{
    if (pfile == NULL)
		return EINVAL;
    *pfile = _tfopen(file, mode);
    if (*pfile != NULL)
        return 0;
    return errno;
}
#endif
