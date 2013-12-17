#pragma once
#include "stdafx.h"
#include <afx.h>
#include "FileStruct.h"
#include "MapEditor1.0View.h"


//函数功能：在文件中加点
//参数： 点坐标
long AppendPnt(CPoint xy,COLORREF pntColor)  
	{
	
		GPntNum++,GPntLNum++;	  //物理点数，逻辑点数自加
	
		if(1==GPntTmpFOpened)
		{

		int len=GPntTmpFile.GetLength();
		GPntTmpFile.Seek(0,CFile::end);     //从文件末尾写

		//结构体中点数据
		PNT_STRU.isDel=0;
		PNT_STRU.color=pntColor;
		PNT_STRU.pattern=0;
		PNT_STRU.x=xy.x;
		PNT_STRU.y=xy.y;
		DotVPtoDP(PNT_STRU);
		GPntTmpFile.Write(&PNT_STRU,sizeof(PNT_STRU));  //讲结构体写入文件

		}

		return 1;
	}


//函数功能：寻找最近点函数1
//参数： 点坐标
long FindNearestPnt(CPoint &xy)				
	{
		// TODO: 在此添加实现寻找最近点函数

		int i=0;
		int temp=0;
		int len=0;
		//char xx[4];
	//	char yy[4];
		long tempX=0;
		long tempY=0;
		long tempJuli=0;
		long resultX=0;                // 找到最短距离的点
		long resultY=0;

		len=GPntTmpFile.GetLength();
		GPntTmpFile.Seek(0,CFile::begin);

		for(i=0;i<len/sizeof(PNT_STRU);i++)   //遍历所有的点,找最近的点
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
					resultY = tempY;     //保存最近点
					tempJuli=temp;
				}
			

		}
		xy.x=resultX;
		xy.y=resultY;      //返回最近点坐标
		return 1;
		
	}

//函数功能：寻找最近点函数2
//参数： 点坐标,点号
long FindNearestPnt(CPoint &xy,int &num)				
{
	// TODO: 在此添加实现寻找最近点函数

	int resultP=0;				
	int resultPnt=0;			//最终返回的点序号

	int i=0;
	int temp=0;
	int len=0;
	char xx[4];
	char yy[4];
	long tempX=0;
	long tempY=0;
	long tempJuli=0;
	long resultX=0;                // 找到最短距离的点
	long resultY=0;


	len=GPntTmpFile.GetLength();
	GPntTmpFile.Seek(0,CFile::begin);

	for(i=0;i<len/sizeof(PNT_STRU);i++)   //遍历所有的点,找最近的点
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
				resultY = tempY;     //保存最近点
				tempJuli=temp;
				resultPnt=resultP;
			}
		}
	}
	xy.x=resultX;
	xy.y=resultY;      //返回最近点坐标
	
	num=resultPnt;
	resultPnt=0;
	resultP=0;
	return 1;
}

//函数功能：寻找最近点函数3
//参数： 点坐标
long FindNearestPnt(CPoint xy,long x,long y,pnt_stru *ptpnt=NULL)		 //寻找最近点函数2
	{
		// TODO: 在此添加实现寻找最近点函数
		int n=0;                       //返回查找到的序号
		int i=0;
		int temp=0;
		int len=0;
		char xx[4];
		char yy[4];
		long tempX=0;
		long tempY=0;
		long tempJuli=0;
		long resultX=0;                // 找到最短距离的点
		long resultY=0;

		len=GPntTmpFile.GetLength();
		GPntTmpFile.Seek(0,CFile::begin);

		for(i=0;i<len/sizeof(PNT_STRU);i++)		//遍历所有的点,找最近的点
		{   n++;								//点序号递增
			GPntTmpFile.Read(&PNT_STRU, sizeof( PNT_STRU ));
			
				DotDPtoVP(PNT_STRU);
				tempX=PNT_STRU.x;
				tempY=PNT_STRU.y;	
				temp=(tempX-xy.x) * (tempX-xy.x) + (tempY -xy.y) * (tempY -xy.y);
				if(i == 0) tempJuli = temp;
				if(temp <= tempJuli)
				{
					resultX = tempX;
					resultY = tempY;     //保存最近点
					tempJuli=temp;
				}

		}
		x=resultX;
		y=resultY;      //返回最近点坐标
		ptpnt->x=resultX;
		ptpnt->y=resultY;
		return n;
	}

//函数功能：在文件中删点
//参数： 点坐标
	long DeletePnt(CPoint xy)  
	{
		GPntLNum--;
		int perPntLen=sizeof(PNT_STRU);   //点结构体长度
		int n=0;                          //要删除的序号
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
			len=GPntTmpFile.GetLength(); //获得文件长度
		}catch(CFileException){          }

		GPntTmpFile.Seek(0,CFile::begin);

		for(i=0;i<len / perPntLen;i++)   //遍历所有的点,找最近的一个点
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
					GPntTmpFile.Write(&TEM_PNT,perPntLen);   //填入修改后的数据
				
				}
			}

		}
		return 1;
		
	}

	//函数功能：在文件中删点2
//参数： 点坐标
long DeletePnt(long toErase)  
{
	GPntLNum--;

	int filelen;
	int perPntLen=sizeof(PNT_STRU);   //点结构体长度
	int num=0;
	num=toErase;									//保存待删除线索引号

	
	GPntTmpFile.Seek( (num -1) * perPntLen,CFile::begin);
	GPntTmpFile.Read(&PNT_STRU,perPntLen);
	GPntTmpFile.Seek(-perPntLen,CFile::current); 
	PNT_STRU.isDel=1;
	GPntTmpFile.Write(&PNT_STRU,perPntLen);   //填入修改后的数据

	return 1;

}

//函数功能：在文件中恢复点1
//参数： 点坐标
	long RecoverPnt(CPoint xy)  
	{
		GPntLNum++;

		int perPntLen=sizeof(PNT_STRU);   //点结构体长度
		int n=0;                          //要恢复的序号
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
			len=GPntTmpFile.GetLength(); //获得文件长度
		}catch(CFileException){          }

		GPntTmpFile.Seek(0,CFile::begin);

		for(i=0;i<len / perPntLen;i++)   //遍历所有的点,找最近的一个点
		{
			GPntTmpFile.Read(&PNT_STRU, perPntLen );
			DotDPtoVP(PNT_STRU);
			if(PNT_STRU.isDel==1&&PNT_STRU.x==tempX && PNT_STRU.y == tempY)
			{
				GPntTmpFile.Seek(-perPntLen,CFile::current); 
				TEM_PNT=PNT_STRU;
				TEM_PNT.isDel=0;
				DotVPtoDP(TEM_PNT);
				GPntTmpFile.Write(&TEM_PNT,perPntLen);   //填入修改后的数据
				
			}

		}

		return 1;
		
	}

	//函数功能：在文件中恢复点2
//参数： 点号
long RecoverPnt(int toRecover)  
{
	GPntLNum++;
	int filelen;
	int perPntLen=sizeof(PNT_STRU);   //点结构体长度
	int num=0;
	num=toRecover;									//保存待删除线索引号

	filelen=GPntTmpFile.GetLength();
	GLinTmpFile1.Seek( (num -1) * perPntLen,CFile::begin);
	GLinTmpFile1.Read(&PNT_STRU,perPntLen);
	GPntTmpFile.Seek(-perPntLen,CFile::current); 
	PNT_STRU.isDel=0;
	GPntTmpFile.Write(&PNT_STRU,perPntLen);   //填入修改后的数据
	return 1;

}


//函数功能：在文件中移动点1
//参数： 点坐标，要移动点坐标

	long MovePnt(CPoint xy,CPoint toMove)  //// 
	{	
		int perPntLen=sizeof(PNT_STRU);			//点结构体长度
		double tempX=0;
		double tempY=0;
		int len=0;
		int i=0;
		int filenow=0;
		pnt_stru TEM_PNT;
		
	    
		//FindNearestPnt(toMove);
		tempX=toMove.x;
		tempY=toMove.y;							 //要移动的点
		len=GPntTmpFile.GetLength();
		GPntTmpFile.Seek(0,CFile::begin);

		for(i=0;i<len / perPntLen;i++)					//遍历所有的点,找最近的一个点
		{
			GPntTmpFile.Read(&PNT_STRU, perPntLen);
			DotDPtoVP(PNT_STRU);
			if(PNT_STRU.isDel==0 && PNT_STRU.x==tempX && PNT_STRU.y == tempY)
			{
				GPntTmpFile.Seek(-perPntLen,CFile::current); 
				TEM_PNT=PNT_STRU;
				TEM_PNT.x=xy.x;
				TEM_PNT.y=xy.y;							//修改后的坐标
				DotVPtoDP(TEM_PNT);
				GPntTmpFile.Write(&TEM_PNT,perPntLen);			//填入修改后的数据
			}
		}
		return (1);
	}

	//函数功能：在文件中移动点2
//参数： 移动后点坐标，要移动点号

long MovePnt(CPoint xy,long toMove)  //// 
{	
	int filelen;
	int perPntLen=sizeof(PNT_STRU);   //点结构体长度
	int num=0;
	num=toMove;									//保存待移动线索引号
	

	GPntTmpFile.Seek( (num -1) * perPntLen,CFile::begin);
	GPntTmpFile.Read(&PNT_STRU,perPntLen);

	PNT_STRU.x=xy.x;
	PNT_STRU.y=xy.y;
	GPntTmpFile.Seek(-perPntLen,CFile::current); 
	GPntTmpFile.Write(&PNT_STRU,perPntLen);			//填入修改后的数据

	return (1);
}

	