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
#include <vector>
#include <time.h>
typedef HANDLE							LGObjId;

typedef int								LGBitMapId;
#define PI 3.141592653
#define BLACK  20.0
#define YELLOW  70.0
enum LGObjType
{
	LGIMG				=				0,
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
	IMGTRUECOLOR,//只包含24位颜色数据
	OTHERDATA
};

enum COLORSPACE
{
	RGB = 0,
	LAB


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
	COLORSPACE colorSpace;//LAB,RGB
	tagBITMAPCOLORDATA & operator= (const tagBITMAPCOLORDATA & map)
	{
		pMatrixColorData = map.pMatrixColorData;
		pMatrixColorTable = map.pMatrixColorTable;
		nMatrixWidth = map.nMatrixWidth;
		nMatrixHeight = map.nMatrixHeight;
		img = map.img;
		colorSpace = map.colorSpace;
		return *this;
	}
}BITMAPCOLORDATA;//主要针对256色的结构，24位则只有颜色数据没有调色板。这里只记录图片本身的属性，至于它是由什么样操作得来的则不记录

typedef struct tagPixelData//定义了RGB
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
//20171107修改
//为了匹配矩阵运算定义rgb三个分量为double类型
typedef struct tagPixelFloatData
{
	double r;
	double g;
	double b;
	tagPixelFloatData & operator= (const tagPixelFloatData & pd)
	{
		b = pd.b;
		g = pd.g;
		r = pd.r;
		return *this;
	}
}PixelFloatData;
//
typedef struct tagLABSpace//lab色彩空间
{
	double l;
	double a;
	double b;
}LABSpace;




//包含一些数学运算s
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
		int dimension;//每一个分量为caltype类型
		/*int nType = double*/
		/*使用double存储数据*/
		tagMATRIX()
		{
			height = 0; width = 0; data = 0; dimension = 1;
		}
	}MATRIX;

	typedef struct tagElement
	{
		int dimension;//每一个分量为caltype类型
	}ELEMENT;
	//所有内存分配全部在函数外进行
	class LD_EXT_CLASS LGMathematicalOperation
	{
	public:
		//初始化
		//注：初始化调用顺序应该是2-> 1 或 3

		//1
		static LGErrorStates InitializationMATRIX(caltype value, _m_in_ _m_out_ MATRIX & matrix);//初始化矩阵中每个元素的值为给定的值
		//2
		static LGErrorStates InitializationMATRIX(_m_in_ _m_out_ MATRIX & matrix, int height, int width, int DataSize);//内存里边分配外部负责释放，这个函数主要初始化矩阵的成员，没有初始化每个元素的值

		//p是初始化矩阵的数据地址，DataSize是这个数据占用的内存大小（几个字节）
		//内存地址由外部分配，应该保证大小不会在赋值过程中溢出
		//是InitializationMATRIX(caltype value, _m_in_ _m_out_ MATRIX & matrix)的增强版
		//也是初始化矩阵中每个元素的值为给定的值
		//3
		static LGErrorStates InitializationMATRIX(void * p , int DataSize , _m_in_ _m_out_ MATRIX & matrix);//初始化
		static LGErrorStates InitializationMATRIXWithData(void * p, int DataSize, _m_in_ _m_out_ MATRIX & matrix);//初始化
		//
		
		
		static LGErrorStates MatrixMultiplication(_m_in_ MATRIX & matrix1, _m_in_ MATRIX & matrix2, _m_out_ MATRIX & matrix3);//矩阵乘法matrix1 * matrix2

		static LGErrorStates HadamardMultiplication(_m_in_ MATRIX & matrix1 , _m_in_ MATRIX & matrix2 , _m_out_ MATRIX & matrix3);//Hadamard乘积，matlab点乘

		static LGErrorStates MatrixAddition(_m_out_ MATRIX & matrix , _m_in_ MATRIX & matrix1 , _m_in_ MATRIX & matrix2);//矩阵加法

		static LGErrorStates Convolution(_m_out_ MATRIX & matrix3, _m_in_ MATRIX & matrix1, _m_in_ MATRIX & kernel);//卷积，矩阵中的元素是double类型的，也就是一维的

		static LGErrorStates AbsoluteValue(_m_out_ MATRIX & matrix, _m_in_ MATRIX & _matrix);//对_matrix中各个元素求绝对值

		static LGErrorStates Multiplication(_m_out_ MATRIX & matrix , _m_in_ MATRIX & _matrix , caltype lmd);//kA=[ka(ij)]

		static LGErrorStates Mean(_m_out_ MATRIX & matrix, _m_in_ MATRIX & _matrix);//对应matlab中的mean方法，求每列的平均数，返回一个行向量matrix

		static LGErrorStates RowVectorMean(_m_out_ caltype & r, _m_in_ MATRIX & _matrix);//求行向量的平均数
																										   //
		static LGErrorStates GaussElimination(_m_out_ MATRIX & matrix , _m_in_ MATRIX & _matrix);//高斯消元法



		//


		//定义复杂元素的运算
		static LGErrorStates HadamardMultiplicationEx(_m_in_ MATRIX & matrix1, _m_in_ MATRIX & matrix2, _m_out_ MATRIX & matrix3);
		static LGErrorStates MatrixAdditionEx(_m_out_ MATRIX & matrix, _m_in_ MATRIX & matrix1, _m_in_ MATRIX & matrix2);


		//
		static LGErrorStates CalculationVariance(_m_in_ MATRIX * _matrix, _m_out_ caltype & r);//_matrix为行向量


	};
}
class LD_EXT_CLASS LGBitMap 
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
	LGErrorStates LGLoadBitMap(LPCTSTR fileName , /*OUT*/LGBitMapId & imgId);//支持单色,16色,256色,24位位图
	LGErrorStates LGExportBitmap(LPCTSTR fileName , void * LGBitMapData);//已知颜色数据生成位图文件

	LGErrorStates LGGetColorData(LGBitMapId id , BITMAPCOLORDATA & colorData);






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

	void ShowImg(LGObjId ScreenId, int x , int y , int nWidth , int nHeight , LGObjId objId);

public:
	//adjustment effect function

	LGErrorStates LGGray255Img(LGBitMapId imgIdIn , LGBitMapId & imgIdOut);
	LGErrorStates LgBinaryzationImg(LGObjId imgIdIn, LGObjId imgIdOut);
	LGErrorStates LGBrightnessAdjust(LGObjId imgIdIn, LGObjId imgIdOut , int nRange);
	LGErrorStates LGContrastAdjust(LGObjId imgIdIn, LGObjId imgIdOut, int nRange);
	LGErrorStates LGGaussianBlur(LGBitMapId imgIdIn, LGBitMapId & imgIdOut, int nRange , double variance);//高斯模糊
	LGErrorStates LGGenerateGaussianMask();//生成一个高斯模板
	//
	LGErrorStates LGGaussianBlur2(LGBitMapId imgIdIn, LGBitMapId & imgIdOut, double nRange);//高斯模糊
	LGErrorStates LGOOctTreeColorTable(LGObjId imgIdIn, LGObjId imgIdOut, int nRange);//八叉树优化的颜色表数据，用于压缩，优化，24位转256色等等...
	LGErrorStates LGPerspectiveCollineation();//透视变换
	LGErrorStates LGEmboss(LGBitMapId imgIdIn, LGBitMapId & imgIdOut);//浮雕效果
	LGErrorStates LGNegative(LGBitMapId imgIdIn, LGBitMapId & imgIdOut);//底片效果
	LGErrorStates LGSketch1(LGBitMapId imgInId , LGBitMapId & imgIdOut);//素描效果
	LGErrorStates LGSketch2(LGBitMapId imgInId, LGBitMapId & imgIdOut);//素描效果http://blog.csdn.net/matrix_space/article/details/38709605
	LGErrorStates LGBilateralFiltering(LGBitMapId imgInId, LGBitMapId & imgIdOut , double nRange , double nRange2);//双边滤波，nRange2为值标准差，nRange为距离标准差
	LGErrorStates LGComicEffect(LGBitMapId imgInId, LGBitMapId & imgIdOut);//漫画效果

	//


	//图像卷积核运算，位图
	//输出为无调色板颜色数据
	LGErrorStates LGConvolutionOperation(LGBitMapId imgInId, LGBitMapId & imgOutId, CONVOLUTIONKERNEL * pKernel);
	//

	//geometric transformation几何变换
	LGErrorStates Symmetry1();//y=x对称
	//

	//边缘检测
	LGErrorStates LGSobelEdgeDetection();//sobel检测
	LGErrorStates LGDogEdgeDetection(LGBitMapId imgInId, LGBitMapId & imgOutId , COLORSPACE space);//Dog检测，用space参数来决定输出是LAB还是RGB
	//

	//颜色空间转换
	void RGB2Lab(double R, double G, double B, double &L, double &a, double &b);//基本的转换函数，下面的转换要依靠它。。
	void Lab2RGB(double L, double a, double b, double &R, double &G, double &B);//基本的转换函数，下面的转换要依靠它。。
	LGErrorStates LGRGB2LAB(LGBitMapId imgInId, LGBitMapId & imgOutId);
	LGErrorStates LGLAB2RGB(LGBitMapId imgInId, LGBitMapId & imgOutId);//20171126新增
	LGErrorStates LGRGB2LAB2(LGBitMapId imgInId, LGBitMapId & imgOutId);//20171127新增，采用另外一种方法进行转换
	//
	LGErrorStates LGDicePaint(LGBitMapId imgInId, LGBitMapId & imgOutId);//骰子画http://blog.csdn.net/bluecol/article/details/47702147


	LGErrorStates LGOilPaintings(LGBitMapId imgInId, LGBitMapId & imgOutId);//油画效果


	LGErrorStates LGASCIIPaint(LGBitMapId imgInId, LGBitMapId & imgOutId);//ASCII画
//http://blog.csdn.net/loving_forever_/article/details/52389862
//https://ostagram.ru/static_pages/lenta?last_days=30&locale=en
	LGErrorStates LGConvolutionalNeuralNetworkPaint();//学习生成另外一张风格的图片

	//
	//颜色量化20171128
	LGErrorStates ColorQuantization1(LGBitMapId imgInId, LGBitMapId & imgOutId , int quantizationLevel);//LAB颜色空间量化，输出为LAB色彩空间的数据，quantizationLevel为量化等级
	//
public:
	LGErrorStates FreeLGObjMem();

protected:
	std::string GetFileName(LPCTSTR filePath);
	std::string GetSuffixName(LPCTSTR fileName);
//20171109修改将保护成员函数修改为公开
public:
	void Convert(/*BITMAPCOLORDATA * pInData , BITMAPCOLORDATA ** pOutData*/LGBitMapId & imgInId , LGBitMapId & imgOutId);//将有调色板数据转换为无调色板数据（24位彩色数据）

	LGErrorStates GenerateGaussianFilter(CONVOLUTIONKERNEL * pKernel , double variance);//产生一个高斯滤波器，矩阵地址由外部分配，variance为方差
	LGErrorStates GenerateGaussianFilter2(CONVOLUTIONKERNEL & kernel, double variance , int nRange);//产生一个高斯滤波器，矩阵地址由外部分配，variance为方差 , nRange为模板直径

	LGErrorStates TwoDimensionalGaussianFunction(/*in*/double * x, /*in*/double * y, /*out*/double * r , double * variance);//二维高斯函数，variance为方差
	LGErrorStates TwoDimensionalGaussianFunction(/*in*/double * distance , /*out*/double * r, double * variance);//二维高斯函数，variance为方差

	LGErrorStates ProcessBoundary1(int nHeight , int nWidth , BITMAPCOLORDATA * data , /*out*/LGBitMapId & imgOutId , LGBitMapId imgInId);//图像滤波处理图像边缘问题，处理方法是拷贝图像对边像素，保证宽高均为奇数
	LGErrorStates ProcessBoundary2(int nHeight, int nWidth, BITMAPCOLORDATA * data, /*out*/LGBitMapId & imgOutId , LGBitMapId imgInId);//图像滤波处理图像边缘问题，处理方法是拷贝图像邻边（x轴或y轴对称）像素，保证宽高均为奇数，nHeight，nWidth分别为卷积核的高宽
	LGErrorStates GenerateBilateralFilter(CONVOLUTIONKERNEL * pKernel, double variance1, double variance2 , /*in*/LGMathematicalOp::MATRIX & matrix);//产生一个双边滤波器variance2是另一个函数（非高斯模糊）的方差，matrix为外部传进来的颜色数据（彩色，若是灰度数据，则应该依据一张已有的表计算，可以加快运算速度）
private:
	std::map<LGObjId , LGObjType>				m_ImgSet;
	std::map<LGObjId , LGObjType>				m_MemDCId;
	std::map<LGBitMapId, BITMAPCOLORDATA>			m_mapColorData;
	//在此存储图片转换的过程1-》2-》3


private:
	LGBitMapId m_id;









};
//一些几何图形
//这里定义一些结构体
typedef struct Direction
{
	LGPoint origin;//开始点的位置
	LGPoint destination;//结束点的位置
};

typedef struct TwoPointPath
{
	const LGPoint * currentPoint;
	const LGPoint * nextPoint;
};

typedef struct AllPointPath
{
	LGPoint * currentPoint;
	AllPointPath * path;
};
//
//定义一个多边形20171116
class LGPolygon
{	
public:
	LGPolygon(LGPoint & startPoint)
	{
		m_StartPoint = 0;
		m_CurrentPoint = 0;
		SetStartPoint(startPoint);
		m_EndPoint = m_StartPoint;
		//SetEndPoint(startPoint);
	}
	~LGPolygon()
	{
		Clear();
	}
	//哈哈这里测试一下
	void Test()
	{
		srand(time(NULL));
		int i = 0;
		int n = 45;
		int m = 521;
		int pos = n;
		int dis = m - n + 1;
		int rn = rand() % dis + pos;

		int j = 0;
		int k = 800;
		int l = 890;
		int _pos = k;
		int _dis = l - k + 1;
		int aaa = rand();
		int bbb = aaa % _dis;
		int _rn = bbb + _pos;
		i = 0;
		n = _rn;
		m = 900;
		pos = n;
		dis = m - n + 1;
		int ry = rand() % dis + pos;
		while (i < 1)
		{
			int n = rn;
			int m = ry;
			int pos = n;
			int dis = m - n + 1;
			int x = rand() % dis + pos;
			int y = rand() % dis + pos;
			LGPoint point;
			point.x = x;
			point.y = y;
			if (AddPoint(&point) == false)
				i--;
			i++;
		}
		
	}
public:
	
	bool AddPoint(LGPoint * point);//默认连接上一个添加过的点
	bool GetPolygon(AllPointPath & path);//获取添加的点连接成的多边形（次序关系），这里觉得这块的内存应该由外部分配
	bool GetNextPoint(LGPoint & point , LGPoint & nextPoint);//获取给出的point的下一个点
	const LGPoint * GetNextPoint();
	const LGPoint * GetLastPoint();
	bool GetLastPoint(LGPoint & point , LGPoint & lastPoint);//获取给出的point的上一个点
	bool GetStartPoint(LGPoint & startPoint);
	bool GetEndPoint(LGPoint & endPoint);
	
private:
	LGPoint * m_CurrentPoint;
protected:
	std::vector<LGPoint *> memory;
	std::map<LGPoint *, LGPoint *> map;//对应关系：点——》下一个点，这里只记录大于等于两个以上的点
	LGPoint * m_StartPoint;//起始点
	LGPoint * m_EndPoint;//
	void SetStartPoint(LGPoint & point);
	void SetEndPoint(LGPoint & point);
	LGPoint * GetNewPoint(double x, double y);
	void Clear();//清空内存
};
class LD_EXT_CLASS LGGeometryControl 
{
	
public:
	LGErrorStates GetRectIntersect(LGRECT & inRect1 , LGRECT & inRect2 , LGRECT & outRect);
	LGErrorStates GetLineIntersect(LGLine & line1 , LGLine & line2 , LGPoint & point);
	bool IsPtInRect(LGRECT & rect , LGPoint & point);
	LGErrorStates GetOrthogonalLineIntersect(LGLine & line1 , LGLine & line2 , LGPoint & point);
	//circle1是小圆的半径，circle2是大圆的半径
	//x,y为输出的坐标
	//sum为总的滚动长度，stepSize应该小于sum
	LGErrorStates LGHypotrochoid(double circle1, double circle2, double h, double & x, double & y, double stepSize, double & sum, double & X, double & Y);//圆内旋轮线，点在小圆的内部，小圆在大圆内部运行，h是小圆圆心到小圆内部点的距离
	
																																						  
	//polygon为点集，proportion为比例，只计算一个点
	LGErrorStates LGBesselCurve(LGPolygon & polygon, double & proportion , LGPoint & result);//贝塞尔曲线



private:
	bool CollatingData(LGPoint & point1 , LGPoint & point2 , LGPoint & point3 , LGPoint & point4 , LGRECT & rect);
	bool IsPointOnLine(float *k , float *b , LGPoint & point , LGRANGE & hRange , LGRANGE & vRange);
	bool GetDistance(LGPoint & point1, LGPoint & point2, double & distance);//获取两个平面直角坐标系点的距离（二维空间）
	bool GetTwoPointSlope(LGPoint & point1 , LGPoint & point2 , double & slope);//获取两点的斜率
	bool GetPoint(LGPoint & point1 , LGPoint & point2 , Direction & direction , double & step , LGPoint & point3);//获取从A点到B点（direction指定的）距离A点step距离长度的点point3
};



