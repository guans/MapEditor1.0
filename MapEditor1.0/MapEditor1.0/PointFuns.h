#pragma once
#include "stdafx.h"
#include <afx.h>
#include "FileStruct.h"
#include "MapEditor1.0View.h"


//�������ܣ����ļ��мӵ�
//������ ������
long AppendPnt(CPoint xy,COLORREF pntColor)  
	{
	
		GPntNum++,GPntLNum++;	  //����������߼������Լ�
	
		if(1==GPntTmpFOpened)
		{

		int len=GPntTmpFile.GetLength();
		GPntTmpFile.Seek(0,CFile::end);     //���ļ�ĩβд

		//�ṹ���е�����
		PNT_STRU.isDel=0;
		PNT_STRU.color=pntColor;
		PNT_STRU.pattern=0;
		PNT_STRU.x=xy.x;
		PNT_STRU.y=xy.y;
		DotVPtoDP(PNT_STRU);
		GPntTmpFile.Write(&PNT_STRU,sizeof(PNT_STRU));  //���ṹ��д���ļ�

		}

		return 1;
	}


//�������ܣ�Ѱ������㺯��1
//������ ������
long FindNearestPnt(CPoint &xy)				
	{
		// TODO: �ڴ����ʵ��Ѱ������㺯��

		int i=0;
		int temp=0;
		int len=0;
		//char xx[4];
	//	char yy[4];
		long tempX=0;
		long tempY=0;
		long tempJuli=0;
		long resultX=0;                // �ҵ���̾���ĵ�
		long resultY=0;

		len=GPntTmpFile.GetLength();
		GPntTmpFile.Seek(0,CFile::begin);

		for(i=0;i<len/sizeof(PNT_STRU);i++)   //�������еĵ�,������ĵ�
		{
			GPntTmpFile.Read(&PNT_STRU, sizeof( PNT_STRU ));
			
				DotDPtoVP(PNT_STRU);
				tempX=PNT_STRU.x;
				tempY=PNT_STRU.y;	
				temp=(tempX-xy.x) * (tempX-xy.x) + (tempY -xy.y) * (tempY -xy.y);
				if(i == 0) tempJuli = temp;
				if(temp <= tempJuli)
				{
					resultX = tempX;
					resultY = tempY;     //���������
					tempJuli=temp;
				}
			

		}
		xy.x=resultX;
		xy.y=resultY;      //�������������
		return 1;
		
	}

//�������ܣ�Ѱ������㺯��2
//������ ������,���
long FindNearestPnt(CPoint &xy,int &num)				
{
	// TODO: �ڴ����ʵ��Ѱ������㺯��

	int resultP=0;				
	int resultPnt=0;			//���շ��صĵ����

	int i=0;
	int temp=0;
	int len=0;
	char xx[4];
	char yy[4];
	long tempX=0;
	long tempY=0;
	long tempJuli=0;
	long resultX=0;                // �ҵ���̾���ĵ�
	long resultY=0;


	len=GPntTmpFile.GetLength();
	GPntTmpFile.Seek(0,CFile::begin);

	for(i=0;i<len/sizeof(PNT_STRU);i++)   //�������еĵ�,������ĵ�
	{
		GPntTmpFile.Read(&PNT_STRU, sizeof( PNT_STRU ));
		resultP++;
		//if(PNT_STRU.isDel==0)
		{
			DotDPtoVP(PNT_STRU);
			tempX=PNT_STRU.x;
			tempY=PNT_STRU.y;	
			temp=(tempX-xy.x) * (tempX-xy.x) + (tempY -xy.y) * (tempY -xy.y);
			if(i == 0) tempJuli = temp;
			if(temp <= tempJuli)
			{
				resultX = tempX;
				resultY = tempY;     //���������
				tempJuli=temp;
				resultPnt=resultP;
			}
		}
	}
	xy.x=resultX;
	xy.y=resultY;      //�������������
	
	num=resultPnt;
	resultPnt=0;
	resultP=0;
	return 1;
}

//�������ܣ�Ѱ������㺯��3
//������ ������
long FindNearestPnt(CPoint xy,long x,long y,pnt_stru *ptpnt=NULL)		 //Ѱ������㺯��2
	{
		// TODO: �ڴ����ʵ��Ѱ������㺯��
		int n=0;                       //���ز��ҵ������
		int i=0;
		int temp=0;
		int len=0;
		char xx[4];
		char yy[4];
		long tempX=0;
		long tempY=0;
		long tempJuli=0;
		long resultX=0;                // �ҵ���̾���ĵ�
		long resultY=0;

		len=GPntTmpFile.GetLength();
		GPntTmpFile.Seek(0,CFile::begin);

		for(i=0;i<len/sizeof(PNT_STRU);i++)		//�������еĵ�,������ĵ�
		{   n++;								//����ŵ���
			GPntTmpFile.Read(&PNT_STRU, sizeof( PNT_STRU ));
			
				DotDPtoVP(PNT_STRU);
				tempX=PNT_STRU.x;
				tempY=PNT_STRU.y;	
				temp=(tempX-xy.x) * (tempX-xy.x) + (tempY -xy.y) * (tempY -xy.y);
				if(i == 0) tempJuli = temp;
				if(temp <= tempJuli)
				{
					resultX = tempX;
					resultY = tempY;     //���������
					tempJuli=temp;
				}

		}
		x=resultX;
		y=resultY;      //�������������
		ptpnt->x=resultX;
		ptpnt->y=resultY;
		return n;
	}

//�������ܣ����ļ���ɾ��
//������ ������
	long DeletePnt(CPoint xy)  
	{
		GPntLNum--;
		int perPntLen=sizeof(PNT_STRU);   //��ṹ�峤��
		int n=0;                          //Ҫɾ�������
		long tempX=0;
		long tempY=0;
		int len=0;
		int i=0;
		int filenow=0;
		pnt_stru TEM_PNT;
		FindNearestPnt(xy);
		tempX=xy.x;
		tempY=xy.y;
		try{
			len=GPntTmpFile.GetLength(); //����ļ�����
		}catch(CFileException){          }

		GPntTmpFile.Seek(0,CFile::begin);

		for(i=0;i<len / perPntLen;i++)   //�������еĵ�,�������һ����
		{
			GPntTmpFile.Read(&PNT_STRU, perPntLen );
			if(PNT_STRU.isDel==0)
			{
				DotDPtoVP(PNT_STRU);
				if(PNT_STRU.x==tempX && PNT_STRU.y == tempY)
				{
					GPntTmpFile.Seek(-perPntLen,CFile::current); 
					TEM_PNT=PNT_STRU;
					TEM_PNT.isDel=1;
					DotVPtoDP(TEM_PNT);
					GPntTmpFile.Write(&TEM_PNT,perPntLen);   //�����޸ĺ������
				
				}
			}

		}
		return 1;
		
	}

	//�������ܣ����ļ���ɾ��2
//������ ������
long DeletePnt(long toErase)  
{
	GPntLNum--;

	int filelen;
	int perPntLen=sizeof(PNT_STRU);   //��ṹ�峤��
	int num=0;
	num=toErase;									//�����ɾ����������

	
	GPntTmpFile.Seek( (num -1) * perPntLen,CFile::begin);
	GPntTmpFile.Read(&PNT_STRU,perPntLen);
	GPntTmpFile.Seek(-perPntLen,CFile::current); 
	PNT_STRU.isDel=1;
	GPntTmpFile.Write(&PNT_STRU,perPntLen);   //�����޸ĺ������

	return 1;

}

//�������ܣ����ļ��лָ���1
//������ ������
	long RecoverPnt(CPoint xy)  
	{
		GPntLNum++;

		int perPntLen=sizeof(PNT_STRU);   //��ṹ�峤��
		int n=0;                          //Ҫ�ָ������
		long tempX=0;
		long tempY=0;
		int len=0;
		int i=0;
		pnt_stru TEM_PNT;
		int filenow=0;
		FindNearestPnt(xy);
		tempX=xy.x;
		tempY=xy.y;
		
		try{
			len=GPntTmpFile.GetLength(); //����ļ�����
		}catch(CFileException){          }

		GPntTmpFile.Seek(0,CFile::begin);

		for(i=0;i<len / perPntLen;i++)   //�������еĵ�,�������һ����
		{
			GPntTmpFile.Read(&PNT_STRU, perPntLen );
			DotDPtoVP(PNT_STRU);
			if(PNT_STRU.isDel==1&&PNT_STRU.x==tempX && PNT_STRU.y == tempY)
			{
				GPntTmpFile.Seek(-perPntLen,CFile::current); 
				TEM_PNT=PNT_STRU;
				TEM_PNT.isDel=0;
				DotVPtoDP(TEM_PNT);
				GPntTmpFile.Write(&TEM_PNT,perPntLen);   //�����޸ĺ������
				
			}

		}

		return 1;
		
	}

	//�������ܣ����ļ��лָ���2
//������ ���
long RecoverPnt(int toRecover)  
{
	GPntLNum++;
	int filelen;
	int perPntLen=sizeof(PNT_STRU);   //��ṹ�峤��
	int num=0;
	num=toRecover;									//�����ɾ����������

	filelen=GPntTmpFile.GetLength();
	GLinTmpFile1.Seek( (num -1) * perPntLen,CFile::begin);
	GLinTmpFile1.Read(&PNT_STRU,perPntLen);
	GPntTmpFile.Seek(-perPntLen,CFile::current); 
	PNT_STRU.isDel=0;
	GPntTmpFile.Write(&PNT_STRU,perPntLen);   //�����޸ĺ������
	return 1;

}


//�������ܣ����ļ����ƶ���1
//������ �����꣬Ҫ�ƶ�������

	long MovePnt(CPoint xy,CPoint toMove)  //// 
	{	
		int perPntLen=sizeof(PNT_STRU);			//��ṹ�峤��
		double tempX=0;
		double tempY=0;
		int len=0;
		int i=0;
		int filenow=0;
		pnt_stru TEM_PNT;
		
	    
		//FindNearestPnt(toMove);
		tempX=toMove.x;
		tempY=toMove.y;							 //Ҫ�ƶ��ĵ�
		len=GPntTmpFile.GetLength();
		GPntTmpFile.Seek(0,CFile::begin);

		for(i=0;i<len / perPntLen;i++)					//�������еĵ�,�������һ����
		{
			GPntTmpFile.Read(&PNT_STRU, perPntLen);
			DotDPtoVP(PNT_STRU);
			if(PNT_STRU.isDel==0 && PNT_STRU.x==tempX && PNT_STRU.y == tempY)
			{
				GPntTmpFile.Seek(-perPntLen,CFile::current); 
				TEM_PNT=PNT_STRU;
				TEM_PNT.x=xy.x;
				TEM_PNT.y=xy.y;							//�޸ĺ������
				DotVPtoDP(TEM_PNT);
				GPntTmpFile.Write(&TEM_PNT,perPntLen);			//�����޸ĺ������
			}
		}
		return (1);
	}

	//�������ܣ����ļ����ƶ���2
//������ �ƶ�������꣬Ҫ�ƶ����

long MovePnt(CPoint xy,long toMove)  //// 
{	
	int filelen;
	int perPntLen=sizeof(PNT_STRU);   //��ṹ�峤��
	int num=0;
	num=toMove;									//������ƶ���������
	

	GPntTmpFile.Seek( (num -1) * perPntLen,CFile::begin);
	GPntTmpFile.Read(&PNT_STRU,perPntLen);

	PNT_STRU.x=xy.x;
	PNT_STRU.y=xy.y;
	GPntTmpFile.Seek(-perPntLen,CFile::current); 
	GPntTmpFile.Write(&PNT_STRU,perPntLen);			//�����޸ĺ������

	return (1);
}

	