// ARX_ex.cpp: 定义 DLL 的初始化例程。
//

#include "pch.h"
#include "framework.h"

#include "CExplDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


extern "C" AcRx::AppRetCode acrxEntryPoint(AcRx::AppMsgCode msg, void* appId)
{
	switch (msg)
	{
	case AcRx::kInitAppMsg:
		acrxUnlockApplication(appId);//加载
		acrxBuildClassHierarchy();
		//注册命令
		acedRegCmds->addCommand(_T("BLKBrowser"), _T("BLKBrowser"), _T("BLKBrowser"), ACRX_CMD_MODAL, CreateDlg);

		break;

	case AcRx::kUnloadAppMsg://卸载
		//注销命令
		acedRegCmds->removeGroup(_T("BLKBrowser"));
		break;

	default:
		break;
	}

	return AcRx::kRetOK;
}