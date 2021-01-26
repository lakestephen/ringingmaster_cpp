#pragma once
#include "MainFrameEventListener.h"

class MethodAnalysis;
class Method;

class TouchAnalysisBase : public MainFrameEventListener
{
public:
	TouchAnalysisBase(void);
public:
	virtual ~TouchAnalysisBase(void);

	virtual CString getCopyData(const Method& method, const MethodAnalysis& analysis) = 0;
	void OnPopupEditCopy(CWnd* pWnd);
};
