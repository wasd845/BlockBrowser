#pragma once


// CExplTree

class CExplTree : public CTreeCtrl
{
	DECLARE_DYNAMIC(CExplTree)

public:
	CExplTree();
	virtual ~CExplTree();

	//��ʾ���ݣ�ˢ������
	void DisplayData();
	void RefreshData();
	void ExpandAll(HTREEITEM hItem);

	//���������¼������ֱ�ߣ�ɾ����������
	void CreateBlockTableRecord();
	void CreateAcDbLine();
	void DeleteSelectedObject();
	void RenameBlockTableRecordByDlg();
	void RenameBlockTableRecordByEditLabel();

	//��������
	void RenameBlockTableRecord(const TCHAR* szName);

	void InsertTextStyleTable(HTREEITEM& itemRoot);

protected:
	DECLARE_MESSAGE_MAP()

	CImageList m_imageList;
public:

	//�Ҽ��˵�����
	afx_msg void OnNMRClick(NMHDR *pNMHDR, LRESULT *pResult);
	//��λ�༭����
	afx_msg void OnTvnEndlabeledit(NMHDR *pNMHDR, LRESULT *pResult);
private:
	//��ݼ�
	HACCEL m_hAccel;
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};


