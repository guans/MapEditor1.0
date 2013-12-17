
// MapEditor1.0View.cpp : CMapEditor10View ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
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
	// ��׼��ӡ����
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

//��ʱ�ṹ
d_dot regPntdat[500];
reg_ndx_stru regNdx;
d_dot linPntdat[500];

// CMapEditor10View ����/����

CMapEditor10View::CMapEditor10View()
{
	// TODO: �ڴ˴���ӹ������
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


	MakingLine=0;      //��ʼ����
    step=0;
    isfirst=0;         //�����ߵ�һ���ߺ�
	issecond=0;			//�����ߵڶ����ߺ�
	LinPntNum=0;     //�ƶ��߸���
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
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CMapEditor10View ����

void CMapEditor10View::OnDraw(CDC* pDC)
{
	CMapEditor10Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
	//����
		int i=0;
		int len=0;
		long tempX=0;
		long tempY=0;
		CPoint temp;

		if(1==GPntTmpFOpened&&isShowPnt==1)
		{
			len=GPntTmpFile.GetLength();
			GPntTmpFile.Seek(0,CFile::begin);

			for(i=0;i<len/sizeof(PNT_STRU);i++)   //�������еĵ�
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

	//����
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

			for(ii=0;ii< ndxlen / linlen;ii++)						 //�������е�������
			{
				GLinTmpFile1.Read(&LIN_NDX_STRU,  linlen);
				if(LIN_NDX_STRU.isDel==0&&GCurLINState==0)            //����û��ɾ������
				{
					dotnum=LIN_NDX_STRU.dotNum;
					datff=LIN_NDX_STRU.datOff;

					GLinTmpFile2.Seek(datff,CFile::begin);
					POINT *p=new POINT[dotnum];
					for(j=0; j < dotnum ; j++)
					{
						GLinTmpFile2.Read(&D_DOT, plen );
						DotDPtoVP(D_DOT);          //�¼ӵ�
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

				else if(LIN_NDX_STRU.isDel==1&&GCurLINState==1)            //����ɾ������
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

		

	//����
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


		//����ƶ�������
		firstlinnum=0;
		seclinnum=0;
		isfirst=0;
		issecond=0;
		MakingLine=0;
		
		//״̬��
		CString sMousePos;
		CString Pntnum="�㣺";
		CString Linnum="�ߣ�";
		CString Regnum="����";
		sMousePos.Format(_T(" %s %d %s %d %s %d"),Pntnum,GPntNum,Linnum,GLinNum,Regnum,GRegNum);
		AfxGetMainWnd()->GetDescendantWindow(AFX_IDW_STATUS_BAR)->SetWindowText(sMousePos);
}


// CMapEditor10View ��ӡ


void CMapEditor10View::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CMapEditor10View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CMapEditor10View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CMapEditor10View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
}

void CMapEditor10View::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	switch(GCurOperState)
	{
	case OPERSTATE_CREAT_REG:			//���ò���״̬Ϊ����
		CreatRegRBU(point);
		break;
	case OPERSTATE_INPUT_LIN:						//����
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


// CMapEditor10View ���

#ifdef _DEBUG
void CMapEditor10View::AssertValid() const
{
	CView::AssertValid();
}

void CMapEditor10View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMapEditor10Doc* CMapEditor10View::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMapEditor10Doc)));
	return (CMapEditor10Doc*)m_pDocument;
}
#endif //_DEBUG


// CMapEditor10View ��Ϣ�������


void CMapEditor10View::OnFileNew()
{
	// TODO: �ڴ���������������
	int type;
	GetNewType(type);
	if(type&0x0001)
		NewPnt();		//�½����ļ�
	if(type&0x0002)
		NewLin();		//�½����ļ�
	if(type&0x0004)
		NewReg();		//�½����ļ�

	
}



void CMapEditor10View::OnFileOpen()
{
	// TODO: �ڴ���������������
	CString FilePathName;
	char sRead[3];
	CFile mFile;
	GisShowDeleted=0;
	if(GetOpenPath(FilePathName)==0)			//�ж��Ƿ����·��
	{
		goto ON_FILE_OPEN_END;
	}
	mFile.Open (FilePathName,CFile::modeRead);
	mFile.Read(sRead,3);
	if(sRead[0]=='P'&&sRead[1]=='N'&&sRead[2]=='T')	//ȷ�����ļ��ǵ��ļ�
	{

		memcpy(GPntFName,FilePathName,256);
		Pnt_FCT(mFile,FilePathName);
		CRect rt;
		CClientDC dc(this);
		GetClientRect(&rt);								//��ȡ�ͻ�������
		dc.FillRect(&rt,new CBrush(RGB(255,255,255)));			//�հ׸��ǿͻ���
	}
	else if(sRead[0]=='L'&&sRead[1]=='I'&&sRead[2]=='N')	//ȷ�����ļ������ļ�
	{
		memcpy(GLinFName,FilePathName,256);
		Lin_FCT(mFile,FilePathName);
		CRect rt;
		CClientDC dc(this);
		GetClientRect(&rt);								//��ȡ�ͻ�������
		dc.FillRect(&rt,new CBrush(RGB(255,255,255)));			//�հ׸��ǿͻ���

	
	}
	else if(sRead[0]=='R'&&sRead[1]=='E'&&sRead[2]=='G')	//ȷ�����ļ������ļ�
	{
		memcpy(GRegFName,FilePathName,256);
		Reg_FCT(mFile,FilePathName);
		CRect rt;
		CClientDC dc(this);
		GetClientRect(&rt);								//��ȡ�ͻ�������
		dc.FillRect(&rt,new CBrush(RGB(255,255,255)));			//�հ׸��ǿͻ���
		DrawRegFile();
	}
	
	mFile.Close();
	ON_FILE_OPEN_END:;
	
}




void CMapEditor10View::OnFileSave()
{
	/*
	// TODO: �ڴ���������������
	CString FilePathName;
	if(GPntChanged==0&&GLinChanged==0&&GRegChanged==0)
	{
		MessageBox(_T("���޸Ĳ�����"));
	}
	if(GPntChanged==1&&GLinChanged==0&&GRegChanged==0)			//���ļ��ѱ��޸�
	{
		if(GPntFName[0]!='\0')			//�Ƿ����Ѵ򿪵��ļ�
		{
			Pnt_TCF((LPCTSTR)GPntFName,10);
		}
		else
		{
			if(GetPntSavePath(FilePathName)==0)			//�ж��Ƿ����·��
			{
				goto ON_FILE_SAVE_END;
			}
			memcpy(GPntFName,FilePathName,256);
			Pnt_TCF(FilePathName,10);
			
		}
	}
	else if(GPntChanged==0&&GLinChanged==1&&GRegChanged==0)			//���ļ��ѱ��޸�
	{
		if(GLinFName[0]!='\0')			//�Ƿ����Ѵ򿪵��ļ�
		{
			Lin_TCF((LPCTSTR)GLinFName,10);
			
		}
		else
		{
			if(GetLinSavePath(FilePathName)==0)			//�ж��Ƿ����·��
			{
				goto ON_FILE_SAVE_END;
			}
			memcpy(GLinFName,FilePathName,256);
			Lin_TCF(FilePathName,10);
			
		}
	}
	else if(GPntChanged==0&&GLinChanged==0&&GRegChanged==1)			//���ļ��ѱ��޸�
	{
		if(GRegFName[0]!='\0')									//�Ƿ����Ѵ򿪵��ļ�
		{
			Reg_TCF((LPCTSTR)GRegFName,10);
		}
		else
		{
			if(GetRegSavePath(FilePathName)==0)			//�ж��Ƿ����·��
			{
				goto ON_FILE_SAVE_END;
			}
			memcpy(GRegFName,FilePathName,256);
			Reg_TCF(FilePathName,10);
			
		}
	}
	ON_FILE_SAVE_END:;
	*/

	// TODO: �ڴ���������������
		CString FilePathName;
		if(GPntChanged==0&&GLinChanged==0&&GRegChanged==0)
		{
			MessageBox(_T("���޸Ĳ�����"));
		}
		if(GPntFCreated==1)			//���ļ��ѱ��޸�
		{
			if(GPntFName[0]!=0)			//�Ƿ����Ѵ򿪵��ļ�
			{
				Pnt_TCF((LPCTSTR)GPntFName,10);
			}
			else
			{
				if(GetPntSavePath(FilePathName)==0)			//�ж��Ƿ����·��
				{
					goto ON_FILE_SAVE_END;
				}
			memcpy(GPntFName,FilePathName,256);
			Pnt_TCF(FilePathName,10);

			}
		}
		if(GLinChanged==1)			//���ļ��ѱ��޸�
		{
			if(GLinFName[0]!=0)			//�Ƿ����Ѵ򿪵��ļ�
			{
				Lin_TCF((LPCTSTR)GLinFName,10);

			}
			else
			{
				if(GetLinSavePath(FilePathName)==0)			//�ж��Ƿ����·��
				{
					goto ON_FILE_SAVE_END;
				}
				memcpy(GLinFName,FilePathName,256);
			Lin_TCF(FilePathName,10);

			}
		}
		if(GRegChanged==1)			//���ļ��ѱ��޸�
		{
			if(GRegFName[0]!=0)			//�Ƿ����Ѵ򿪵��ļ�
			{
				Reg_TCF((LPCTSTR)GRegFName,10);
			}
			else
			{
				if(GetRegSavePath(FilePathName)==0)			//�ж��Ƿ����·��
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
	// TODO: �ڴ���������������
	CString FilePathName;
	if(GPntChanged==0&&GLinChanged==0&&GRegChanged==0)		//�ļ�û�б��޸�
	{
		MessageBox(_T("���޸Ĳ�����"));
	}
	else if(GPntFCreated==1)		//���ļ����޸�
	{
		if(GetPntSaveAsPath(FilePathName)==0)			//�ж��Ƿ����·��
			{
				goto ON_FILE_SAVEAS_END;
			}
		memcpy(GPntFName,FilePathName,256);
		Pnt_TCF(FilePathName,10);
		
		
	}
	else if(GLinChanged==1)	//���ļ����޸�
	{
		if(GetLinSaveAsPath(FilePathName)==0)			//�ж��Ƿ����·��
		{
			goto ON_FILE_SAVEAS_END;
		}
		memcpy(GLinFName,FilePathName,256);
		Lin_TCF(FilePathName,10);
		
		
	}
	else if(GRegChanged==1)	//���ļ����޸�
	{
		if(GetRegSaveAsPath(FilePathName)==0)			//�ж��Ƿ����·��
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
	// TODO: �ڴ���������������
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
	// TODO: �ڴ���������������
	GCurOperState=OPERSTATE_ZOOM_IN;  //���õ�ǰ״̬Ϊ�Ŵ�
}


void CMapEditor10View::ZoomOut()
{
	// TODO: �ڴ���������������
	GCurOperState=OPERSTATE_ZOOM_OUT;  //���õ�ǰ״̬Ϊ��С
}

long CMapEditor10View::ZoomInLBU(POINT myPoint)
{
	if(GPntTmpFOpened==0&&GRegTmpFOpened==0)
		return (0);
	CRect rect;							//����һ������
	double yp;							//����y�������
	double xp;							//����x�������
	long len;
	int i,j;
	double zoom;
	double xOffset;								//��������ڴ�������ϵ��xƫ����yƫ��
	double yOffset;	
	CPoint temp;
	CClientDC dc(this);
	GetClientRect(&rect);								//��ȡ�ͻ�������
	dc.FillRect(&rect,new CBrush(RGB(255,255,255)));			//�հ׸��ǿͻ���
	if(zoomstyle==0)
	{
		xOffset=myPoint.x-(rect.right-rect.left)/2;		//��λ����ͻ������ĵ�λ�ü���ƫ��
		yOffset=myPoint.y-(rect.bottom-rect.top)/2;
		GxOffset+=xOffset/GZoom;
		GyOffset+=yOffset/GZoom;
		GZoom*=1.2;
		Invalidate();
	}
	else 
	{
		yp=double(rect.bottom-rect.top)/double(myPoint.y-m_startPnt.y);		//�������ϵ���ͻ������ϵ����ƫ��
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
	CRect rect;							//����һ������
	long len;
	int i,j;
	double xOffset;								//��������ڴ�������ϵ��xƫ����yƫ��
	double yOffset;
	CPoint temp;
	CClientDC dc(this);
	GetClientRect(&rect);
	dc.FillRect(&rect,new CBrush(RGB(255,255,255)));
	xOffset=myPoint.x-(rect.right-rect.left)/2;		//��λ����ͻ������ĵ�λ�ü���ƫ��
	yOffset=myPoint.y-(rect.bottom-rect.top)/2;
	GxOffset+=xOffset/GZoom;
	GyOffset+=yOffset/GZoom;
	GZoom*=0.8;
	Invalidate();
	return (1);
}
void CMapEditor10View::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	
	switch(GCurOperState)
	{
	case OPERSTATE_INPUT_PNT:						//���
		AppendPnt(point,pntColor);					//���¼ӵĵ�д����ʱ�ļ�
		DrawPoint(point,pntColor);					//������ǰ�ĵ�
		break;	
	case OPERSTATE_DEL_PNT:							//ɾ����
		DeletePntLBU(point);
		break;
	case OPERSTATE_MOV_PNT:							//�ƶ���
		MovePntLBU(point);							//�ļ����ƶ���   (�ƶ����ĵ㣬Ҫ�ƶ��ĵ�)
		Invalidate();								//ˢ��ͼ��	
		break;
	case OPERSTATE_RECOVER_PNT:						//�ָ���
		RecoverPnt(point);
		ErasePoint(point);
		break;
	case OPERSTATE_ZOOM_IN:			//���ò���״̬Ϊ�Ŵ�
		ZoomInLBU(point);
		break;
	case OPERSTATE_ZOOM_OUT:			//���ò���״̬Ϊ��С
		ZoomOutLBU(point);
		break;
	case OPERSTATE_CREAT_REG:			//���ò���״̬Ϊ����
		CreatRegLBU(point);
		break;
	case OPERSTATE_DELETE_REG:			//���ò���״̬Ϊɾ����
		DeleteRegLBU(point);
		break;
	case OPERSTATE_MOVE_REG:			//���ò���״̬Ϊ�ƶ���
		MoveRegLBU(point);
		break;
	case OPERSTATE_WINDOW_MOVE:			//���ò���״̬Ϊ�����ƶ�
		WindowMoveLBU(point);
		break;
	case OPERSTATE_RECOVER_REG:			//���ò���״̬Ϊ�ָ���
		RecoverDeletedRegLBU(point);
		break;
	case OPERSTATE_AMMENDPAR_REG:			//���ò���״̬Ϊ�޸�������
		AmendRegPARLBU(point);
		break;
	case OPERSTATE_INPUT_LIN:						 //����
			CreateLinLBU(point);
			break;
		case OPERSTATE_DEL_LIN:							//ɾ����
			DeleteLinLBU(point);
			break;
		case OPERSTATE_MOVE_LIN:								 //�ƶ���
			MoveLinLBU(point);
			Invalidate();										//ˢ��ͼ��
			break;		
		case OPERSTATE_CONNECT_LIN:								//������
			ConLinLBU(point);
			break;
		
		case OPERSTATE_RECOVER_LIN:								//�ָ���
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
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	switch(GCurOperState)
	{
	case OPERSTATE_ZOOM_IN:				//���ò���״̬Ϊ�Ŵ�
		ZoomInLBD(point);
		break;
	case OPERSTATE_MOV_PNT:				//�ƶ���
		MovePntLBD(point);
		break;
	case OPERSTATE_MOVE_REG:			//���ò���״̬Ϊ�ƶ���
		MoveRegLBD(point);
		break;
	case OPERSTATE_WINDOW_MOVE:			//���ò���״̬Ϊ�����ƶ�
		WindowMoveLBD(point);
		break;
	case OPERSTATE_MOVE_LIN:						 //���ò���״̬Ϊ�ƶ���
		MoveLinLBD(point);
		break;
	
	//default:GPntChanged=1;
	//	break;
			
	}

	CView::OnLButtonDown(nFlags, point);
}


void CMapEditor10View::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	switch(GCurOperState)
	{
	case OPERSTATE_ZOOM_IN:			//���ò���״̬Ϊ�Ŵ�
		ZoomInMM(point);
		break;
	case OPERSTATE_MOV_PNT:							//�ƶ���
		MovePoint(point);
		break;
	case OPERSTATE_CREAT_REG:
		CreatRegMM(point);
		break;
	case OPERSTATE_MOVE_REG:			//���ò���״̬Ϊ�ƶ���
		MoveRegMM(point);
		break;
	case OPERSTATE_WINDOW_MOVE:			//���ò���״̬Ϊ�����ƶ�
		WindowMoveMM(point);
		break;
	case OPERSTATE_MOVE_LIN:						  //�ƶ���
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
	//1.����
	CPen pen(0,1,regColor);
	dc.SelectObject(&pen);
	dc.SetROP2(R2_NOTXORPEN);
	dc.MoveTo(regPnt[0]);
	dc.LineTo(m_lastPnt);
	dc.MoveTo(m_startPnt);
	dc.LineTo(m_lastPnt);						//ɾ�����µ���
	PntSize=regPntNum*sizeof(D_DOT);			//�������е�Ĵ�С
	dc.MoveTo (m_startPnt);
	dc.LineTo (regPnt[0]);
	CBrush brush(regColor);
	dc.SelectObject (&brush);
	dc.Polygon(regPnt,regPntNum);
	//2.����ʱ�ļ��д洢
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
	// TODO: �ڴ���������������
	GCurOperState=OPERSTATE_CREAT_REG;			//���õ�ǰ״̬Ϊ����
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
	// TODO: �ڴ���������������
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
	if(m_step==0||m_step==1)						//��δѡ������δ�ƶ�ʱ
	{
		m_step = 0;
		m_startPnt.x = 0;
		m_startPnt.y = 0;
		m_lastPnt.x = 0;
		m_lastPnt.y = 0;
		return (0);
	}
	DrawAReg(regPnt,regPntNum,&dc);	//�����ʼ����
	for(i=0;i<regPntNum;i++)			//�ı���������
	{
		newRegPnt[i].x=regPnt[i].x+xy.x-m_startPnt.x;
		newRegPnt[i].y=regPnt[i].y+xy.y-m_startPnt.y;
	}
	for(i=0;i<regPntNum;i++)			//�ı���������
	{
		newRegPntdat[i].x=regPntdat[i].x+xy.x-m_startPnt.x;
		newRegPntdat[i].y=regPntdat[i].y+xy.y-m_startPnt.y;
	}
	dc.Polygon(newRegPnt,regPntNum);
	//д����ʱ�ļ�2
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
	// TODO: �ڴ���������������
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
	xOffset=m_startPnt.x-xy.x;							//���x�����ϵ�ƫ��
	yOffset=m_startPnt.y-xy.y;							//���y�����ϵ�ƫ��
	GxOffset+=xOffset/GZoom;
	GyOffset+=yOffset/GZoom;
	GetClientRect(&rect);								//��ȡ�ͻ�������
	dc.FillRect(&rect,new CBrush(RGB(255,255,255)));			//�հ׸��ǿͻ���
	dc.SetROP2(R2_NOTXORPEN);
	if(GPntTmpFOpened==1&&GPntNum!=0)
	{
		GPntTmpFile.Seek(0,CFile::begin);
		for(i=0;i<GPntNum;i++)   //�������еĵ�
			{
				GPntTmpFile.Read(&PNT_STRU, sizeof( PNT_STRU ));
				if(PNT_STRU.isDel==GisShowDeleted)            //����û��ɾ���ĵ�
				{
				temp.x=PNT_STRU.x;
				temp.y=PNT_STRU.y;	
				DotDPtoVP(temp);				//ת������
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
			if(GetAReg(i,regNdx,regPntdat,500)==1)				//�õ�һ����
			{
				if(regNdx.isDel==GisShowDeleted)							//�������û��ɾ��
				{
					CPen pen(PS_SOLID,1,regNdx.color);
					dc.SelectObject(&pen);
					CBrush brush(regNdx.color);
					dc.SelectObject (&brush);
					SomeDotDPtoVP(regPntdat,regNdx.dotNum);	//ת������
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
	// TODO: �ڴ���������������
	GCurOperState=OPERSTATE_WINDOW_MOVE;
	isShowPnt=1;
	isShowLin=1;
	isShowReg=1;
}


void CMapEditor10View::WindowReset()
{
	// TODO: �ڴ���������������
	CRect rect;
	CRect rt;							//����һ�����λ����׿ͻ���
	double yp;							//����y�������
	double xp;							//����x�������
	int i,j;
	double zoom;
	double xOffset;								//��������ڴ�������ϵ��xƫ����yƫ��
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
	GetClientRect(&rt);								//��ȡ�ͻ�������
	dc.FillRect(&rt,new CBrush(RGB(255,255,255)));			//�հ׸��ǿͻ���
	yp=double(rt.bottom-rt.top)/double(bottomRight.y-topLeft.y);		//�������ϵ���ͻ������ϵ����ƫ��
	xp=double(rt.right-rt.left)/double(bottomRight.x-topLeft.x);
	zoom=((xp>yp)?yp:xp);							//ѡ�����С��ϵ����ʾ��ȫ
	xOffset=topLeft.x;
	yOffset=topLeft.y;
	GxOffset+=xOffset/GZoom;
	GyOffset+=yOffset/GZoom;
	GZoom*=zoom;
	if(GPntTmpFOpened==1)
	{
		
		GPntTmpFile.Seek(0,CFile::begin);
		for(i=0;i<GPntNum;i++)   //�������еĵ�,������ĵ�
			{
				GPntTmpFile.Read(&PNT_STRU, sizeof( PNT_STRU ));
				if(PNT_STRU.isDel==GisShowDeleted)            //����û��ɾ���ĵ�
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
	// TODO: �ڴ���������������
	CRect rt;
	CClientDC dc(this);
	GCurOperState=0;
	GisShowDeleted=0;
	isShowPnt=0;
	isShowLin=0;
	isShowReg=1;
	GetClientRect(&rt);								//��ȡ�ͻ�������
	dc.FillRect(&rt,new CBrush(RGB(255,255,255)));			//�հ׸��ǿͻ���
	DrawRegFile();
}




void CMapEditor10View::ShowDeletedReg()
{
	// TODO: �ڴ���������������
	CRect rt;
	CClientDC dc(this);
	GCurOperState=0;
	GisShowDeleted=1;
	isShowPnt=0;
	isShowLin=0;
	isShowReg=1;
	GetClientRect(&rt);								//��ȡ�ͻ�������
	dc.FillRect(&rt,new CBrush(RGB(255,255,255)));			//�հ׸��ǿͻ���
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
	// TODO: �ڴ���������������
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
	// TODO: �ڴ���������������
	GisShowDeleted=0;
	GCurOperState=OPERSTATE_AMMENDPAR_REG;
	isShowPnt=1;
	isShowLin=1;
	isShowReg=1;
}


void CMapEditor10View::AmmendDefultPAR()
{
	// TODO: �ڴ���������������
	GCurOperState=0;
	SetDefultColor();
}
//�������ܣ��㻭ͼ����ʵ��
//������������
	long CMapEditor10View::DrawPoint(CPoint tempxy,COLORREF color)       //ͼ���ϻ���
	{
	
		//��ñ����ڻ�ǰ���ͼ
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
	//�������ܣ�ͼ���ϲ�����
	//������������
	long CMapEditor10View::ErasePoint(CPoint tempxy)		 
	{

		CPoint toErase;
		FindNearestPnt(tempxy) ;
		toErase=tempxy;									 //�����ɾ����
		CPen newpen(PS_SOLID,2,RGB(255,255,255));		//���컭�ʶ���
		//��ñ����ڻ�ǰ���ͼ
		CClientDC dc(this);
		CPen *pOldPen=dc.SelectObject(&newpen);			//������ѡ��DC��SelectObject����������ǰ����
		//�������ģʽ����
		//����Ĩȥһ��Сʮ��
		dc.MoveTo(toErase.x,toErase.y-6);
		dc.LineTo(toErase.x,toErase.y+6);
		dc.MoveTo(toErase.x-6,toErase.y);
		dc.LineTo(toErase.x+6,toErase.y);
		GPntChanged=1;
		::ReleaseDC(m_hWnd,dc);
		return 1;

	}
	//�������ܣ��ƶ���
	//������
	long CMapEditor10View::MovePntLBD(CPoint point)       
	{
		nowPoint=point;	 //�洢��ǰ��Ϊǰһ��
		CPoint ptMove;
		FindNearestPnt(point);
		m_step++;
		ptMove=point;
		toMove=point;
		CPen newpen(PS_SOLID,2,RGB(255,0,0));	 //���컭�ʶ���
		CClientDC dc(this);	 //��ñ����ڻ�ǰ���ͼ
		CPen *pOldPen=dc.SelectObject(&newpen);	 //������ѡ��DC��SelectObject����������ǰ����
		//�������Ҫ�ƶ��ĵ�
		dc.MoveTo(ptMove.x,ptMove.y-6);
		dc.LineTo(ptMove.x,ptMove.y+6);
		dc.MoveTo(ptMove.x-6,ptMove.y);
		dc.LineTo(ptMove.x+6,ptMove.y);
		::ReleaseDC(m_hWnd,dc);

		return 1;

	}

	//�������ܣ�ɾ����
	//������
	long CMapEditor10View::DeletePntLBU(CPoint point)       
	{
		int pnum=0;
		CPoint ptemp;
		FindNearestPnt(point,pnum);
		ptemp.x=point.x;
		ptemp.y=point.y;

		if(DeletePnt(pnum)==1);								//�ļ���ɾ����
		{
			ErasePoint(point);							//ͼ����ɾ����
		}

		return 1;

	}

	//�������ܣ��ƶ���
	//������
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


	//�������ܣ�ͼ�����ƶ���
	//��������ǰ������
	long CMapEditor10View::MovePoint(CPoint tempxy)    
	{
		if(m_step==0)
			return (0);
		CPen newpen(PS_SOLID,2,RGB(255,255,255));//���컭�ʶ���
		//��ñ����ڻ�ǰ���ͼ
		CClientDC dc(this);
		CPen *pOldPen=dc.SelectObject(&newpen);//������ѡ��DC��SelectObject����������ǰ����
		//�������ģʽ����
		//dc.SetROP2(R2_NOT);	
		//����Ĩȥһ��Сʮ��
		dc.MoveTo(nowPoint.x,nowPoint.y-6);
		dc.LineTo(nowPoint.x,nowPoint.y+6);
		dc.MoveTo(nowPoint.x-6,nowPoint.y);
		dc.LineTo(nowPoint.x+6,nowPoint.y);
		CPen oldpen(PS_SOLID,2,RGB(192,192,192));	//���컭�ʶ���
		dc.SelectObject(&oldpen);					//������ѡ��DC��SelectObject����������ǰ����
		//���ﻭһ��Сʮ��
		dc.MoveTo(tempxy.x,tempxy.y-6);
		dc.LineTo(tempxy.x,tempxy.y+6);
		dc.MoveTo(tempxy.x-6,tempxy.y);
		dc.LineTo(tempxy.x+6,tempxy.y);
		nowPoint=tempxy;							//�ѵ�ǰ���Ϊ��һ��
		GPntChanged=1;
		::ReleaseDC(m_hWnd,dc);
		return (1);

	}


	//�������ܣ��ƶ���
	//������
	long CMapEditor10View::MoveLinLBD(CPoint point)       
	{
		int num;
		startPnt=point;	 //�洢��ǰ��Ϊǰһ��
		FindNearLine(point,num);
		SelectLin(num);
		EraseLine(num);
		linToMove=num; //����Ҫ�ƶ����ߺ�
		step=1;
		return 1;

	}
	//�������ܣ�����
	//������
	long CMapEditor10View::CreateLinLBU(CPoint point)       
	{
		MakingLine=1;	 // �����������״̬
		nowPoint=point;	 //�洢ǰһ���ڵ�
		linPnt[LinPntNum]=point;
	
		DrawLine(point); //������ǰ�ĵ�
		LinPntNum++;     //���ϵ�������

		return 1;
	}
	//�������ܣ�ɾ��
	//������
	long CMapEditor10View::DeleteLinLBU(CPoint point)       
	{

		CString str;
		int LineNum;
		FindNearLine(point,LineNum);
		EraseLine(LineNum);	 //ͼ����ɾ�����LineNum����
		DelLine(LineNum);
		return 1;
	}
	//�������ܣ�������
	//������
	long CMapEditor10View::ConLinLBU(CPoint point)       
	{
		int firstnum;
		int secnum;
		if(isfirst==0)	 //ѡ�е�һ����
		{
			FindNearLine(point,firstnum);
			firstlinnum=firstnum;	 //�����һ���ߺ�
			isfirst=1;
			DrawConLine(firstnum);
			/*CString sMousePos;
			CString nowstr="�Ѿ�ѡ���һ���ߣ���ѡ��ڶ���";
			sMousePos.Format(_T(" %s "),nowstr);
			AfxGetMainWnd()->GetDescendantWindow(AFX_IDW_STATUS_BAR)->SetWindowText(sMousePos);
			*/
		}
		else if(issecond==0)
		{
			FindNearLine(point,secnum);
			if(firstlinnum!=secnum)
			{
				seclinnum=secnum;	 //����ڶ����ߺ�
				issecond=1;
				DrawConLine(secnum);
			}
		}
		else if(firstlinnum!=0 &&seclinnum!=0 ) ConnectLins(firstlinnum,seclinnum); //����������

		return 1;
	}
	//�������ܣ�����
	//������
	long CMapEditor10View::CreateLinMM(CPoint point)       
	{
		if(MakingLine==1) {
			DrawPoint(point,pntColor);
			CPen newpen(PS_SOLID,2,RGB(255,0,0));			//���컭�ʶ���
			CPen oldpen(PS_SOLID,2,RGB(255,255,255));		//���컭�ʶ���

			CClientDC dc(this);								//��ñ����ڻ�ǰ���ͼ
			CPen *pOldPen=dc.SelectObject(&newpen);			//������ѡ��DC��SelectObject����������ǰ����
			//�������Ҫ�ƶ��ĵ�
			dc.MoveTo(lastPoint.x,lastPoint.y);
			dc.LineTo(point.x,point.y);

			dc.SelectObject(&oldpen);						//������ѡ��DC��SelectObject����������ǰ����
			//�������Ҫ�ƶ��ĵ�
			dc.MoveTo(lastPoint.x,lastPoint.y);
			dc.LineTo(templastPoint.x,templastPoint.y);
			//����ɾ����һ����
			dc.MoveTo(templastPoint.x,templastPoint.y-6);
			dc.LineTo(templastPoint.x,templastPoint.y+6);
			dc.MoveTo(templastPoint.x-6,templastPoint.y);
			dc.LineTo(templastPoint.x+6,templastPoint.y);

			templastPoint=point;
			::ReleaseDC(m_hWnd,dc);
		}

		return 1;
	}
	//�������ܣ�����
	//������
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

	//�������ܣ��ָ���
	//������
	long CMapEditor10View::RecoverLinLBU(CPoint point)       
	{
		
		int linnum;
		RecoverLin(point,linnum);
		EraseLine(linnum);
		Invalidate();
		return 1;
	}



	//�������ܣ�ͼ���ϻ���
	//��������ǰ������
	long CMapEditor10View::DrawLine(CPoint tempxy)    
	{
		if(LinPntNum!=0)
		{
			//��ñ����ڻ�ǰ���ͼ
			CClientDC dc(this);
			//�������ģʽ����
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

	//�������ܣ�ͼ���ϲ�����
	//������Ҫ�����ߵı��
	long CMapEditor10View::EraseLine(long toErase)     //
	{
		int num=0;

		num=toErase;										//�����ɾ����������
		CPen newpen(PS_SOLID,2,RGB(255,255,255));			//���컭�ʶ���
		CClientDC dc(this);									//��ñ����ڻ�ǰ���ͼ
		CPen *pOldPen=dc.SelectObject(&newpen);				//������ѡ��DC��SelectObject����������ǰ����
		//�������ģʽ���� //dc.SetROP2(R2_NOT);	
		//����Ĩȥһ��һ����

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
			GLinTmpFile1.Seek( (toErase -1) * linlen,CFile::begin);  //�ҵ�������ΪtoErase����
			GLinTmpFile1.Read(&LIN_NDX_STRU,  linlen);
			dotnum=LIN_NDX_STRU.dotNum;
			datff=LIN_NDX_STRU.datOff;
			GLinTmpFile2.Seek(datff,CFile::begin);				  //�������ҵ�������ΪtoErase��������

			GLinTmpFile2.Read(&D_DOT, plen );				 //�ߵ�һ����
			ptemp.x=D_DOT.x;
			ptemp.y=D_DOT.y;
			dc.MoveTo(ptemp);
			for(j=1 ; j < dotnum ; j++)						//�ӵ�2���㿪ʼ
			{
				GLinTmpFile2.Read(&D_DOT, plen );
				todel.x=D_DOT.x;
				todel.y=D_DOT.y;
				dc.LineTo(todel);
			}

		}
		return 1;

	}

	//�������ܣ��ļ��в�����
	//������Ҫ�����ߵı��
	long CMapEditor10View::DelLine(long toErase)     
	{
		lin_ndx_stru TEM_LIN;
		int num=0;
		num=toErase;									//�����ɾ����������
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
		//�޸������ļ�
		if(1==GLinFCreated)
		{
			ndxlen=GLinTmpFile1.GetLength();
			GLinTmpFile1.Seek( (num -1) * linlen,CFile::begin);
			GLinTmpFile1.Read(&TEM_LIN,linlen);
			TEM_LIN.isDel= 1;
			LIN_NDX_STRU=TEM_LIN;
			GLinTmpFile1.Seek( -linlen,CFile::current);			 //�ҵ�������ΪtoErase����

			GLinTmpFile1.Write(&LIN_NDX_STRU,linlen);


		}
		return 1;
	}

	//�������ܣ�ͼ�����ƶ���
	//�����������
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
			CClientDC dc(this);							//��ñ����ڻ�ǰ���ͼ
			CPen newpen(PS_SOLID,2,RGB(255,255,255));	//���컭�ʶ���
			CPen *pOldPen=dc.SelectObject(&newpen);		//������ѡ��DC��SelectObject����������ǰ����

			long disX=point.x - startPnt.x;				//��������ƫ��
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
			CPen oldpen(PS_SOLID,2,linColor);		//���컭�ʶ���
			dc.SelectObject(&oldpen);						//������ѡ��DC��SelectObject����������ǰ����

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
	//�������ܣ��洢ѡ������
	//������Ҫ�洢���ߺ�
	long CMapEditor10View::SelectLin(int num)     //�洢ѡ�����ߵ�
	{	
		CPen newpen(PS_SOLID,2,RGB(255,255,255));	//���컭�ʶ���
		CClientDC dc(this);							//��ñ����ڻ�ǰ���ͼ
		CPen *pOldPen=dc.SelectObject(&newpen);		//������ѡ��DC��SelectObject����������ǰ����

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

			GLinTmpFile1.Seek( (num -1) * linlen,CFile::begin);  //�ҵ�������Ϊnum����
			GLinTmpFile1.Read(&LIN_NDX_STRU,  linlen);
			dotnum=LIN_NDX_STRU.dotNum;
			datff=LIN_NDX_STRU.datOff;
			GLinTmpFile2.Seek(datff,CFile::begin);           //�������ҵ�������Ϊnum��������

			for(j=0 ; j < dotnum ; j++)                //�ӵ�2���㿪ʼ
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

	//�������ܣ�д�ƶ������
	//������������
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
		//�޸������ļ�
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

		LinPntNum=0;			//��յ���ʱ����
		return 1;


	}


	//�������ܣ����ļ���д������������
	//������
	long CMapEditor10View::AppendLin()  
	{
		int lendata;

		lendata=GLinTmpFile2.GetLength();
		GLinTmpFile2.Seek(lendata,CFile::begin);     //���ݴ��ļ�ĩβд

		GLinNum++,GLinLNum++;

		int lenndx;

		int lenofl=sizeof(LIN_NDX_STRU);        //�����ṹ����
		int lenofp = sizeof(D_DOT);           //��ṹ����


		lenndx=GLinTmpFile1.GetLength();
		GLinTmpFile1.Seek(lenndx,CFile::begin);     //�������ļ�ĩβд

		//д��ǰ������
		LIN_NDX_STRU.color=linColor;
		LIN_NDX_STRU.datOff= GLinTmpFile2.GetLength();       //���ݴ洢��ʼλ�� һ����ṹ����*�����*�ߺ�
		LIN_NDX_STRU.dotNum=LinPntNum;
		LIN_NDX_STRU.isDel=0;
		LIN_NDX_STRU.pattern=0;
		GLinTmpFile1.Write(&LIN_NDX_STRU,lenofl);  //���ṹ��д���ļ�

		int i=0;
		for(i=0;i < LinPntNum ; i++)
		{
			
			D_DOT.x=linPnt[i].x;
			D_DOT.y=linPnt[i].y;
			GLinTmpFile2.Write(&D_DOT,lenofp);  //���ݽṹ��д���ļ�

		}

		return 1;
	}

	//�������ܣ�����������
	//�������ߺ�1 2
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
		else														//��ʼ����
		{
			if(Lin1[linnum1].x < Lin2[0].x)						//��һ���ߵ�β �ӵڶ���ͷ
			{
				for(i=0;i< linnum1;i++)
					linPnt[i]=Lin1[i];

				for(i=linnum1;i< linnum2+linnum1;i++)
				{
					linPnt[i]=Lin2[i-linnum1];
				}
			}
			else if(Lin1[linnum1].x > Lin2[0].x)				//��һ��β �� �ڶ���β
			{
				for(i=0;i< linnum1;i++)
					linPnt[i]=Lin1[i];
				for(i= linnum1+linnum2;i>=linnum1;i--)
				{
					linPnt[i]=Lin2[i];
				}
			}
			else if(Lin1[0].x < Lin2[linnum2-1].x)				//��һ��ͷ �ӵڶ���ͷ
			{

			}
			else if(Lin1[0].x > Lin2[linnum2-1].x)			   //��һ�� ͷ �ӵڶ��� β
			{

			}
		}

		LinPntNum=linnum1+linnum2;
		EraseLine(lin1);									//����������
		EraseLine(lin2);
		DelLine(lin1);								     //�ļ���ɾ��������
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
		CClientDC dc(this);							//��ñ����ڻ�ǰ���ͼ
		CPen newpen(PS_SOLID,2,linColor);	//���컭�ʶ���
		CPen *pOldPen=dc.SelectObject(&newpen);		//������ѡ��DC��SelectObject����������ǰ����
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
		GLinTmpFile2.Seek(lendata,CFile::begin);     //���ݴ��ļ�ĩβд

		int lenndx;

		int lenofl=sizeof(LIN_NDX_STRU);        //�����ṹ����
		int lenofp = sizeof(D_DOT);           //��ṹ����

		lenndx=GLinTmpFile1.GetLength();
		GLinTmpFile1.Seek(lenndx,CFile::begin);     //�������ļ�ĩβд

		//д��ǰ������
		LIN_NDX_STRU.color=0;
		LIN_NDX_STRU.datOff= GLinTmpFile2.GetLength();       //���ݴ洢��ʼλ�� һ����ṹ����*�����*�ߺ�
		LIN_NDX_STRU.dotNum=LinPntNum;
		LIN_NDX_STRU.isDel=0;
		LIN_NDX_STRU.pattern=0;
		GLinTmpFile1.Write(&LIN_NDX_STRU,lenofl);				//���ṹ��д���ļ�

		int i=0;
		for(i=0;i < LinPntNum ; i++)
		{
		
			D_DOT.x=linPnt[i].x;
			D_DOT.y=linPnt[i].y;
			GLinTmpFile2.Write(&D_DOT,lenofp);				//���ݽṹ��д���ļ�

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
		//�޸������ļ�
		if(1==GLinFCreated)
		{
			datalen=GLinTmpFile2.GetLength();
			ndxlen=GLinTmpFile1.GetLength();

			GLinTmpFile1.Seek( (num -1) * linlen,CFile::begin);  //�ҵ�������ΪtoErase����
			GLinTmpFile1.Read(&LIN_NDX_STRU,  linlen);
			dotnum=LIN_NDX_STRU.dotNum;
			datff=LIN_NDX_STRU.datOff;
			GLinTmpFile2.Seek(datff,CFile::begin);           //�������ҵ�������ΪtoErase��������

			CClientDC dc(this);							//��ñ����ڻ�ǰ���ͼ
			CPen newpen(PS_SOLID,2,RGB(192,0,0));	//���컭�ʶ���
			CPen *pOldPen=dc.SelectObject(&newpen);		//������ѡ��DC��SelectObject����������ǰ����


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
		// TODO: �ڴ���������������
		GCurOperState=OPERSTATE_INPUT_PNT;  //���õ�ǰ״̬Ϊ���
		isShowPnt=1;
		isShowLin=1;
		isShowReg=1;
		
	}


	void CMapEditor10View::OnDeletePnt()
	{
		// TODO: �ڴ���������������
		GCurOperState=OPERSTATE_DEL_PNT;  //���õ�ǰ״̬Ϊɾ����
		isShowPnt=1;
		isShowLin=1;
		isShowReg=1;
	}


	void CMapEditor10View::OnMovePnt()
	{
		// TODO: �ڴ���������������
		GCurOperState=OPERSTATE_MOV_PNT;  //���õ�ǰ״̬Ϊ�ƶ�
		GisShowDeleted=0;
		isShowPnt=1;
		isShowLin=1;
		isShowReg=1;
	}


	void CMapEditor10View::OnShowDeletedPnt()
	{
		// TODO: �ڴ���������������
		GCurOperState=0;
		GisShowDeleted=1;
		isShowPnt=1;
		isShowLin=0;
		isShowReg=0;
		Invalidate();
	}


	void CMapEditor10View::OnRecoverPnt()
	{
		// TODO: �ڴ���������������
		GCurOperState=OPERSTATE_RECOVER_PNT;                         //�ָ���
		GisShowDeleted=1;
		isShowPnt=1;
		isShowLin=0;
		isShowReg=0;
	}


	void CMapEditor10View::OnSetPnt()
	{
		// TODO: �ڴ���������������
		GetPntColor(pntColor);
	}


	void CMapEditor10View::OnShowPnt()
	{
		// TODO: �ڴ���������������
		GCurOperState=0;
		GisShowDeleted=0;
		isShowPnt=1;
		isShowLin=0;
		isShowReg=0;
		Invalidate();
	}


	void CMapEditor10View::OnCreateLin()
	{
		// TODO: �ڴ���������������
		GCurOperState=OPERSTATE_INPUT_LIN;								  //������
		
	}


	void CMapEditor10View::OnMoveLin()
	{
		// TODO: �ڴ���������������
		GCurOperState=OPERSTATE_MOVE_LIN	 ;                                //�ƶ���
	
	}

	
	void CMapEditor10View::OnDelLin()
	{
		// TODO: �ڴ���������������
		GCurOperState=OPERSTATE_DEL_LIN	 	 ;                           //ɾ����
	
	}


	void CMapEditor10View::OnConnectLin()
	{
		// TODO: �ڴ���������������
		GCurOperState=OPERSTATE_CONNECT_LIN	 	 ;                           //������
	}


	void CMapEditor10View::OnSetlin()
	{
		// TODO: �ڴ���������������									//����������
			GetPntColor(linColor);
	}


	void CMapEditor10View::OnShowLin()
	{
		// TODO: �ڴ���������������									//��ʾ��
		GCurLINState=0;
		Invalidate();
	}


	void CMapEditor10View::OnShowDelLin()
	{
		// TODO: �ڴ���������������									��ʾɾ����
		GCurLINState=1;
		Invalidate();
	}


	void CMapEditor10View::OnRecoverLin()
	{
		// TODO: �ڴ���������������									�ָ���
		GCurOperState=OPERSTATE_RECOVER_LIN;  
	}



	
