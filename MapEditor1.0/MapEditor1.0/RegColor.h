#pragma once
#include "afxcolorbutton.h"


// CRegColor 对话框

class CRegColor : public CDialogEx
{
	DECLARE_DYNAMIC(CRegColor)

public:
	CRegColor(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRegColor();

// 对话框数据
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	COLORREF m_reg_color;
	CMFCColorButton m_reg_coloc_ctrl;
};
