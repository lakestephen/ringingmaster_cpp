#if !defined(AFX_MAPDOC_H__93E03DE4_FAEF_11D6_B63D_000255162CF0__INCLUDED_)
#define AFX_MAPDOC_H__93E03DE4_FAEF_11D6_B63D_000255162CF0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MapDoc.h : header file
//

class CxImage;
class MapView;
class MapDocumentInfo;

#include "MapGridRef.h"
#include "MapManagerEventListener.h"
#include "MainFrameEventListener.h"
#include "RecordTower.h"
#include "RecordManagerEventListener.h"
#include "Scale.h"	// Added by ClassView

/////////////////////////////////////////////////////////////////////////////
// MapDoc document

class MapDoc : public CDocument, MapManagerEventListener, MainFrameEventListener, RecordManagerEventListener
{
protected:
	MapDoc();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(MapDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(MapDoc)
	public:
	virtual void Serialize(CArchive& ar);   // overridden for document i/o
	virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
	protected:
	virtual BOOL OnNewDocument();
	//}}AFX_VIRTUAL

// Implementation
public:	   
	MapView* getMapView();
	
	MapDocumentInfo* getMapDocumentInfo();

	bool saveAsImage(CString fileName, ENUM_CXIMAGE_FORMATS format);
	bool saveAsRTF(CString fileName);

	void setNewScale(ScaleType scale);
	ScaleType getScale();
	int getMapRefId();
	void requestMapId(MapGridRef gridRef, ScaleType scaleType);
	RecordTower* getContextTower();
	void setContextTower(RecordTower* tower);
	void setSelectedTowerIndex(int selectedTowerIndex);
	void getStatusBarText(CString& pane1);
	void setCurrentGridRef(MapGridRef mapGridRef);
	MapGridRef getCurrentGridRef();
	MapCacheItem* getMapCacheItem();
	RecordTowers& getTowersOnMap();
	void mainFrame_notifyUpdate(ViewType viewType);
	void mapManager_notifyMapLoaded(int mapId);

		RecordTower* recordManager_notifyGetSelectedTower(ViewType viewType);

	void setInitialData(const RecordTower* tower);
	void setInitialData(MapDocumentInfo* info);

	virtual ~MapDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif


	BOOL OnPreparePrinting(CPrintInfo* pInfo, CView* view);
	void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo);
	void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	void OnFilePrintPreview(CView* view);
	void OnFilePrint();
	void OnFilePrintToolbar();
	void OnFilePrintMenu();
	BOOL _showPrintSetupDlg;

	// Generated message map functions
protected:
	
	int _mapId;
	CString _name;
	RecordTowers _towersOnMap;
	int _selectedTowerIndex;
	MapGridRef _currentMapGridRef;

	RecordTower* _contextTower;

	//{{AFX_MSG(MapDoc)
	afx_msg void OnMapEast();
	afx_msg void OnMapNorth();
	afx_msg void OnMapSouth();
	afx_msg void OnMapWest();
	afx_msg void OnReloadMap();
	afx_msg void OnFileSaveAs();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAPDOC_H__93E03DE4_FAEF_11D6_B63D_000255162CF0__INCLUDED_)
