#pragma once
/**
* @brief ����ͼ����� \n
* �����ϸ����
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
typedef HANDLE							LGObjId;

typedef int								LGBitMapId;
#define PI 3.141592653
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
//����
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
//�߶�
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
	IMGBMP24,//��ӦPixelData�ṹ
	IMGBMP32,
	IMGPNG,
	IMGTRUECOLOR,//ֻ����24λ��ɫ����
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
}CONVOLUTIONKERNEL;//����˾���



typedef struct tagBITMAPCOLORDATA
{
	void * pMatrixColorData;//24λ��ΪPixelData
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
}BITMAPCOLORDATA;//��Ҫ���256ɫ�Ľṹ��24λ��ֻ����ɫ����û�е�ɫ�塣����ֻ��¼ͼƬ��������ԣ�����������ʲô�������������򲻼�¼

typedef struct tagPixelData//������RGB
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
//20171107�޸�
//Ϊ��ƥ��������㶨��rgb��������Ϊdouble����
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
typedef struct tagLABSpace//labɫ�ʿռ�
{
	double l;
	double a;
	double b;
}LABSpace;




//����һЩ��ѧ����s
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
		int dimension;//ÿһ������Ϊcaltype����
		/*int nType = double*/
		/*ʹ��double�洢����*/
		tagMATRIX()
		{
			height = 0; width = 0; data = 0; dimension = 1;
		}
	}MATRIX;

	typedef struct tagElement
	{
		int dimension;//ÿһ������Ϊcaltype����
	}ELEMENT;
	//�����ڴ����ȫ���ں��������
	class LD_EXT_CLASS LGMathematicalOperation
	{
	public:
		//��ʼ��
		//ע����ʼ������˳��Ӧ����2-> 1 �� 3

		//1
		static LGErrorStates InitializationMATRIX(caltype value, _m_in_ _m_out_ MATRIX & matrix);//��ʼ��������ÿ��Ԫ�ص�ֵΪ������ֵ
		//2
		static LGErrorStates InitializationMATRIX(_m_in_ _m_out_ MATRIX & matrix, int height, int width, int DataSize);//�ڴ���߷����ⲿ�����ͷţ����������Ҫ��ʼ������ĳ�Ա��û�г�ʼ��ÿ��Ԫ�ص�ֵ

		//p�ǳ�ʼ����������ݵ�ַ��DataSize���������ռ�õ��ڴ��С�������ֽڣ�
		//�ڴ��ַ���ⲿ���䣬Ӧ�ñ�֤��С�����ڸ�ֵ���������
		//��InitializationMATRIX(caltype value, _m_in_ _m_out_ MATRIX & matrix)����ǿ��
		//3
		static LGErrorStates InitializationMATRIX(void * p , int DataSize , _m_in_ _m_out_ MATRIX & matrix);//��ʼ��
		//
		
		
		static LGErrorStates MatrixMultiplication(_m_in_ MATRIX & matrix1, _m_in_ MATRIX & matrix2, _m_out_ MATRIX & matrix3);//����˷�matrix1 * matrix2

		static LGErrorStates HadamardMultiplication(_m_in_ MATRIX & matrix1 , _m_in_ MATRIX & matrix2 , _m_out_ MATRIX & matrix3);//Hadamard�˻���matlab���

		static LGErrorStates MatrixAddition(_m_out_ MATRIX & matrix , _m_in_ MATRIX & matrix1 , _m_in_ MATRIX & matrix2);//����ӷ�

		static LGErrorStates Convolution(_m_out_ MATRIX & matrix3, _m_in_ MATRIX & matrix1, _m_in_ MATRIX & kernel);//���

		static LGErrorStates AbsoluteValue(_m_out_ MATRIX & matrix, _m_in_ MATRIX & _matrix);//��_matrix�и���Ԫ�������ֵ

		static LGErrorStates Multiplication(_m_out_ MATRIX & matrix , _m_in_ MATRIX & _matrix , caltype lmd);//kA=[ka(ij)]

		static LGErrorStates Mean(_m_out_ MATRIX & matrix, _m_in_ MATRIX & _matrix);//��Ӧmatlab�е�mean��������ÿ�е�ƽ����������һ��������matrix

		static LGErrorStates RowVectorMean(_m_out_ caltype & r, _m_in_ MATRIX & _matrix);//����������ƽ����
																										   //
		static LGErrorStates GaussElimination(_m_out_ MATRIX & matrix , _m_in_ MATRIX & _matrix);//��˹��Ԫ��



		//


		//���帴��Ԫ�ص�����
		static LGErrorStates HadamardMultiplicationEx(_m_in_ MATRIX & matrix1, _m_in_ MATRIX & matrix2, _m_out_ MATRIX & matrix3);
		static LGErrorStates MatrixAdditionEx(_m_out_ MATRIX & matrix, _m_in_ MATRIX & matrix1, _m_in_ MATRIX & matrix2);
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
	* @param ImgId        ͼ��ID @
	*
	* @return ����˵��
	*        -<em>LGErrorStates</em> fail
	*
	*/
	LGErrorStates LGLoadBitMap(LPCTSTR fileName , /*OUT*/LGBitMapId & imgId);//֧�ֵ�ɫ,16ɫ,256ɫ,24λλͼ
	LGErrorStates LGExportBitmap(LPCTSTR fileName , void * LGBitMapData);//��֪��ɫ��������λͼ�ļ�

	LGErrorStates LGGetColorData(LGBitMapId id , BITMAPCOLORDATA & colorData);






	LGErrorStates LGLoadJPEG(LPCTSTR fileName, LGObjId & imgId);








	LGErrorStates LGLoadBitmapIntoMemDC(LGObjId imgId , /*OUT*/LGObjId & memDCId);
	
	
	
	/**
	* @brief GetWindowsObjHandle
	* @param LGObjId        ID @
	*
	* @return HANDLE��������û�ʹ�ã����û������ͷ�
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
	LGErrorStates LGGaussianBlur(LGBitMapId imgIdIn, LGBitMapId & imgIdOut, int nRange , double variance);//��˹ģ��
	LGErrorStates LGGenerateGaussianMask();//����һ����˹ģ��
	//
	LGErrorStates LGGaussianBlur2(LGBitMapId imgIdIn, LGBitMapId & imgIdOut, double nRange);//��˹ģ��
	LGErrorStates LGOOctTreeColorTable(LGObjId imgIdIn, LGObjId imgIdOut, int nRange);//�˲����Ż�����ɫ�����ݣ�����ѹ�����Ż���24λת256ɫ�ȵ�...
	LGErrorStates LGPerspectiveCollineation();//͸�ӱ任
	LGErrorStates LGEmboss(LGBitMapId imgIdIn, LGBitMapId & imgIdOut);//����Ч��
	LGErrorStates LGNegative(LGBitMapId imgIdIn, LGBitMapId & imgIdOut);//��ƬЧ��
	LGErrorStates LGSketch1(LGBitMapId imgInId , LGBitMapId & imgIdOut);//����Ч��
	LGErrorStates LGSketch2(LGBitMapId imgInId, LGBitMapId & imgIdOut);//����Ч��http://blog.csdn.net/matrix_space/article/details/38709605
	LGErrorStates LGBilateralFiltering(LGBitMapId imgInId, LGBitMapId & imgIdOut , double nRange , double nRange2);//˫���˲���nRange2Ϊֵ��׼�nRangeΪ�����׼��
	//


	//ͼ���������㣬λͼ
	//���Ϊ�޵�ɫ����ɫ����
	LGErrorStates LGConvolutionOperation(LGBitMapId imgInId, LGBitMapId & imgOutId, CONVOLUTIONKERNEL * pKernel);
	//

	//geometric transformation���α任
	LGErrorStates Symmetry1();//y=x�Գ�
	//

	//
	LGErrorStates LGSobelEdgeDetection();//sobel���

	//

	//��ɫ�ռ�ת��
	LGErrorStates LGRGB2LAB(LGBitMapId imgInId, LGBitMapId & imgOutId);
	LGErrorStates LGLAB2RGB(LGBitMapId imgInId, LGBitMapId & imgOutId);//20171126����
	//
	LGErrorStates LGDicePaint(LGBitMapId imgInId, LGBitMapId & imgOutId);//���ӻ�http://blog.csdn.net/bluecol/article/details/47702147


	LGErrorStates LGOilPaintings(LGBitMapId imgInId, LGBitMapId & imgOutId);//�ͻ�Ч��


	LGErrorStates LGASCIIPaint(LGBitMapId imgInId, LGBitMapId & imgOutId);//ASCII��
//http://blog.csdn.net/loving_forever_/article/details/52389862
//https://ostagram.ru/static_pages/lenta?last_days=30&locale=en
	LGErrorStates LGConvolutionalNeuralNetworkPaint();//ѧϰ��������һ�ŷ���ͼƬ

	//
public:
	LGErrorStates FreeLGObjMem();

protected:
	std::string GetFileName(LPCTSTR filePath);
	std::string GetSuffixName(LPCTSTR fileName);
//20171109�޸Ľ�������Ա�����޸�Ϊ����
public:
	void Convert(/*BITMAPCOLORDATA * pInData , BITMAPCOLORDATA ** pOutData*/LGBitMapId & imgInId , LGBitMapId & imgOutId);//���е�ɫ������ת��Ϊ�޵�ɫ�����ݣ�24λ��ɫ���ݣ�

	LGErrorStates GenerateGaussianFilter(CONVOLUTIONKERNEL * pKernel , double variance);//����һ����˹�˲����������ַ���ⲿ���䣬varianceΪ����
	LGErrorStates TwoDimensionalGaussianFunction(/*in*/double * x, /*in*/double * y, /*out*/double * r , double * variance);//��ά��˹������varianceΪ����
	LGErrorStates TwoDimensionalGaussianFunction(/*in*/double * distance , /*out*/double * r, double * variance);//��ά��˹������varianceΪ����

	LGErrorStates ProcessBoundary1(int nHeight , int nWidth , BITMAPCOLORDATA * data , /*out*/LGBitMapId & imgOutId , LGBitMapId imgInId);//ͼ���˲�����ͼ���Ե���⣬�������ǿ���ͼ��Ա����أ���֤��߾�Ϊ����
	LGErrorStates ProcessBoundary2(int nHeight, int nWidth, BITMAPCOLORDATA * data, /*out*/LGBitMapId & imgOutId , LGBitMapId imgInId);//ͼ���˲�����ͼ���Ե���⣬�������ǿ���ͼ���ڱߣ�x���y��Գƣ����أ���֤��߾�Ϊ������nHeight��nWidth�ֱ�Ϊ����˵ĸ߿�
	LGErrorStates GenerateBilateralFilter(CONVOLUTIONKERNEL * pKernel, double variance1, double variance2 , /*in*/LGMathematicalOp::MATRIX & matrix);//����һ��˫���˲���variance2����һ���������Ǹ�˹ģ�����ķ��matrixΪ�ⲿ����������ɫ���ݣ���ɫ�����ǻҶ����ݣ���Ӧ������һ�����еı���㣬���Լӿ������ٶȣ�
private:
	std::map<LGObjId , LGObjType>				m_ImgSet;
	std::map<LGObjId , LGObjType>				m_MemDCId;
	std::map<LGBitMapId, BITMAPCOLORDATA>			m_mapColorData;
	//�ڴ˴洢ͼƬת���Ĺ���1-��2-��3


private:
	LGBitMapId m_id;









};
//һЩ����ͼ��
//���ﶨ��һЩ�ṹ��
typedef struct Direction
{
	LGPoint origin;//��ʼ���λ��
	LGPoint destination;//�������λ��
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
//����һ�������20171116
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
public:
	
	bool AddPoint(LGPoint * point);//Ĭ��������һ����ӹ��ĵ�
	bool GetPolygon(AllPointPath & path);//��ȡ��ӵĵ����ӳɵĶ���Σ������ϵ����������������ڴ�Ӧ�����ⲿ����
	bool GetNextPoint(LGPoint & point , LGPoint & nextPoint);//��ȡ������point����һ����
	const LGPoint * GetNextPoint();
	const LGPoint * GetLastPoint();
	bool GetLastPoint(LGPoint & point , LGPoint & lastPoint);//��ȡ������point����һ����
	bool GetStartPoint(LGPoint & startPoint);
	bool GetEndPoint(LGPoint & endPoint);
	
private:
	LGPoint * m_CurrentPoint;
protected:
	std::vector<LGPoint *> memory;
	std::map<LGPoint *, LGPoint *> map;//��Ӧ��ϵ���㡪������һ���㣬����ֻ��¼���ڵ����������ϵĵ�
	LGPoint * m_StartPoint;//��ʼ��
	LGPoint * m_EndPoint;//
	void SetStartPoint(LGPoint & point);
	void SetEndPoint(LGPoint & point);
	LGPoint * GetNewPoint(double x, double y);
	void Clear();//����ڴ�
};
class LD_EXT_CLASS LGGeometryControl 
{
	
public:
	LGErrorStates GetRectIntersect(LGRECT & inRect1 , LGRECT & inRect2 , LGRECT & outRect);
	LGErrorStates GetLineIntersect(LGLine & line1 , LGLine & line2 , LGPoint & point);
	bool IsPtInRect(LGRECT & rect , LGPoint & point);
	LGErrorStates GetOrthogonalLineIntersect(LGLine & line1 , LGLine & line2 , LGPoint & point);
	//circle1��СԲ�İ뾶��circle2�Ǵ�Բ�İ뾶
	//x,yΪ���������
	//sumΪ�ܵĹ������ȣ�stepSizeӦ��С��sum
	LGErrorStates LGHypotrochoid(double circle1, double circle2, double h, double & x, double & y, double stepSize, double & sum, double & X, double & Y);//Բ�������ߣ�����СԲ���ڲ���СԲ�ڴ�Բ�ڲ����У�h��СԲԲ�ĵ�СԲ�ڲ���ľ���
	
																																						  
	//polygonΪ�㼯��proportionΪ����
	LGErrorStates LGBesselCurve(LGPolygon & polygon, double & proportion , LGPoint & result);//����������



private:
	bool CollatingData(LGPoint & point1 , LGPoint & point2 , LGPoint & point3 , LGPoint & point4 , LGRECT & rect);
	bool IsPointOnLine(float *k , float *b , LGPoint & point , LGRANGE & hRange , LGRANGE & vRange);
	bool GetDistance(LGPoint & point1, LGPoint & point2, double & distance);//��ȡ����ƽ��ֱ������ϵ��ľ��루��ά�ռ䣩
	bool GetTwoPointSlope(LGPoint & point1 , LGPoint & point2 , double & slope);//��ȡ�����б��
	bool GetPoint(LGPoint & point1 , LGPoint & point2 , Direction & direction , double & step , LGPoint & point3);//��ȡ��A�㵽B�㣨directionָ���ģ�����A��step���볤�ȵĵ�point3
};



