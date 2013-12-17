
// MapEditor1.0View.h : CMapEditor10View ��Ľӿ�
//

#pragma once
//#ifndef _FILESTRUCT_H
//#include "FileStruct.h"
//#define _FILESTRUCT_H 1
//#endif

class CMapEditor10View : public CView
{
protected: // �������л�����
	CMapEditor10View();
	DECLARE_DYNCREATE(CMapEditor10View)

// ����
public:
	CMapEditor10Doc* GetDocument() const;

// ����
public:
	short	m_step;
	POINT	m_startPnt;
	POINT	m_lastPnt;
	int zoomstyle;
	long ZoomInLBU(POINT myPoint);
	long ZoomInMM(POINT myPoint);
	long ZoomInLBD(POINT myPoint);
	long ZoomOutLBU(POINT myPoint);
//ʵ�ֻ�ͼ������װ
	//��ʾ
	int isShowPnt;
	int isShowLin;
	int isShowReg;
	 //���㺯��
	long DrawPoint(CPoint tempxy,COLORREF color); 
	long MovePoint(CPoint tempxy);  //�ƶ��㺯��
	long ErasePoint(CPoint tempxy); // ɾ����
	long MovePntLBD(CPoint point);  //�ƶ��㰴��
	long DeletePntLBU(CPoint point);//ɾ���㵯��
	long MovePntLBU(CPoint point);//�ƶ��㵯��

	//��������
	CPoint nowPoint; // ��־��һ�����
	CPoint toMove; // ��Ҫ�ƶ��������


	/////////////////////////////////////////////////���ߺ���������/////////////////////////////////////////////
	long DrawLine(CPoint tempxy);  //���ߺ���.
	long AppendLin();
	long EraseLine(long toErase);
	long DelLine(long toErase);
	long SelectLin(int num);			//�洢��ǰҪ�ƶ�����
	long MoveLin(CPoint point);
	long MoveLinLBU(CPoint point);	//��Ӧ�ƶ���굯��
	long MoveLinLBD(CPoint point);	//2
	long CreateLinLBU(CPoint point);//3
	long DeleteLinLBU(CPoint point);//4
	long ConLinLBU(CPoint point);//5
	long CreateLinMM(CPoint point);//6
	long CreateLinRBU(CPoint point);//7
	long RecoverLinLBU(CPoint point);//8

	 int MakingLine;
	 CPoint firstPoint; //
	 CPoint lastPoint; //����ʱ��һ�����
	 CPoint templastPoint; //����ʱ��һ�����
	 POINT linPnt[500];                 //������ϵĵ�
	 int LinPntIdx;
	 int LinPntNum;
	 CPoint startPnt;					 //����ƶ��߿�ʼ��
	int step;							//�ƶ��ߵ�ǰ״̬
	int linToMove;
    POINT linPntTemp1[500];                 //���������1
	int  linnum1;							//���������1�����
	POINT linPntTemp2[500];                 //���������2
	int  linnum2;							//���������2�����

	  //������
   int firstlinnum;          
   int seclinnum;
   int isfirst;
   int issecond;
   long ConnectLins(int lin1,int lin2);	
   long DrawConLine();						 //�����Ӻ����
   long WriteConLine();						 //д���Ӻ����
   long DrawConLine(int num);				//����ĳ����



	/////////////////////////////////////////////////���ߺ���������/////////////////////////////////////////////


	long CreatRegLBU(POINT xy);
	long CreatRegMM(POINT xy);
	long CreatRegRBU(POINT xy);
 //��ɫ
	  COLORREF pntColor;
	  COLORREF linColor;
//{�������ṹ
	
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
	

	

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ʵ��
public:
	virtual ~CMapEditor10View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
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

#ifndef _DEBUG  // MapEditor1.0View.cpp �еĵ��԰汾
inline CMapEditor10Doc* CMapEditor10View::GetDocument() const
   { return reinterpret_cast<CMapEditor10Doc*>(m_pDocument); }
#endif

