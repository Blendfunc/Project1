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
	IMGTRUECOLOR//ֻ����24λ��ɫ����
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
	tagBITMAPCOLORDATA & operator= (const tagBITMAPCOLORDATA & map)
	{
		pMatrixColorData = map.pMatrixColorData;
		pMatrixColorTable = map.pMatrixColorTable;
		nMatrixWidth = map.nMatrixWidth;
		nMatrixHeight = map.nMatrixHeight;
		img = map.img;
		return *this;
	}
}BITMAPCOLORDATA;//��Ҫ���256ɫ�Ľṹ��24λ��ֻ����ɫ����û�е�ɫ�塣����ֻ��¼ͼƬ��������ԣ�����������ʲô�������������򲻼�¼

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

//����һЩ��ѧ����
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
		/*ʹ��double�洢����*/
	}MATRIX;

	//�����ڴ����ȫ���ں��������
	class LD_EXT_CLASS LGMathematicalOperation
	{
	public:

		static LGErrorStates MatrixMultiplication(_m_in_ MATRIX & matrix1, _m_in_ MATRIX & matrix2, _m_out_ MATRIX & matrix3);//����˷�

		static LGErrorStates HadamardMultiplication();//Hadamard�˻�

		static LGErrorStates MatrixAddition(_m_out_ MATRIX & matrix, _m_in_ MATRIX & matrix1, _m_in_ MATRIX & matrix2);//����ӷ�

		static LGErrorStates Convolution(_m_out_ MATRIX & matrix3, _m_in_ MATRIX & matrix1, _m_in_ MATRIX & kernel);//���

		static LGErrorStates AbsoluteValue(_m_out_ MATRIX & matrix, _m_in_ MATRIX & _matrix);//��_matrix�и���Ԫ�������ֵ
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
	* @param ImgId        ͼ��ID @
	*
	* @return ����˵��
	*        -<em>LGErrorStates</em> fail
	*
	*/
	LGErrorStates LGLoadBitMap(LPCTSTR fileName, /*OUT*/LGBitMapId & imgId);//֧�ֵ�ɫ,16ɫ,256ɫ,24λλͼ
	LGErrorStates LGExportBitmap(LPCTSTR fileName, void * LGBitMapData);//��֪��ɫ��������λͼ�ļ�

	LGErrorStates LGGetColorData(LGBitMapId id, BITMAPCOLORDATA & colorData);






	LGErrorStates LGLoadJPEG(LPCTSTR fileName, LGObjId & imgId);








	LGErrorStates LGLoadBitmapIntoMemDC(LGObjId imgId, /*OUT*/LGObjId & memDCId);



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

	void ShowImg(LGObjId ScreenId, int x, int y, int nWidth, int nHeight, LGObjId objId);

public:
	//adjustment effect function

	LGErrorStates LGGray255Img(LGBitMapId imgIdIn, LGBitMapId & imgIdOut);
	LGErrorStates LgBinaryzationImg(LGObjId imgIdIn, LGObjId imgIdOut);
	LGErrorStates LGBrightnessAdjust(LGObjId imgIdIn, LGObjId imgIdOut, int nRange);
	LGErrorStates LGContrastAdjust(LGObjId imgIdIn, LGObjId imgIdOut, int nRange);
	LGErrorStates LGGaussianBlur(LGBitMapId imgIdIn, LGBitMapId & imgIdOut, int nRange, double variance);//��˹ģ��
	LGErrorStates LGGenerateGaussianMask();//����һ����˹ģ��
										   //
	LGErrorStates LGGaussianBlur2(LGBitMapId imgIdIn, LGBitMapId & imgIdOut, double nRange);//��˹ģ��
	LGErrorStates LGOOctTreeColorTable(LGObjId imgIdIn, LGObjId imgIdOut, int nRange);//�˲����Ż�����ɫ�����ݣ�����ѹ�����Ż���24λת256ɫ�ȵ�...
	LGErrorStates LGPerspectiveCollineation();//͸�ӱ任
	LGErrorStates LGEmboss(LGBitMapId imgIdIn, LGBitMapId & imgIdOut);//����Ч��
	LGErrorStates LGNegative(LGBitMapId imgIdIn, LGBitMapId & imgIdOut);//��ƬЧ��
	LGErrorStates LGSketch1(LGBitMapId imgInId, LGBitMapId & imgIdOut);//����Ч��
	LGErrorStates LGSketch2(LGBitMapId imgInId, LGBitMapId & imgIdOut);//����Ч��http://blog.csdn.net/matrix_space/article/details/38709605
																	   //

																	   //ͼ���������㣬λͼ
																	   //���Ϊ�޵�ɫ����ɫ����
	LGErrorStates LGConvolutionOperation(LGBitMapId imgInId, LGBitMapId & imgOutId, CONVOLUTIONKERNEL * pKernel);


	LGErrorStates LGDicePaint(LGBitMapId imgInId, LGBitMapId & imgOutId);//���ӻ�http://blog.csdn.net/bluecol/article/details/47702147


	LGErrorStates LGOilPaintings(LGBitMapId imgInId, LGBitMapId & imgOutId);//�ͻ�Ч��


	LGErrorStates LGASCIIPaint(LGBitMapId imgInId, LGBitMapId & imgOutId);//ASCII��



	//http://blog.csdn.net/loving_forever_/article/details/52389862
	//https://ostagram.ru/static_pages/lenta?last_days=30&locale=en
	LGErrorStates LGConvolutionalNeuralNetworkPaint();//ѧϰ��������һ�ŷ���ͼƬ
	//
	//geometric transformation���α任
	LGErrorStates Symmetry1();//y=x�Գ�
							  //

							  //
	LGErrorStates LGSobelEdgeDetection();//sobel���

										 //


	//һЩ����Ч��
	LGErrorStates LGWaterRippleEffect();//ˮ��Ч��
public:
	LGErrorStates FreeLGObjMem();

protected:
	std::string GetFileName(LPCTSTR filePath);
	std::string GetSuffixName(LPCTSTR fileName);
protected:
	void Convert(/*BITMAPCOLORDATA * pInData , BITMAPCOLORDATA ** pOutData*/LGBitMapId & imgInId, LGBitMapId & imgOutId);//���е�ɫ������ת��Ϊ�޵�ɫ�����ݣ�24λ��ɫ���ݣ�

	LGErrorStates GenerateGaussianFilter(CONVOLUTIONKERNEL * pKernel, double variance);//����һ����˹�˲����������ַ���ⲿ���䣬varianceΪ����
	LGErrorStates TwoDimensionalGaussianFunction(/*in*/double * x, /*in*/double * y, /*out*/double * r, double * variance);//��ά��˹������varianceΪ����
	LGErrorStates ProcessBoundary1(int nHeight, int nWidth, BITMAPCOLORDATA * data, /*out*/LGBitMapId & imgOutId, LGBitMapId imgInId);//ͼ���˲�����ͼ���Ե���⣬�������ǿ���ͼ��Ա����أ���֤��߾�Ϊ����
	LGErrorStates ProcessBoundary2(int nHeight, int nWidth, BITMAPCOLORDATA * data, /*out*/LGBitMapId & imgOutId, LGBitMapId imgInId);//ͼ���˲�����ͼ���Ե���⣬�������ǿ���ͼ���ڱߣ�x���y��Գƣ����أ���֤��߾�Ϊ������nHeight��nWidth�ֱ�Ϊ����˵ĸ߿�
private:
	std::map<LGObjId, LGObjType>				m_ImgSet;
	std::map<LGObjId, LGObjType>				m_MemDCId;
	std::map<LGBitMapId, BITMAPCOLORDATA>			m_mapColorData;
	//�ڴ˴洢ͼƬת���Ĺ���1-��2-��3


private:
	LGBitMapId m_id;









};

//һЩ���㼸�ε��㷨
class LD_EXT_CLASS LGGeometryControl final
{
public:
	LGErrorStates GetRectIntersect(LGRECT & inRect1, LGRECT & inRect2, LGRECT & outRect);
	LGErrorStates GetLineIntersect(LGLine & line1, LGLine & line2, LGPoint & point);
	bool IsPtInRect(LGRECT & rect, LGPoint & point);
	LGErrorStates GetOrthogonalLineIntersect(LGLine & line1, LGLine & line2, LGPoint & point);
	//����ͼ��



	//

private:
	bool CollatingData(LGPoint & point1, LGPoint & point2, LGPoint & point3, LGPoint & point4, LGRECT & rect);
	bool IsPointOnLine(float *k, float *b, LGPoint & point, LGRANGE & hRange, LGRANGE & vRange);
};



