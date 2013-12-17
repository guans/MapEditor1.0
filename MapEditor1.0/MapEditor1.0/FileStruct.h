#pragma once
#ifndef _FILESTRUCT_H
#define _FILESTRUCT_H 

#include "stdafx.h"
#include <afx.h>
#include "CreatDlg.h"
#include "RegColor.h"
extern int GCurOperState;
extern int GisShowDeleted;							//是否显示删除后的图形
extern int GCurLINState;
//1.点文件{
extern CFile GPntTmpFile,GPntFile;						//点临时文件、点永久文件
extern int GPntNum,GPntLNum;							//物理点数、逻辑点数
extern int GPntFCreated;								//点文件是否创建
extern int GPntChanged;									//点文件是否改变
extern char GPntFName[256];								//点文件名	
extern int GPntTmpFOpened;								//点临时文件是否被打开
//}
//2.线文件{
extern CFile GLinTmpFile1,GLinTmpFile2,GLinFile;		//线临时文件、线永久文件
extern int GLinNum,GLinLNum;							//物理线数、逻辑线数
extern int GLinFCreated;								//线文件是否创建
extern int GLinChanged;									//线文件是否改变
extern char GLinFName[256];								//线文件名	
extern int GLinTmpFOpened;								//线临时文件是否被打开
//}
//3.区文件{
extern CFile GRegTmpFile1,GRegTmpFile2,GRegFile;		//区临时文件、区永久文件
extern int GRegNum,GRegLNum;							//物理区数、逻辑区数
extern int GRegFCreated;								//区文件是否创建
extern int GRegChanged;									//区文件是否改变
extern char GRegFName[256];								//区文件名	
extern int GRegTmpFOpened;								//区临时文件是否被打开
extern int GIsMakingReg;								//是否正在造区
extern COLORREF GregDefultColor;	
//}
//4.{
extern double GZoom;									//比例系数
extern double GxOffset;								//保存x偏移与y偏移
extern double GyOffset;								
//}

//定义宏
#define NEW_PNT 0x0001;
#define NEW_LIN 0x0002;
#define NEW_REG 0x0004;
//点操作
#define OPERSTATE_INPUT_PNT		1      //造点
#define OPERSTATE_DEL_PNT		2	   //删除点
#define OPERSTATE_MOV_PNT	    3      //移动点
#define OPERSTATE_SHOW_DEL_PNT	4	   //显示删除点
#define OPERSTATE_RECOVER_PNT   5      //恢复删除点
#define OPERSTATE_EDIT_PNT      6      //编辑点参数
#define OPERSTATE_SET_PNT       7      //设置点缺省参数
//视图坐标系操作
#define OPERSTATE_ZOOM_IN		8      //放大
#define OPERSTATE_ZOOM_OUT		9      //缩小
//线操作
#define OPERSTATE_INPUT_LIN			10       //造线
#define OPERSTATE_MOVE_LIN			11      //移动线
#define OPERSTATE_DEL_LIN			12      //删除线
#define OPERSTATE_SHOW_LIN			13      //显示删除线
#define OPERSTATE_RECOVER_LIN		14      //恢复线
#define OPERSTATE_ADD_PNT			15      //线上加点
#define OPERSTATE_DEL_PNT			16      //线上删点
#define OPERSTATE_CONNECT_LIN		17      //连接线
#define OPERSTATE_EDIT_LIN			18      //修改线属性
#define OPERSTATE_SET_LIN			19      //设置线缺省参数
//区操作
#define OPERSTATE_CREAT_REG		20      //造区
#define OPERSTATE_DELETE_REG    21	   //删除区
#define OPERSTATE_MOVE_REG      22	   //移动区

#define OPERSTATE_WINDOW_MOVE   23	   //窗口移动
#define OPERSTATE_RECOVER_REG	24	   //恢复区
#define OPERSTATE_AMMENDPAR_REG	25	   //修改区参数






struct pnt_stru
{
	char			isDel;		//是否被删除？
	COLORREF		color;		//点颜色
	int				pattern;	//点图案（号）
	double			x;			//点位坐标
	double			y;  
}PNT_STRU;

struct pnt_version
{
		char 	flag[3];	//标志符‘PNT’
		int		version;	//10
}PNT_VERSION;

struct lin_ndx_stru
{
		char				isDel;		//是否被删除？
		COLORREF			color;		//线颜色
		int				pattern;		//线型（号）
		long 				dotNum;		//线坐标点数
		long				datOff;		//线坐标数据存储位置  
}LIN_NDX_STRU;

struct lin_version
{
	char	flag[3];	//标识符‘LIN’
	int		version;	//10
}LIN_VERSION;

struct reg_ndx_stru
{
		char				isDel;		//是否被删除？
		COLORREF			color;		//区颜色
		long 				dotNum;		//区坐标点数
		long				datOff;		//区坐标数据存储位置  
}REG_NDX_STRU;

struct reg_version
{
	char	flag[3];	//标识符‘REG’
	int		version;	//10
}REG_VERSION;
struct d_dot
{
	double			x;			//点位坐标
	double			y;  
}D_DOT;

long GetNewType(int& mtype)
{
	CreatDlg mdlg;
	if(mdlg.DoModal()==IDOK)
	{
		int type=0x0000;;
		if(mdlg.m_new_pnt==TRUE)
			type=type|NEW_PNT;
		if(mdlg.m_new_lin==TRUE)
			type=type|NEW_LIN;
		if(mdlg.m_new_reg==TRUE)
			type=type|NEW_REG;
		mtype=type;
	}
	return (1);
}
long NewPnt()
{
	if(GPntFCreated==1)
	{
		AfxMessageBox(_T("点文件已创建！"));
		return (0);
	}
	else

		GPntFCreated = 1;
		GPntTmpFile.Open (_T("E:\\GPntTmpFile.pnt"),CFile:: modeCreate|CFile:: modeReadWrite);
		GPntTmpFOpened = 1;
	return (1);
}
long NewLin()
{
	if(GLinFCreated==1)
	{
		AfxMessageBox(_T("线文件已创建！"));
		return (0);
	}
	else

		GLinFCreated = 1;
		GLinTmpFile1.Open (_T("E:\\GLinTmpFile1.lin"),CFile:: modeCreate|CFile:: modeReadWrite);
		GLinTmpFile2.Open (_T("E:\\GLinTmpFile2.lin"),CFile:: modeCreate|CFile:: modeReadWrite);
		GLinTmpFOpened = 1;
	return (1);
}
long NewReg()
{
	if(GRegFCreated==1)
	{
		AfxMessageBox(_T("区文件已创建！"));
		return (0);
	}
	else

		GRegFCreated = 1;
		GRegTmpFile1.Open (_T("E:\\GRegTmpFile1.regn"),CFile:: modeCreate|CFile::modeReadWrite);
		GRegTmpFile2.Open (_T("E:\\GRegTmpFile2.regn"),CFile:: modeCreate|CFile::modeReadWrite);
		GRegTmpFOpened = 1;
	return (1);
}
//将点永久文件转化成点临时文件的函数
long Pnt_FCT(CFile &PntFile,CString FilePathName)
{
	int Off=sizeof(PNT_VERSION)+sizeof(GPntNum)+sizeof(GPntLNum);
	pnt_stru pnt;
	if(!GPntTmpFOpened)						//如果点临时文件没有打开则新建
		NewPnt();
	GPntTmpFile.Seek(0,CFile::begin);		//将点临时文件指针定义到开始
	PntFile.Seek(sizeof(PNT_VERSION),CFile::begin);
	PntFile.Read(&GPntNum,sizeof(int));
	PntFile.Read(&GPntLNum,sizeof(int));
	PntFile.Seek(Off,CFile::begin);			//点永久文件指针跳过版本及物理数逻辑数
	for(int i=0;i<GPntNum;i++)
	{
		PntFile.Read(&pnt,sizeof(PNT_STRU));
		GPntTmpFile.Write(&pnt,sizeof(PNT_STRU));
	}
	
	return (1);
}
//将点临时文件转化成永久文件的函数
long Pnt_TCF(CString FilePathName,int version)
{
	char flag[3];
	pnt_stru pnt;
	if(!GPntTmpFOpened)
		return (0);
	GPntFile.Open(FilePathName,CFile:: modeCreate|CFile:: modeReadWrite);
	GPntFile.Seek(0,CFile::begin);
	GPntTmpFile.Seek(0,CFile::begin);
	flag[0] = 'P';
	flag[1] = 'N';
	flag[2] = 'T';
	GPntFile.Write(flag,4);
	GPntFile.Write (&version,sizeof(int));
	GPntFile.Write (&GPntNum,sizeof(int));
	GPntFile.Write (&GPntLNum,sizeof(int));
	for(int i=0;i<GPntNum;i++)
		{
			GPntTmpFile.Read(&pnt,sizeof(PNT_STRU));
			GPntFile.Write(&pnt,sizeof(PNT_STRU));

		}
	GPntFile.Close();
	GPntChanged=0;
	return (1);

}
//将线永久文件转化成临时文件的函数
long Lin_FCT(CFile &LinFile,CString FilePathName)
{
	int i;
	int ndxOff=sizeof(LIN_VERSION)+sizeof(GLinNum)+sizeof(GLinLNum);
	int tmpDatOff;
	lin_ndx_stru linNdx;
	int linDatSize;
	char *linDatBuf;
	if(!GLinTmpFOpened)
		NewLin();
	GLinTmpFile1.Seek(0,CFile::begin);		//将线临时文件1指针定义到开始
	tmpDatOff=0;
	LinFile.Seek(sizeof(LIN_VERSION),CFile::begin);
	LinFile.Read(&GLinNum,sizeof(int));
	LinFile.Read(&GLinLNum,sizeof(int));
	LinFile.Seek(ndxOff,CFile::begin);		//将线永久文件指针跳过版本及逻辑数物理数
	for(i=0;i<GLinNum;i++)					
		{
			GLinTmpFile2.Seek(tmpDatOff,CFile::begin);		//将线临时文件2指针定义到tmpdatoff
			LinFile.Read(&linNdx,sizeof(LIN_NDX_STRU));			//读第i条线的索引
			ndxOff+=sizeof(LIN_NDX_STRU);
			linDatSize=linNdx.dotNum*sizeof(D_DOT);				//计算当前线的坐标点所需内存字节数
			linDatBuf=new char [linDatSize];					//申请存放当前线坐标数据所需内存
			LinFile.Seek(linNdx.datOff,CFile::begin);			//定位到当前线坐标数据起始位置
			LinFile.Read(linDatBuf,linDatSize);					//读当前线的坐标数据
			GLinTmpFile2.Write(linDatBuf,linDatSize);			//将线坐标数据存入临时文件2
			linNdx.datOff=tmpDatOff;
			tmpDatOff+=linDatSize;
			GLinTmpFile1.Write(&linNdx,sizeof(LIN_NDX_STRU)); //将新索引结构存于临时文件1
			LinFile.Seek(ndxOff,CFile::begin);					//重新定位到索引结构处
			delete []linDatBuf;

		}
	
	return (1);
}
//将线临时文件转化成永久文件的函数
long Lin_TCF(CString FilePathName,int version)
{
	int i;
	lin_ndx_stru linNdx;
	int linDatSize;
	char *linDatBuf;
	int dataOff=sizeof(LIN_VERSION)+sizeof(GLinNum)+sizeof(GLinLNum)+GLinNum*sizeof(LIN_NDX_STRU);
	int ndxOff=sizeof(LIN_VERSION)+sizeof(GLinNum)+sizeof(GLinLNum);
	if(!GLinTmpFOpened)
		return (0);
	GLinFile.Open(FilePathName,CFile:: modeCreate|CFile:: modeReadWrite);
	GLinFile.Seek(0,CFile::begin);
	GLinTmpFile1.Seek(0,CFile::begin);
	GLinTmpFile2.Seek(0,CFile::begin);
	char flag[3];
	flag[0] = 'L';
	flag[1] = 'I';
	flag[2] = 'N';
	GLinFile.Write(flag,4);
	GLinFile.Write(&version,sizeof(int));
	GLinFile.Write(&GLinNum,sizeof(int));
	GLinFile.Write(&GLinLNum,sizeof(int));
	for(i=0;i<GLinNum;i++)
	{
		GLinTmpFile1.Read(&linNdx,sizeof(LIN_NDX_STRU));	
		linDatSize=linNdx.dotNum*sizeof(D_DOT);				//计算当前线的坐标点所需内存字节数
		linDatBuf=new char [linDatSize];					//申请存放当前线坐标数据所需内存
		GLinTmpFile2.Seek(linNdx.datOff,CFile::begin);		//定位到索引的坐标数据
		GLinTmpFile2.Read(linDatBuf,linDatSize);
		GLinFile.Seek(dataOff,CFile::begin);
		GLinFile.Write(linDatBuf,linDatSize);
		linNdx.datOff=dataOff;
		GLinFile.Seek(ndxOff,CFile::begin);
		GLinFile.Write(&linNdx,sizeof(LIN_NDX_STRU));
		dataOff+=linDatSize;
		ndxOff+=sizeof(LIN_NDX_STRU);
		delete []linDatBuf;
	}

	GLinFile.Close();
	GLinChanged=0;	
	return (1);

}
//将区永久文件转化为临时文件的函数
long Reg_FCT(CFile &RegFile,CString FilePathName)
{
	int i;
	int ndxOff=sizeof(REG_VERSION)+sizeof(GRegNum)+sizeof(GRegLNum);
	int tmpDatOff;
	reg_ndx_stru regNdx;
	int regDatSize;
	char *regDatBuf;
	if(!GRegTmpFOpened)
		NewReg();
	GRegTmpFile1.Seek(0,CFile::begin);		//将区临时文件1指针定义到开始
	tmpDatOff=0;
	RegFile.Seek(sizeof(REG_VERSION),CFile::begin);
	RegFile.Read(&GRegNum,sizeof(int));
	RegFile.Read(&GRegLNum,sizeof(int));
	RegFile.Seek(ndxOff,CFile::begin);		//将区永久文件指针跳过版本及逻辑数物理数
	for(i=0;i<GRegNum;i++)					
		{
			GRegTmpFile2.Seek(tmpDatOff,CFile::begin);		//将区临时文件2指针定义到tepdatoff
			RegFile.Read(&regNdx,sizeof(REG_NDX_STRU));			//读第i个区的索引
			ndxOff+=sizeof(REG_NDX_STRU);
			regDatSize=regNdx.dotNum*sizeof(D_DOT);				//计算当前区的坐标点所需内存字节数
			regDatBuf=new char [regDatSize];					//申请存放当前区坐标数据所需内存
			RegFile.Seek(regNdx.datOff,CFile::begin);			//定位到当前区坐标数据起始位置
			RegFile.Read(regDatBuf,regDatSize);					//读当前区的坐标数据
			GRegTmpFile2.Write(regDatBuf,regDatSize);			//将区坐标数据存入临时文件2
			regNdx.datOff=tmpDatOff;
			tmpDatOff += regDatSize;	
			GRegTmpFile1.Write(&regNdx,sizeof(REG_NDX_STRU)); //将新索引结构存于临时文件1
			RegFile.Seek(ndxOff,CFile::begin);					//重新定位到索引结构处
			delete []regDatBuf;

		}
	
	return (1);
}
//将区临时文件转化成永久性文件的函数
long Reg_TCF(CString FilePathName,int version)
{
	int i;
	reg_ndx_stru regNdx;
	int regDatSize;
	char *regDatBuf;
	int dataOff=sizeof(REG_VERSION)+sizeof(GRegNum)+sizeof(GRegLNum)+GRegNum*sizeof(REG_NDX_STRU);
	int ndxOff=sizeof(REG_VERSION)+sizeof(GRegNum)+sizeof(GRegLNum);
	if(!GRegTmpFOpened)
		return (0);
	GRegFile.Open(FilePathName,CFile:: modeCreate|CFile::modeReadWrite);
	GRegFile.Seek(0,CFile::begin);
	GRegTmpFile1.Seek(0,CFile::begin);
	GRegTmpFile2.Seek(0,CFile::begin);
	char flag[3];
	flag[0] = 'R';
	flag[1] = 'E';
	flag[2] = 'G';
	GRegFile.Write(flag,4);
	GRegFile.Write(&version,sizeof(int));
	GRegFile.Write(&GRegNum,sizeof(int));
	GRegFile.Write(&GRegLNum,sizeof(int));
	for(i=0;i<GRegNum;i++)
	{
		GRegTmpFile1.Read(&regNdx,sizeof(REG_NDX_STRU));	
		regDatSize=regNdx.dotNum*sizeof(D_DOT);				//计算当前线的坐标点所需内存字节数
		regDatBuf=new char [regDatSize];					//申请存放当前线坐标数据所需内存
		GRegTmpFile2.Seek(regNdx.datOff,CFile::begin);		//定位到索引的坐标数据
		GRegTmpFile2.Read(regDatBuf,regDatSize);
		GRegFile.Seek(dataOff,CFile::begin);
		GRegFile.Write(regDatBuf,regDatSize);
		regNdx.datOff=dataOff;
		GRegFile.Seek(ndxOff,CFile::begin);
		GRegFile.Write(&regNdx,sizeof(REG_NDX_STRU));
		dataOff+=regDatSize;
		ndxOff+=sizeof(REG_NDX_STRU);
		delete []regDatBuf;
	}

	GRegFile.Close();
	GRegChanged=0;	
	return (1);

}



 long GetOpenPath(CString &pathname)
{
	CFileDialog mFileDlg(TRUE,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT|OFN_ALLOWMULTISELECT,_T("点线区文件 |*.pnt;*.lin;*.regn||"),AfxGetMainWnd());
	if(mFileDlg.DoModal()!=IDOK)
	{
		goto GET_OPEN_PATH_END;
	}
	pathname =mFileDlg.GetPathName();
	return 1;

GET_OPEN_PATH_END: 
	return	0;

}

long GetPntSavePath(CString &pathname)
{
	CFileDialog mFileDlg(FALSE,_T("pnt"),_T("未命名"),NULL,_T("点线区文件 |*.pnt;*.lin;*.regn||"),AfxGetMainWnd());
	mFileDlg.m_ofn.lpstrTitle = _T("保存");	//设置对话框标题
	if(mFileDlg.DoModal()!=IDOK)
	{
		goto GET_SAVE_PATH_END;	
	}
	pathname=mFileDlg.GetPathName();	
	return 1; 

GET_SAVE_PATH_END:
	return 0;
}
long GetLinSavePath(CString &pathname)
{
	CFileDialog mFileDlg(FALSE,_T("lin"),_T("未命名"),NULL,_T("点线区文件 |*.pnt;*.lin;*.regn||"),AfxGetMainWnd());
	mFileDlg.m_ofn.lpstrTitle = _T("保存");	//设置对话框标题
	if(mFileDlg.DoModal()!=IDOK)
	{
		goto GET_SAVE_PATH_END;	
	}
	pathname=mFileDlg.GetPathName();
	return 1; 	

GET_SAVE_PATH_END:
	return	0;
}
long GetRegSavePath(CString &pathname)
{
	CFileDialog mFileDlg(FALSE,_T("regn"),_T("未命名"),NULL,_T("点线区文件 |*.pnt;*.lin;*.regn||"),AfxGetMainWnd());
	mFileDlg.m_ofn.lpstrTitle = _T("保存");	//设置对话框标题
	if(mFileDlg.DoModal()!=IDOK)
	{
		goto GET_SAVE_PATH_END;	
	}
	pathname=mFileDlg.GetPathName();
	return 1;	

GET_SAVE_PATH_END:
	return	0;
}
long GetPntSaveAsPath(CString &pathname)
{
	CFileDialog mFileDlg(FALSE,_T("pnt"),(LPCTSTR)GPntFName,NULL,_T("点线区文件 |*.pnt;*.lin;*.regn||"),AfxGetMainWnd());
	mFileDlg.m_ofn.lpstrTitle = _T("另存为");	//设置对话框标题
	if(mFileDlg.DoModal()!=IDOK)
	{
		goto GET_SAVEAS_PATH_END;	
	}
	pathname=mFileDlg.GetPathName();
	return 1;	

GET_SAVEAS_PATH_END:
	return 0;
}
long GetLinSaveAsPath(CString &pathname)
{
	CFileDialog mFileDlg(FALSE,_T("lin"),(LPCTSTR)GRegFName,NULL,_T("点线区文件 |*.pnt;*.lin;*.regn||"),AfxGetMainWnd());
	mFileDlg.m_ofn.lpstrTitle = _T("保存");	//设置对话框标题
	if(mFileDlg.DoModal()!=IDOK)
	{
		goto GET_SAVEAS_PATH_END;	
	}
	pathname=mFileDlg.GetPathName();
	return 1;	

GET_SAVEAS_PATH_END:
	return 0;
}
long GetRegSaveAsPath(CString &pathname)
{
	CFileDialog mFileDlg(FALSE,_T("regn"),(LPCTSTR)GRegFName,NULL,_T("点线区文件 |*.pnt;*.lin;*.regn||"),AfxGetMainWnd());
	mFileDlg.m_ofn.lpstrTitle = _T("保存");	//设置对话框标题
	if(mFileDlg.DoModal()!=IDOK)
	{
		goto GET_SAVEAS_PATH_END;	
	}
	pathname=mFileDlg.GetPathName();
	return 1; 	

GET_SAVEAS_PATH_END:
	return 0;
}
long GetPntExitPath(CString &pathname)
{
	int a=AfxMessageBox(_T("你已经新建或修改了文档,请先保存!"),MB_YESNOCANCEL|MB_ICONQUESTION);
	if(a==IDCANCEL)
		return 0;
	else if(a==IDNO)
	{
		return 2;
	}
	else
	{
		CFileDialog mFileDlg(FALSE,_T("pnt"),_T("未命名"),NULL,_T("点线区文件 |*.pnt;*.lin;*.regn||"),AfxGetMainWnd());
		mFileDlg.m_ofn.lpstrTitle = _T("保存");	//设置对话框标题
		if(mFileDlg.DoModal()!=IDOK)
		{
			goto GET_SAVE_PATH_END;	
		}
		pathname=mFileDlg.GetPathName();	
		return 1; 
	}
	GET_SAVE_PATH_END:
	return 0;
}
long GetLinExitPath(CString &pathname)
{
	int a=AfxMessageBox(_T("你已经新建或修改了文档,请先保存!"),MB_YESNOCANCEL|MB_ICONQUESTION);
	if(a==IDCANCEL)
		return 0;
	else if(a==IDNO)
	{
		return 2;
	}
	else
	{
		CFileDialog mFileDlg(FALSE,_T("lin"),_T("未命名"),NULL,_T("点线区文件 |*.pnt;*.lin;*.regn||"),AfxGetMainWnd());
		mFileDlg.m_ofn.lpstrTitle = _T("保存");	//设置对话框标题
		if(mFileDlg.DoModal()!=IDOK)
		{
			goto GET_SAVE_PATH_END;	
		}
		pathname=mFileDlg.GetPathName();	
		return 1; 
	}
	GET_SAVE_PATH_END:
	return 0;
}
long GetRegExitPath(CString &pathname)
{
	int a=AfxMessageBox(_T("你已经新建或修改了文档,请先保存!"),MB_YESNOCANCEL|MB_ICONQUESTION);
	if(a==IDCANCEL)
		return 0;
	else if(a==IDNO)
	{
		return 2;
	}
	else
	{
		CFileDialog mFileDlg(FALSE,_T("regn"),_T("未命名"),NULL,_T("点线区文件 |*.pnt;*.lin;*.regn||"),AfxGetMainWnd());
		mFileDlg.m_ofn.lpstrTitle = _T("保存");	//设置对话框标题
		if(mFileDlg.DoModal()!=IDOK)
		{
			goto GET_SAVE_PATH_END;	
		}
		pathname=mFileDlg.GetPathName();	
		return 1; 
	}
	GET_SAVE_PATH_END:
	return 0;
}

//获得区颜色
long GetRegColor(COLORREF &regColor)
{
	CRegColor dlg;
	if(dlg.DoModal()==IDOK)
	{
		
		regColor=dlg.m_reg_coloc_ctrl.GetColor();;
	}
	else regColor=GregDefultColor;
	return (1);
}

long AppendAReg(COLORREF regColor,int RegPntNum,int RegDatOff,d_dot *ptRegPnt)
{
	//1.在坐标数据临时文件2中写入数据

	GRegTmpFile2.Seek(0,CFile::end);
	GRegTmpFile2.Write(ptRegPnt,RegPntNum*sizeof(D_DOT));
	//2.在索引临时文件1中写入数据
	REG_NDX_STRU.isDel=0;
	REG_NDX_STRU.color=regColor;
	REG_NDX_STRU.datOff=RegDatOff;
	REG_NDX_STRU.dotNum=RegPntNum;
	GRegTmpFile1.Seek(0,CFile::end);
	GRegTmpFile1.Write(&REG_NDX_STRU,sizeof(REG_NDX_STRU));
	return (1);
}
//函数功能：得到第ri个区的索引及坐标数据
//参数：第ri个区；区索引参数的引用，用来保存读出来的区；区边界点数据指针，保存点坐标数据；指针长度，与点数相对应
long GetAReg(int ri,reg_ndx_stru &regNdx,d_dot *ptRegDatBuf, long bufLen)
{
	if(ri<=0)
		return (0);
	if(GRegTmpFOpened==0)
		return (0);
	if(GRegTmpFile1.GetLength()==0)
		return (0);
	int off=(ri-1)*sizeof(REG_NDX_STRU);
	int pntSize;
	GRegTmpFile1.Seek(off,CFile::begin);
	GRegTmpFile1.Read(&regNdx,sizeof(REG_NDX_STRU));
	if(regNdx.dotNum>bufLen)
		return (0);
	pntSize=regNdx.dotNum*sizeof(D_DOT);
	GRegTmpFile2.Seek(regNdx.datOff,CFile::begin);
	GRegTmpFile2.Read(ptRegDatBuf,pntSize);
	return (1);
	


}
//函数功能：在临时文件中删除第ri个区
//参数：临时文件中第ri个区
long DeleteAReg(int ri)
{
	if(ri<=0)
		return (0);
	reg_ndx_stru regNdx;
	int size=(ri-1)*sizeof(REG_NDX_STRU);
	GRegTmpFile1.Seek(size,CFile::begin);
	GRegTmpFile1.Read(&regNdx,sizeof(REG_NDX_STRU));
	regNdx.isDel=1;
	GRegTmpFile1.Seek(size,CFile::begin);
	GRegTmpFile1.Write(&regNdx,sizeof(REG_NDX_STRU));
	return (1);

}

//函数功能：判断一个点是否在一个区内,我们通过该点在y方向上的射线与区的交点数来判断
//参数：点指针，指向区坐标数据；该区坐标点数；需要判断的点
long IsInReg(POINT *regPnt,int regPntNum,POINT xy)
{
	int i;
	int sum=0;
	int ymax,xmin,xmax,ymin;
	double k,b,yy;
	for(i=0;i<regPntNum-1;i++)
	{
		if(regPnt[i].x==xy.x&&regPnt[i].y==xy.y)			//如果该点是区上线的交点则我们约定它在区中
			return (1);
		if(regPnt[i].y>regPnt[i+1].y)						//计算出每一条线坐标x最大最小值与y最大最小值
		{
			ymax = regPnt[i].y;
			ymin = regPnt[i+1].y;
		}
		else
		{
			ymax = regPnt[i+1].y;
			ymin = regPnt[i].y;
		}
		if(regPnt[i].x>regPnt[i+1].x)
		{
			xmax = regPnt[i].x;
			xmin = regPnt[i+1].x;
		}
		else
		{
			xmax = regPnt[i+1].x;
			xmin = regPnt[i].x;
		}
													//结束算最大最小值
		if(regPnt[i+1].y==regPnt[i].y)				//如果该线平行于y方向
		{
			if(xy.y==regPnt[i].y&&xy.x>=xmin&&xy.x<=xmax)		//如果该点在该线上则交点加1
				sum++;
		}
		else if(regPnt[i+1].x!=regPnt[i].x)
		{
			k=double((regPnt[i+1].x-regPnt[i].x))/double((regPnt[i+1].y-regPnt[i].y));	//计算出该直线斜率式方程的斜率k
			b=double((regPnt[i+1].y*regPnt[i].x-regPnt[i+1].x*regPnt[i].y))/double((regPnt[i+1].y-regPnt[i].y));	//计算出该直线斜率式方程的斜率b
			
			yy=(xy.x-b)/k;		//计算出该点x下的y值
			if(yy>=xy.y&&yy<=ymax&&yy>=ymin)	//如果y在最大最小y值中，则交点加1
			{
				sum++;
			}

		}
	}
	//计算最后一个点与第一个点形成的直线
	if(regPnt[regPntNum-1].y>regPnt[0].y)
		{
			ymax = regPnt[regPntNum-1].y;
			ymin = regPnt[0].y;
		}
		else
		{
			ymax = regPnt[0].y;
			ymin = regPnt[regPntNum-1].y;
		}
		if(regPnt[regPntNum-1].x>regPnt[0].x)
		{
			xmax = regPnt[regPntNum-1].x;
			xmin = regPnt[0].x;
		}
		else
		{
			xmax = regPnt[0].x;
			xmin = regPnt[regPntNum-1].x;
		}
		if(regPnt[regPntNum-1].y==regPnt[0].y)
		{
			if(xy.y==regPnt[0].y&&xy.x>=xmin&&xy.x<=xmax)
				sum++;
		}
		else if(regPnt[regPntNum-1].x!=regPnt[0].x)
		{
			k=double((regPnt[regPntNum-1].x-regPnt[0].x))/double((regPnt[regPntNum-1].y-regPnt[0].y));
			b=double((regPnt[regPntNum-1].y*regPnt[0].x-regPnt[regPntNum-1].x*regPnt[0].y))/double((regPnt[regPntNum-1].y-regPnt[0].y));
			
			yy=(xy.x-b)/k;
			if(yy>=xy.y&&yy<=ymax&&yy>=ymin)
			{		
				sum++;
			}
		}
		

	if(sum%2==1)
	{
		return (1);
	}
	else
		return (0);

}
//函数功能：实现一个点的数据坐标系与窗口坐标系的转化
//参数：点xy
long DotDPtoVP(d_dot &xy)
{
	xy.x-=GxOffset;
	xy.y-=GyOffset;
	xy.x*=GZoom;
	xy.y*=GZoom;
	return (1);
}
//函数功能：实现一个点的数据坐标系与窗口坐标系的转化
//参数：点xy
long DotDPtoVP(pnt_stru &xy)
{
	xy.x-=GxOffset;
	xy.y-=GyOffset;
	xy.x*=GZoom;
	xy.y*=GZoom;
	return (1);
}
//函数功能：实现一些点的数据坐标系与窗口坐标系的转化
//参数：点指针，点数量
long SomeDotDPtoVP(d_dot *ptPntDat,int datLen)
{
	int i;
	for(i=0;i<datLen;i++)
	{
		DotDPtoVP(ptPntDat[i]);
	}
	return (1);
}
//函数功能：实现一个点的窗口坐标系与数据坐标系的转化
//参数：点xy
long DotVPtoDP(d_dot &xy)
{
	xy.x/=GZoom;
	xy.y/=GZoom;
	xy.x+=GxOffset;
	xy.y+=GyOffset;
	return (1);
}
//函数功能：实现一个点的窗口坐标系与数据坐标系的转化
//参数：点xy
long DotVPtoDP(pnt_stru &xy)
{
	xy.x/=GZoom;
	xy.y/=GZoom;
	xy.x+=GxOffset;
	xy.y+=GyOffset;
	return (1);
}
//函数功能：实现一些点的窗口坐标系与数据坐标系的转化
//参数：点指针，点数量
long SomeDotVPtoDP(d_dot *ptPntDat,int datLen)
{
	int i;
	for(i=0;i<datLen;i++)
	{
		DotVPtoDP(ptPntDat[i]);
	}
	return (1);
}
//函数功能：得到包含点线区图形的最小外包矩形
//参数：矩形rect的引用
long GetMBR(CRect &rect)
{
	int i,j;
	d_dot temp;
	double minx,miny,maxx,maxy;
	if(GPntTmpFOpened==0&&GLinTmpFOpened==0&&GRegTmpFOpened==0)
		return (0);
	if(GPntTmpFOpened==1&&GPntNum!=0)				//对minx,miny,maxx,maxy进行初始化，由于我们不知道文件是否打开则需每个都初始化
	{
		GPntTmpFile.Seek(0,CFile::begin);
		do{
			GPntTmpFile.Read(&PNT_STRU, sizeof( PNT_STRU ));
		}while(PNT_STRU.isDel==GisShowDeleted);
		minx=PNT_STRU.x;
		maxx=PNT_STRU.x;
		miny=PNT_STRU.y;
		maxy=PNT_STRU.y;
	}
	if(GLinTmpFOpened==1&&GLinNum!=0)
	{
		GLinTmpFile1.Seek(0,CFile::begin);
		do{
			GLinTmpFile1.Read(&LIN_NDX_STRU,sizeof(LIN_NDX_STRU));
		}while(LIN_NDX_STRU.isDel!=GisShowDeleted);
		GLinTmpFile2.Seek(LIN_NDX_STRU.datOff,CFile::begin);
		GLinTmpFile2.Read(&temp,sizeof(D_DOT));					//取出一个点进行初始化
		minx=temp.x;
		maxx=temp.x;
		miny=temp.y;
		maxy=temp.y;
	}
	if(GRegTmpFOpened==1&&GRegNum!=0)
	{
		GRegTmpFile1.Seek(0,CFile::begin);
		do{
			GRegTmpFile1.Read(&REG_NDX_STRU,sizeof(REG_NDX_STRU));
		}while(REG_NDX_STRU.isDel!=GisShowDeleted);
		GRegTmpFile2.Seek(REG_NDX_STRU.datOff,CFile::begin);
		GRegTmpFile2.Read(&temp,sizeof(D_DOT));
		minx=temp.x;
		maxx=temp.x;
		miny=temp.y;
		maxy=temp.y;
	}
	if(GPntTmpFOpened==1&&GPntNum!=0)
	{
		GPntTmpFile.Seek(0,CFile::begin);
		for(i=0;i<GPntNum;i++)   //遍历所有的点
			{
				GPntTmpFile.Read(&PNT_STRU, sizeof( PNT_STRU ));
				if(PNT_STRU.isDel==GisShowDeleted)            
				{
					if(PNT_STRU.x<minx)					
						minx=PNT_STRU.x;					
					if(PNT_STRU.x>maxx)				
						maxx=PNT_STRU.x;					
					if(PNT_STRU.y<miny)					
						miny=PNT_STRU.y;					
					if(PNT_STRU.y>maxy)					
						maxy=PNT_STRU.y;
				}
			}
	}
	if(GLinTmpFOpened==1&&GLinNum!=0)
	{
		GLinTmpFile1.Seek(0,CFile::begin);
		for(i=0;i<GLinNum;i++)
		{
			GLinTmpFile1.Read(&LIN_NDX_STRU,sizeof(LIN_NDX_STRU));
			if(LIN_NDX_STRU.isDel==GisShowDeleted)
			{
				GLinTmpFile2.Seek(LIN_NDX_STRU.datOff,CFile::begin);
				for(j=0;j<LIN_NDX_STRU.dotNum;j++)
				{
					GLinTmpFile2.Read(&temp,sizeof(D_DOT));
					if(temp.x<minx)					
						minx=temp.x;					
					if(temp.x>maxx)				
						maxx=temp.x;					
					if(temp.y<miny)					
						miny=temp.y;					
					if(temp.y>maxy)					
						maxy=temp.y;
				}
			}
		}
	}
	if(GRegTmpFOpened==1&&GRegNum!=0)
	{
		GRegTmpFile1.Seek(0,CFile::begin);
		for(i=0;i<GRegNum;i++)
		{
			GRegTmpFile1.Read(&REG_NDX_STRU,sizeof(REG_NDX_STRU));
			if(REG_NDX_STRU.isDel==GisShowDeleted)
			{
				GRegTmpFile2.Seek(REG_NDX_STRU.datOff,CFile::begin);
				for(j=0;j<REG_NDX_STRU.dotNum;j++)
				{
					GRegTmpFile2.Read(&temp,sizeof(D_DOT));
					if(temp.x<minx)					
						minx=temp.x;					
					if(temp.x>maxx)				
						maxx=temp.x;					
					if(temp.y<miny)					
						miny=temp.y;					
					if(temp.y>maxy)					
						maxy=temp.y;
				}
			}
		}
	}
	
	rect.bottom=maxy;
	rect.top=miny;
	rect.left=minx;
	rect.right=maxx;
	return (1);
}
//函数功能：画一个无填充区
//参数：点坐标指针；点数；设备上下文
long DrawAHollowReg(POINT *ptRegPnt,int pntNum,CClientDC *pDC)
{
	int i;
	for(i=0;i<pntNum-1;i++)			
	{
		pDC->MoveTo(ptRegPnt[i]);
		pDC->LineTo(ptRegPnt[i+1]);
	}
	pDC->MoveTo(ptRegPnt[pntNum-1]);
	pDC->LineTo(ptRegPnt[0]);
	return (1);

}
//函数功能：画一个区
//参数：点坐标指针；点数；设备上下文
long DrawAReg(POINT *ptRegPnt,int pntNum,CClientDC *pDC)
	{
	int i;
	for(i=0;i<pntNum-1;i++)			
	{
		pDC->MoveTo(ptRegPnt[i]);
		pDC->LineTo(ptRegPnt[i+1]);
	}
	pDC->MoveTo(ptRegPnt[pntNum-1]);
	pDC->LineTo(ptRegPnt[0]);
	pDC->Polygon(ptRegPnt,pntNum);
	return (1);

}
//函数功能：在临时文件中删除第ri个区
//参数：临时文件中第ri个区
long RecoverReg(int ri)
{
	if(ri<=0)
		return (0);
	reg_ndx_stru regNdx;
	int size=(ri-1)*sizeof(REG_NDX_STRU);
	GRegTmpFile1.Seek(size,CFile::begin);
	GRegTmpFile1.Read(&regNdx,sizeof(REG_NDX_STRU));
	regNdx.isDel=0;
	GRegTmpFile1.Seek(size,CFile::begin);
	GRegTmpFile1.Write(&regNdx,sizeof(REG_NDX_STRU));
	return (1);

}
long AmendRegPAR(COLORREF newRegColor,reg_ndx_stru &regNdx,int i)
{
	int off=(i-1)*sizeof(REG_NDX_STRU);
	regNdx.color=newRegColor;
	GRegTmpFile1.Seek(off,CFile::begin);
	GRegTmpFile1.Write(&regNdx,sizeof(REG_NDX_STRU));
	return (1);
}
//函数功能：设置默认颜色
long SetDefultColor()
{
	CRegColor dlg;
	if(dlg.DoModal()==IDOK)
	{
		
		GregDefultColor=dlg.m_reg_coloc_ctrl.GetColor();
		return (1);
	}
	
	return (0);
}
//获得点颜色
long GetPntColor(COLORREF &pntColor)
{
	CRegColor dlg;
	if(dlg.DoModal()==IDOK)
	{
		
		pntColor=dlg.m_reg_coloc_ctrl.GetColor();;
		return (1);
	}
	return (0);
}
#endif