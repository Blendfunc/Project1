#pragma once
//define base datastruct this file
//#include "windows.h"
#include "LinkGameBlock.h"





typedef CLinkGameBlock *				pLGBlock;


class INotify
{
	virtual void OneClickProcessCallBack(pLGBlock * pBlock)				=	0;
};



typedef struct tagCoordinate
{
	int				x;
	int				y;
}LGCoordinate;


typedef struct tagRGPosion
{
	int				nWidtg;
	int				nHwight;
	LGCoordinate	lgcpos;/*!< �������Ͻ����� */
}LGRGPosion;


typedef struct tagMainWndInfo
{
	int				xWndWidth;
	int				yWndHeight;
	LGRGPosion		gameViewArea;
	LGRGPosion		TitleArea;
	LGRGPosion		wndBottomArea;
}LGMainWndInfoSt;

//typedef struct tag