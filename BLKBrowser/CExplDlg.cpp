// CExplDlg.cpp: 实现文件
//

#include "pch.h"
#include "CExplDlg.h"
#include "afxdialogex.h"


void CreateDlg()
{
	PushResource tempRes;//资源切换

	CExplDlg().DoModal();
}


// CExplDlg 对话框

IMPLEMENT_DYNAMIC(CExplDlg, CAcUiDialog)

CExplDlg::CExplDlg(CWnd* pParent /*=nullptr*/)
	: CAcUiDialog(IDD_DIALOG1, pParent)
{
}

CExplDlg::~CExplDlg()
{
}

void CExplDlg::DoDataExchange(CDataExchange* pDX)
{
	CAcUiDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, m_tree);
}


BEGIN_MESSAGE_MAP(CExplDlg, CAcUiDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &CExplDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CExplDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CExplDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CExplDlg::OnBnClickedButton4)
END_MESSAGE_MAP()


// CExplDlg 消息处理程序


BOOL CExplDlg::OnInitDialog()
{
	CAcUiDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_tree.RefreshData();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

//创建块表记录
void CExplDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	m_tree.CreateBlockTableRecord();
}

//创建直线对象
void CExplDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	m_tree.CreateAcDbLine();
}

//删除对象
void CExplDlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	m_tree.DeleteSelectedObject();
}

//重命名
void CExplDlg::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
	m_tree.RenameBlockTableRecordByDlg();
}


