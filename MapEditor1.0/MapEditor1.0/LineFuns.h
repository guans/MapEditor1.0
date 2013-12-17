#pragma once
#include "stdafx.h"
#include <afx.h>
#include "FileStruct.h"
#include "MapEditor1.0View.h"
#include <math.h>
#include <vector>


//�������ܣ���㵽�߶���̾���  
//������������㣬�߶ε�1���߶ε�2��
double PointToSegDist(long x, long y, long x1, long y1, long x2, long y2)
{
	/*double cross = (x2 - x1) * (x - x1) + (y2 - y1) * (y - y1);
	if (cross <= 0) return sqrt((double)(x - x1) * (x - x1) + (y - y1) * (y - y1));

	double d2 = (x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1);
	if (cross >= d2) return sqrt((double)(x - x2) * (x - x2) + (y - y2) * (y - y2));

	double r = cross / d2;
	double px = x1 + (x2 - x1) * r;
	double py = y1 + (y2 - y1) * r;

	return sqrt((x - px) * (x - px) + (py - y1) * (py - y1));
	*/
	
	
	
    double px = x2 - x1;
    double py = y2 - y1;
    double som = px * px + py * py;
    double u =  ((x - x1) * px + (y - y1) * py) / som;
    if (u > 1) {
        u = 1;
    }
    if (u < 0) {
        u = 0;
    }
    //the closest point
    double _x = x1 + u * px;
    double _y = y1 + u * py;
    double dx = _x - x;
    double dy = _y - y;      
    double dist = sqrt(dx*dx + dy*dy);
	return dist;

}



//�������ܣ����ļ��в����������  
//������ �����꣬���
long FindNearLine(CPoint point,int &num)  
{
	int toDel=0;		//���ص�������
	//CFile linedata;
	//CFile linendx;

	double temp;
	long resultX;
	long resultY;
	long tempX;
	long tempY;
	double tempdist=0;
	double resultdist=0;

	int resultL=0;				//���շ��ص������
	int resultLine=0;			//���շ��ص������

	int ii=0;
	int j=0;
	int k=0;
	int datalen=0;
	int ndxlen=0;
	int dotnum=0;
	int datff=0;
	CPoint ptemp;
	int ptemp2=0;
	bool isStrat=0;
	int linlen=sizeof(LIN_NDX_STRU);
	int plen=sizeof(D_DOT);
	tempX=point.x;
	tempY=point.y;

	if(1==GLinFCreated)
	{
		datalen=GLinTmpFile2.GetLength();
		ndxlen=GLinTmpFile1.GetLength();

		GLinTmpFile2.Seek(0,CFile::begin);
		GLinTmpFile1.Seek(0,CFile::begin);

		for(ii=0;ii< ndxlen / linlen;ii++)   //�������е�������
		{

			GLinTmpFile1.Read(&LIN_NDX_STRU, linlen);
			resultL++;

			if(LIN_NDX_STRU.isDel==0)            //��û��ɾ������
			{
				dotnum=LIN_NDX_STRU.dotNum;
				datff=LIN_NDX_STRU.datOff;
				POINT *pp=new POINT[dotnum];
				GLinTmpFile2.Seek(datff,CFile::begin);
				for(j=0; j < dotnum ; j++)       // ĳ�������е����
				{
					GLinTmpFile2.Read(&D_DOT, plen );
					ptemp.x=D_DOT.x;
					ptemp.y=D_DOT.y;
					pp[j]=ptemp;
				}
				if(dotnum>0)
				{
					ptemp2++;;

					for(j=0; j < dotnum-1 ; j++)       // �����߷�Ϊ�߶�,��㵽�߶���̾���
					{
						k=j + 1;                    
						temp=PointToSegDist(point.x, point.y, pp[j].x, pp[j].y, pp[k].x, pp[k].y);
						if(j == 0) tempdist = temp;   //��һ���߶�
						if(temp <= tempdist)
						{
							tempdist =temp;
						}
					}
				}


				if( ptemp2==1) resultdist=tempdist;
				if(tempdist <= resultdist)
				{
					resultdist =tempdist;
					resultLine=resultL;		//���������ߺ�
				}
				isStrat=0;

				//resultdi//=tempdist//
			}

		}

		num=resultLine;
		resultLine=0;
		resultL=0;
		

	}

	return ( 1 );

}


//�������ܣ��洢ѡ������
//������Ҫ�洢���ߺ�,��Ҫ�������
long SelectLinCon(int num, POINT *v)     //�洢ѡ�����ߵ�
{	

	//	CFile linedata;
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

	if(1==GLinFCreated)
	{
		datalen=GLinTmpFile2.GetLength();
		ndxlen=GLinTmpFile1.GetLength();

		GLinTmpFile1.Seek( (num -1) * linlen,CFile::begin);  //�ҵ�������ΪtoErase����
		GLinTmpFile1.Read(&LIN_NDX_STRU,  linlen);
		dotnum=LIN_NDX_STRU.dotNum;
		datff=LIN_NDX_STRU.datOff;
		GLinTmpFile2.Seek(datff,CFile::begin);           //�������ҵ�������ΪtoErase��������

		for(j=0 ; j < dotnum ; j++)                //�ӵ�2���㿪ʼ
		{
			GLinTmpFile2.Read(&D_DOT, plen );
			todel.x=D_DOT.x;
			todel.y=D_DOT.y;
			v[j]=todel;
		}
	}

	return dotnum;


}


//�������ܣ����ļ��лָ���
//������ �ߺ�
long RecoverLin(CPoint point,int &num)  
{


	int toDel=0;		//���ص�������

	double temp;
	long resultX;
	long resultY;
	long tempX;
	long tempY;
	double tempdist=0;
	double resultdist=0;

	int resultL=0;				//���շ��ص������
	int resultLine=0;			//���շ��ص������

	int ii=0;
	int j=0;
	int k=0;
	int datalen=0;
	int ndxlen=0;
	int dotnum=0;
	int datff=0;
	CPoint ptemp;
	int ptemp2=0;
	bool isStrat=0;
	int linlen=sizeof(LIN_NDX_STRU);
	int plen=sizeof(D_DOT);
	tempX=point.x;
	tempY=point.y;
	if(1==GLinFCreated)
	{
		datalen=GLinTmpFile2.GetLength();
		ndxlen=GLinTmpFile1.GetLength();

		GLinTmpFile2.Seek(0,CFile::begin);
		GLinTmpFile1.Seek(0,CFile::begin);

		for(ii=0;ii< ndxlen / linlen;ii++)   //�������е�������
		{

			GLinTmpFile1.Read(&LIN_NDX_STRU, linlen);
			resultL++;

			//if(LIN_NDX_STRU.isDel==0)            //��û��ɾ������
			{
				dotnum=LIN_NDX_STRU.dotNum;
				datff=LIN_NDX_STRU.datOff;
				POINT *pp=new POINT[dotnum];
				GLinTmpFile2.Seek(datff,CFile::begin);
				for(j=0; j < dotnum ; j++)       // ĳ�������е����
				{
					GLinTmpFile2.Read(&D_DOT, plen );
					ptemp.x=D_DOT.x;
					ptemp.y=D_DOT.y;
					pp[j]=ptemp;
				}
				if(dotnum>0)
				{
					ptemp2++;

					for(j=0; j < dotnum-1 ; j++)       // �����߷�Ϊ�߶�,��㵽�߶���̾���
					{
						k=j + 1;                    
						temp=PointToSegDist(point.x, point.y, pp[j].x, pp[j].y, pp[k].x, pp[k].y);
						if(j == 0) tempdist = temp;   //��һ���߶�
						if(temp <= tempdist)
						{
							tempdist =temp;
						}
					}
				}


				if( ptemp2==1) resultdist=tempdist;
				if(tempdist <= resultdist)
				{
					resultdist =tempdist;
					resultLine=resultL;		//���������ߺ�
				}
				isStrat=0;

				//resultdi//=tempdist//
			}



			num=resultLine;
			resultLine=0;
			resultL=0;


			lin_ndx_stru TEM_LIN;

			int linnum=num;
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
				GLinTmpFile1.Seek( (linnum -1) * linlen,CFile::begin);
				GLinTmpFile1.Read(&TEM_LIN,linlen);
				TEM_LIN.isDel= 0;
				LIN_NDX_STRU=TEM_LIN;
				GLinTmpFile1.Seek( -linlen,CFile::current);			 //�ҵ�������ΪtoErase����

				GLinTmpFile1.Write(&LIN_NDX_STRU,linlen);


			}
			return 1;
		}	
	}
}
