#include "StdAfx.h"
#include ".\docmanagerex.h"
#include "ringingmaster.h"
AFX_STATIC_DATA const TCHAR _afxShellOpenFmt[] = _T("%s\\shell\\open\\%s");
AFX_STATIC_DATA const TCHAR _afxShellPrintFmt[] = _T("%s\\shell\\print\\%s");
AFX_STATIC_DATA const TCHAR _afxDefaultIconFmt[] = _T("%s\\DefaultIcon");
AFX_STATIC_DATA const TCHAR _afxShellNewFmt[] = _T("%s\\ShellNew");

#define DEFAULT_ICON_INDEX 0

AFX_STATIC_DATA const TCHAR _afxIconIndexFmt[] = _T(",%d");
AFX_STATIC_DATA const TCHAR _afxCommand[] = _T("command");
AFX_STATIC_DATA const TCHAR _afxOpenArg[] = _T(" \"%1\"");
AFX_STATIC_DATA const TCHAR _afxPrintArg[] = _T(" /p \"%1\"");

AFX_STATIC_DATA const TCHAR _afxDDEExec[] = _T("ddeexec");

AFX_STATIC_DATA const TCHAR _afxShellNewValueName[] = _T("NullFile");
AFX_STATIC_DATA const TCHAR _afxShellNewValue[] = _T("");


AFX_STATIC BOOL AFXAPI
_AfxSetRegKey(LPCTSTR lpszKey, LPCTSTR lpszValue, LPCTSTR lpszValueName = NULL)
{
	if (lpszValueName == NULL)
	{
		if (::RegSetValue(HKEY_CLASSES_ROOT, lpszKey, REG_SZ,
			  lpszValue, lstrlen(lpszValue) * sizeof(TCHAR)) != ERROR_SUCCESS)
		{
			TRACE(traceAppMsg, 0, _T("Warning: registration database update failed for key '%s'.\n"),
				lpszKey);
			return FALSE;
		}
		return TRUE;
	}
	else
	{
		HKEY hKey;

		if(::RegCreateKey(HKEY_CLASSES_ROOT, lpszKey, &hKey) == ERROR_SUCCESS)
		{
			LONG lResult = ::RegSetValueEx(hKey, lpszValueName, 0, REG_SZ,
				(CONST BYTE*)lpszValue, (lstrlen(lpszValue) + 1) * sizeof(TCHAR));

			if(::RegCloseKey(hKey) == ERROR_SUCCESS && lResult == ERROR_SUCCESS)
				return TRUE;
		}
		TRACE(traceAppMsg, 0, _T("Warning: registration database update failed for key '%s'.\n"), lpszKey);
		return FALSE;
	}
}




IMPLEMENT_DYNAMIC(CDocManagerEx, CDocManager)

CDocManagerEx::CDocManagerEx(void)
{
}

CDocManagerEx::~CDocManagerEx(void)
{
}


//NOTE: This is a copy of the base class that only registers the classes I want registered.
void CDocManagerEx::RegisterShellFileTypes(BOOL /*bCompat*/)
{
	ASSERT(!m_templateList.IsEmpty());  // must have some doc templates

	CString strPathName, strTemp;

	strPathName = "\"" + app()->_rootPathFull + "\"";

	POSITION pos = m_templateList.GetHeadPosition();
	for (int nTemplateIndex = 1; pos != NULL; nTemplateIndex++)
	{
		CDocTemplate* pTemplate = (CDocTemplate*)m_templateList.GetNext(pos);

		CString strOpenCommandLine = strPathName;
		CString strPrintCommandLine = strPathName;
		CString strDefaultIconCommandLine = strPathName;

		CString strIconIndex;

		strIconIndex.Format(_afxIconIndexFmt, nTemplateIndex);

		strDefaultIconCommandLine += strIconIndex;

		CString strFilterExt, strFileTypeId, strFileTypeName;
		if (pTemplate->GetDocString(strFileTypeId,
		   CDocTemplate::regFileTypeId) && !strFileTypeId.IsEmpty()
		   
		   //************************************************
		   //My New Code
		   &&(strFileTypeId == "Touch.Document" || 
			  strFileTypeId == "Crib.Document" ||
			  strFileTypeId == "Strike.Document" )
		   //************************************************
		   )
		{
			// enough info to register it
			if (!pTemplate->GetDocString(strFileTypeName,
			   CDocTemplate::regFileTypeName))
				strFileTypeName = strFileTypeId;    // use id name

			ASSERT(strFileTypeId.Find(' ') == -1);  // no spaces allowed

			// first register the type ID of our server
			if (!_AfxSetRegKey(strFileTypeId, strFileTypeName))
				continue;       // just skip it

			// path\DefaultIcon = path,1
			strTemp.Format(_afxDefaultIconFmt, (LPCTSTR)strFileTypeId);
			if (!_AfxSetRegKey(strTemp, strDefaultIconCommandLine))
				continue;       // just skip it

			// If MDI Application
			// path\shell\open\ddeexec = [open("%1")]
			strTemp.Format(_afxShellOpenFmt, (LPCTSTR)strFileTypeId,
				(LPCTSTR)_afxDDEExec);
			if (!_AfxSetRegKey(strTemp, ""))
				continue;       // just skip it


			// path\shell\print\ddeexec = [print("%1")]
			strTemp.Format(_afxShellPrintFmt, (LPCTSTR)strFileTypeId,
				(LPCTSTR)_afxDDEExec);
			if (!_AfxSetRegKey(strTemp, ""))
				continue;       // just skip it


			// path\shell\open\command = path /dde
			// path\shell\print\command = path /dde
			strOpenCommandLine += _afxOpenArg;
			strPrintCommandLine += _afxPrintArg;

			// path\shell\open\command = path filename
			strTemp.Format(_afxShellOpenFmt, (LPCTSTR)strFileTypeId,
				(LPCTSTR)_afxCommand);
			if (!_AfxSetRegKey(strTemp, strOpenCommandLine))
				continue;       // just skip it

			// path\shell\print\command = path /p filename
			strTemp.Format(_afxShellPrintFmt, (LPCTSTR)strFileTypeId,
				(LPCTSTR)_afxCommand);
			if (!_AfxSetRegKey(strTemp, strPrintCommandLine))
				continue;       // just skip it

			pTemplate->GetDocString(strFilterExt, CDocTemplate::filterExt);
			if (!strFilterExt.IsEmpty())
			{
				ASSERT(strFilterExt[0] == '.');

				LONG lSize = _MAX_PATH * 2;
				LONG lResult = ::RegQueryValue(HKEY_CLASSES_ROOT, strFilterExt,
					strTemp.GetBuffer(lSize), &lSize);
				strTemp.ReleaseBuffer();

				if (lResult != ERROR_SUCCESS || strTemp.IsEmpty() ||
					strTemp != strFileTypeId)
				{
					// no association for that suffix
					if (!_AfxSetRegKey(strFilterExt, strFileTypeId))
						continue;

					if (strFileTypeId != "Strike.Document")
					{
						strTemp.Format(_afxShellNewFmt, (LPCTSTR)strFilterExt);
						(void)_AfxSetRegKey(strTemp, _afxShellNewValue, _afxShellNewValueName);
					}
				}
			}
		}
	}
}