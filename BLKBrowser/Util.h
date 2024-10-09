#pragma once

#include "pch.h"

class Util {
public :

	static AcDbBlockTableRecord*    OpenSpaceBlock(AcDb::OpenMode eMode, AcDbObjectId idBlockTableRecord);
	static Acad::ErrorStatus		AddToSpace(AcDbEntity* pNewEnt, AcDbObjectId idBlockTableRecord);
	static Acad::ErrorStatus		AddToSpaceAndClose(AcDbEntity* pNewEnt, AcDbObjectId idBlockTableRecord);

	static AcDbObjectId				CreateBlockTableRecord(const TCHAR* strBlockName);
};