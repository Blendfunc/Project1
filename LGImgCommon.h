#pragma once
/**
* @brief 公共图像控制 \n
* 类的详细概述
*/
#include "windows.h"
#include "BaseHeader.h"
#include <map>
typedef HANDLE							LGObjId;

enum LGObjType
{
	LGIMG				=				0,
	LGSCREEN,
	LGPEN,
	LGBRUSH
};


class LD_EXT_CLASS LGBitMap final
{
public:
	/**
	* @brief LGLoadBitMap
	* @param fileName    
	* @param ImgId        图像ID @
	*
	* @return 返回说明
	*        -<em>LGErrorStates</em> fail
	*
	*/
	LGErrorStates LGLoadBitMap(LPCTSTR fileName , /*OUT*/LGObjId & imgId);









	LGErrorStates LGLoadJPEG(LPCTSTR fileName, LGObjId & imgId);








	LGErrorStates LGLoadBitmapIntoMemDC(LGObjId imgId , /*OUT*/LGObjId & memDCId);
	
	
	
	/**
	* @brief GetWindowsObjHandle
	* @param LGObjId        ID @
	*
	* @return HANDLE，句柄由用户使用，由用户负责释放
	*        -<em>LGErrorStates</em> fail
	*
	*/
	HANDLE		GetWindowsObjHandle(LGObjId objId);

	//void LGLoadBitMapEx(std::wstring strFileName);

	void ShowImg(LGObjId ScreenId, int x , int y , int nWidth , int nHeight);

public:
	//adjustment effect function

	LGErrorStates LGGray255Img(LGObjId imgIdIn , LGObjId imgIdOut);
	LGErrorStates LgBinaryzationImg(LGObjId imgIdIn, LGObjId imgIdOut);
	LGErrorStates LGBrightnessAdjust(LGObjId imgIdIn, LGObjId imgIdOut , int nRange);
	LGErrorStates LGContrastAdjust(LGObjId imgIdIn, LGObjId imgIdOut, int nRange);
	LGErrorStates LGGaussianBlur(LGObjId imgIdIn, LGObjId imgIdOut, int nRange);
	//
public:
	LGErrorStates FreeLGObjMem();



private:
	std::map<LGObjId , LGObjType>				m_ImgSet;
	std::map<LGObjId , LGObjType>				m_MemDCId;
};



