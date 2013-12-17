
// MapEditor1.0View.cpp : CMapEditor10View 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "MapEditor1.0.h"
#endif

#include "MapEditor1.0Doc.h"
#include "MapEditor1.0View.h"


#include"FileStruct.h"
#include "PointFuns.h"
#include"LineFuns.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMapEditor10View

IMPLEMENT_DYNCREATE(CMapEditor10View, CView)

BEGIN_MESSAGE_MAP(CMapEditor10View, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CMapEditor10View::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_FILE_NEW, &CMapEditor10View::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CMapEditor10View::OnFileOpen)
	ON_COMMAND(ID_FILE_SAVE, &CMapEditor10View::OnFileSave)
	ON_COMMAND(ID_FILE_SAVE_AS, &CMapEditor10View::OnFileSaveAs)
	ON_COMMAND(ID_APP_EXIT, &CMapEditor10View::OnAppExit)
	ON_COMMAND(ID_32771, &CMapEditor10View::ZoomIn)
	ON_COMMAND(ID_32772, &CMapEditor10View::ZoomOut)
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_32773, &CMapEditor10View::CreatReg)
	ON_COMMAND(ID_32774, &CMapEditor10View::DeleteReg)
	ON_COMMAND(ID_32775, &CMapEditor10View::MoveReg)
	ON_COMMAND(ID_32776, &CMapEditor10View::WindowMove)
	ON_COMMAND(ID_32777, &CMapEditor10View::WindowReset)
	ON_COMMAND(ID_32778, &CMapEditor10View::ShowReg)
//	ON_COMMAND(ID_32779, &CMapEditor10View::On32779)
ON_COMMAND(ID_32779, &CMapEditor10View::ShowDeletedReg)
ON_COMMAND(ID_32780, &CMapEditor10View::RecoverAReg)
ON_COMMAND(ID_32781, &CMapEditor10View::AmmendARegPAR)
ON_COMMAND(ID_32782, &CMapEditor10View::AmmendDefultPAR)
ON_COMMAND(ID_32783, &CMapEditor10View::OnCreatePnt)
ON_COMMAND(ID_32784, &CMapEditor10View::OnDeletePnt)
ON_COMMAND(ID_32785, &CMapEditor10View::OnMovePnt)
ON_COMMAND(ID_32786, &CMapEditor10View::OnShowDeletedPnt)
ON_COMMAND(ID_32787, &CMapEditor10View::OnRecoverPnt)
ON_COMMAND(ID_32788, &CMapEditor10View::OnSetPnt)
ON_COMMAND(ID_32789, &CMapEditor10View::OnShowPnt)
ON_COMMAND(ID_CREATE_LIN, &CMapEditor10View::OnCreateLin)
ON_COMMAND(ID_MOVE_LIN, &CMapEditor10View::OnMoveLin)
ON_COMMAND(ID_CONNECT_LIN, &CMapEditor10View::OnConnectLin)
ON_COMMAND(ID_SETLIN, &CMapEditor10View::OnSetlin)
ON_COMMAND(ID_SHOW_LIN, &CMapEditor10View::OnShowLin)
ON_COMMAND(ID_SHOW_DEL_LIN, &CMapEditor10View::OnShowDelLin)
ON_COMMAND(ID_RECOVER_LIN, &CMapEditor10View::OnRecoverLin)
ON_COMMAND(ID_DEL_LIN, &CMapEditor10View::OnDelLin)
//ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()

//临时结构
d_dot regPntdat[500];
reg_ndx_stru regNdx;
d_dot linPntdat[500];

// CMapEditor10View 构造/析构

CMapEditor10View::CMapEditor10View()
{
	// TODO: 在此处添加构造代码
	isShowPnt=1;
	isShowLin=1;
	isShowReg=1;
	m_step = 0;
	m_startPnt.x = 0;
	m_startPnt.y = 0;
	m_lastPnt.x = 0;
	m_lastPnt.y = 0;
	slectedPnt.x=0;
	slectedPnt.y=0;
	zoomstyle=0;
	regPntNum=0;
	regDatOff=0;
	RegPntIdx=0;


	MakingLine=0;      //开始画线
    step=0;
    isfirst=0;         //连接线第一个线号
	issecond=0;			//连接线第二个线号
	LinPntNum=0;     //移动线个数
	LinPntIdx=0;

}

CMapEditor10View::~CMapEditor10View()
{
	m_step = 0;
	m_startPnt.x = 0;
	m_startPnt.y = 0;
	m_lastPnt.x = 0;
	m_lastPnt.y = 0;
	slectedPnt.x=0;
	slectedPnt.y=0;
	
	
}

BOOL CMapEditor10View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CMapEditor10View 绘制

void CMapEditor10View::OnDraw(CDC* pDC)
{
	CMapEditor10Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
	//画点
		int i=0;
		int len=0;
		long tempX=0;
		long tempY=0;
		CPoint temp;

		if(1==GPntTmpFOpened&&isShowPnt==1)
		{
			len=GPntTmpFile.GetLength();
			GPntTmpFile.Seek(0,CFile::begin);

			for(i=0;i<len/sizeof(PNT_STRU);i++)   //遍历所有的点
			{
				GPntTmpFile.Read(&PNT_STRU, sizeof( PNT_STRU ));
				if(PNT_STRU.isDel==GisShowDeleted)            
				{
					DotDPtoVP(PNT_STRU);
					temp.x=PNT_STRU.x;
					temp.y=PNT_STRU.y;	
					COLORREF Color=PNT_STRU.color;
					DrawPoint(temp,Color);
				}
				
			}
		}

	//画线
		int ii=0;
		int j=0;
		int datalen=0;
		int ndxlen=0;
		int dotnum=0;
		int datff=0;
		CPoint ptemp;
		int linlen=sizeof(LIN_NDX_STRU);
		int plen=sizeof(D_DOT);

		if(1==GLinFCreated)
		{
			datalen=GLinTmpFile2.GetLength();
			ndxlen=GLinTmpFile1.GetLength();
			GLinTmpFile2.Seek(0,CFile::begin);
			GLinTmpFile1.Seek(0,CFile::begin);

			for(ii=0;ii< ndxlen / linlen;ii++)						 //遍历所有的线索引
			{
				GLinTmpFile1.Read(&LIN_NDX_STRU,  linlen);
				if(LIN_NDX_STRU.isDel==0&&GCurLINState==0)            //画出没被删掉的线
				{
					dotnum=LIN_NDX_STRU.dotNum;
					datff=LIN_NDX_STRU.datOff;

					GLinTmpFile2.Seek(datff,CFile::begin);
					POINT *p=new POINT[dotnum];
					for(j=0; j < dotnum ; j++)
					{
						GLinTmpFile2.Read(&D_DOT, plen );
						DotDPtoVP(D_DOT);          //新加的
						ptemp.x=D_DOT.x;
						ptemp.y=D_DOT.y;
						p[j]=ptemp;
					}
					for(j=0; j < dotnum ; j++)
					{
						if(j==0)
							pDC->MoveTo(p[j]);
						else
							pDC->LineTo(p[j]);
					}
				}

				else if(LIN_NDX_STRU.isDel==1&&GCurLINState==1)            //画被删掉的线
				{
					dotnum=LIN_NDX_STRU.dotNum;
					datff=LIN_NDX_STRU.datOff;

					GLinTmpFile2.Seek(datff,CFile::begin);
					POINT *p=new POINT[dotnum];
					for(j=0; j < dotnum ; j++)
					{
						GLinTmpFile2.Read(&D_DOT, plen );
						ptemp.x=D_DOT.x;
						ptemp.y=D_DOT.y;
						p[j]=ptemp;
					}
					for(j=0; j < dotnum ; j++)
					{
						if(j==0)
							pDC->MoveTo(p[j]);
						else
							pDC->LineTo(p[j]);
					}
				}
			}
			ReleaseDC(pDC);

		}

		

	//画区
		CClientDC dc(this);
		if(GRegTmpFOpened==1&&isShowReg==1)
		{
			for(i=1;i<=GRegNum;i++)
			{
				if(GetAReg(i,regNdx,regPntdat,500)==1)
				{
					if(regNdx.isDel==GisShowDeleted)
					{
						CPen pen(PS_SOLID,1,regNdx.color);
						dc.SelectObject(&pen);
						CBrush brush(regNdx.color);
						dc.SelectObject (&brush);
						dc.SetROP2(R2_NOTXORPEN);
						SomeDotDPtoVP(regPntdat,regNdx.dotNum);
						for(j=0;j<regNdx.dotNum;j++)
						{
							regPnt[j].x=regPntdat[j].x;
							regPnt[j].y=regPntdat[j].y;
						}
						DrawAReg(regPnt,regNdx.dotNum,&dc);
					}
				}
			}
		}
		
		ReleaseDC(pDC);


		//清空移动线数据
		firstlinnum=0;
		seclinnum=0;
		isfirst=0;
		issecond=0;
		MakingLine=0;
		
		//状态栏
		CString sMousePos;
		CString Pntnum="点：";
		CString Linnum="线：";
		CString Regnum="区：";
		sMousePos.Format(_T(" %s %d %s %d %s %d"),Pntnum,GPntNum,Linnum,GLinNum,Regnum,GRegNum);
		AfxGetMainWnd()->GetDescendantWindow(AFX_IDW_STATUS_BAR)->SetWindowText(sMousePos);
}


// CMapEditor10View 打印


void CMapEditor10View::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CMapEditor10View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CMapEditor10View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CMapEditor10View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}

void CMapEditor10View::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	switch(GCurOperState)
	{
	case OPERSTATE_CREAT_REG:			//设置操作状态为造区
		CreatRegRBU(point);
		break;
	case OPERSTATE_INPUT_LIN:						//造线
		CreateLinRBU(point);
		break;
	
	}
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CMapEditor10View::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CMapEditor10View 诊断

#ifdef _DEBUG
void CMapEditor10View::AssertValid() const
{
	CView::AssertValid();
}

void CMapEditor10View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMapEditor10Doc* CMapEditor10View::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMapEditor10Doc)));
	return (CMapEditor10Doc*)m_pDocument;
}
#endif //_DEBUG


// CMapEditor10View 消息处理程序


void CMapEditor10View::OnFileNew()
{
	// TODO: 在此添加命令处理程序代码
	int type;
	GetNewType(type);
	if(type&0x0001)
		NewPnt();		//新建点文件
	if(type&0x0002)
		NewLin();		//新建线文件
	if(type&0x0004)
		NewReg();		//新建区文件

	
}



void CMapEditor10View::OnFileOpen()
{
	// TODO: 在此添加命令处理程序代码
	CString FilePathName;
	char sRead[3];
	CFile mFile;
	GisShowDeleted=0;
	if(GetOpenPath(FilePathName)==0)			//判断是否打开了路径
	{
		goto ON_FILE_OPEN_END;
	}
	mFile.Open (FilePathName,CFile::modeRead);
	mFile.Read(sRead,3);
	if(sRead[0]=='P'&&sRead[1]=='N'&&sRead[2]=='T')	//确定该文件是点文件
	{

		memcpy(GPntFName,FilePathName,256);
		Pnt_FCT(mFile,FilePathName);
		CRect rt;
		CClientDC dc(this);
		GetClientRect(&rt);								//获取客户区矩形
		dc.FillRect(&rt,new CBrush(RGB(255,255,255)));			//空白覆盖客户区
	}
	else if(sRead[0]=='L'&&sRead[1]=='I'&&sRead[2]=='N')	//确定该文件是线文件
	{
		memcpy(GLinFName,FilePathName,256);
		Lin_FCT(mFile,FilePathName);
		CRect rt;
		CClientDC dc(this);
		GetClientRect(&rt);								//获取客户区矩形
		dc.FillRect(&rt,new CBrush(RGB(255,255,255)));			//空白覆盖客户区

	
	}
	else if(sRead[0]=='R'&&sRead[1]=='E'&&sRead[2]=='G')	//确定该文件是区文件
	{
		memcpy(GRegFName,FilePathName,256);
		Reg_FCT(mFile,FilePathName);
		CRect rt;
		CClientDC dc(this);
		GetClientRect(&rt);								//获取客户区矩形
		dc.FillRect(&rt,new CBrush(RGB(255,255,255)));			//空白覆盖客户区
		DrawRegFile();
	}
	
	mFile.Close();
	ON_FILE_OPEN_END:;
	
}




void CMapEditor10View::OnFileSave()
{
	/*
	// TODO: 在此添加命令处理程序代码
	CString FilePathName;
	if(GPntChanged==0&&GLinChanged==0&&GRegChanged==0)
	{
		MessageBox(_T("无修改操作！"));
	}
	if(GPntChanged==1&&GLinChanged==0&&GRegChanged==0)			//点文件已被修改
	{
		if(GPntFName[0]!='\0')			//是否是已打开的文件
		{
			Pnt_TCF((LPCTSTR)GPntFName,10);
		}
		else
		{
			if(GetPntSavePath(FilePathName)==0)			//判断是否打开了路径
			{
				goto ON_FILE_SAVE_END;
			}
			memcpy(GPntFName,FilePathName,256);
			Pnt_TCF(FilePathName,10);
			
		}
	}
	else if(GPntChanged==0&&GLinChanged==1&&GRegChanged==0)			//线文件已被修改
	{
		if(GLinFName[0]!='\0')			//是否是已打开的文件
		{
			Lin_TCF((LPCTSTR)GLinFName,10);
			
		}
		else
		{
			if(GetLinSavePath(FilePathName)==0)			//判断是否打开了路径
			{
				goto ON_FILE_SAVE_END;
			}
			memcpy(GLinFName,FilePathName,256);
			Lin_TCF(FilePathName,10);
			
		}
	}
	else if(GPntChanged==0&&GLinChanged==0&&GRegChanged==1)			//区文件已被修改
	{
		if(GRegFName[0]!='\0')									//是否是已打开的文件
		{
			Reg_TCF((LPCTSTR)GRegFName,10);
		}
		else
		{
			if(GetRegSavePath(FilePathName)==0)			//判断是否打开了路径
			{
				goto ON_FILE_SAVE_END;
			}
			memcpy(GRegFName,FilePathName,256);
			Reg_TCF(FilePathName,10);
			
		}
	}
	ON_FILE_SAVE_END:;
	*/

	// TODO: 在此添加命令处理程序代码
		CString FilePathName;
		if(GPntChanged==0&&GLinChanged==0&&GRegChanged==0)
		{
			MessageBox(_T("无修改操作！"));
		}
		if(GPntFCreated==1)			//点文件已被修改
		{
			if(GPntFName[0]!=0)			//是否是已打开的文件
			{
				Pnt_TCF((LPCTSTR)GPntFName,10);
			}
			else
			{
				if(GetPntSavePath(FilePathName)==0)			//判断是否打开了路径
				{
					goto ON_FILE_SAVE_END;
				}
			memcpy(GPntFName,FilePathName,256);
			Pnt_TCF(FilePathName,10);

			}
		}
		if(GLinChanged==1)			//线文件已被修改
		{
			if(GLinFName[0]!=0)			//是否是已打开的文件
			{
				Lin_TCF((LPCTSTR)GLinFName,10);

			}
			else
			{
				if(GetLinSavePath(FilePathName)==0)			//判断是否打开了路径
				{
					goto ON_FILE_SAVE_END;
				}
				memcpy(GLinFName,FilePathName,256);
			Lin_TCF(FilePathName,10);

			}
		}
		if(GRegChanged==1)			//区文件已被修改
		{
			if(GRegFName[0]!=0)			//是否是已打开的文件
			{
				Reg_TCF((LPCTSTR)GRegFName,10);
			}
			else
			{
				if(GetRegSavePath(FilePathName)==0)			//判断是否打开了路径
				{
					goto ON_FILE_SAVE_END;
				}
				memcpy(GRegFName,FilePathName,256);
			Reg_TCF(FilePathName,10);

			}
		}
ON_FILE_SAVE_END:;
}


void CMapEditor10View::OnFileSaveAs()
{
	// TODO: 在此添加命令处理程序代码
	CString FilePathName;
	if(GPntChanged==0&&GLinChanged==0&&GRegChanged==0)		//文件没有被修改
	{
		MessageBox(_T("无修改操作！"));
	}
	else if(GPntFCreated==1)		//点文件被修改
	{
		if(GetPntSaveAsPath(FilePathName)==0)			//判断是否打开了路径
			{
				goto ON_FILE_SAVEAS_END;
			}
		memcpy(GPntFName,FilePathName,256);
		Pnt_TCF(FilePathName,10);
		
		
	}
	else if(GLinChanged==1)	//线文件被修改
	{
		if(GetLinSaveAsPath(FilePathName)==0)			//判断是否打开了路径
		{
			goto ON_FILE_SAVEAS_END;
		}
		memcpy(GLinFName,FilePathName,256);
		Lin_TCF(FilePathName,10);
		
		
	}
	else if(GRegChanged==1)	//区文件被修改
	{
		if(GetRegSaveAsPath(FilePathName)==0)			//判断是否打开了路径
		{
			goto ON_FILE_SAVEAS_END;
		}
		memcpy(GRegFName,FilePathName,256);
		Reg_TCF(FilePathName,10);
	}
	ON_FILE_SAVEAS_END:;
}


void CMapEditor10View::OnAppExit()
{
	// TODO: 在此添加命令处理程序代码
	CString FilePathName;
	long value;
	if(GPntChanged==0&&GLinChanged==0&&GRegChanged==0)
		{
			if(GPntTmpFOpened)
			GPntTmpFile.Close();
			if(GLinTmpFOpened)
			{
				GLinTmpFile1.Close();
				GLinTmpFile2.Close();
			}
			if(GRegTmpFOpened)
			{
				GRegTmpFile1.Close();
				GRegTmpFile2.Close();
			}
			exit(0);
		}
	else
	{
		if(GPntChanged==1)
		{
			value=GetPntExitPath(FilePathName);
			if(value==0)
				goto ON_APPEXIT_END;
			if(value==2)
			{
				if(GPntTmpFOpened)
				GPntTmpFile.Close();

				exit(0);
			}
			memcpy(GPntFName,FilePathName,256);
			Pnt_TCF(FilePathName,10);
			if(GPntTmpFOpened)
			GPntTmpFile.Close();
			exit(0);
		}
		else if(GLinChanged==1)
		{
			value=GetLinExitPath(FilePathName);
			if(value==0)
				goto ON_APPEXIT_END;
			if(value==2)
			{
				if(GLinTmpFOpened)
				{
					GLinTmpFile1.Close();
					GLinTmpFile2.Close();
				}
					exit(0);
			}
			memcpy(GLinFName,FilePathName,256);
			Lin_TCF(FilePathName,10);
			if(GLinTmpFOpened)
			{
				GLinTmpFile1.Close();
				GLinTmpFile2.Close();
			}
			exit(0);
		}
		else if(GRegChanged==1)
		{
			value=GetRegExitPath(FilePathName);
			if(value==0)
				goto ON_APPEXIT_END;
			if(value==2)
			{
				if(GRegTmpFOpened)
				{	
					GRegTmpFile1.Close();
					GRegTmpFile2.Close();
				}
					exit(0);
			}
			memcpy(GRegFName,FilePathName,256);
			Reg_TCF(FilePathName,10);
			if(GRegTmpFOpened)
			{
				GRegTmpFile1.Close();
				GRegTmpFile2.Close();
			}
			exit(0);
		}
	}
	ON_APPEXIT_END:;
}


void CMapEditor10View::ZoomIn()
{
	// TODO: 在此添加命令处理程序代码
	GCurOperState=OPERSTATE_ZOOM_IN;  //设置当前状态为放大
}


void CMapEditor10View::ZoomOut()
{
	// TODO: 在此添加命令处理程序代码
	GCurOperState=OPERSTATE_ZOOM_OUT;  //设置当前状态为缩小
}

long CMapEditor10View::ZoomInLBU(POINT myPoint)
{
	if(GPntTmpFOpened==0&&GRegTmpFOpened==0)
		return (0);
	CRect rect;							//定义一个矩形
	double yp;							//计算y方向比例
	double xp;							//计算x方向比例
	long len;
	int i,j;
	double zoom;
	double xOffset;								//保存相对于窗口坐标系的x偏移与y偏移
	double yOffset;	
	CPoint temp;
	CClientDC dc(this);
	GetClientRect(&rect);								//获取客户区矩形
	dc.FillRect(&rect,new CBrush(RGB(255,255,255)));			//空白覆盖客户区
	if(zoomstyle==0)
	{
		xOffset=myPoint.x-(rect.right-rect.left)/2;		//点位置与客户区中心点位置计算偏移
		yOffset=myPoint.y-(rect.bottom-rect.top)/2;
		GxOffset+=xOffset/GZoom;
		GyOffset+=yOffset/GZoom;
		GZoom*=1.2;
		Invalidate();
	}
	else 
	{
		yp=double(rect.bottom-rect.top)/double(myPoint.y-m_startPnt.y);		//矩形左上点与客户区左上点计算偏移
		xp=double(rect.right-rect.left)/double(myPoint.x-m_startPnt.x);
		zoom=((xp>yp)?yp:xp);
		xOffset=m_startPnt.x;
		yOffset=m_startPnt.y;
		GxOffset+=xOffset/GZoom;
		GyOffset+=yOffset/GZoom;
		GZoom*=zoom;
		Invalidate();
	}
		m_step = 0;
		m_startPnt.x = 0;
		m_startPnt.y = 0;
		m_lastPnt.x = 0;
		m_lastPnt.y = 0;

		return (1);
}
long CMapEditor10View::ZoomInMM(POINT myPoint)
{
	if(GPntTmpFOpened==0&&GRegTmpFOpened==0)
		return (0);
	CClientDC dc(this);
	if(m_step!=0)
	{
		dc.SetROP2(R2_NOTXORPEN);
		if (m_lastPnt.x!=0 && m_lastPnt.y!=0)
			dc.Rectangle(m_startPnt.x, m_startPnt.y, m_lastPnt.x, m_lastPnt.y);

		dc.Rectangle(m_startPnt.x, m_startPnt.y, myPoint.x, myPoint.y);
		m_lastPnt = myPoint;
		zoomstyle=1;
	}

	return (1);
}
long CMapEditor10View::ZoomInLBD(POINT myPoint)
{
	if(GPntTmpFOpened==0&&GRegTmpFOpened==0)
		return (0);
	if(m_step!=0)
		{
			
		}
	else
		{
			m_step++;
			m_startPnt = myPoint;
		}

	return (1);
}
long CMapEditor10View::ZoomOutLBU(POINT myPoint)
{
	if(GPntTmpFOpened==0&&GRegTmpFOpened==0)
		return (0);
	CRect rect;							//定义一个矩形
	long len;
	int i,j;
	double xOffset;								//保存相对于窗口坐标系的x偏移与y偏移
	double yOffset;
	CPoint temp;
	CClientDC dc(this);
	GetClientRect(&rect);
	dc.FillRect(&rect,new CBrush(RGB(255,255,255)));
	xOffset=myPoint.x-(rect.right-rect.left)/2;		//点位置与客户区中心点位置计算偏移
	yOffset=myPoint.y-(rect.bottom-rect.top)/2;
	GxOffset+=xOffset/GZoom;
	GyOffset+=yOffset/GZoom;
	GZoom*=0.8;
	Invalidate();
	return (1);
}
void CMapEditor10View::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	
	switch(GCurOperState)
	{
	case OPERSTATE_INPUT_PNT:						//造点
		AppendPnt(point,pntColor);					//把新加的点写入临时文件
		DrawPoint(point,pntColor);					//画出当前的点
		break;	
	case OPERSTATE_DEL_PNT:							//删除点
		DeletePntLBU(point);
		break;
	case OPERSTATE_MOV_PNT:							//移动点
		MovePntLBU(point);							//文件中移动点   (移动到的点，要移动的点)
		Invalidate();								//刷新图像	
		break;
	case OPERSTATE_RECOVER_PNT:						//恢复点
		RecoverPnt(point);
		ErasePoint(point);
		break;
	case OPERSTATE_ZOOM_IN:			//设置操作状态为放大
		ZoomInLBU(point);
		break;
	case OPERSTATE_ZOOM_OUT:			//设置操作状态为缩小
		ZoomOutLBU(point);
		break;
	case OPERSTATE_CREAT_REG:			//设置操作状态为造区
		CreatRegLBU(point);
		break;
	case OPERSTATE_DELETE_REG:			//设置操作状态为删除区
		DeleteRegLBU(point);
		break;
	case OPERSTATE_MOVE_REG:			//设置操作状态为移动区
		MoveRegLBU(point);
		break;
	case OPERSTATE_WINDOW_MOVE:			//设置操作状态为窗口移动
		WindowMoveLBU(point);
		break;
	case OPERSTATE_RECOVER_REG:			//设置操作状态为恢复区
		RecoverDeletedRegLBU(point);
		break;
	case OPERSTATE_AMMENDPAR_REG:			//设置操作状态为修改区参数
		AmendRegPARLBU(point);
		break;
	case OPERSTATE_INPUT_LIN:						 //造线
			CreateLinLBU(point);
			break;
		case OPERSTATE_DEL_LIN:							//删除线
			DeleteLinLBU(point);
			break;
		case OPERSTATE_MOVE_LIN:								 //移动线
			MoveLinLBU(point);
			Invalidate();										//刷新图像
			break;		
		case OPERSTATE_CONNECT_LIN:								//连接线
			ConLinLBU(point);
			break;
		
		case OPERSTATE_RECOVER_LIN:								//恢复线
			RecoverLinLBU(point);
			break;
		default:

			break;
	}
	GLinChanged=1;
	CView::OnLButtonUp(nFlags, point);
}


void CMapEditor10View::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	switch(GCurOperState)
	{
	case OPERSTATE_ZOOM_IN:				//设置操作状态为放大
		ZoomInLBD(point);
		break;
	case OPERSTATE_MOV_PNT:				//移动点
		MovePntLBD(point);
		break;
	case OPERSTATE_MOVE_REG:			//设置操作状态为移动区
		MoveRegLBD(point);
		break;
	case OPERSTATE_WINDOW_MOVE:			//设置操作状态为窗口移动
		WindowMoveLBD(point);
		break;
	case OPERSTATE_MOVE_LIN:						 //设置操作状态为移动线
		MoveLinLBD(point);
		break;
	
	//default:GPntChanged=1;
	//	break;
			
	}

	CView::OnLButtonDown(nFlags, point);
}


void CMapEditor10View::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	switch(GCurOperState)
	{
	case OPERSTATE_ZOOM_IN:			//设置操作状态为放大
		ZoomInMM(point);
		break;
	case OPERSTATE_MOV_PNT:							//移动点
		MovePoint(point);
		break;
	case OPERSTATE_CREAT_REG:
		CreatRegMM(point);
		break;
	case OPERSTATE_MOVE_REG:			//设置操作状态为移动区
		MoveRegMM(point);
		break;
	case OPERSTATE_WINDOW_MOVE:			//设置操作状态为窗口移动
		WindowMoveMM(point);
		break;
	case OPERSTATE_MOVE_LIN:						  //移动线
		if(nFlags==MK_LBUTTON) MoveLin(point);
		break;	
	default:
		break;
	}	

	CView::OnMouseMove(nFlags, point);
}


long CMapEditor10View::CreatRegLBU(POINT xy)
{
	CClientDC	dc(this);
	CPen pen(PS_SOLID,1,regColor);
	if(!GRegTmpFOpened)
		return (0);
	dc.SelectObject(&pen);
	if (GIsMakingReg!=0)
	{
		dc.MoveTo (m_lastPnt);
		dc.LineTo (xy);
	}
	GIsMakingReg=1;
	m_startPnt = xy;
	regPnt[RegPntIdx++]=xy;
	regPntNum++;
	return (1);
}
long CMapEditor10View::CreatRegMM(POINT xy)
{
	CClientDC dc(this);
	CPen pen(PS_SOLID,1,regColor);
	dc.SelectObject(&pen);
	if (GIsMakingReg!=0)
	{
		dc.SetROP2(R2_NOTXORPEN);
		if (m_lastPnt.x!=0 && m_lastPnt.y!=0)
		{
			dc.MoveTo(m_startPnt);
			dc.LineTo(m_lastPnt);
			dc.MoveTo(regPnt[0]);
			dc.LineTo(m_lastPnt);
		}
		
		dc.MoveTo(m_startPnt);
		dc.LineTo(xy);
		dc.MoveTo(regPnt[0]);
		dc.LineTo(xy);
		m_lastPnt = xy;
	}
	return (1);
}
long CMapEditor10View::CreatRegRBU(POINT xy)
{
	int PntSize;
	int i;
	CClientDC dc(this);
	if(!GRegTmpFOpened)
		return (0);
	//1.画区
	CPen pen(0,1,regColor);
	dc.SelectObject(&pen);
	dc.SetROP2(R2_NOTXORPEN);
	dc.MoveTo(regPnt[0]);
	dc.LineTo(m_lastPnt);
	dc.MoveTo(m_startPnt);
	dc.LineTo(m_lastPnt);						//删掉留下的线
	PntSize=regPntNum*sizeof(D_DOT);			//该区所有点的大小
	dc.MoveTo (m_startPnt);
	dc.LineTo (regPnt[0]);
	CBrush brush(regColor);
	dc.SelectObject (&brush);
	dc.Polygon(regPnt,regPntNum);
	//2.在临时文件中存储
	if(regPntNum>2)
	{
		for(i=0;i<regPntNum;i++)
		{
			regPntdat[i].x=regPnt[i].x;
			regPntdat[i].y=regPnt[i].y;
		}
		regDatOff=GRegTmpFile2.GetLength();
		SomeDotVPtoDP(regPntdat,regPntNum);
		AppendAReg(regColor,regPntNum,regDatOff,regPntdat);
		GRegNum++;
		GRegLNum++;
		GRegChanged=1;
	}
	GIsMakingReg=0;
	RegPntIdx=0;
	m_startPnt.x=0;
	m_startPnt.y=0;
	m_lastPnt.x=0;
	m_lastPnt.y=0;
	regPntNum=0;
	
	return (1);

}

void CMapEditor10View::CreatReg()
{
	// TODO: 在此添加命令处理程序代码
	GCurOperState=OPERSTATE_CREAT_REG;			//设置当前状态为造区
	GetRegColor(regColor);
	GisShowDeleted=0;
	isShowPnt=1;
	isShowLin=1;
	isShowReg=1;
}
long CMapEditor10View::EraseAReg(COLORREF regColor,int RegPntNum,POINT *ptRegPnt)
{
	CClientDC dc(this);
	int i;
	CPen pen(PS_SOLID,1,regColor);
	dc.SelectObject(&pen);
	CBrush brush(regColor);
	dc.SelectObject (&brush);
	dc.SetROP2(R2_NOTXORPEN);
	DrawAReg(ptRegPnt,RegPntNum,&dc);
	return (1);
}
long CMapEditor10View::DeleteRegLBU(POINT xy)
{
	int i,j;
	for(i=1;i<=GRegNum;i++)
	{
		GetAReg(i,regNdx,regPntdat,500);
		SomeDotDPtoVP(regPntdat,regNdx.dotNum);
		for(j=0;j<regNdx.dotNum;j++)
		{
			regPnt[j].x=regPntdat[j].x;
			regPnt[j].y=regPntdat[j].y;
		}
		if(regNdx.isDel==GisShowDeleted)
		{
			if(IsInReg(regPnt,regNdx.dotNum,xy)==1)
			{
				EraseAReg(regNdx.color,regNdx.dotNum,regPnt);
				DeleteAReg(i);
				GRegLNum--;
				return (1);
			}
		}
	}
}

void CMapEditor10View::DeleteReg()
{
	// TODO: 在此添加命令处理程序代码
	GCurOperState=OPERSTATE_DELETE_REG;
	GisShowDeleted=0;
	isShowPnt=1;
	isShowLin=1;
	isShowReg=1;
}
long CMapEditor10View::DrawRegFile()
{
	CClientDC dc(this);
	int i,j;
	for(i=1;i<=GRegNum;i++)
	{
		if(GetAReg(i,regNdx,regPntdat,500)==1)
		{
			if(regNdx.isDel==GisShowDeleted)
			{
				CPen pen(PS_SOLID,1,regNdx.color);
				dc.SelectObject(&pen);
				CBrush brush(regNdx.color);
				dc.SelectObject (&brush);
				dc.SetROP2(R2_NOTXORPEN);
				SomeDotDPtoVP(regPntdat,regNdx.dotNum);
				for(j=0;j<regNdx.dotNum;j++)
				{
					regPnt[j].x=regPntdat[j].x;
					regPnt[j].y=regPntdat[j].y;
				}
				DrawAReg(regPnt,regNdx.dotNum,&dc);
			}
		}
	}
	return (1);
}
long CMapEditor10View::MoveRegLBD(POINT xy)
{
	int i,j;
	for(i=1;i<=GRegNum;i++)
	{
		if(GetAReg(i,regNdx,regPntdat,500)==1)
		{
			if(regNdx.isDel==GisShowDeleted)
			{
				SomeDotDPtoVP(regPntdat,regNdx.dotNum);
				for(j=0;j<regNdx.dotNum;j++)
				{
					regPnt[j].x=regPntdat[j].x;
					regPnt[j].y=regPntdat[j].y;
				}
				if(IsInReg(regPnt,regNdx.dotNum,xy)==1)
				{
					regColor=regNdx.color;
					regPntNum=regNdx.dotNum;
					regDatOff=regNdx.datOff;
					m_step++;
					m_startPnt=xy;
					return (1);
				}
			}
		}
	}
	return (0);
}
long CMapEditor10View::MoveRegMM(POINT xy)
{
	CClientDC dc(this);
	int i;
	CPen pen(PS_SOLID,1,regColor);
	dc.SelectObject(&pen);
	dc.SetROP2(R2_NOTXORPEN);
	if(m_step!=0)
	{
		if (m_lastPnt.x!=0 && m_lastPnt.y!=0)
		{
			POINT *lastRegPnt=new POINT[regPntNum];
			for(i=0;i<regPntNum;i++)
			{
				lastRegPnt[i].x=regPnt[i].x+m_lastPnt.x-m_startPnt.x;
				lastRegPnt[i].y=regPnt[i].y+m_lastPnt.y-m_startPnt.y;
			}
			DrawAHollowReg(lastRegPnt,regPntNum,&dc);
			delete []lastRegPnt;
		}
			POINT *newRegPnt=new POINT[regPntNum];
			for(i=0;i<regPntNum;i++)
			{
				newRegPnt[i].x=regPnt[i].x+xy.x-m_startPnt.x;
				newRegPnt[i].y=regPnt[i].y+xy.y-m_startPnt.y;
			}
			DrawAHollowReg(newRegPnt,regPntNum,&dc);
			delete []newRegPnt;
			m_lastPnt=xy;
			m_step++;
	}

	return (1);
}
long CMapEditor10View::MoveRegLBU(POINT xy)
{
	CClientDC dc(this);
	int i;
	POINT newRegPnt[500];
	d_dot newRegPntdat[500];
	CPen pen(PS_SOLID,1,regColor);
	dc.SelectObject(&pen);
	CBrush brush(regColor);
	dc.SelectObject (&brush);
	dc.SetROP2(R2_NOTXORPEN);
	if(m_step==0||m_step==1)						//当未选中区或未移动时
	{
		m_step = 0;
		m_startPnt.x = 0;
		m_startPnt.y = 0;
		m_lastPnt.x = 0;
		m_lastPnt.y = 0;
		return (0);
	}
	DrawAReg(regPnt,regPntNum,&dc);	//擦掉最开始的区
	for(i=0;i<regPntNum;i++)			//改变坐标数据
	{
		newRegPnt[i].x=regPnt[i].x+xy.x-m_startPnt.x;
		newRegPnt[i].y=regPnt[i].y+xy.y-m_startPnt.y;
	}
	for(i=0;i<regPntNum;i++)			//改变坐标数据
	{
		newRegPntdat[i].x=regPntdat[i].x+xy.x-m_startPnt.x;
		newRegPntdat[i].y=regPntdat[i].y+xy.y-m_startPnt.y;
	}
	dc.Polygon(newRegPnt,regPntNum);
	//写入临时文件2
	GRegTmpFile2.Seek(regDatOff,CFile::begin);
	SomeDotVPtoDP(newRegPntdat,regPntNum);
	GRegTmpFile2.Write(newRegPntdat,regPntNum*sizeof(D_DOT));
	m_step = 0;
	m_startPnt.x = 0;
	m_startPnt.y = 0;
	m_lastPnt.x = 0;
	m_lastPnt.y = 0;
	return (1);

}

void CMapEditor10View::MoveReg()
{
	// TODO: 在此添加命令处理程序代码
	GCurOperState=OPERSTATE_MOVE_REG;
	GisShowDeleted=0;
	isShowPnt=1;
	isShowLin=1;
	isShowReg=1;
}

long CMapEditor10View::WindowMoveLBD(POINT xy)
{
	m_startPnt=xy;
	slectedPnt=xy;
	return (1);
}
long CMapEditor10View::WindowMoveMM(POINT xy)
{
	
	CRect rect;
	CDC *pdc;
	pdc=this->GetDC();
	GetClientRect(&rect);
	if (m_startPnt.x!=0 && m_startPnt.y!=0)
	{
		pdc->BitBlt(xy.x-slectedPnt.x,xy.y-slectedPnt.y,rect.Width(),rect.Height(),pdc,0,0,SRCCOPY);
		slectedPnt=xy;
	}
	return (1);
}
long CMapEditor10View::WindowMoveLBU(POINT xy)
{
	int i,j;
	d_dot temp;
	CRect rect;
	CClientDC dc(this);
	double xOffset;
	double yOffset;
	xOffset=m_startPnt.x-xy.x;							//获得x方向上的偏移
	yOffset=m_startPnt.y-xy.y;							//获得y方向上的偏移
	GxOffset+=xOffset/GZoom;
	GyOffset+=yOffset/GZoom;
	GetClientRect(&rect);								//获取客户区矩形
	dc.FillRect(&rect,new CBrush(RGB(255,255,255)));			//空白覆盖客户区
	dc.SetROP2(R2_NOTXORPEN);
	if(GPntTmpFOpened==1&&GPntNum!=0)
	{
		GPntTmpFile.Seek(0,CFile::begin);
		for(i=0;i<GPntNum;i++)   //遍历所有的点
			{
				GPntTmpFile.Read(&PNT_STRU, sizeof( PNT_STRU ));
				if(PNT_STRU.isDel==GisShowDeleted)            //画出没被删掉的点
				{
				temp.x=PNT_STRU.x;
				temp.y=PNT_STRU.y;	
				DotDPtoVP(temp);				//转换坐标
				CPen  newPen(PS_SOLID,2,RGB(192,192,192));
				dc.MoveTo(temp.x,temp.y-6);
				dc.LineTo(temp.x,temp.y+6);
				dc.MoveTo(temp.x-6,temp.y);
				dc.LineTo(temp.x+6,temp.y);

				}
			
			}
	}
	if(GLinTmpFOpened==1&&GLinNum!=0)
	{}
	if(GRegTmpFOpened==1&&GRegNum!=0)
	{
		for(i=1;i<=GRegNum;i++)
		{
			if(GetAReg(i,regNdx,regPntdat,500)==1)				//得到一个区
			{
				if(regNdx.isDel==GisShowDeleted)							//如果该区没被删除
				{
					CPen pen(PS_SOLID,1,regNdx.color);
					dc.SelectObject(&pen);
					CBrush brush(regNdx.color);
					dc.SelectObject (&brush);
					SomeDotDPtoVP(regPntdat,regNdx.dotNum);	//转换坐标
					for(j=0;j<regNdx.dotNum;j++)
					{
						regPnt[j].x=regPntdat[j].x;
						regPnt[j].y=regPntdat[j].y;
					}
					DrawAReg(regPnt,regNdx.dotNum,&dc);
				}
			}
		}
		
	}
	m_startPnt.x = 0;
	m_startPnt.y = 0;
	slectedPnt.x=0;
	slectedPnt.y=0;
	return (1);
}

void CMapEditor10View::WindowMove()
{
	// TODO: 在此添加命令处理程序代码
	GCurOperState=OPERSTATE_WINDOW_MOVE;
	isShowPnt=1;
	isShowLin=1;
	isShowReg=1;
}


void CMapEditor10View::WindowReset()
{
	// TODO: 在此添加命令处理程序代码
	CRect rect;
	CRect rt;							//定义一个矩形获得填白客户区
	double yp;							//计算y方向比例
	double xp;							//计算x方向比例
	int i,j;
	double zoom;
	double xOffset;								//保存相对于窗口坐标系的x偏移与y偏移
	double yOffset;	
	d_dot temp;
	d_dot topLeft;
	d_dot bottomRight;
	CClientDC dc(this);
	GetMBR(rect);
	topLeft.x=rect.TopLeft().x;
	topLeft.y=rect.TopLeft().y;
	bottomRight.x=rect.BottomRight().x;
	bottomRight.y=rect.BottomRight().y;
	DotDPtoVP(topLeft);
	DotDPtoVP(bottomRight);
	GetClientRect(&rt);								//获取客户区矩形
	dc.FillRect(&rt,new CBrush(RGB(255,255,255)));			//空白覆盖客户区
	yp=double(rt.bottom-rt.top)/double(bottomRight.y-topLeft.y);		//矩形左上点与客户区左上点计算偏移
	xp=double(rt.right-rt.left)/double(bottomRight.x-topLeft.x);
	zoom=((xp>yp)?yp:xp);							//选择比例小的系数显示完全
	xOffset=topLeft.x;
	yOffset=topLeft.y;
	GxOffset+=xOffset/GZoom;
	GyOffset+=yOffset/GZoom;
	GZoom*=zoom;
	if(GPntTmpFOpened==1)
	{
		
		GPntTmpFile.Seek(0,CFile::begin);
		for(i=0;i<GPntNum;i++)   //遍历所有的点,找最近的点
			{
				GPntTmpFile.Read(&PNT_STRU, sizeof( PNT_STRU ));
				if(PNT_STRU.isDel==GisShowDeleted)            //画出没被删掉的点
				{
				temp.x=PNT_STRU.x;
				temp.y=PNT_STRU.y;	
				DotDPtoVP(temp);
				CPen  newPen(PS_SOLID,2,RGB(192,192,192));
				dc.MoveTo(temp.x,temp.y-6);
				dc.LineTo(temp.x,temp.y+6);
				dc.MoveTo(temp.x-6,temp.y);
				dc.LineTo(temp.x+6,temp.y);

				}
			
			}
	}
	else if(GRegTmpFOpened==1)
	{
		for(i=1;i<=GRegNum;i++)
		{
			if(GetAReg(i,regNdx,regPntdat,500)==1)
			{
				if(regNdx.isDel==GisShowDeleted)
				{
					CPen pen(PS_SOLID,1,regNdx.color);
					dc.SelectObject(&pen);
					CBrush brush(regNdx.color);
					dc.SelectObject (&brush);
					dc.SetROP2(R2_NOTXORPEN);
					SomeDotDPtoVP(regPntdat,regNdx.dotNum);
					for(j=0;j<regNdx.dotNum;j++)
					{
						regPnt[j].x=regPntdat[j].x;
						regPnt[j].y=regPntdat[j].y;
					}
					DrawAReg(regPnt,regNdx.dotNum,&dc);
				}
			}
		}
	}

}


void CMapEditor10View::ShowReg()
{
	// TODO: 在此添加命令处理程序代码
	CRect rt;
	CClientDC dc(this);
	GCurOperState=0;
	GisShowDeleted=0;
	isShowPnt=0;
	isShowLin=0;
	isShowReg=1;
	GetClientRect(&rt);								//获取客户区矩形
	dc.FillRect(&rt,new CBrush(RGB(255,255,255)));			//空白覆盖客户区
	DrawRegFile();
}




void CMapEditor10View::ShowDeletedReg()
{
	// TODO: 在此添加命令处理程序代码
	CRect rt;
	CClientDC dc(this);
	GCurOperState=0;
	GisShowDeleted=1;
	isShowPnt=0;
	isShowLin=0;
	isShowReg=1;
	GetClientRect(&rt);								//获取客户区矩形
	dc.FillRect(&rt,new CBrush(RGB(255,255,255)));			//空白覆盖客户区
	DrawRegFile();
}

long CMapEditor10View::RecoverDeletedRegLBU(POINT xy)
{
	int i,j;
	for(i=1;i<=GRegNum;i++)
	{
		GetAReg(i,regNdx,regPntdat,500);
		SomeDotDPtoVP(regPntdat,regNdx.dotNum);
		for(j=0;j<regNdx.dotNum;j++)
		{
			regPnt[j].x=regPntdat[j].x;
			regPnt[j].y=regPntdat[j].y;
		}
		if(regNdx.isDel==GisShowDeleted)
		{
			if(IsInReg(regPnt,regNdx.dotNum,xy)==1)
			{
				EraseAReg(regNdx.color,regNdx.dotNum,regPnt);
				RecoverReg(i);
				GRegLNum++;
				return (1);
			}
		}
	}
	return (0);
}

void CMapEditor10View::RecoverAReg()
{
	// TODO: 在此添加命令处理程序代码
	GisShowDeleted=1;
	GCurOperState=OPERSTATE_RECOVER_REG;
	isShowPnt=0;
	isShowLin=0;
	isShowReg=1;
}
long CMapEditor10View::AmendRegPARLBU(POINT xy)
{
	int i,j;
	CClientDC dc(this);
	for(i=1;i<=GRegNum;i++)
	{
		GetAReg(i,regNdx,regPntdat,500);
		SomeDotDPtoVP(regPntdat,regNdx.dotNum);
		for(j=0;j<regNdx.dotNum;j++)
		{
			regPnt[j].x=regPntdat[j].x;
			regPnt[j].y=regPntdat[j].y;
		}
		if(regNdx.isDel==GisShowDeleted)
		{
			if(IsInReg(regPnt,regNdx.dotNum,xy)==1)
			{
				GetRegColor(regColor);
				CPen pen(PS_SOLID,1,regColor);
				dc.SelectObject(&pen);
				CBrush brush(regColor);
				dc.SelectObject (&brush);
				dc.SetROP2(R2_NOTXORPEN);
				EraseAReg(regNdx.color,regNdx.dotNum,regPnt);
				DrawAReg(regPnt,regNdx.dotNum,&dc);
				AmendRegPAR(regColor,regNdx,i);
				return (1);
			}
		}
	}
	return (0);
}

void CMapEditor10View::AmmendARegPAR()
{
	// TODO: 在此添加命令处理程序代码
	GisShowDeleted=0;
	GCurOperState=OPERSTATE_AMMENDPAR_REG;
	isShowPnt=1;
	isShowLin=1;
	isShowReg=1;
}


void CMapEditor10View::AmmendDefultPAR()
{
	// TODO: 在此添加命令处理程序代码
	GCurOperState=0;
	SetDefultColor();
}
//函数功能：点画图函数实现
//参数：点坐标
	long CMapEditor10View::DrawPoint(CPoint tempxy,COLORREF color)       //图像上画点
	{
	
		//获得本窗口或当前活动视图
		if(!GPntTmpFOpened)
		return (0);
		CClientDC dc(this);
		CPen  newPen;
		CPen *oldPen;
		newPen.CreatePen(PS_SOLID,2,color);
		oldPen = dc.SelectObject(&newPen);
		dc.MoveTo(tempxy.x,tempxy.y-6);
		dc.LineTo(tempxy.x,tempxy.y+6);
		dc.MoveTo(tempxy.x-6,tempxy.y);
		dc.LineTo(tempxy.x+6,tempxy.y);
		dc.SelectObject(oldPen);
		GPntChanged=1;
		::ReleaseDC(m_hWnd,dc);
		return (1);
	
	}
	//函数功能：图像上擦除点
	//参数：点坐标
	long CMapEditor10View::ErasePoint(CPoint tempxy)		 
	{

		CPoint toErase;
		FindNearestPnt(tempxy) ;
		toErase=tempxy;									 //保存待删除点
		CPen newpen(PS_SOLID,2,RGB(255,255,255));		//构造画笔对象
		//获得本窗口或当前活动视图
		CClientDC dc(this);
		CPen *pOldPen=dc.SelectObject(&newpen);			//将画笔选入DC，SelectObject函数返回先前画笔
		//设置异或模式画点
		//这里抹去一个小十字
		dc.MoveTo(toErase.x,toErase.y-6);
		dc.LineTo(toErase.x,toErase.y+6);
		dc.MoveTo(toErase.x-6,toErase.y);
		dc.LineTo(toErase.x+6,toErase.y);
		GPntChanged=1;
		::ReleaseDC(m_hWnd,dc);
		return 1;

	}
	//函数功能：移动点
	//参数：
	long CMapEditor10View::MovePntLBD(CPoint point)       
	{
		nowPoint=point;	 //存储当前点为前一点
		CPoint ptMove;
		FindNearestPnt(point);
		m_step++;
		ptMove=point;
		toMove=point;
		CPen newpen(PS_SOLID,2,RGB(255,0,0));	 //构造画笔对象
		CClientDC dc(this);	 //获得本窗口或当前活动视图
		CPen *pOldPen=dc.SelectObject(&newpen);	 //将画笔选入DC，SelectObject函数返回先前画笔
		//这里标亮要移动的点
		dc.MoveTo(ptMove.x,ptMove.y-6);
		dc.LineTo(ptMove.x,ptMove.y+6);
		dc.MoveTo(ptMove.x-6,ptMove.y);
		dc.LineTo(ptMove.x+6,ptMove.y);
		::ReleaseDC(m_hWnd,dc);

		return 1;

	}

	//函数功能：删除点
	//参数：
	long CMapEditor10View::DeletePntLBU(CPoint point)       
	{
		int pnum=0;
		CPoint ptemp;
		FindNearestPnt(point,pnum);
		ptemp.x=point.x;
		ptemp.y=point.y;

		if(DeletePnt(pnum)==1);								//文件中删除点
		{
			ErasePoint(point);							//图像中删除点
		}

		return 1;

	}

	//函数功能：移动点
	//参数：
	long CMapEditor10View::MovePntLBU(CPoint point)       
	{

		m_step=0;
		int pnum=0;
		FindNearestPnt(toMove,pnum);
		if(MovePnt(point,pnum)==1)
			return 1;
		else
			return 0;

	}


	//函数功能：图像上移动点
	//参数：当前点坐标
	long CMapEditor10View::MovePoint(CPoint tempxy)    
	{
		if(m_step==0)
			return (0);
		CPen newpen(PS_SOLID,2,RGB(255,255,255));//构造画笔对象
		//获得本窗口或当前活动视图
		CClientDC dc(this);
		CPen *pOldPen=dc.SelectObject(&newpen);//将画笔选入DC，SelectObject函数返回先前画笔
		//设置异或模式画点
		//dc.SetROP2(R2_NOT);	
		//这里抹去一个小十字
		dc.MoveTo(nowPoint.x,nowPoint.y-6);
		dc.LineTo(nowPoint.x,nowPoint.y+6);
		dc.MoveTo(nowPoint.x-6,nowPoint.y);
		dc.LineTo(nowPoint.x+6,nowPoint.y);
		CPen oldpen(PS_SOLID,2,RGB(192,192,192));	//构造画笔对象
		dc.SelectObject(&oldpen);					//将画笔选入DC，SelectObject函数返回先前画笔
		//这里画一个小十字
		dc.MoveTo(tempxy.x,tempxy.y-6);
		dc.LineTo(tempxy.x,tempxy.y+6);
		dc.MoveTo(tempxy.x-6,tempxy.y);
		dc.LineTo(tempxy.x+6,tempxy.y);
		nowPoint=tempxy;							//把当前点存为上一点
		GPntChanged=1;
		::ReleaseDC(m_hWnd,dc);
		return (1);

	}


	//函数功能：移动线
	//参数：
	long CMapEditor10View::MoveLinLBD(CPoint point)       
	{
		int num;
		startPnt=point;	 //存储当前点为前一点
		FindNearLine(point,num);
		SelectLin(num);
		EraseLine(num);
		linToMove=num; //保存要移动的线号
		step=1;
		return 1;

	}
	//函数功能：造线
	//参数：
	long CMapEditor10View::CreateLinLBU(CPoint point)       
	{
		MakingLine=1;	 // 设置正在造点状态
		nowPoint=point;	 //存储前一个节点
		linPnt[LinPntNum]=point;
	
		DrawLine(point); //画出当前的点
		LinPntNum++;     //线上点数自增

		return 1;
	}
	//函数功能：删线
	//参数：
	long CMapEditor10View::DeleteLinLBU(CPoint point)       
	{

		CString str;
		int LineNum;
		FindNearLine(point,LineNum);
		EraseLine(LineNum);	 //图像中删除编号LineNum的线
		DelLine(LineNum);
		return 1;
	}
	//函数功能：连接线
	//参数：
	long CMapEditor10View::ConLinLBU(CPoint point)       
	{
		int firstnum;
		int secnum;
		if(isfirst==0)	 //选中第一条线
		{
			FindNearLine(point,firstnum);
			firstlinnum=firstnum;	 //保存第一条线号
			isfirst=1;
			DrawConLine(firstnum);
			/*CString sMousePos;
			CString nowstr="已经选择第一条线，请选择第二条";
			sMousePos.Format(_T(" %s "),nowstr);
			AfxGetMainWnd()->GetDescendantWindow(AFX_IDW_STATUS_BAR)->SetWindowText(sMousePos);
			*/
		}
		else if(issecond==0)
		{
			FindNearLine(point,secnum);
			if(firstlinnum!=secnum)
			{
				seclinnum=secnum;	 //保存第二条线号
				issecond=1;
				DrawConLine(secnum);
			}
		}
		else if(firstlinnum!=0 &&seclinnum!=0 ) ConnectLins(firstlinnum,seclinnum); //连接两条线

		return 1;
	}
	//函数功能：造线
	//参数：
	long CMapEditor10View::CreateLinMM(CPoint point)       
	{
		if(MakingLine==1) {
			DrawPoint(point,pntColor);
			CPen newpen(PS_SOLID,2,RGB(255,0,0));			//构造画笔对象
			CPen oldpen(PS_SOLID,2,RGB(255,255,255));		//构造画笔对象

			CClientDC dc(this);								//获得本窗口或当前活动视图
			CPen *pOldPen=dc.SelectObject(&newpen);			//将画笔选入DC，SelectObject函数返回先前画笔
			//这里标亮要移动的点
			dc.MoveTo(lastPoint.x,lastPoint.y);
			dc.LineTo(point.x,point.y);

			dc.SelectObject(&oldpen);						//将画笔选入DC，SelectObject函数返回先前画笔
			//这里标亮要移动的点
			dc.MoveTo(lastPoint.x,lastPoint.y);
			dc.LineTo(templastPoint.x,templastPoint.y);
			//这里删除上一个点
			dc.MoveTo(templastPoint.x,templastPoint.y-6);
			dc.LineTo(templastPoint.x,templastPoint.y+6);
			dc.MoveTo(templastPoint.x-6,templastPoint.y);
			dc.LineTo(templastPoint.x+6,templastPoint.y);

			templastPoint=point;
			::ReleaseDC(m_hWnd,dc);
		}

		return 1;
	}
	//函数功能：造线
	//参数：
	long CMapEditor10View::CreateLinRBU(CPoint point)       
	{
		AppendLin();
		LinPntNum=0;
		lastPoint.x=0;
		lastPoint.y=0;
		MakingLine=0;

		Invalidate();

		return 1;
	}

	//函数功能：恢复线
	//参数：
	long CMapEditor10View::RecoverLinLBU(CPoint point)       
	{
		
		int linnum;
		RecoverLin(point,linnum);
		EraseLine(linnum);
		Invalidate();
		return 1;
	}



	//函数功能：图像上画线
	//参数：当前点坐标
	long CMapEditor10View::DrawLine(CPoint tempxy)    
	{
		if(LinPntNum!=0)
		{
			//获得本窗口或当前活动视图
			CClientDC dc(this);
			//设置异或模式画点
			//dc.SetROP2(R2_NOT);
			DrawPoint(lastPoint,pntColor);   
			DrawPoint(tempxy,pntColor);       
			CPen  newPen(PS_SOLID,2,linColor);
			dc.MoveTo(lastPoint.x,lastPoint.y);
			dc.LineTo(tempxy.x,tempxy.y);

			::ReleaseDC(m_hWnd,dc);
		}
		else
		{
			DrawPoint(tempxy,pntColor); 
		}
		lastPoint=tempxy;
		return (1);
	}

	//函数功能：图像上擦除线
	//参数：要擦除线的编号
	long CMapEditor10View::EraseLine(long toErase)     //
	{
		int num=0;

		num=toErase;										//保存待删除线索引号
		CPen newpen(PS_SOLID,2,RGB(255,255,255));			//构造画笔对象
		CClientDC dc(this);									//获得本窗口或当前活动视图
		CPen *pOldPen=dc.SelectObject(&newpen);				//将画笔选入DC，SelectObject函数返回先前画笔
		//设置异或模式画点 //dc.SetROP2(R2_NOT);	
		//这里抹去一个一条线

		int ii=0;
		int j=0;
		int datalen=0;
		int ndxlen=0;
		int dotnum=0;
		int datff=0;
		CPoint ptemp;
		CPoint todel;
		int linlen=sizeof(LIN_NDX_STRU);
		int plen=sizeof(D_DOT);

		if(1==GLinFCreated)
		{
			datalen=GLinTmpFile2.GetLength();
			ndxlen=GLinTmpFile1.GetLength();
			//linedata.Seek(0,CFile::begin);
			GLinTmpFile1.Seek( (toErase -1) * linlen,CFile::begin);  //找到索引号为toErase的线
			GLinTmpFile1.Read(&LIN_NDX_STRU,  linlen);
			dotnum=LIN_NDX_STRU.dotNum;
			datff=LIN_NDX_STRU.datOff;
			GLinTmpFile2.Seek(datff,CFile::begin);				  //数据中找到索引号为toErase的线数据

			GLinTmpFile2.Read(&D_DOT, plen );				 //线第一个点
			ptemp.x=D_DOT.x;
			ptemp.y=D_DOT.y;
			dc.MoveTo(ptemp);
			for(j=1 ; j < dotnum ; j++)						//从第2个点开始
			{
				GLinTmpFile2.Read(&D_DOT, plen );
				todel.x=D_DOT.x;
				todel.y=D_DOT.y;
				dc.LineTo(todel);
			}

		}
		return 1;

	}

	//函数功能：文件中擦除线
	//参数：要擦除线的编号
	long CMapEditor10View::DelLine(long toErase)     
	{
		lin_ndx_stru TEM_LIN;
		int num=0;
		num=toErase;									//保存待删除线索引号
		//	CFile linendx;
		int ii=0;
		int j=0;
		int datalen=0;
		int ndxlen=0;
		int dotnum=0;
		int datff=0;
		CPoint ptemp;
		CPoint todel;
		int linlen=sizeof(LIN_NDX_STRU);
		int plen=sizeof(D_DOT);
		//修改索引文件
		if(1==GLinFCreated)
		{
			ndxlen=GLinTmpFile1.GetLength();
			GLinTmpFile1.Seek( (num -1) * linlen,CFile::begin);
			GLinTmpFile1.Read(&TEM_LIN,linlen);
			TEM_LIN.isDel= 1;
			LIN_NDX_STRU=TEM_LIN;
			GLinTmpFile1.Seek( -linlen,CFile::current);			 //找到索引号为toErase的线

			GLinTmpFile1.Write(&LIN_NDX_STRU,linlen);


		}
		return 1;
	}

	//函数功能：图像中移动线
	//参数：坐标点
	long CMapEditor10View::MoveLin(CPoint point)			 //
	{	
		int i;
		POINT pt;
		POINT lins[500];
		if(step==0)
		{
			startPnt.x=0;
			startPnt.y=0;
			return (0);
		}
		else if(step==1)
		{
			CClientDC dc(this);							//获得本窗口或当前活动视图
			CPen newpen(PS_SOLID,2,RGB(255,255,255));	//构造画笔对象
			CPen *pOldPen=dc.SelectObject(&newpen);		//将画笔选入DC，SelectObject函数返回先前画笔

			long disX=point.x - startPnt.x;				//计算坐标偏移
			long disY=point.y - startPnt.y;

			for(i=0;i< LinPntNum;i++)
			{
				if(i==0)
					dc.MoveTo(linPnt[i]);
				else
					dc.LineTo(linPnt[i]);

				pt.x=linPnt[i].x + disX;
				pt.y=linPnt[i].y + disY;

				lins[i]=pt;
			}
			CPen oldpen(PS_SOLID,2,linColor);		//构造画笔对象
			dc.SelectObject(&oldpen);						//将画笔选入DC，SelectObject函数返回先前画笔

			for(i=0;i<LinPntNum;i++)
			{
				linPnt[i]=lins[i];
				if(i==0)
					dc.MoveTo(lins[i]);
				else
					dc.LineTo(lins[i]);
			}


			startPnt=point;
			::ReleaseDC(m_hWnd,dc);
		}
		return (1);
	}
	//函数功能：存储选定的线
	//参数：要存储的线号
	long CMapEditor10View::SelectLin(int num)     //存储选定的线点
	{	
		CPen newpen(PS_SOLID,2,RGB(255,255,255));	//构造画笔对象
		CClientDC dc(this);							//获得本窗口或当前活动视图
		CPen *pOldPen=dc.SelectObject(&newpen);		//将画笔选入DC，SelectObject函数返回先前画笔

		int ii=0;
		int j=0;
		int datalen=0;
		int ndxlen=0;
		int dotnum=0;
		int datff=0;
		CPoint ptemp;
		CPoint todel;
		int linlen=sizeof(LIN_NDX_STRU);
		int plen=sizeof(D_DOT);

		if(1==GLinFCreated)
		{
			datalen=GLinTmpFile2.GetLength();
			ndxlen=GLinTmpFile1.GetLength();

			GLinTmpFile1.Seek( (num -1) * linlen,CFile::begin);  //找到索引号为num的线
			GLinTmpFile1.Read(&LIN_NDX_STRU,  linlen);
			dotnum=LIN_NDX_STRU.dotNum;
			datff=LIN_NDX_STRU.datOff;
			GLinTmpFile2.Seek(datff,CFile::begin);           //数据中找到索引号为num的线数据

			for(j=0 ; j < dotnum ; j++)                //从第2个点开始
			{
				GLinTmpFile2.Read(&D_DOT, plen );
				todel.x=D_DOT.x;
				todel.y=D_DOT.y;
				linPnt[j]=todel;

			}

		}

		LinPntNum=dotnum;
		return 1;


	}

	//函数功能：写移动后的线
	//参数：点坐标
	long CMapEditor10View::MoveLinLBU(CPoint point)     
	{	
		//	CFile linedata;
		//	CFile linendx;
	lin_ndx_stru TEM_LIN;
		int ii=0;
		int j=0;
		int datalen=0;
		int ndxlen=0;
		int dotnum=0;
		int datff=0;
		CPoint ptemp;
		CPoint todel;
		int linlen=sizeof(LIN_NDX_STRU);
		int plen=sizeof(D_DOT);
		//修改索引文件
		if(1==GLinFCreated)
		{
			ndxlen=GLinTmpFile1.GetLength();
			GLinTmpFile1.Seek( (linToMove -1) * linlen,CFile::begin);
			GLinTmpFile1.Read(&TEM_LIN,linlen);

			dotnum=LIN_NDX_STRU.dotNum;
			datff=LIN_NDX_STRU.datOff;

			GLinTmpFile2.Seek(datff,CFile::begin);

			for(j=0; j < dotnum ; j++)
			{

				D_DOT.x=linPnt[j].x;
				D_DOT.y=linPnt[j].y;

				GLinTmpFile2.Write(&D_DOT,plen);
			}


		}

		LinPntNum=0;			//清空点临时数组
		return 1;


	}


	//函数功能：在文件中写索引和线数据
	//参数：
	long CMapEditor10View::AppendLin()  
	{
		int lendata;

		lendata=GLinTmpFile2.GetLength();
		GLinTmpFile2.Seek(lendata,CFile::begin);     //数据从文件末尾写

		GLinNum++,GLinLNum++;

		int lenndx;

		int lenofl=sizeof(LIN_NDX_STRU);        //索引结构长度
		int lenofp = sizeof(D_DOT);           //点结构长度


		lenndx=GLinTmpFile1.GetLength();
		GLinTmpFile1.Seek(lenndx,CFile::begin);     //索引从文件末尾写

		//写当前线索引
		LIN_NDX_STRU.color=linColor;
		LIN_NDX_STRU.datOff= GLinTmpFile2.GetLength();       //数据存储开始位置 一个点结构长度*点个数*线号
		LIN_NDX_STRU.dotNum=LinPntNum;
		LIN_NDX_STRU.isDel=0;
		LIN_NDX_STRU.pattern=0;
		GLinTmpFile1.Write(&LIN_NDX_STRU,lenofl);  //讲结构体写入文件

		int i=0;
		for(i=0;i < LinPntNum ; i++)
		{
			
			D_DOT.x=linPnt[i].x;
			D_DOT.y=linPnt[i].y;
			GLinTmpFile2.Write(&D_DOT,lenofp);  //数据结构体写入文件

		}

		return 1;
	}

	//函数功能：连接两条线
	//参数：线号1 2
	long CMapEditor10View::ConnectLins(int lin1,int lin2)  
	{
		int i=0;
		int lin1num;
		int lin2num;
		double disX;
		double disY;
		CPoint temp;
		POINT Lin1[500];
		POINT Lin2[500];


		linnum1=SelectLinCon(lin1,Lin1);
		linnum2=SelectLinCon(lin2,Lin2);

		if(linnum1==0 || linnum1==0)
			return 0;
		else														//开始操作
		{
			if(Lin1[linnum1].x < Lin2[0].x)						//第一条线的尾 接第二个头
			{
				for(i=0;i< linnum1;i++)
					linPnt[i]=Lin1[i];

				for(i=linnum1;i< linnum2+linnum1;i++)
				{
					linPnt[i]=Lin2[i-linnum1];
				}
			}
			else if(Lin1[linnum1].x > Lin2[0].x)				//第一个尾 接 第二个尾
			{
				for(i=0;i< linnum1;i++)
					linPnt[i]=Lin1[i];
				for(i= linnum1+linnum2;i>=linnum1;i--)
				{
					linPnt[i]=Lin2[i];
				}
			}
			else if(Lin1[0].x < Lin2[linnum2-1].x)				//第一条头 接第二个头
			{

			}
			else if(Lin1[0].x > Lin2[linnum2-1].x)			   //第一条 头 接第二个 尾
			{

			}
		}

		LinPntNum=linnum1+linnum2;
		EraseLine(lin1);									//擦除两条线
		EraseLine(lin2);
		DelLine(lin1);								     //文件中删除两条线
		DelLine(lin2);
		WriteConLine();
		DrawConLine();
		firstlinnum=0;
		seclinnum=0;
		isfirst=0;
		issecond=0;
		LinPntNum=0;
		return 1;
	}



	long CMapEditor10View::DrawConLine( )
	{
		int i=0;
		CClientDC dc(this);							//获得本窗口或当前活动视图
		CPen newpen(PS_SOLID,2,linColor);	//构造画笔对象
		CPen *pOldPen=dc.SelectObject(&newpen);		//将画笔选入DC，SelectObject函数返回先前画笔
		for(i=0;i<LinPntNum;i++)
		{
			if(i==0)
				dc.MoveTo(linPnt[i]);
			else
				dc.LineTo(linPnt[i]);
		}

		return 1;
	}

	long CMapEditor10View::WriteConLine( )
	{
		int lendata;

		lendata=GLinTmpFile2.GetLength();
		GLinTmpFile2.Seek(lendata,CFile::begin);     //数据从文件末尾写

		int lenndx;

		int lenofl=sizeof(LIN_NDX_STRU);        //索引结构长度
		int lenofp = sizeof(D_DOT);           //点结构长度

		lenndx=GLinTmpFile1.GetLength();
		GLinTmpFile1.Seek(lenndx,CFile::begin);     //索引从文件末尾写

		//写当前线索引
		LIN_NDX_STRU.color=0;
		LIN_NDX_STRU.datOff= GLinTmpFile2.GetLength();       //数据存储开始位置 一个点结构长度*点个数*线号
		LIN_NDX_STRU.dotNum=LinPntNum;
		LIN_NDX_STRU.isDel=0;
		LIN_NDX_STRU.pattern=0;
		GLinTmpFile1.Write(&LIN_NDX_STRU,lenofl);				//讲结构体写入文件

		int i=0;
		for(i=0;i < LinPntNum ; i++)
		{
		
			D_DOT.x=linPnt[i].x;
			D_DOT.y=linPnt[i].y;
			GLinTmpFile2.Write(&D_DOT,lenofp);				//数据结构体写入文件

		}

		return 1;

	}


	long CMapEditor10View::DrawConLine(int num)
	{

		int ii=0;
		int j=0;
		int datalen=0;
		int ndxlen=0;
		int dotnum=0;
		int datff=0;
		CPoint ptemp;

		int linlen=sizeof(LIN_NDX_STRU);
		int plen=sizeof(D_DOT);
		//修改索引文件
		if(1==GLinFCreated)
		{
			datalen=GLinTmpFile2.GetLength();
			ndxlen=GLinTmpFile1.GetLength();

			GLinTmpFile1.Seek( (num -1) * linlen,CFile::begin);  //找到索引号为toErase的线
			GLinTmpFile1.Read(&LIN_NDX_STRU,  linlen);
			dotnum=LIN_NDX_STRU.dotNum;
			datff=LIN_NDX_STRU.datOff;
			GLinTmpFile2.Seek(datff,CFile::begin);           //数据中找到索引号为toErase的线数据

			CClientDC dc(this);							//获得本窗口或当前活动视图
			CPen newpen(PS_SOLID,2,RGB(192,0,0));	//构造画笔对象
			CPen *pOldPen=dc.SelectObject(&newpen);		//将画笔选入DC，SelectObject函数返回先前画笔


			for(j=0; j < dotnum ; j++)
			{
				GLinTmpFile2.Read(&D_DOT,plen);
				ptemp.x=D_DOT.x;
				ptemp.y=D_DOT.y;
				if(j==0)
					dc.MoveTo(ptemp);
				else
					dc.LineTo(ptemp);
			}


		}
		return 1;	

	}

	void CMapEditor10View::OnCreatePnt()
	{
		// TODO: 在此添加命令处理程序代码
		GCurOperState=OPERSTATE_INPUT_PNT;  //设置当前状态为造点
		isShowPnt=1;
		isShowLin=1;
		isShowReg=1;
		
	}


	void CMapEditor10View::OnDeletePnt()
	{
		// TODO: 在此添加命令处理程序代码
		GCurOperState=OPERSTATE_DEL_PNT;  //设置当前状态为删除点
		isShowPnt=1;
		isShowLin=1;
		isShowReg=1;
	}


	void CMapEditor10View::OnMovePnt()
	{
		// TODO: 在此添加命令处理程序代码
		GCurOperState=OPERSTATE_MOV_PNT;  //设置当前状态为移动
		GisShowDeleted=0;
		isShowPnt=1;
		isShowLin=1;
		isShowReg=1;
	}


	void CMapEditor10View::OnShowDeletedPnt()
	{
		// TODO: 在此添加命令处理程序代码
		GCurOperState=0;
		GisShowDeleted=1;
		isShowPnt=1;
		isShowLin=0;
		isShowReg=0;
		Invalidate();
	}


	void CMapEditor10View::OnRecoverPnt()
	{
		// TODO: 在此添加命令处理程序代码
		GCurOperState=OPERSTATE_RECOVER_PNT;                         //恢复点
		GisShowDeleted=1;
		isShowPnt=1;
		isShowLin=0;
		isShowReg=0;
	}


	void CMapEditor10View::OnSetPnt()
	{
		// TODO: 在此添加命令处理程序代码
		GetPntColor(pntColor);
	}


	void CMapEditor10View::OnShowPnt()
	{
		// TODO: 在此添加命令处理程序代码
		GCurOperState=0;
		GisShowDeleted=0;
		isShowPnt=1;
		isShowLin=0;
		isShowReg=0;
		Invalidate();
	}


	void CMapEditor10View::OnCreateLin()
	{
		// TODO: 在此添加命令处理程序代码
		GCurOperState=OPERSTATE_INPUT_LIN;								  //创建线
		
	}


	void CMapEditor10View::OnMoveLin()
	{
		// TODO: 在此添加命令处理程序代码
		GCurOperState=OPERSTATE_MOVE_LIN	 ;                                //移动线
	
	}

	
	void CMapEditor10View::OnDelLin()
	{
		// TODO: 在此添加命令处理程序代码
		GCurOperState=OPERSTATE_DEL_LIN	 	 ;                           //删除线
	
	}


	void CMapEditor10View::OnConnectLin()
	{
		// TODO: 在此添加命令处理程序代码
		GCurOperState=OPERSTATE_CONNECT_LIN	 	 ;                           //连接线
	}


	void CMapEditor10View::OnSetlin()
	{
		// TODO: 在此添加命令处理程序代码									//设置线属性
			GetPntColor(linColor);
	}


	void CMapEditor10View::OnShowLin()
	{
		// TODO: 在此添加命令处理程序代码									//显示线
		GCurLINState=0;
		Invalidate();
	}


	void CMapEditor10View::OnShowDelLin()
	{
		// TODO: 在此添加命令处理程序代码									显示删除线
		GCurLINState=1;
		Invalidate();
	}


	void CMapEditor10View::OnRecoverLin()
	{
		// TODO: 在此添加命令处理程序代码									恢复线
		GCurOperState=OPERSTATE_RECOVER_LIN;  
	}



	
