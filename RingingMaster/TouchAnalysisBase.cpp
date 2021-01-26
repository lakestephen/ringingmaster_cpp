#include "StdAfx.h"
#include "TouchAnalysisBase.h"
#include "TouchDoc.h"


TouchAnalysisBase::TouchAnalysisBase(void)
{
}

TouchAnalysisBase::~TouchAnalysisBase(void)
{
}


void TouchAnalysisBase::OnPopupEditCopy(CWnd* pWnd)
{
	TouchDoc * pTouchDoc = mainFrame()->getActiveTouchDoc();
	
	if ((pTouchDoc)&&(pTouchDoc->getMethod())&&(pTouchDoc->getMethod()->hasAnalysis())&& pWnd->OpenClipboard())
	{
		Method * method = pTouchDoc->getMethod();
		const MethodAnalysis* analysis = method->getAnalysis(); 

		EmptyClipboard();

		CWaitCursor wait;

		CString strSource = getCopyData(*method, *analysis);
		
		HGLOBAL strBuff;
		char * buffer;
		
		//str
		strBuff = GlobalAlloc(GMEM_DDESHARE, strSource.GetLength()+1);
		buffer = (char*)GlobalLock(strBuff);
		strcpy(buffer, LPCSTR(strSource));
		GlobalUnlock(strBuff);
		SetClipboardData(CF_TEXT,strBuff);

		CloseClipboard();
	}
}
