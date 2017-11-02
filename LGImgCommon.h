#pragma once
/**
* @brief 公共图像控制 \n
* 类的详细概述
*/
#ifdef _LG_IMG_COMMON_
#define LD_EXT_CLASS __declspec(dllexport)
#else
#define LD_EXT_CLASS __declspec(dllimport)
#endif
#include "windows.h"
#include "BaseHeader.h"
#include <map>
#include <utility>  
#include <assert.h>
#include <atlconv.h>
#include <algorithm>
#include "math.h"
typedef HANDLE							LGObjId;

typedef int								LGBitMapId;
#define PI 3.141592653
enum LGObjType
{
	LGIMG = 0,
	LGSCREEN,
	LGPEN,
	LGBRUSH
};
typedef struct tagLGPoint
{
	float x;
	float y;
	tagLGPoint()
	{
		x = 0;
		y = 0;
	}
}LGPoint;
//矩形
typedef struct tagLGRECT
{
	LGPoint nLefTopPt;
	LGPoint nLeftDownPt;
	LGPoint nRightTopPt;
	LGPoint nRightDownPt;
	/*int nWidth;
	int nHeight;*/
	tagLGRECT()
	{
		/*nLefTopPt = 0; nLeftDownPt = 0; nRightTopPt = 0; nRightDownPt = 0;*/
	}
	void Clear();
	float GetWidth();
	float GetHeight();
}LGRECT;
//线段
typedef struct tagLGLine
{
	LGPoint point1;
	LGPoint point2;
}LGLine;

enum LGIMGTYPE
{
	IMGJPEG = 0,
	IMGBMP1,
	IMGBMP4,
	IMGBMP8,
	IMGBMP16,
	IMGBMP24,//对应PixelData结构
	IMGBMP32,
	IMGPNG,
	IMGTRUECOLOR//只包含24位颜色数据
};
enum DATATYPE
{
	INTEGER = 0,
	FLOATNUMBER,
	DOUBLENUMBER
};
typedef struct tagCONVOLUTIONKERNEL
{
	int nHeight;
	int nWidth;
	void * pData;
	DATATYPE type;
}CONVOLUTIONKERNEL;//卷积核矩阵



typedef struct tagBITMAPCOLORDATA
{
	void * pMatrixColorData;//24位则为PixelData
	void * pMatrixColorTable;
	int nMatrixWidth;
	int nMatrixHeight;
	LGIMGTYPE img;
	tagBITMAPCOLORDATA & operator= (const tagBITMAPCOLORDATA & map)
	{
		pMatrixColorData = map.pMatrixColorData;
		pMatrixColorTable = map.pMatrixColorTable;
		nMatrixWidth = map.nMatrixWidth;
		nMatrixHeight = map.nMatrixHeight;
		img = map.img;
		return *this;
	}
}BITMAPCOLORDATA;//主要针对256色的结构，24位则只有颜色数据没有调色板。这里只记录图片本身的属性，至于它是由什么样操作得来的则不记录

typedef struct tagPixelData
{
	byte r;
	byte g;
	byte b;
	byte reserve;
	tagPixelData & operator= (const tagPixelData & pd)
	{
		b = pd.b;
		g = pd.g;
		r = pd.r;
		reserve = pd.reserve;
		return *this;
	}
}PixelData;

//包含一些数学运算
#define USEDOUBLE
namespace LGMathematicalOp
{
#define _m_in_
#define  _m_out_

#ifdef USEDOUBLE
#define caltype double
#endif
#ifdef USEFLOAT
#define caltype float
#endif
	typedef struct tagMATRIX
	{
		int height;
		int width;
		void* data;
		/*int nType = double*/
		/*使用double存储数据*/
	}MATRIX;

	//所有内存分配全部在函数外进行
	class LD_EXT_CLASS LGMathematicalOperation
	{
	public:

		static LGErrorStates MatrixMultiplication(_m_in_ MATRIX & matrix1, _m_in_ MATRIX & matrix2, _m_out_ MATRIX & matrix3);//矩阵乘法

		static LGErrorStates HadamardMultiplication();//Hadamard乘积

		static LGErrorStates MatrixAddition(_m_out_ MATRIX & matrix, _m_in_ MATRIX & matrix1, _m_in_ MATRIX & matrix2);//矩阵加法

		static LGErrorStates Convolution(_m_out_ MATRIX & matrix3, _m_in_ MATRIX & matrix1, _m_in_ MATRIX & kernel);//卷积

		static LGErrorStates AbsoluteValue(_m_out_ MATRIX & matrix, _m_in_ MATRIX & _matrix);//对_matrix中各个元素求绝对值
	};
}
class LD_EXT_CLASS LGBitMap final
{
public:
	LGBitMap();
	~LGBitMap();
	void Test();
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
	LGErrorStates LGLoadBitMap(LPCTSTR fileName, /*OUT*/LGBitMapId & imgId);//支持单色,16色,256色,24位位图
	LGErrorStates LGExportBitmap(LPCTSTR fileName, void * LGBitMapData);//已知颜色数据生成位图文件

	LGErrorStates LGGetColorData(LGBitMapId id, BITMAPCOLORDATA & colorData);






	LGErrorStates LGLoadJPEG(LPCTSTR fileName, LGObjId & imgId);








	LGErrorStates LGLoadBitmapIntoMemDC(LGObjId imgId, /*OUT*/LGObjId & memDCId);



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

	void ShowImg(LGObjId ScreenId, int x, int y, int nWidth, int nHeight, LGObjId objId);

public:
	//adjustment effect function

	LGErrorStates LGGray255Img(LGBitMapId imgIdIn, LGBitMapId & imgIdOut);
	LGErrorStates LgBinaryzationImg(LGObjId imgIdIn, LGObjId imgIdOut);
	LGErrorStates LGBrightnessAdjust(LGObjId imgIdIn, LGObjId imgIdOut, int nRange);
	LGErrorStates LGContrastAdjust(LGObjId imgIdIn, LGObjId imgIdOut, int nRange);
	LGErrorStates LGGaussianBlur(LGBitMapId imgIdIn, LGBitMapId & imgIdOut, int nRange, double variance);//高斯模糊
	LGErrorStates LGGenerateGaussianMask();//生成一个高斯模板
										   //
	LGErrorStates LGGaussianBlur2(LGBitMapId imgIdIn, LGBitMapId & imgIdOut, double nRange);//高斯模糊
	LGErrorStates LGOOctTreeColorTable(LGObjId imgIdIn, LGObjId imgIdOut, int nRange);//八叉树优化的颜色表数据，用于压缩，优化，24位转256色等等...
	LGErrorStates LGPerspectiveCollineation();//透视变换
	LGErrorStates LGEmboss(LGBitMapId imgIdIn, LGBitMapId & imgIdOut);//浮雕效果
	LGErrorStates LGNegative(LGBitMapId imgIdIn, LGBitMapId & imgIdOut);//底片效果
	LGErrorStates LGSketch1(LGBitMapId imgInId, LGBitMapId & imgIdOut);//素描效果
	LGErrorStates LGSketch2(LGBitMapId imgInId, LGBitMapId & imgIdOut);//素描效果http://blog.csdn.net/matrix_space/article/details/38709605
																	   //

																	   //图像卷积核运算，位图
																	   //输出为无调色板颜色数据
	LGErrorStates LGConvolutionOperation(LGBitMapId imgInId, LGBitMapId & imgOutId, CONVOLUTIONKERNEL * pKernel);


	LGErrorStates LGDicePaint(LGBitMapId imgInId, LGBitMapId & imgOutId);//骰子画http://blog.csdn.net/bluecol/article/details/47702147


	LGErrorStates LGOilPaintings(LGBitMapId imgInId, LGBitMapId & imgOutId);//油画效果


	LGErrorStates LGASCIIPaint(LGBitMapId imgInId, LGBitMapId & imgOutId);//ASCII画



	//http://blog.csdn.net/loving_forever_/article/details/52389862
	//https://ostagram.ru/static_pages/lenta?last_days=30&locale=en
	LGErrorStates LGConvolutionalNeuralNetworkPaint();//学习生成另外一张风格的图片
	//
	//geometric transformation几何变换
	LGErrorStates Symmetry1();//y=x对称
							  //

							  //
	LGErrorStates LGSobelEdgeDetection();//sobel检测

										 //


	//一些物理效果
	LGErrorStates LGWaterRippleEffect();//水波效果
public:
	LGErrorStates FreeLGObjMem();

protected:
	std::string GetFileName(LPCTSTR filePath);
	std::string GetSuffixName(LPCTSTR fileName);
protected:
	void Convert(/*BITMAPCOLORDATA * pInData , BITMAPCOLORDATA ** pOutData*/LGBitMapId & imgInId, LGBitMapId & imgOutId);//将有调色板数据转换为无调色板数据（24位彩色数据）

	LGErrorStates GenerateGaussianFilter(CONVOLUTIONKERNEL * pKernel, double variance);//产生一个高斯滤波器，矩阵地址由外部分配，variance为方差
	LGErrorStates TwoDimensionalGaussianFunction(/*in*/double * x, /*in*/double * y, /*out*/double * r, double * variance);//二维高斯函数，variance为方差
	LGErrorStates ProcessBoundary1(int nHeight, int nWidth, BITMAPCOLORDATA * data, /*out*/LGBitMapId & imgOutId, LGBitMapId imgInId);//图像滤波处理图像边缘问题，处理方法是拷贝图像对边像素，保证宽高均为奇数
	LGErrorStates ProcessBoundary2(int nHeight, int nWidth, BITMAPCOLORDATA * data, /*out*/LGBitMapId & imgOutId, LGBitMapId imgInId);//图像滤波处理图像边缘问题，处理方法是拷贝图像邻边（x轴或y轴对称）像素，保证宽高均为奇数，nHeight，nWidth分别为卷积核的高宽
private:
	std::map<LGObjId, LGObjType>				m_ImgSet;
	std::map<LGObjId, LGObjType>				m_MemDCId;
	std::map<LGBitMapId, BITMAPCOLORDATA>			m_mapColorData;
	//在此存储图片转换的过程1-》2-》3


private:
	LGBitMapId m_id;









};

//一些计算几何的算法
class LD_EXT_CLASS LGGeometryControl final
{
public:
	LGErrorStates GetRectIntersect(LGRECT & inRect1, LGRECT & inRect2, LGRECT & outRect);
	LGErrorStates GetLineIntersect(LGLine & line1, LGLine & line2, LGPoint & point);
	bool IsPtInRect(LGRECT & rect, LGPoint & point);
	LGErrorStates GetOrthogonalLineIntersect(LGLine & line1, LGLine & line2, LGPoint & point);
	//分形图形



	//

private:
	bool CollatingData(LGPoint & point1, LGPoint & point2, LGPoint & point3, LGPoint & point4, LGRECT & rect);
	bool IsPointOnLine(float *k, float *b, LGPoint & point, LGRANGE & hRange, LGRANGE & vRange);
};



