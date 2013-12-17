// CreatDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MapEditor1.0.h"
#include "CreatDlg.h"
#include "afxdialogex.h"


// CreatDlg 对话框

IMPLEMENT_DYNAMIC(CreatDlg, CDialogEx)

CreatDlg::CreatDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CreatDlg::IDD, pParent)
	, m_new_pnt(FALSE)
	, m_new_lin(FALSE)
	, m_new_reg(FALSE)
{
	m_new_pnt=TRUE;
	m_new_lin=TRUE;
	m_new_reg=TRUE;
}

CreatDlg::~CreatDlg()
{
}

void CreatDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_NEW_PNT, m_new_pnt);
	DDX_Check(pDX, IDC_NEW_LIN, m_new_lin);
	DDX_Check(pDX, IDC_NEW_REG, m_new_reg);
}


BEGIN_MESSAGE_MAP(CreatDlg, CDialogEx)
	ON_BN_CLICKED(IDC_NEW_PNT, &CreatDlg::OnBnClickedNewPnt)
END_MESSAGE_MAP()


// CreatDlg 消息处理程序


void CreatDlg::OnBnClickedNewPnt()
{
	// TODO: 在此添加控件通知处理程序代码
	
}
