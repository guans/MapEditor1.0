// RegColor.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MapEditor1.0.h"
#include "RegColor.h"
#include "afxdialogex.h"


// CRegColor �Ի���

IMPLEMENT_DYNAMIC(CRegColor, CDialogEx)

CRegColor::CRegColor(CWnd* pParent /*=NULL*/)
	: CDialogEx(CRegColor::IDD, pParent)
{

}

CRegColor::~CRegColor()
{
}

void CRegColor::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_MFCCOLORBUTTON1, m_reg_color);
	DDX_Control(pDX, IDC_MFCCOLORBUTTON1, m_reg_coloc_ctrl);
}


BEGIN_MESSAGE_MAP(CRegColor, CDialogEx)
END_MESSAGE_MAP()


// CRegColor ��Ϣ�������
