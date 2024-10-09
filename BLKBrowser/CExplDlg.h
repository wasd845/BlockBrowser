#pragma once

#include "CExplTree.h"

void CreateDlg();


// CExplDlg 对话框

class CExplDlg : public CAcUiDialog
{
	DECLARE_DYNAMIC(CExplDlg)

public:
	CExplDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CExplDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CExplTree m_tree;
	virtual BOOL OnInitDialog();
	//创建块表记录
	afx_msg void OnBnClickedButton1();
	//创建直线对象
	afx_msg void OnBnClickedButton2();
	//删除对象
	afx_msg void OnBnClickedButton3();
	//重命名
	afx_msg void OnBnClickedButton4();
};
