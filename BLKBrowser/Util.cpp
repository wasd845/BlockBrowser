#include "pch.h"

#include "Util.h"


AcDbBlockTableRecord* Util::OpenSpaceBlock(AcDb::OpenMode eMode, AcDbObjectId idBlockTableRecord)
{
	AcDbBlockTableRecord* pBlkRec;

	if (idBlockTableRecord.isNull() || !eOkVerify(acdbOpenObject(pBlkRec, idBlockTableRecord, eMode)))
		return NULL;
	else
		return pBlkRec;
}


Acad::ErrorStatus Util::AddToSpace(AcDbEntity* pNewEnt, AcDbObjectId idBlockTableRecord)
{
	AcDbBlockTableRecord* pBlkRec = OpenSpaceBlock(AcDb::kForWrite, idBlockTableRecord);

	if (pBlkRec == NULL)
		return Acad::eInvalidInput;

	Acad::ErrorStatus es = pBlkRec->appendAcDbEntity(pNewEnt);
	pBlkRec->close();

	return es;
}


Acad::ErrorStatus Util::AddToSpaceAndClose(AcDbEntity* pNewEnt, AcDbObjectId idBlockTableRecord)
{
	Acad::ErrorStatus es = AddToSpace(pNewEnt, idBlockTableRecord);
	if (es == Acad::eOk)
		pNewEnt->close();
	else
		delete pNewEnt;

	return es;
}


//----------------------------------------------------------------
// 描述:
// 		创建一个块.
// 参数:
// 		strBlockName 	- 要创建的图块名字
// 返回值:
// 		创建的图块的ID
//---------------------------------------------------------------
AcDbObjectId Util::CreateBlockTableRecord(const TCHAR* strBlockName)
{
	AcDbObjectId idBlockTableRecord;  //创建的图块的ID

	//创建一个块表记录
	AcDbObjectPointer<AcDbBlockTableRecord> pSqBlockTableRecord;
	if (!eOkVerify(pSqBlockTableRecord.create()))
		return AcDbObjectId::kNull;

	pSqBlockTableRecord->setName(strBlockName);

	//把块表记录加到块表里
	AcDbObjectPointer<AcDbBlockTable> pSqBlockTable(acdbCurDwg()->blockTableId(), AcDb::kForWrite);
	if (!eOkVerify(pSqBlockTable.openStatus()) || !eOkVerify(pSqBlockTable->add(idBlockTableRecord, pSqBlockTableRecord.object())))
		return AcDbObjectId::kNull;


	return idBlockTableRecord;
}


