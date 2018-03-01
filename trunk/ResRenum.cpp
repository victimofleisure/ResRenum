// Copyleft 2001 Chris Korda
// This program is free software; you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by the Free
// Software Foundation; either version 2 of the License, or any later version.
/*
        chris korda
 
		revision history:
		rev		date	comments
        00      16apr01 initial version
        01      30sep04 sort resource names
        02      21oct16	refactor to use CString and support Unicode
 
        renumber resource IDs in an MFC project
 
*/

// ResRenum.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "ResRenum.h"
#include <afxtempl.h> 

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif 

/////////////////////////////////////////////////////////////////////////////
// The one and only application object

CWinApp theApp;

CMap<CString, LPCTSTR, int, int>	mapID;

void AddIDStr(const CString& sID)
{
	mapID.SetAt(sID, 0);
}

void ReadResourceIDs(LPCTSTR szPath)
{
	struct SECTION_INFO {
		LPCTSTR	szName;
		UINT	nFlags;
	};
	enum {	// resource sections
		RS_ACCELERATORS,
		RS_BITMAP,
		RS_CURSOR,
		RS_DESIGNINFO,
		RS_DIALOG,
		RS_DIALOGEX,
		RS_DLGINIT,
		RS_ICON,
		RS_MENU,
		RS_STRINGTABLE,
		RS_TOOLBAR,
		RS_VERSIONINFO,
		SECTIONS
	};
	enum {	// section flags
		SF_SINGLE	= 0x01,		// single-line (no begin/end pair)
		SF_NAMED	= 0x02,		// section identifier is meaningful
	};
	static const SECTION_INFO SecInfo[SECTIONS] = {
		{_T("ACCELERATORS"),	SF_NAMED},
		{_T("BITMAP"),			SF_SINGLE},
		{_T("CURSOR"),			SF_SINGLE},
		{_T("DESIGNINFO"),		0},
		{_T("DIALOG"),			SF_NAMED},
		{_T("DIALOGEX"),		SF_NAMED},
		{_T("DLGINIT"),			0},
		{_T("ICON"),			SF_SINGLE},
		{_T("MENU"),			SF_NAMED},
		{_T("STRINGTABLE"),		0},
		{_T("TOOLBAR"),			SF_NAMED},
		{_T("VERSIONINFO"),		0},
	};
	enum {	// parse states
		PS_FIND_SECTION,		// searching for section header
		PS_FIND_BEGIN,			// searching for section begin tag
		PS_IN_SECTION,			// processing section contents
		PARSE_STATES
	};
	FILE	*pfIn;
	if (_tfopen_s(&pfIn, szPath, _T("rt, ccs=UNICODE")))
		CFileException::ThrowErrno(errno, szPath);
    CStdioFile fIn(pfIn);
	CString	sLine;
	int	iState = PS_FIND_SECTION;
	int	iCurSec = 0;
	while (fIn.ReadString(sLine)) {
		switch (iState) {
		case PS_FIND_BEGIN:
			if (sLine == _T("BEGIN"))	// if start of section found
				iState = PS_IN_SECTION;
			break;
		case PS_IN_SECTION:
			if (sLine == _T("END")) {	// if end of section reached
				iState = PS_FIND_SECTION;
				break;
			}
			switch (iCurSec) {
			case RS_ACCELERATORS:
				{
					int	iPos = 0;
					CString	sChar(Tokenize(sLine, _T(" "), iPos));
					if (sChar.IsEmpty())
						break;
					CString	sID(Tokenize(sLine, _T(" ,"), iPos));
					if (sID.IsEmpty())
						break;
					AddIDStr(sID);
				}
				break;
			case RS_DIALOG:
			case RS_DIALOGEX:
				{
					static const LPCTSTR szIDFirst[] = {
						_T("EDITTEXT"),
						_T("BEDIT"),
						_T("HEDIT"),
						_T("IEDIT"),
						_T("LISTBOX"),
						_T("COMBOBOX"),
						_T("SCROLLBAR"),
					};
					const int nCtrlWidth = 20;
					CString	sCtrl(sLine.Left(nCtrlWidth));
					CString	sParam(sLine.Mid(nCtrlWidth));
					sCtrl.TrimRight();
					sCtrl.TrimLeft();
					if (sCtrl.IsEmpty())
						break;
					int	nIDFirsts = _countof(szIDFirst);
					int	iItem;
					for (iItem = 0; iItem < nIDFirsts; iItem++) {
						if (sCtrl == szIDFirst[iItem])
							break;
					}
					if (iItem < nIDFirsts) {	// if ID is first parameter
						int	iPos = 0;
						CString	sID(Tokenize(sParam, _T(" ,"), iPos));
						if (sID.IsEmpty())
							break;
						AddIDStr(sID);
					} else {	// ID is second parameter; first is caption
						int	iPos = 0;
						if (sParam[0] == '"') {	// if first parameter is quoted
							if (sParam.Left(3) == _T("\"\",")) {	// if empty string
								sParam = sParam.Mid(3);
							} else {	// non-empty string; parse caption
								CString	sText(Tokenize(sParam, _T("\""), iPos));
								if (sText.IsEmpty())
									break;
							}
						} else {	// first parameter not quoted; happens with icons
							CString	sText(Tokenize(sParam, _T(","), iPos));
							if (sText.IsEmpty())
								break;
						}
						CString	sID(Tokenize(sParam, _T(",\""), iPos));
						if (sID.IsEmpty()) {	// if identifier not found
							fIn.ReadString(sParam);	// try next line
							sParam.TrimLeft();
							iPos = 0;
							sID = Tokenize(sParam, _T(" ,"), iPos);
							if (sID.IsEmpty())
								break;
						}
						if (sID == _T("IDC_STATIC"))	// ignore static controls
							break;
						AddIDStr(sID);
					}
				}
				break;
			case RS_MENU:
				{
					int	iPos = 0;
					CString	sItem(Tokenize(sLine, _T(" "), iPos));
					if (sItem != _T("MENUITEM"))
						break;
					CString	sText(Tokenize(sLine, _T("\""), iPos));
					if (sText.IsEmpty())
						break;
					CString	sID(Tokenize(sLine, _T(", "), iPos));
					if (sID.IsEmpty())
						break;
					AddIDStr(sID);
				}
				break;
			case RS_STRINGTABLE:
				{
					int	iPos = 0;
					CString	sID(Tokenize(sLine, _T(" "), iPos));
					if (sID.IsEmpty() || sID[0] == '"')
						break;
					AddIDStr(sID);
				}
				break;
			case RS_TOOLBAR:
				{
					int	iPos = 0;
					CString	sItem(Tokenize(sLine, _T(" "), iPos));
					if (sItem.IsEmpty())
						break;
					CString	sID(Tokenize(sLine, _T(" "), iPos));
					if (sID.IsEmpty())
						break;
					AddIDStr(sID);
				}
				break;
			}
			break;
		default:	// searching for section header
			if (sLine.Left(2) == _T("//"))	// if comment line
				continue;
			int	iPos = 0;
			CString	sID(Tokenize(sLine, _T(" "), iPos));
			if (sID.IsEmpty())
				continue;
			if (sID == _T("STRINGTABLE")) {	// if string table
				iCurSec = RS_STRINGTABLE;
				iState = PS_FIND_BEGIN;	// find start of section
				continue;
			}
			CString	sTag(Tokenize(sLine, _T(" "), iPos));
			if (sTag.IsEmpty())
				continue;
			int	iSec;
			for (iSec = 0; iSec < SECTIONS; iSec++) {
				if (sTag == SecInfo[iSec].szName)
					break;
			}
			if (iSec >= SECTIONS)	// if unknown section
				continue;
			if (SecInfo[iSec].nFlags & SF_SINGLE) {	// if single line
				AddIDStr(sID);
			} else {	// proper section with begin/end pair
				if (SecInfo[iSec].nFlags & SF_NAMED)
					AddIDStr(sID);
				iCurSec = iSec;
				iState = PS_FIND_BEGIN;	// find start of section
			}
		}
	}
}

class CDefine {
public:
	CString	m_sName;
	UINT	m_nVal;
};

enum {	// resource ID types
	RIT_APP,
	RIT_BITMAP,
	RIT_ICON,
	RIT_DIALOG,
	RIT_MENU,
	RIT_STRING,
	RIT_STRING_ALT,
	RIT_CONTROL,
	RIT_COMMAND,
	RIT_CONTROL_ALT,
	RES_ID_TYPES = 10,
};

static const LPCTSTR szPrefix[RES_ID_TYPES] = {
    _T("IDR_"),
    _T("IDB_"),
    _T("IDI_"),
    _T("IDD_"),
    _T("IDM_"),
    _T("IDS_"),
    _T("IDP_"),
    _T("IDC_"),
    _T("ID_"),
    _T("IDW_"),
};

enum {
	NEXT_SYMED = 101,
	NEXT_CONTROL = 1001,
	NEXT_COMMAND = 32771,
	NEXT_RESOURCE = 101,
	RESERVED_IDS = IDHELP,
};

bool HasPrefix(LPCTSTR szName, LPCTSTR szPrefix)
{
	return !_tcsncmp(szName, szPrefix, _tcslen(szPrefix));
}

int SortCompareFunc(const void *p1, const void *p2)
{
	const CDefine *pDef1 = (CDefine *)p1;
	const CDefine *pDef2 = (CDefine *)p2;
	// first ID without '_'
	if (!_tcschr(pDef1->m_sName, '_')) {
		if (!_tcschr(pDef2->m_sName, '_'))
			return _tcscmp(pDef1->m_sName, pDef2->m_sName);
		return -1;
	}
	if (!_tcschr(pDef2->m_sName, '_')) {
		if (!_tcschr(pDef1->m_sName, '_'))
			return _tcscmp(pDef1->m_sName, pDef2->m_sName);
		return 1;
	}
	// next extract IDR_MAINFRAME as first IDR_
	if (!_tcscmp(pDef1->m_sName, _T("IDR_MAINFRAME"))) 
		return -1;
	if (!_tcscmp(pDef2->m_sName, _T("IDR_MAINFRAME"))) 
		return 1;
	// next ID?_
	for (int iType = 0; iType < RES_ID_TYPES; iType++) {
		bool	bHasPrefix1 = HasPrefix(pDef1->m_sName, szPrefix[iType]);
		bool	bHasPrefix2 = HasPrefix(pDef2->m_sName, szPrefix[iType]);
		if (bHasPrefix1 || bHasPrefix2) {	// if either has prefix
			if (!bHasPrefix1) {
				if (bHasPrefix2)
					return 1;
			} else {
				if (!bHasPrefix2)
					return -1;
			}
			return _tcscmp(pDef1->m_sName, pDef2->m_sName);
		}
	}
	return _tcscmp(pDef1->m_sName, pDef2->m_sName);
}

bool RenumberResourceHeader(LPCTSTR szSrcPath, LPCTSTR szDstPath)
{
	CString	sAppName;
	CArray<CDefine, CDefine&> aDef;
	int	iDef, iRes;
	int nDefs = 0;
	FILE	*pfIn;
	if (_tfopen_s(&pfIn, szSrcPath, _T("rt, ccs=UNICODE")))
		CFileException::ThrowErrno(errno, szSrcPath);
	CStdioFile fIn(pfIn);
	CString	sLine; 
	while (fIn.ReadString(sLine)) {
		// get application name from resource.h file
		static const TCHAR USED_BY[] = _T("Used by ");
		int	iPos = sLine.Find(USED_BY);
		if (iPos >= 0) {
			iPos += _countof(USED_BY) - 1;
			sAppName = Tokenize(sLine, _T(" "), iPos);
			continue;
		}
		static const TCHAR DEFINE[] = _T("#define");
		iPos = sLine.Find(DEFINE);
		if (iPos >= 0) {
			iPos += _countof(DEFINE) - 1;
			CString	sName(Tokenize(sLine, _T(" "), iPos));
			if (sName.IsEmpty())
				continue;
			CString	sVal(Tokenize(sLine, _T(" "), iPos));
			if (sVal.IsEmpty())
				continue;
			if (HasPrefix(sName, _T("_APS_")))	// skip APS lines
				continue;
			int	iVal;
			if (mapID.GetCount() && !mapID.Lookup(sName, iVal)) {
				_tprintf(_T("deleted %s\n"), sName);
				continue;
			}
			for (iDef = 0; iDef < nDefs; iDef++) {	// find symbol name in list
				if (aDef[iDef].m_sName == sName) 
					break;
			}
			if (iDef >= nDefs) {	// if not found
				CDefine def;
				def.m_sName = sName;
				if (sVal.Find('x'))
					def.m_nVal = (int)_tcstol(sVal, NULL, 16);
				else
					def.m_nVal = _ttoi(sVal);
				aDef.Add(def);
				nDefs++;
			}
		}
	}
	fIn.Close();
	if (!nDefs)	// if no definitions found
		return false;
	UINT	nNextSymed = NEXT_SYMED;
	UINT	nNextControl = NEXT_CONTROL;
	UINT	nNextCommand = NEXT_COMMAND;
	UINT	nNextResource = NEXT_RESOURCE;
	UINT	nSingleWordIDs = RESERVED_IDS + 1;
	// sort define array by ID
	qsort(aDef.GetData(), nDefs, sizeof(CDefine), SortCompareFunc);
	// first renumber IDs without '_'
	for (iDef = 0; iDef < nDefs; iDef++) {
		if (_tcschr(aDef[iDef].m_sName, '_')) 
			break;
		aDef[iDef].m_nVal = nSingleWordIDs;
		nSingleWordIDs++;
	}
	// check resource start
	if (nNextResource < nSingleWordIDs)
		nNextResource = nSingleWordIDs;
	// renumber resources in first section
	for (iDef = 0; iDef < nDefs; iDef++) {
		for (iRes = RIT_APP; iRes <= RIT_MENU; iRes++)
			if (HasPrefix(aDef[iDef].m_sName, szPrefix[iRes]))
				aDef[iDef].m_nVal = nNextResource++;
	}
	// start strings on next hundred
	nNextResource = (nNextResource / 100 + 1) * 100;
	// renumber string resources
	for (iDef = 0; iDef < nDefs; iDef++) {
		for (iRes = RIT_STRING; iRes <= RIT_STRING_ALT; iRes++) {
			if (HasPrefix(aDef[iDef].m_sName, szPrefix[iRes]))
				aDef[iDef].m_nVal = nNextResource++;
		}
	}
	// start controls on next thousand
	if (nNextControl < nNextResource) 
		nNextControl = (nNextControl / 1000 + 1) * 1000;
	// renumber controls
	for (iDef = 0; iDef < nDefs; iDef++) {
		if (HasPrefix(aDef[iDef].m_sName, szPrefix[RIT_CONTROL]))
			aDef[iDef].m_nVal = nNextControl++;
	}
	// renumber commands
	for (iDef = 0; iDef < nDefs; iDef++) {
		if (HasPrefix(aDef[iDef].m_sName, szPrefix[RIT_COMMAND]))
			aDef[iDef].m_nVal = nNextCommand++;
	}
	// create new resource file
	CStdioFile	fOut(szDstPath, CFile::modeWrite | CFile::modeCreate);
	fOut.WriteString(
		_T("//{{NO_DEPENDENCIES}}\n")
		_T("// Microsoft Developer Studio generated include file.\n"));
	sLine.Format(_T("// Used by %s\n"), sAppName);
	fOut.WriteString(sLine);
	fOut.WriteString(_T("//\n"));
	for (iDef = 0; iDef < nDefs; iDef++) {
		const CDefine&	def = aDef[iDef];
		if (HasPrefix(def.m_sName, szPrefix[RIT_CONTROL_ALT]))
			sLine.Format(_T("#define %-31s 0x%X\n"), def.m_sName, def.m_nVal);
		else
			sLine.Format(_T("#define %-31s %u\n"), def.m_sName, def.m_nVal);
		fOut.WriteString(sLine);
	}
	fOut.WriteString(
		_T("\n")
		_T("// Next default values for new objects\n")
		_T("//\n")
		_T("#ifdef APSTUDIO_INVOKED\n")
		_T("#ifndef APSTUDIO_READONLY_SYMBOLS\n"));
	sLine.Format(_T("#define %-31s %u\n"), _T("_APS_3D_CONTROLS"), 1);
	fOut.WriteString(sLine);
	sLine.Format(_T("#define %-31s %u\n"), _T("_APS_NEXT_RESOURCE_VALUE"), nNextResource);
	fOut.WriteString(sLine);
	sLine.Format(_T("#define %-31s %u\n"), _T("_APS_NEXT_COMMAND_VALUE"), nNextCommand);
	fOut.WriteString(sLine);
	sLine.Format(_T("#define %-31s %u\n"), _T("_APS_NEXT_CONTROL_VALUE"), nNextControl);
	fOut.WriteString(sLine);
	sLine.Format(_T("#define %-31s %u\n"), _T("_APS_NEXT_SYMED_VALUE"), nNextSymed);
	fOut.WriteString(sLine);
	fOut.WriteString(
		_T("#endif\n")
		_T("#endif\n"));
	return true;
}

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	UNREFERENCED_PARAMETER(envp);
	int nRetCode = 0;
	// initialize MFC and print and error on failure
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0)) {
		printf("Fatal Error: MFC initialization failed\n");
		nRetCode = 1;
	} else {
		if (argc > 1) {
			TRY {
				if (argc > 2)
					ReadResourceIDs(argv[2]);
				RenumberResourceHeader(argv[1], argv[1]);
			}
			CATCH (CException, e) {
				TCHAR	szMsg[MAX_PATH];
				e->GetErrorMessage(szMsg, _countof(szMsg));
				_tprintf(_T("%s\n"), szMsg);
				nRetCode = 1;
			}
			END_CATCH
		} else {
			_tprintf(_T("not enough arguments\n"));
			nRetCode = 1;
		}
	}
	return nRetCode;
}
