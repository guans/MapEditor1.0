#pragma once
#include "afxcolorbutton.h"


// CRegColor �Ի���

class CRegColor : public CDialogEx
{
	DECLARE_DYNAMIC(CRegColor)

public:
	CRegColor(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CRegColor();

// �Ի�������
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	COLORREF m_reg_color;
	CMFCColorButton m_reg_coloc_ctrl;
};
