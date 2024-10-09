// CExplTree.cpp: 实现文件
//

#include "pch.h"
#include "CExplTree.h"

#include "Util.h"
#include "CInputDlg.h"


// CExplTree

IMPLEMENT_DYNAMIC(CExplTree, CTreeCtrl)

CExplTree::CExplTree()
{
	m_hAccel = ::LoadAccelerators(PushResource::s_hInstanceDefault, MAKEINTRESOURCE(IDR_ACCEL));
}

CExplTree::~CExplTree()
{
}


BEGIN_MESSAGE_MAP(CExplTree, CTreeCtrl)
	ON_NOTIFY_REFLECT(NM_RCLICK, &CExplTree::OnNMRClick)
	ON_COMMAND(IDM_CREATE, &CExplTree::CreateBlockTableRecord)
	ON_COMMAND(IDM_DELETE, &CExplTree::DeleteSelectedObject)
	ON_COMMAND(IDM_ADD, &CExplTree::CreateAcDbLine)
	ON_COMMAND(IDM_RENAME, &CExplTree::RenameBlockTableRecordByEditLabel)
	ON_NOTIFY_REFLECT(TVN_ENDLABELEDIT, &CExplTree::OnTvnEndlabeledit)
END_MESSAGE_MAP()



// CExplTree 消息处理程序

// 在树控件中显示元素
void CExplTree::DisplayData()
{
	// TODO: 在此处添加实现代码.
	//创建图标 
	m_imageList.Create(IDB_BITMAP1, 24, 4, RGB(0, 0, 0));
	SetImageList(&m_imageList, TVSIL_NORMAL);

	//根节点 - AcDbDatabase
	HTREEITEM itemRoot = InsertItem(_T("AcDbDatabase"), 0, 0, TVI_ROOT);

	//二级节点 - AcDbBlockTable
	HTREEITEM itemBlockTable = InsertItem(_T("AcDbBlockTable"), 1, 1, itemRoot);
	SetItemData(itemBlockTable, acdbCurDwg()->blockTableId().asOldId());

	AcDbObjectPointer<AcDbBlockTable> pSqBlockTable(acdbCurDwg()->blockTableId());
	if (!eOkVerify(pSqBlockTable.openStatus()))
		return;

	AcDbBlockTableIterator *pBTItr;
	if (eOkVerify(pSqBlockTable->newIterator(pBTItr)))
	{
		while (!pBTItr->done()) 
		{
			AcDbObjectId idBlockTableRecord;
			AcDbObjectPointer<AcDbBlockTableRecord> pSqBlockTableRecord;
			if (!eOkVerify(pBTItr->getRecordId(idBlockTableRecord)) || !eOkVerify(pSqBlockTableRecord.open(idBlockTableRecord)) )
				return;

			const TCHAR *pName;
			pSqBlockTableRecord->getName(pName);

			//三级节点 - AcDbBlockTableRecord
			HTREEITEM itemBlockTableRecord = InsertItem(pName, 2, 2, itemBlockTable);
			SetItemData(itemBlockTableRecord, pSqBlockTableRecord->objectId().asOldId());

			AcDbBlockTableRecordIterator* pBTRIter;
			if (eOkVerify(pSqBlockTableRecord->newIterator(pBTRIter)))
			{
				for (pBTRIter->start(); !pBTRIter->done(); pBTRIter->step()) 
				{
					AcDbObjectId idEntity;
					AcDbObjectPointer<AcDbEntity> pSqEntity;
					if (!eOkVerify(pBTRIter->getEntityId(idEntity)) || !eOkVerify(pSqEntity.open(idEntity)))
						return;

					AcDbHandle hEntity = idEntity.handle();
					TCHAR strHandle[20];
					hEntity.getIntoAsciiBuffer(strHandle);
					const TCHAR *pCname = pSqEntity->isA()->name();
					CString str;
					str.Format(_T("%s - %s - %lx"), strHandle, pCname, pSqEntity->objectId().asOldId());

					//四级节点 - AcDbEntity
					HTREEITEM itemEntity = InsertItem(str, 3, 3, itemBlockTableRecord);
					SetItemData(itemEntity, pSqEntity->objectId().asOldId());
				}
				delete pBTRIter;
			}

			pBTItr->step();
		}
		delete pBTItr;
	}

	InsertTextStyleTable(itemRoot);

	//二级节点 - AcDbDictionary
	HTREEITEM itemNamedObjectsDictionary = InsertItem(_T("NamedObjectsDictionary"), 1, 1, itemRoot);
	SetItemData(itemNamedObjectsDictionary, acdbCurDwg()->namedObjectsDictionaryId().asOldId());

	AcDbObjectPointer<AcDbDictionary> pSqDictionary(acdbCurDwg()->namedObjectsDictionaryId());
	if (!eOkVerify(pSqDictionary.openStatus()))
		return;

	Adesk::UInt32 nNum = pSqDictionary->numEntries();
	AcDbDictionaryIterator* pDictItr = pSqDictionary->newIterator();
	if (pDictItr != NULL)
	{
		while (!pDictItr->done())
		{
			const TCHAR *pName = pDictItr->name();

			//三级节点 - AcDbObject
			HTREEITEM itemAcDbObject = InsertItem(pName, 2, 2, itemNamedObjectsDictionary);
			SetItemData(itemAcDbObject, pDictItr->objectId().asOldId());

			pDictItr->next();
		}
		delete pDictItr;
	}
}

void CExplTree::RefreshData()
{
	DeleteAllItems();
	DisplayData();
	ExpandAll(GetRootItem());
	SelectItem(GetRootItem());
}


void CExplTree::ExpandAll(HTREEITEM hItem)
{
	if (!ItemHasChildren(hItem))
		return;

	//若树控件的根节点有子节点则获取根节点的子节点
	HTREEITEM hNextItem = GetChildItem(hItem);

	while (hNextItem)
	{
		//递归，展开子节点下的所有子节点
		ExpandAll(hNextItem);
		hNextItem = GetNextItem(hNextItem, TVGN_NEXT);
	}
	Expand(hItem, TVE_EXPAND);
}


void CExplTree::CreateBlockTableRecord()
{
	CInputDlg dlg;
	if (dlg.DoModal() != IDOK)
		return;

	Util::CreateBlockTableRecord(dlg.m_strName);

	RefreshData();
}

void CExplTree::CreateAcDbLine()
{
	AcDbObjectId idSelected = AcDbObjectId().setFromOldId(GetItemData(GetSelectedItem()));
	if (idSelected.objectClass()->isDerivedFrom(AcDbBlockTableRecord::desc()))
	{
		AcDbLine* pLine = new AcDbLine(AcGePoint3d(rand() % 4000 - 2000, rand() % 4000 - 2000, 0.), AcGePoint3d(rand() % 4000 - 2000, rand() % 4000 - 2000, 0.));
		Util::AddToSpaceAndClose(pLine, idSelected);
	}

	RefreshData();
}

void CExplTree::DeleteSelectedObject()
{
	AcDbObjectId idSelected = AcDbObjectId().setFromOldId(GetItemData(GetSelectedItem()));
	if (idSelected.objectClass()->isDerivedFrom(AcDbBlockTableRecord::desc()))
	{
		AcDbObjectPointer<AcDbBlockTableRecord> pSqBlockTableRecord(idSelected, AcDb::kForWrite);
		if (!eOkVerify(pSqBlockTableRecord.openStatus()) || pSqBlockTableRecord->isLayout())
			return;

		AcDbBlockTableRecordIterator* pIter;
		if (eOkVerify(pSqBlockTableRecord->newIterator(pIter)))
		{
			for (pIter->start(); !pIter->done(); pIter->step())
			{
				AcDbObjectId idEntity;
				AcDbObjectPointer<AcDbEntity> pSqEntity;
				if (!eOkVerify(pIter->getEntityId(idEntity)) || !eOkVerify(pSqEntity.open(idEntity, AcDb::kForWrite)))
					return;

				//删除实体
				pSqEntity->erase();
			}
			delete pIter;
			
		}

		//删除块表记录
		pSqBlockTableRecord->erase();
	}
	else if (idSelected.objectClass()->isDerivedFrom(AcDbEntity::desc()))
	{
		AcDbObjectPointer<AcDbEntity> pSqEntity(idSelected, AcDb::kForWrite);
		if (!eOkVerify(pSqEntity.openStatus()))
			return;

		//删除实体
		pSqEntity->erase();
	}
	else if (idSelected.objectClass()->isDerivedFrom(AcDbDictionary::desc()))
	{
		AcDbObjectPointer<AcDbDictionary> pSqDict(idSelected, AcDb::kForWrite);
		if (!eOkVerify(pSqDict.openStatus()))
			return;

		//删除字典
		pSqDict->erase();
	}
	else //删除字典里的对象
	{
		AcDbObjectPointer<AcDbDictionary> pSqDictionary(acdbCurDwg()->namedObjectsDictionaryId(), AcDb::kForWrite);
		if (!eOkVerify(pSqDictionary.openStatus()))
			return;

		Adesk::UInt32 nNum = pSqDictionary->numEntries();
		AcDbDictionaryIterator* pDictItr = pSqDictionary->newIterator();
		if (pDictItr != NULL)
		{
			while (!pDictItr->done())
			{
				const TCHAR *pName = pDictItr->name();
				if (pDictItr->objectId().asOldId() == GetItemData(GetSelectedItem()))
					pSqDictionary->remove(pDictItr->objectId());

				pDictItr->next();
			}
			delete pDictItr;
		}
	}

	RefreshData();
}

void CExplTree::RenameBlockTableRecordByDlg()
{
	CInputDlg dlg;
	if (dlg.DoModal() != IDOK)
		return;

	RenameBlockTableRecord(dlg.m_strName);

	RefreshData();
}

void CExplTree::RenameBlockTableRecordByEditLabel()
{
	EditLabel(GetSelectedItem());
}

void CExplTree::RenameBlockTableRecord(const TCHAR* szName)
{
	AcDbObjectId idSelected = AcDbObjectId().setFromOldId(GetItemData(GetSelectedItem()));
	if (idSelected.objectClass()->isDerivedFrom(AcDbBlockTableRecord::desc()))
	{
		AcDbObjectPointer<AcDbBlockTableRecord> pSqBlockTableRecord(idSelected, AcDb::kForWrite);
		if (!eOkVerify(pSqBlockTableRecord.openStatus()))
			return;

		pSqBlockTableRecord->setName(szName);
	}
}

void CExplTree::InsertTextStyleTable(HTREEITEM& itemRoot)
{
	//二级节点 - AcDbTextStyleTable
	HTREEITEM itemTextStyleTable = InsertItem(_T("AcDbTextStyleTable"), 1, 1, itemRoot);
	SetItemData(itemTextStyleTable, acdbCurDwg()->textStyleTableId().asOldId());

	AcDbObjectPointer<AcDbTextStyleTable> pSqTextStyle(acdbCurDwg()->textStyleTableId());
	if (!eOkVerify(pSqTextStyle.openStatus()))
		return;

	AcDbTextStyleTableIterator *pBTItr;
	if (eOkVerify(pSqTextStyle->newIterator(pBTItr)))
	{
		while (!pBTItr->done())
		{
			AcDbObjectId idTextStyleTableRecord;
			AcDbObjectPointer<AcDbTextStyleTableRecord> pSqTextStyleTableRecord;
			if (!eOkVerify(pBTItr->getRecordId(idTextStyleTableRecord)) || !eOkVerify(pSqTextStyleTableRecord.open(idTextStyleTableRecord)))
				return;

			const TCHAR *pName;
			pSqTextStyleTableRecord->getName(pName);

			//三级节点 - AcDbTextStyleTableRecord
			HTREEITEM itemBlockTableRecord = InsertItem(pName, 2, 2, itemTextStyleTable);
			SetItemData(itemBlockTableRecord, pSqTextStyleTableRecord->objectId().asOldId());

			pBTItr->step();
		}
		delete pBTItr;
	}
}

//右键菜单
void CExplTree::OnNMRClick(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	//获取点击点
	CPoint point;
	GetCursorPos(&point);
	CPoint pointInTree = point;
	ScreenToClient(&pointInTree);

	//获取点击节点
	HTREEITEM item;
	UINT flag = TVHT_ONITEM;
	item = HitTest(pointInTree, &flag);
	if (item == NULL)
		return;

	SelectItem(item);

	//获取节点层级
	int nLevel = 0;
	while (item)
	{
		item = GetParentItem(item);
		nLevel++;
	}

	//创建右键菜单，绑定消息处理
	CMenu menu;
	menu.CreatePopupMenu();
	switch (nLevel)
	{
		case 1 :
			menu.AppendMenu(MF_STRING, IDM_NULL, _T("AcDbDataBase"));
			break;
		case 2:
			menu.AppendMenu(MF_STRING, IDM_CREATE, _T("新建 AcDbDataBaseTableRecord"));
			break;
		case 3:
			menu.AppendMenu(MF_STRING, IDM_ADD, _T("新建 AcDbEntity (AcDbLine)"));
			menu.AppendMenu(MF_STRING, IDM_RENAME, _T("重命名"));
			menu.AppendMenu(MF_STRING, IDM_DELETE, _T("删除"));
			break;
		case 4:
			menu.AppendMenu(MF_STRING, IDM_DELETE, _T("删除"));
			break;
		default :
			break;
	}
	menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this, NULL);
	menu.DestroyMenu();

	*pResult = 0;
}



void CExplTree::OnTvnEndlabeledit(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTVDISPINFO pTVDispInfo = reinterpret_cast<LPNMTVDISPINFO>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	RenameBlockTableRecord(CString(pTVDispInfo->item.pszText));

	*pResult = TRUE;
}


BOOL CExplTree::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (m_hAccel != NULL)
		if (::TranslateAccelerator(m_hWnd, m_hAccel, pMsg))
			return TRUE;

	return CTreeCtrl::PreTranslateMessage(pMsg);
}
