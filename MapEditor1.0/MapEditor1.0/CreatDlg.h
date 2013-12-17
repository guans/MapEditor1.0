#pragma once
#include "resource.h"

// CreatDlg 对话框

class CreatDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CreatDlg)

public:
	CreatDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CreatDlg();

// 对话框数据
	enum { IDD = IDD_CREATDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	BOOL m_new_pnt;
	BOOL m_new_lin;
	BOOL m_new_reg;
	afx_msg void OnBnClickedNewPnt();
};
