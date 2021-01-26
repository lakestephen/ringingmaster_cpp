// MatrixEntityBase.cpp: implementation of the MatrixEntityBase class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RingingMaster.h"

#include "MatrixEntityBase.h"
#include "MatrixRelation.h"

#include "MatrixFilterSort.h"
#include "MatrixItemBase.h"
#include "MatrixManager.h"
#include "MatrixManagerColumnFactory.h"




#include "LogicDlg.h"
#include "DragDropTarget.h"




//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MatrixFilterSort* MatrixEntityBase::_filterSort = NULL;

MatrixEntityBase::MatrixEntityBase() :
_contextClickObject(0)
{
}

MatrixEntityBase::~MatrixEntityBase()
{
	//never delete the items in this container
	_allColumns.RemoveAll();

	for (int i=0;i<_primaryColumns.GetSize();i++)
		delete _primaryColumns.GetAt(i);
	_primaryColumns.RemoveAll();

	for (int i=0;i<_relations.GetSize();i++)
		delete _relations.GetAt(i);
	_relations.RemoveAll();
}

int MatrixEntityBase::getRowCount()
{
	return _ids.GetSize();
}
  
int MatrixEntityBase::getColumnCount()
{
	return _allColumns.GetSize();
}

MatrixItemBase* MatrixEntityBase::getColumn(int col)
{
	ASSERT((col >= 0)&&(col < _allColumns.GetSize()));

	if ((col >= 0)&&(col < _allColumns.GetSize()))
	{
		return _allColumns.GetAt(col);
	}
	
	return NULL;
}

void MatrixEntityBase::sortListCol(int column, bool bAscending)
{
	MatrixItemBase* item = getColumn(column);
	if (item)
	{
		item->setSortAscending(bAscending);

		sortListSimple(*item);
	}
}

void MatrixEntityBase::applyDefaultSort()
{
	MatrixItemBase* sortColumn = MatrixManagerColumnFactory::getInstance()->createColumnUsingFactory(getDefaultSortToken());
	if (sortColumn)
	{
		sortColumn->setSortAscending(true);
		sortListSimple(*sortColumn);
		delete sortColumn;
	}
}

void MatrixEntityBase::sortListSimple(const MatrixItemBase& item)
{
	// Create a new simple sort to be used.
	MatrixFilterSort simpleSort(getMatrixEntityToken());
	
	simpleSort.setSort(item);
	applyFilterSort(&simpleSort);
}

void MatrixEntityBase::filterListSimple(const MatrixItemBase& item)
{
	MatrixFilterSort simpleFilter(getMatrixEntityToken());
	
	simpleFilter.setFilter(item);	 
	applyFilterSort(&simpleFilter);
}

void MatrixEntityBase::applyFilterSort(MatrixFilterSort* filterSort)
{
	//this must never get called from a seperate thread as it is using a static for _filterSort
	ASSERT(_filterSort == NULL); 
	ASSERT(filterSort != NULL);

	if (filterSort != NULL)
	{           
		//ASSERT(filterSort->getUniqueID() == _filterID);
		//set the static so that the compare can see it.
		_filterSort = filterSort;

		//get an array of untyped entitys
		SaveableObjects initialArray;
		if (retrieveEntityArrayFromIDs(initialArray))
		{     
			ASSERT(initialArray.GetSize() == _ids.GetSize());

			//filter first so teh sort is quicker
			if (_filterSort->getLogicType() == lt_and)
			{
				for (int i=0;i<_filterSort->getFilterCount();i++)
					_filterSort->getFilter(i)->doAndFilter(initialArray);
			}
			else if(_filterSort->getLogicType() == lt_or)
			{
				CMap<SaveableObject*,SaveableObject*,int,int> wantedOriginalItems;

				for (int i=0;i<_filterSort->getFilterCount();i++)
					_filterSort->getFilter(i)->doOrFilter(initialArray, wantedOriginalItems);

				//the map contains all the entitys that are needed.
				for (int i=0;i<initialArray.GetSize();i++)
				{
					SaveableObject* originalEntity = initialArray.GetAt(i);
					int temp; 
					
					//if the item is NOT on the list, then remove from original list
					if (!wantedOriginalItems.Lookup(originalEntity, temp))
					{
						initialArray.RemoveAt(i--);
					}
				}	
			}

			//now do the sort
			//here we use the raw data in the MFC array.
			//this is OK as the sort operation does not add or remove items.
			qsort((void*)initialArray.GetData(), 
				(size_t)initialArray.GetSize(), 
				sizeof(int), 
				sortCompare);	
			
			//go back to the id's
			retrieveIDsFromEntityArray(initialArray);
		}

		//reset the static
		_filterSort = NULL;
	}
}

// static 
int MatrixEntityBase::sortCompare( const void * arg1, const void * arg2 )
{
	if (_filterSort)
	{	
		//get the saveableObjects
		SaveableObject* so1 = (*(SaveableObject**)arg1);
		SaveableObject* so2 = (*(SaveableObject**)arg2);

		ASSERT(so1);
		ASSERT(so2);

		for (int i=0;i<_filterSort->getSortCount();i++)
		{
			MatrixItemBase* item = _filterSort->getSort(i);

			int result;
			if (item->isSortAscending())
				result = item->doCompare( so1,  so2);
			else
				result = item->doCompare( so2,  so1);

			if (result != 0)
				return result;
		}	 
	}

	//if none of the compare's have produced a non zero, then for the 
	// purposes of this compare, the rows are identical
	return 0;
}



void MatrixEntityBase::init(void)
{
	//create the primary columns, and copy them to the all columns list
	createColumns(_primaryColumns);

	for (int i=0;i<_primaryColumns.GetSize();i++)
	{
		_allColumns.Add(_primaryColumns.GetAt(i));
	}


	//now create the relations, and add them to the all columns list
	createRelations(_relations);

	for (int i=0;i<_relations.GetSize();i++)
	{
		MatrixRelation* relation = _relations.GetAt(i);
		for (int j=0;j< relation->getColumnCount();j++)
		{
			_allColumns.Add(relation->getColumn(j));
		}
	}

	resetIdList(_ids);
	applyDefaultSort();
}

bool MatrixEntityBase::retrieveEntityArrayFromIDs(SaveableObjects& array)
{
	bool allOK = true;
	for (int i=0;i<_ids.GetSize();i++)
	{
		SaveableObject* entity;
		VERIFY(getEntityFromId(_ids.GetAt(i), entity));
		ASSERT(entity);
		if (!entity)
			allOK = false;
		array.Add(entity);
	}

	return allOK;
}

bool MatrixEntityBase::retrieveIDsFromEntityArray(SaveableObjects& entityArray)
{
	_ids.RemoveAll();
	
	for (int i=0;i<entityArray.GetSize();i++)
	{
		SaveableObject* entity = entityArray.GetAt(i);

		long id;
		VERIFY(getIdFromEntity(entity, id));
		_ids.Add(id);
	}	

	return true;
}



long MatrixEntityBase::getIDFromRow(int row)
{
	ASSERT((row >=0)&&(row < _ids.GetSize()));
	if ((row >=0)&&(row < _ids.GetSize()))
	{
		//get the entity id
		return _ids.GetAt(row);
	}

	return 0;//
}

SaveableObject* MatrixEntityBase::getEntityFromRow(int row)
{
	ASSERT((row >=0)&&(row < _ids.GetSize()));
	if ((row >=0)&&(row < _ids.GetSize()))
	{
		//get the entity id
		long id = _ids.GetAt(row);

		//get the entity
		SaveableObject* entity;
		VERIFY(getEntityFromId(id, entity));

		return entity;
	}
	return NULL;

}

int MatrixEntityBase::getRowFromID(long id)
{
	for (int i=0;i<_ids.GetSize();i++)
	{
		if (_ids.GetAt(i) == id)
			return i;
	}

	return -1;
}


void MatrixEntityBase::mouseOver(int row)
{
	if ((row >= 0)&&
		(row < getRowCount()))
	{
		SaveableObject* originalEntity = getEntityFromRow(row);
		ASSERT(originalEntity);

		if (originalEntity)
		{
			return mouseOver(originalEntity);
		}
	}  
}

void MatrixEntityBase::mouseOver(SaveableObject* /*originalEntity*/)
{
	//do nothing
}

void MatrixEntityBase::doubleClick(int row, CWnd* parent)
{
	if ((row >= 0)&&
		(row < getRowCount()))
	{
		SaveableObject* originalEntity = getEntityFromRow(row);
		ASSERT(originalEntity);

		if (originalEntity)
		{
			return doubleClick(originalEntity, parent);
		}
	}    

}

void MatrixEntityBase::doubleClick(SaveableObject* /*originalEntity*/, CWnd* /*parent*/)
{
	//do nothing
}

BOOL MatrixEntityBase::contextClick(const CArray<long, long>& selectedRows, int row, CWnd* parent)
{
	SaveableObjects originalEntitys;

	if ((row >= 0)&&
		(row < getRowCount()))
	{
		SaveableObject* selectedOriginalEntity = getEntityFromRow(row);
		ASSERT(selectedOriginalEntity);

		if (selectedOriginalEntity)
		{	 
			//assemble the list of selected entitys

			BOOL selectedRowInList = FALSE;
			for (int i=0;i<selectedRows.GetSize();i++)
			{
				long id = selectedRows.GetAt(i);

				if (id == row)
					selectedRowInList = TRUE;

				SaveableObject* originalEntity = getEntityFromRow(id);

				ASSERT(originalEntity);

				if(originalEntity)
					originalEntitys.Add(originalEntity);
			}

			//if the selected row is not in the  list then the list will contain just the item clicked on. 
			if (!selectedRowInList)
			{
				originalEntitys.RemoveAll();
				originalEntitys.Add(selectedOriginalEntity);
			}
			
			return contextClick(originalEntitys, selectedOriginalEntity, parent);
		}
	}

	return contextClick(originalEntitys, NULL, parent);

}

//override this if you do special right click processing.
BOOL MatrixEntityBase::contextClick(const SaveableObjects& contextSelectedObjects, SaveableObject* contextClickObject, CWnd* parent)
{
	//get the right click menu
	UINT menuID = getContextMenuID(contextSelectedObjects, contextClickObject);

	if (menuID > 0)
	{          
		CPoint point;
		::GetCursorPos(&point);

		CMenu menu;
		menu.LoadMenu(menuID);
		CMenu* pPopup = menu.GetSubMenu(0);

		ASSERT(pPopup);
		if (pPopup)				  
		{
			  
			pPopup->InsertMenu(0, MF_BYPOSITION | MF_SEPARATOR, NULL, "");

			CMenu logicSub;
			logicSub.LoadMenu(IDR_MATRIX_LOGIC);
			pPopup->InsertMenu(0,MF_BYPOSITION | MF_STRING |  MF_POPUP, 
				(UINT_PTR)logicSub.GetSubMenu(0)->GetSafeHmenu(), "Apply Logic");

			//add the filter menus
			CMenu userFilterSub;
			matrixManager()->setupFilterMenu(userFilterSub, getMatrixEntityToken(), mfst_user, 2, ID_USERFILTERDUMMY);
			if (userFilterSub.GetMenuItemCount() > 0)
				pPopup->InsertMenu(0,MF_BYPOSITION | MF_STRING |  MF_POPUP, 
					(UINT_PTR)userFilterSub.GetSafeHmenu(), "Apply User Filter");

			CMenu stockFilterSub;
			matrixManager()->setupFilterMenu(stockFilterSub, getMatrixEntityToken(), mfst_stock, 2, ID_STOCKFILTERDUMMY);
			if (stockFilterSub.GetMenuItemCount() > 0)
				pPopup->InsertMenu(0, MF_BYPOSITION | MF_STRING |  MF_POPUP, 
					(UINT_PTR)stockFilterSub.GetSafeHmenu(), "Apply Filter");


			//have do this so that the menu item is still in scope when createAndDisplayPopupMenu is called
			//this is because CMenu destroys the (windows) menu in the destructor. 
			CMenu* relatedMenus = new CMenu[_relations.GetSize()];
			bool addedSeperator = false;
		   //todo can we mode this into seperate function?
			//now run around all the relations
			for (int i=0;i<_relations.GetSize();i++)
			{
				MatrixRelation* relation = _relations.GetAt(i);
				UINT relatedID = relation->getContextMenuID(contextSelectedObjects, contextClickObject);

				if (relatedID > 0)
				{
					relatedMenus[i].LoadMenu(relatedID);
					if ((relatedMenus[i].GetSubMenu(0))&&(relatedMenus[i].GetSubMenu(0)->GetMenuItemCount() > 0))
					{
						//if we have not already done so, then add a seperator
						if (!addedSeperator)
						{
							pPopup->AppendMenu(MF_SEPARATOR,-1);
							addedSeperator = true;
						}
						//add the related menu.
						pPopup->AppendMenu( MF_POPUP | MF_STRING, 
											(UINT) (relatedMenus[i].GetSubMenu(0)->GetSafeHmenu()), 
											relation->getRelationName());
					}
				}
			}

	
			CBCGPPopupMenu* pPopupMenu = new CBCGPPopupMenu;
			pPopupMenu->Create(parent, point.x, point.y, pPopup->Detach (), FALSE, FALSE);		

			delete[] relatedMenus;

		}

		return TRUE;
	}       

	return FALSE;
}					   



UINT MatrixEntityBase::getContextMenuID(const SaveableObjects& originalEntitys, SaveableObject* selectedOriginalEntity)
{
	return 0;
}


bool MatrixEntityBase::canStartDrag()
{
	return false;
}

bool MatrixEntityBase::canAcceptDrop(DragDropTargetInfo* info)
{
	return false;		 
}

bool MatrixEntityBase::performDrop(DragDropTargetInfo* info, long iRow)
{
	return false;
}  

COLORREF MatrixEntityBase::getTextColor(int row, int column)
{
	if ((column >= 0)&&
		(column < getColumnCount())&&
		(row >= 0)&&
		(row < getRowCount()))
	{
				//get the matrix entity column.
		MatrixItemBase* pColumn = getColumn(column);

		if (pColumn)
		{  								
			SaveableObject* originalEntity = getEntityFromRow(row);
			ASSERT(originalEntity);

			SaveableObject* translatedEntity = pColumn->translateEntity(originalEntity);
			ASSERT(translatedEntity);

			if (translatedEntity)
			{
				return pColumn->getTextColor(translatedEntity);
			}
		}
	}    
	
	return UINT_MAX;
}

CString MatrixEntityBase::getText(int row, int column)
{
	if ((column >= 0)&&
		(column < getColumnCount())&&
		(row >= 0)&&
		(row < getRowCount()))
	{
				//get the matrix entity column.
		MatrixItemBase* pColumn = getColumn(column);

		if (pColumn)
		{  								
			SaveableObject* originalEntity = getEntityFromRow(row);
			ASSERT(originalEntity);

			SaveableObject* translatedEntity = pColumn->translateEntity(originalEntity);

			if (translatedEntity)
			{
				return pColumn->getText(translatedEntity);
			}
		}
	}    
	
	return "";
}

MatrixGridImages MatrixEntityBase::getImage(int row, int column)
{
	if ((column >= 0)&&
		(column < getColumnCount())&&
		(row >= 0)&&
		(row < getRowCount()))
	{
				//get the matrix entity column.
		MatrixItemBase* pColumn = getColumn(column);

		if (pColumn)
		{  								
			SaveableObject* originalEntity = getEntityFromRow(row);
			ASSERT(originalEntity);

			SaveableObject* translatedEntity = pColumn->translateEntity(originalEntity);

			if (translatedEntity)
			{
				return pColumn->getImage(translatedEntity);
			}
		}
	}    
	
	return mgi_none;
}


void MatrixEntityBase::getEntitysFromRows(SaveableObjects& selectedEntitys, const CArray<long, long>& selectedRows)
{
	for (int i=0;i<selectedRows.GetSize();i++)
	{
		//get the entity id for this row.
		SaveableObject* originalEntity = getEntityFromRow(selectedRows.GetAt(i));
			        
		ASSERT(originalEntity);

		selectedEntitys.Add(originalEntity);
	} 
}

void MatrixEntityBase::getIDsFromRows(CArray<long, long>& selectedIDs, const CArray<long, long>& selectedRows)
{
	for (int i=0;i<selectedRows.GetSize();i++)
	{
		//get the entity id for this row.
		long id = getIDFromRow(selectedRows.GetAt(i));
			        
		ASSERT(id != 0);

		selectedIDs.Add(id);
	} 
}

//NOTE: filterSort can be altered, and therefore should be copied first
void MatrixEntityBase::print(MatrixFilterSort& filterSort, CArray<int, int>* selectionRows)
{/*todo
    CDC dc;
    CPrintDialog printDlg(FALSE);
   
	if (selectionRows && selectionRows->GetSize() > 0)
	{
		// Enable the Selection button.
		printDlg.m_pd.Flags &= ~PD_NOSELECTION;

		if (selectionRows->GetSize() > 1)
			// Checked the Selection button.
			printDlg.m_pd.Flags |= PD_SELECTION;
	}

    if (printDlg.DoModal() == IDCANCEL)         // Get printer settings from user
        return;

    dc.Attach(printDlg.GetPrinterDC());         // Attach a printer DC
    dc.m_bPrinting = TRUE;

    CString strTitle;                           // Get the application title
    strTitle.LoadString(AFX_IDS_APP_TITLE);

    DOCINFO di;                                 // Initialise print document details
    ::ZeroMemory (&di, sizeof (DOCINFO));
    di.cbSize = sizeof (DOCINFO);
    di.lpszDocName = strTitle;

    BOOL bPrintingOK = dc.StartDoc(&di);        // Begin a new print job

    // Get the printing extents and store in the m_rectDraw field of a 
    // CPrintInfo object
    CPrintInfo Info;
    Info.m_rectDraw.SetRect(0,0, 
                            dc.GetDeviceCaps(HORZRES), 
                            dc.GetDeviceCaps(VERTRES));

	PrintData* printData = new PrintData;
	Info.m_lpUserData = printData;

	printData->_filterSort = &filterSort;

	//have we got a selection, if so, alter the filter to reflecty the GUI selected rows?
	CArray<long, long> ids;
	if (printDlg.PrintSelection() && selectionRows)
	{
		//convert rows to ids.
		for (int i=0;i<selectionRows->GetSize();i++)
		{
			long id = getIDFromRow(selectionRows->GetAt(i));
			ids.Add(id);
		}

		printData->_filterSort->setSpecificIds(ids);
		resetIdList(_ids);
		applyFilterSort(printData->_filterSort);//NOTE:We are not setting the _filterID. 
	}

	//do the printing
    for (UINT page = Info.GetMinPage(); 
         page <= Info.GetMaxPage() && bPrintingOK && Info.m_bContinuePrinting; 
         page++)
    {
        dc.StartPage();                         // begin new page
        Info.m_nCurPage = page;
        OnPrint(&dc, &Info);                    // Call your "Print page" function
        bPrintingOK = (dc.EndPage() > 0);       // end page
    }
	
	delete printData;
	
	Info.m_lpUserData = NULL;

    if (bPrintingOK)
        dc.EndDoc();                            // end a print job
    else
        dc.AbortDoc();                          // abort job.

    dc.Detach();                                // detach the printer DC

	//reapply the original data
	reapplyFilter();
	*///todo
}


void MatrixEntityBase::OnPrint(CDC* pDC, CPrintInfo* pInfo)
{/*
	PrintData* printData = (PrintData*)pInfo->m_lpUserData;	   

	CPage*	page= new CPage(pInfo->m_rectDraw,pDC,MM_TEXT);

	if ((printData)&&(printData->_filterSort))
	{
		//set some defaults
		COLORREF oldColor = page->SetColor(RGB(0,0,0));
		CString oldFont = page->SetFont("Arial");

		//work out some constants				
		const double maxWidth = page->getWidthinches();
		const double maxHeight = page->getLengthInches() - PrintUtils::getTitleHeight() - PrintUtils::getFootnoteHeight();
		const double linePitch = 1.0/6.0; //inches
		const int linesPerPage = (int)(maxHeight / linePitch);

		//work out the number of pages
		MatrixFilterSort* filterSort =  printData->_filterSort;
  		double columnEnd = 0.0;  

		if (!printData->_inited)
		{
			//horizontal pages
			printData->_numPagesHorz = 1;
			for (int i=0;i<filterSort->getColumnCount();i++)
			{
				MatrixItemBase* pColumn = filterSort->getColumn(i);
				ASSERT(pColumn);
				if (pColumn)
				{
					double width = ((double)pColumn->getColumnWidth())/96.0;
					//have we got enough room for this column
					if (columnEnd + width >= maxWidth)
					{
						printData->_numPagesHorz++;      
						columnEnd = width;
					}
					else
					{
						columnEnd += width;
					}
				}
			}             

			//vertical pages
			printData->_numPagesVert = (int)(getRowCount() / linesPerPage) + 1;	

			//we are done			
			printData->_inited = true;
		}

		double nextRow = 0.0;		
		const int curVertPage = printData->getVertPage(pInfo->m_nCurPage);
		const int curHorzPage = printData->getHorzPage(pInfo->m_nCurPage);
		const int startLine   = min((curVertPage -1) * linesPerPage, getRowCount());
		const int endLine     = min((curVertPage)    * linesPerPage, getRowCount());

		//do the printing
		if (curVertPage == 1)
		{
			PrintUtils::printTitle(page, ((curHorzPage == 1)? getFrameTitle():"") , nextRow, false);
		}
		nextRow += PrintUtils::getTitleHeight();
		
		nextRow = printMatrix(page, nextRow, filterSort, startLine, endLine, curHorzPage);
		
		CString pageText;
		if (printData->getPagesTotal() > 1)
			pageText.Format("Page %d of %d", pInfo->m_nCurPage, printData->getPagesTotal());
		PrintUtils::printFootnote(page, pageText, PrintUtils::getFootnoteHeight());

		//tidy up	
		page->SetFont(oldFont);
		page->SetColor(oldColor);
		delete page;

		//tell the parent routine if we have finished
		pInfo->m_bContinuePrinting =  (pInfo->m_nCurPage < (printData->_numPagesVert * printData->_numPagesHorz ));
		return;
	}

	ASSERT(FALSE);
	pInfo->m_bContinuePrinting =  false;*///todo
}


double MatrixEntityBase::printMatrix(CPage* page, double nextRow, MatrixFilterSort* filterSort, 
								   int startRow, int endRow, const unsigned int curHorzPage)
{     
/*todo	if (filterSort)
	{   
		TABLEHEADER* pTable =new TABLEHEADER;        
		pTable->PointSize=	8;
		pTable->LineSize=	1;
		pTable->UseInches=	TRUE;
		pTable->AutoSize=	FALSE;
			pTable->Border=		FALSE;
		pTable->FillFlag=	FILL_LTGRAY;
		pTable->NumColumns=	0;
		pTable->NumRows=	endRow - startRow;
		pTable->StartRow=	nextRow;
		pTable->StartCol=	0.0;
		pTable->EndCol=		0.1;
		pTable->HLines =	FALSE;
		pTable->VLines =	FALSE;
		pTable->NoHeader =	FALSE;

  		double columnEnd = 0.0;
		const double maxWidth = page->getWidthinches();
		unsigned int pageHorz = 1;
		int initialColumn = 0;

		//set up the columns
		for (int i=0;i<filterSort->getColumnCount();i++)
		{
			MatrixItemBase* pColumn = filterSort->getColumn(i);
			ASSERT(pColumn);
			if (pColumn)
			{
				double width = ((double)pColumn->getColumnWidth())/96.0;
				CString name = pColumn->getName();
				//have we got enough room for this column
				if (columnEnd + width >= maxWidth)
				{
					//have we finished
					if (pageHorz >= curHorzPage)
					{
						break;
					}

					//set up for the next page
					pageHorz++;      
					columnEnd = 0;
					initialColumn = i;
				}

				if (columnEnd + width < maxWidth)
				{
					columnEnd += width;
					if (pageHorz == curHorzPage)
					{
						pTable->ColDesc[pTable->NumColumns].Init(width, name, FILL_NONE);
						pTable->NumColumns++;						
						pTable->EndCol = columnEnd;
					}
				}
			}
		}

		page->Table(pTable);

		int hStart = page->ConvertToMappedUnits(.08, HORZRES);//bitmap cell offset
		const int vertSpacing = pTable->pClsTable->GetVerticalSpacing(FALSE);
		const int vertSpacing5Percent = (int)((double)vertSpacing * .05);
		const int vertSpacing90Percent = vertSpacing - (vertSpacing5Percent * 2);
		const int tableTop = page->ConvertToMappedUnits(nextRow, VERTRES);

										
		for (int i=0;i< pTable->NumColumns ;i++)
		{

			ASSERT(i+ initialColumn < filterSort->getColumnCount());

			MatrixItemBase* pColumn = filterSort->getColumn(i+ initialColumn);
                        
			if (pColumn)
			{  								
				for (int j=startRow;j<endRow;j++)
				{    
					SaveableObject* originalEntity = getEntityFromRow(j);
					ASSERT(originalEntity);

					SaveableObject* translatedEntity = pColumn->translateEntity(originalEntity);

					if (translatedEntity)
					{
						CArray<int,int> items;
						CImageList* imageList = pColumn->getImageList(translatedEntity, items);
						
						CString leadIn;

						if ((items.GetSize() > 0)&&(imageList))
						{
            				int vStart = tableTop + (vertSpacing * (j+1));
							leadIn = "         ";//make room for the icon.
						
							//createa compatible device context
							CDC dcMem; 
							dcMem.CreateCompatibleDC (page->m_PrtDesc.pDC); 

							//get the size of the image.
							IMAGEINFO info;
							imageList->GetImageInfo(items.GetAt(0), &info);
							CRect imageRect(info.rcImage);

							//create the bitmap
							CBitmap bitmap;
							bitmap.CreateCompatibleBitmap(&dcMem, imageRect.Width(), imageRect.Height());

							//select the bitmap. 
							dcMem.SelectObject (&bitmap);

							//erase first, the draw the icon.
							dcMem.FillSolidRect(0,0,16,16,RGB(255,255,255));
							imageList->Draw(&dcMem, items.GetAt(0), CPoint(0,0), ILD_NORMAL);

							//stretch the bitmap onto the printer dc.
							page->m_PrtDesc.pDC->StretchBlt (hStart, vStart + vertSpacing5Percent, vertSpacing90Percent, vertSpacing90Percent, 
															 &dcMem, 
															  0, 0, 16,16,
															  SRCCOPY);
						}   

						//draw the cells text
						CString fieldName = leadIn + pColumn->getText(translatedEntity);
						COLORREF oldColor = page->SetColor(pColumn->getTextColor(translatedEntity));
						page->Print(pTable,j - startRow,i, 8, TEXT_LEFT, 0.0, 0.0, fieldName.GetBuffer(fieldName.GetLength()));
						page->SetColor(oldColor);
						fieldName.ReleaseBuffer();

					}
				}
			}
			hStart += (int)pTable->ColDesc[i].Width;
		}

		nextRow = pTable->EndRow + .1;

		delete pTable;
		pTable = NULL;


		if (endRow - startRow <=0)
			nextRow = page->Print(nextRow, 0.5,TEXT_BOLD, 8, "There are no items to show in this entity"); 

	}
*///todo
	return nextRow;
                        
}

void MatrixEntityBase::showTooltips(const CPoint& cursorPos, int row)
{
	if ((row >= 0)&&
		(row < getRowCount()))
	{
		SaveableObject* originalEntity = getEntityFromRow(row);
		ASSERT(originalEntity);

		if (originalEntity)
		{
			return showTooltips(cursorPos, originalEntity);
		}
	}  
}

void MatrixEntityBase::CreateColumnHelper(MatrixItemBases& columns, MatrixItemToken matrixItemToken)
{
	MatrixItemBase* matrixItemBase = MatrixManagerColumnFactory::getInstance()->createColumnUsingFactory(matrixItemToken);
	ASSERT(matrixItemBase);
	if (matrixItemBase)
		columns.Add(matrixItemBase);
}

void MatrixEntityBase::addItemToFilter(long id, bool memory)
{
	CArray<long, long>& ids = (memory?_memoryIds:_ids);

	bool found = false;
	for (int i=0;i<ids.GetSize();i++)
	{
		if (ids.GetAt(i) == id)
		{
			found = true;
			break;
		}
	}
	if (!found)
		ids.InsertAt(0,id);
}

void MatrixEntityBase::removeItemFromFilter(long id, bool memory)
{
	CArray<long, long>& ids = (memory?_memoryIds:_ids);

	bool found = false;
	for (int i=0;i<ids.GetSize();i++)
	{
		if (ids.GetAt(i) == id)
		{
			ids.RemoveAt(i--);
		}
	}
}
void MatrixEntityBase::resetIdList()
{
	resetIdList(_ids);
}

int MatrixEntityBase::getIdCount() const
{
	return _ids.GetCount();
}

void MatrixEntityBase::invertSelection()
{
	//add the stored ids to a map.
	CMap<long, long, long, long> storedIdMap;
	for ( int i=0;i<_ids.GetSize();i++)
		storedIdMap.SetAt(_ids.GetAt(i), _ids.GetAt(i));

	//get all the ids.
	resetIdList(_ids);

	//remove stored ids from the map
	long dummy;
	for (int i=0;i<_ids.GetSize();i++)
	{

		if (storedIdMap.Lookup(_ids.GetAt(i), dummy))
			_ids.RemoveAt(i--);
	}
}

void MatrixEntityBase::memoryAssign()
{
	_memoryIds.RemoveAll();

	for (int i=0;i<_ids.GetSize();i++)
		_memoryIds.Add(_ids.GetAt(i));	
}

void MatrixEntityBase::mainAssign()
{
	_ids.RemoveAll();

	for (int i=0;i<_memoryIds.GetSize();i++)
		_ids.Add(_memoryIds.GetAt(i));	
}

void MatrixEntityBase::memorySwap()
{
	CArray<long,long> temp;

	for (int i=0;i<_memoryIds.GetSize();i++)
		temp.Add(_memoryIds.GetAt(i));	

	_memoryIds.RemoveAll();
	for (int i=0;i<_ids.GetSize();i++)
		_memoryIds.Add(_ids.GetAt(i));	

	_ids.RemoveAll();
	for (int i=0;i<temp.GetSize();i++)
		_ids.Add(temp.GetAt(i));	
}

void MatrixEntityBase::memoryOr()
{
	CMap<long, long, long, long> tempTowers;
	long dummy;

	//add all the display tothe map.
	for (int i=0;i<_ids.GetSize();i++)
		tempTowers.SetAt(_ids.GetAt(i), _ids.GetAt(i));

	//add any non duplicates to the display
	for (int i=0;i<_memoryIds.GetSize();i++)
		if (!tempTowers.Lookup(_memoryIds.GetAt(i), dummy))
			_ids.Add(_memoryIds.GetAt(i));

	//todo sort?
}

void MatrixEntityBase::memoryMinus()
{
	//todo test 

	//remove all memory from the display
	for (int i=0;i<_ids.GetSize();i++)
	{
		for (int j=0;j<_memoryIds.GetSize();j++)
		{
			if (_ids.GetAt(i) == _memoryIds.GetAt(j))
			{
				_ids.RemoveAt(i--);
				break;
			}
		}					
	}
	//todo sort?
}

void MatrixEntityBase::memoryAnd()
{
	//todo test 

	CArray<long, long> tempTowers;
	long dummy;

	//add all common towers to the temp tower
	for (int i=0;i<_ids.GetSize();i++)
	{
		for (int j=0;j<_memoryIds.GetSize();j++)
		{
			if (_ids.GetAt(i) == _memoryIds.GetAt(j))
			{
				tempTowers.Add(_ids.GetAt(i));
				break;
			}
		}					
	}

	//re-add to the filter
	_ids.RemoveAll();				 
	for (int i=0;i<tempTowers.GetSize();i++)
		_ids.Add(tempTowers.GetAt(i));

	//todo sort?
}

void MatrixEntityBase::getStatusBarText(CString& pane1, CString& pane2, CString& pane3)
{
	getStatusBarText(pane1);

	if (_memoryIds.GetSize() == getUnfilteredIdCount())
		pane2.Format("All %ss", getName()); 
	else
		pane2.Format("%d/%d %ss", _memoryIds.GetSize(), getUnfilteredIdCount(), getName());

	if (getIdCount() == getUnfilteredIdCount())
		pane3.Format("All %ss", getName()); 
	else
		pane3.Format("%d/%d %ss", getIdCount(), getUnfilteredIdCount(), getName());

}

