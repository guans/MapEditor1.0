
// MapEditor1.0View.h : CMapEditor10View 类的接口
//

#pragma once
//#ifndef _FILESTRUCT_H
//#include "FileStruct.h"
//#define _FILESTRUCT_H 1
//#endif

class CMapEditor10View : public CView
{
protected: // 仅从序列化创建
	CMapEditor10View();
	DECLARE_DYNCREATE(CMapEditor10View)

// 特性
public:
	CMapEditor10Doc* GetDocument() const;

// 操作
public:
	short	m_step;
	POINT	m_startPnt;
	POINT	m_lastPnt;
	int zoomstyle;
	long ZoomInLBU(POINT myPoint);
	long ZoomInMM(POINT myPoint);
	long ZoomInLBD(POINT myPoint);
	long ZoomOutLBU(POINT myPoint);
//实现画图函数封装
	//显示
	int isShowPnt;
	int isShowLin;
	int isShowReg;
	 //画点函数
	long DrawPoint(CPoint tempxy,COLORREF color); 
	long MovePoint(CPoint tempxy);  //移动点函数
	long ErasePoint(CPoint tempxy); // 删除点
	long MovePntLBD(CPoint point);  //移动点按下
	long DeletePntLBU(CPoint point);//删除点弹起
	long MovePntLBU(CPoint point);//移动点弹起

	//画点数据
	CPoint nowPoint; // 标志上一坐标点
	CPoint toMove; // 需要移动点的坐标


	/////////////////////////////////////////////////画线函数和数据/////////////////////////////////////////////
	long DrawLine(CPoint tempxy);  //画线函数.
	long AppendLin();
	long EraseLine(long toErase);
	long DelLine(long toErase);
	long SelectLin(int num);			//存储当前要移动的线
	long MoveLin(CPoint point);
	long MoveLinLBU(CPoint point);	//响应移动鼠标弹起
	long MoveLinLBD(CPoint point);	//2
	long CreateLinLBU(CPoint point);//3
	long DeleteLinLBU(CPoint point);//4
	long ConLinLBU(CPoint point);//5
	long CreateLinMM(CPoint point);//6
	long CreateLinRBU(CPoint point);//7
	long RecoverLinLBU(CPoint point);//8

	 int MakingLine;
	 CPoint firstPoint; //
	 CPoint lastPoint; //画线时上一坐标点
	 CPoint templastPoint; //画线时上一坐标点
	 POINT linPnt[500];                 //存放线上的点
	 int LinPntIdx;
	 int LinPntNum;
	 CPoint startPnt;					 //存放移动线开始点
	int step;							//移动线当前状态
	int linToMove;
    POINT linPntTemp1[500];                 //存放连接线1
	int  linnum1;							//存放连接线1点个数
	POINT linPntTemp2[500];                 //存放连接线2
	int  linnum2;							//存放连接线2点个数

	  //连接线
   int firstlinnum;          
   int seclinnum;
   int isfirst;
   int issecond;
   long ConnectLins(int lin1,int lin2);	
   long DrawConLine();						 //画连接后的线
   long WriteConLine();						 //写连接后的线
   long DrawConLine(int num);				//标亮某条线



	/////////////////////////////////////////////////画线函数和数据/////////////////////////////////////////////


	long CreatRegLBU(POINT xy);
	long CreatRegMM(POINT xy);
	long CreatRegRBU(POINT xy);
 //颜色
	  COLORREF pntColor;
	  COLORREF linColor;
//{区索引结构
	
	COLORREF regColor;
	int regPntNum;
	int regDatOff;
//}

	POINT regPnt[500];
	int RegPntIdx;
	
	long DeleteRegLBU(POINT xy);
	long EraseAReg(COLORREF regColor,int RegPntNum,POINT *ptRegPnt);

	long DrawRegFile();

	long MoveRegLBD(POINT xy);
	long MoveRegLBU(POINT xy);
	long MoveRegMM(POINT xy);

	long WindowMoveLBD(POINT xy);
	long WindowMoveMM(POINT xy);
	long WindowMoveLBU(POINT xy);
	POINT slectedPnt;

	long RecoverDeletedRegLBU(POINT xy);

	long AmendRegPARLBU(POINT xy);
	

	

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CMapEditor10View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnFileNew();
	afx_msg void OnFileOpen();
	afx_msg void OnFileSave();
	afx_msg void OnFileSaveAs();
	afx_msg void OnAppExit();
	afx_msg void ZoomIn();
	afx_msg void ZoomOut();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void CreatReg();
	afx_msg void DeleteReg();
	afx_msg void MoveReg();
	afx_msg void WindowMove();
	afx_msg void WindowReset();
	afx_msg void ShowReg();
//	afx_msg void On32779();
	afx_msg void ShowDeletedReg();
	afx_msg void RecoverAReg();
	afx_msg void AmmendARegPAR();
	afx_msg void AmmendDefultPAR();
	afx_msg void OnCreatePnt();
	afx_msg void OnDeletePnt();
	afx_msg void OnMovePnt();
	afx_msg void OnShowDeletedPnt();
	afx_msg void OnRecoverPnt();
	afx_msg void OnSetPnt();
	afx_msg void OnShowPnt();
	afx_msg void OnCreateLin();
	afx_msg void OnMoveLin();
	afx_msg void OnConnectLin();
	afx_msg void OnSetlin();
	afx_msg void OnShowLin();
	afx_msg void OnShowDelLin();
	afx_msg void OnRecoverLin();
	afx_msg void OnDelLin();
//	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
};

#ifndef _DEBUG  // MapEditor1.0View.cpp 中的调试版本
inline CMapEditor10Doc* CMapEditor10View::GetDocument() const
   { return reinterpret_cast<CMapEditor10Doc*>(m_pDocument); }
#endif

