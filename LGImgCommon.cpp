#define _CRT_SECURE_NO_WARNINGS
//#define LGTESTMODE
#include "LGImgCommon.h"

LGBitMap::LGBitMap()
{
	m_id = 0;
}
LGBitMap::~LGBitMap()
{
}
void LGBitMap::Test()
{
	//fread();
	FILE * pFile = fopen("C:\\Users\\Public\\Pictures\\Sample Pictures\\testimg256.bmp", "rb+");
	assert(pFile);
	int nPalette = sizeof(RGBQUAD) * 256;//256
	int nBitmapInfo = sizeof(BITMAPINFOHEADER) + sizeof(BITMAPFILEHEADER);
	fseek(pFile, nBitmapInfo + sizeof(RGBQUAD) * 232/*+ nPalette - 1*/, SEEK_SET);

	void * buffer = malloc(100);
	fread(buffer, 100, 1, pFile);
	int * nrgb = (int *)buffer;
	//byte b1 = 
	fclose(pFile);
	GetFileName(TEXT("C:\\Program Files\\Everything\\123\\456\\789\\3.exe"));
	LGBitMapId id;
	LGLoadBitMap(TEXT("C:\\Users\\Public\\Pictures\\Sample Pictures\\testimg256.bmp"), id);
}

LGErrorStates LGBitMap::LGLoadBitMap(LPCTSTR fileName, LGBitMapId & imgId)
{
	//return LG_ERR_NOINTERSECT;
	m_id++;
	USES_CONVERSION;
	std::string strName = GetFileName(fileName);
	std::string suffix = GetSuffixName(A2T((LPSTR)strName.data()));
	transform(suffix.begin(), suffix.end(), suffix.begin(), toupper);
	if (suffix.compare("BMP") == 0)
	{
		//is a bmp
	}
	else
	{
		return LG_ERR_ILLEGALiMG;
	}
	//开始解析图像
	//顺序 fileheader->infoheader->palette->colordata
	BITMAPFILEHEADER bmFileHeader;
	BITMAPINFOHEADER bmInfoHeader;
	long offset = 0;
	memset(&bmFileHeader, 0, sizeof(BITMAPFILEHEADER));
	FILE * file = fopen(T2A((LPTSTR)fileName), "rb+");
	if (NULL == file)
	{
		return LG_ERR_ILLEGALiMG;
	}
	fseek(file, offset, SEEK_SET);
	fread(&bmFileHeader, sizeof(BITMAPFILEHEADER), 1, file);
	//从文件头判断图片类型
	if (bmFileHeader.bfType == 19778)
	{
		//'BM'
	}
	else
	{
		return LG_ERR_ILLEGALiMG;
	}

	//
	offset = offset + sizeof(BITMAPFILEHEADER);
	fseek(file, offset, SEEK_SET);
	fread(&bmInfoHeader, sizeof(BITMAPINFOHEADER), 1, file);
	if (bmInfoHeader.biCompression == BI_RGB)
	{
		//assert(0);
	}
	if (bmInfoHeader.biBitCount == 1)
	{
		//单色
	}
	else if (bmInfoHeader.biBitCount == 4)
	{
		//16色
	}
	else if (bmInfoHeader.biBitCount == 8)
	{
		//256色
		/*****************************************************************************/
		if (bmInfoHeader.biHeight > 0)
		{
		}
		else
		{
			//需要重新调整位图显示位置
			//<0的时候显示是正常的，不需要做翻转
			assert(0);
		}
		/*****************************************************************************/
		offset = offset + sizeof(BITMAPINFOHEADER);
		void * pColorTable = malloc(sizeof(RGBQUAD) * 256);
		void * pMatrixColorData = malloc(bmInfoHeader.biWidth * bmInfoHeader.biHeight);
		fseek(file, offset, SEEK_SET);
		fread(pColorTable, sizeof(RGBQUAD) * 256, 1, file);
		//读取颜色数据，注意位图的真实宽度，读取的颜色是从左下角开始的，转到左上角
		int nBlank = 0;
		if (bmInfoHeader.biWidth % 4 != 0)
		{
			nBlank = 4 - (bmInfoHeader.biWidth % 4);
		}
		for (int i = 0; i < bmInfoHeader.biHeight; i++)
		{
			for (int j = 0; j < bmInfoHeader.biWidth + nBlank; j++)
			{
				if (j > bmInfoHeader.biWidth - 1)
				{
					//这里千万要注意要调用fread，以免造成位图显示不正常
					byte color;
					fread(&color, sizeof(byte), 1, file);
				}
				else
				{
					byte color;
					fread(&color, sizeof(byte), 1, file);
					int pCur = (int)pMatrixColorData;
					int ii = bmInfoHeader.biHeight - 1 - i;
					pCur = pCur + ((j * bmInfoHeader.biHeight) + ii);
					memcpy((void *)pCur, &color, sizeof(byte));
				}
			}
		}
		BITMAPCOLORDATA data;
		data.img = IMGBMP8;
		data.nMatrixWidth = bmInfoHeader.biWidth;
		data.nMatrixHeight = bmInfoHeader.biHeight;
		data.pMatrixColorData = pMatrixColorData;
		data.pMatrixColorTable = pColorTable;
		m_mapColorData.insert(std::pair<LGBitMapId, BITMAPCOLORDATA>(m_id, data));
#ifdef LGTESTMODE
		for (int i = 0; i < bmInfoHeader.biHeight; i++)
		{
			for (int j = 0; j < bmInfoHeader.biWidth; j++)
			{
				int pCur = (int)pMatrixColorData;
				pCur = pCur + ((j * bmInfoHeader.biHeight) + i);
				byte* pb = (byte *)pCur;
				printf("%d", *pb);
				printf("-----------------");
			}
			printf("\n");
		}
#endif
		imgId = m_id;
		/*m_id++;*/
	}
	else if (bmInfoHeader.biBitCount == 16)
	{
		//buzhichi
	}
	else if (bmInfoHeader.biBitCount == 24)
	{
		//24位
		offset = offset + sizeof(BITMAPINFOHEADER);
		void * pMatrixColorData = malloc(bmInfoHeader.biWidth * bmInfoHeader.biHeight * sizeof(PixelData));
		fseek(file, offset, SEEK_SET);
		//fread(pMatrixColorData, sizeof(RGBQUAD) * 256, 1, file);
		//读取颜色数据，注意位图的真实宽度，读取的颜色是从左下角开始的，转到左上角
		if (bmInfoHeader.biHeight > 0)
		{
		}
		else
		{
			//需要重新调整位图显示位置
			//<0的时候显示是正常的，不需要做翻转
			assert(0);
		}
		int nBlank = 0;
		if ((bmInfoHeader.biWidth * (sizeof(byte) * 3)) % 4 != 0)
		{
			nBlank = 4 - ((bmInfoHeader.biWidth * (sizeof(byte) * 3)) % 4);
		}
		for (int i = 0; i < bmInfoHeader.biHeight; i++)
		{
			for (int j = 0; j < (bmInfoHeader.biWidth * (sizeof(byte) * 3)) + nBlank; j++)
			{
				if (j >(bmInfoHeader.biWidth * (sizeof(byte) * 3)) - 1)
				{
					//assert(0);
					//这里千万要注意要调用fread，以免造成位图显示不正常
					byte color;//bgr
					fread(&color, sizeof(byte), 1, file);
				}
				else
				{
					byte color;//bgr
					fread(&color, sizeof(byte), 1, file);
					int nAddress = (int)pMatrixColorData;
					int x = j / (sizeof(byte) * 3);
					int y = bmInfoHeader.biHeight - 1 - i;
					nAddress = nAddress + (((x * bmInfoHeader.biHeight) + y) * sizeof(PixelData));
					PixelData * ppd = (PixelData *)nAddress;
					int nIndex = j % (sizeof(byte) * 3);
					if (nIndex == 0)
					{
						//b
						ppd->b = color;
					}
					if (nIndex == 1)
					{
						//g
						ppd->g = color;
					}
					if (nIndex == 2)
					{
						//r
						ppd->r = color;
					}
				}
			}
		}
		BITMAPCOLORDATA data;
		data.img = IMGBMP24;
		data.nMatrixWidth = bmInfoHeader.biWidth;
		data.nMatrixHeight = bmInfoHeader.biHeight;
		data.pMatrixColorData = pMatrixColorData;
		data.pMatrixColorTable = 0;
		m_mapColorData.insert(std::pair<LGBitMapId, BITMAPCOLORDATA>(m_id, data));
#ifdef LGTESTMODE
		for (int i = 0; i < bmInfoHeader.biHeight; i++)
		{
			for (int j = 0; j < bmInfoHeader.biWidth; j++)
			{
				int pCur = (int)pMatrixColorData;
				pCur = pCur + ((j * bmInfoHeader.biHeight) + i);
				byte* pb = (byte *)pCur;
				printf("%d", *pb);
				printf("-----------------");
			}
			printf("\n");
		}
#endif
		imgId = m_id;
		/*m_id++;*/
	}
	else if (bmInfoHeader.biBitCount == 32)
	{
		//buzhichi
	}
	fclose(file);
}

LGErrorStates LGBitMap::LGGetColorData(LGBitMapId id, BITMAPCOLORDATA & colorData)
{
	if (m_mapColorData.count(id))
	{
		colorData = m_mapColorData[id];
		return LG_ERR_OTHER;
	}
	else
	{
		return LG_ERR_ERRID;
	}
}

HANDLE LGBitMap::GetWindowsObjHandle(LGObjId objId)
{
	if (m_ImgSet.count(objId))
	{
		m_ImgSet.erase(objId);
	}
	else if (m_MemDCId.count(objId))
	{
	}
	return 0;
}

void tagLGRECT::Clear()
{
}

float tagLGRECT::GetWidth()
{
	return nRightDownPt.x - nLeftDownPt.x;
}

float tagLGRECT::GetHeight()
{
	return nLeftDownPt.y - nLefTopPt.y;
}

LGErrorStates LGGeometryControl::GetRectIntersect(LGRECT & inRect1, LGRECT & inRect2, LGRECT & outRect)
{
	typedef struct PtInfo
	{
		LGPoint pt;
		bool isAssign;
		PtInfo()
		{
			isAssign = false;
		}
	};
	LGLine line1, _line1; LGLine line1v, _line1v;
	LGLine line2, _line2; LGLine line2h, _line2h;
	line2.point1.x = inRect2.nLefTopPt.x;
	line2.point1.y = inRect2.nLefTopPt.y;
	line2.point2.x = inRect2.nLeftDownPt.x;
	line2.point2.y = inRect2.nLeftDownPt.y;

	_line2.point1.x = inRect2.nRightTopPt.x;
	_line2.point1.y = inRect2.nRightTopPt.y;
	_line2.point2.x = inRect2.nRightDownPt.x;
	_line2.point2.y = inRect2.nRightDownPt.y;

	line2h.point1.x = inRect2.nLefTopPt.x;
	line2h.point1.y = inRect2.nLefTopPt.y;
	line2h.point2.x = inRect2.nRightTopPt.x;
	line2h.point2.y = inRect2.nRightTopPt.y;

	_line2h.point1.x = inRect2.nLeftDownPt.x;
	_line2h.point1.y = inRect2.nLeftDownPt.y;
	_line2h.point2.x = inRect2.nRightDownPt.x;
	_line2h.point2.y = inRect2.nRightDownPt.y;
	///////////////////////////////////////////////////////////////////
	line1.point1.x = inRect1.nLefTopPt.x;
	line1.point1.y = inRect1.nLefTopPt.y;
	line1.point2.x = inRect1.nRightTopPt.x;
	line1.point2.y = inRect1.nRightTopPt.y;

	_line1.point1.x = inRect1.nLeftDownPt.x;
	_line1.point1.y = inRect1.nLeftDownPt.y;
	_line1.point2.x = inRect1.nRightDownPt.x;
	_line1.point2.y = inRect1.nRightDownPt.y;

	line1v.point1.x = inRect1.nLefTopPt.x;
	line1v.point1.y = inRect1.nLefTopPt.y;
	line1v.point2.x = inRect1.nLeftDownPt.x;
	line1v.point2.y = inRect1.nLeftDownPt.y;

	_line1v.point1.x = inRect1.nRightTopPt.x;
	_line1v.point1.y = inRect1.nRightTopPt.y;
	_line1v.point2.x = inRect1.nRightDownPt.x;
	_line1v.point2.y = inRect1.nRightDownPt.y;

	PtInfo pt1, pt2, pt3, pt4;
	LGPoint _pt;
	std::map<int, PtInfo*> map;
	map.insert(std::pair<int, PtInfo*>(0, &pt1));
	map.insert(std::pair<int, PtInfo*>(1, &pt2));
	map.insert(std::pair<int, PtInfo*>(2, &pt3));
	map.insert(std::pair<int, PtInfo*>(3, &pt4));
	int _n = 0;
	if (_n != 4 && GetLineIntersect(line2, line1, _pt) == LG_ERR_HAVEINTERSECT)
	{
		_n++;
		if (map.size() >= _n)
		{
			map[_n - 1]->isAssign = true;
			map[_n - 1]->pt.x = _pt.x;
			map[_n - 1]->pt.y = _pt.y;
		}
		else
		{
			assert(0);
		}
	}
	if (_n != 4 && GetLineIntersect(line2, _line1, _pt) == LG_ERR_HAVEINTERSECT)
	{
		_n++;
		if (map.size() >= _n)
		{
			map[_n - 1]->isAssign = true;
			map[_n - 1]->pt.x = _pt.x;
			map[_n - 1]->pt.y = _pt.y;
		}
		else
		{
			assert(0);
		}
	}
	if (_n != 4 && GetLineIntersect(_line2, line1, _pt) == LG_ERR_HAVEINTERSECT)
	{
		_n++;
		if (map.size() >= _n)
		{
			map[_n - 1]->isAssign = true;
			map[_n - 1]->pt.x = _pt.x;
			map[_n - 1]->pt.y = _pt.y;
		}
		else
		{
			assert(0);
		}
	}
	if (_n != 4 && GetLineIntersect(_line2, _line1, _pt) == LG_ERR_HAVEINTERSECT)
	{
		_n++;
		if (map.size() >= _n)
		{
			map[_n - 1]->isAssign = true;
			map[_n - 1]->pt.x = _pt.x;
			map[_n - 1]->pt.y = _pt.y;
		}
		else
		{
			assert(0);
		}
	}
	if (_n != 4 && GetLineIntersect(line2h, line1v, _pt) == LG_ERR_HAVEINTERSECT)
	{
		_n++;
		if (map.size() >= _n)
		{
			map[_n - 1]->isAssign = true;
			map[_n - 1]->pt.x = _pt.x;
			map[_n - 1]->pt.y = _pt.y;
		}
		else
		{
			assert(0);
		}
	}
	if (_n != 4 && GetLineIntersect(line2h, _line1v, _pt) == LG_ERR_HAVEINTERSECT)
	{
		_n++;
		if (map.size() >= _n)
		{
			map[_n - 1]->isAssign = true;
			map[_n - 1]->pt.x = _pt.x;
			map[_n - 1]->pt.y = _pt.y;
		}
		else
		{
			assert(0);
		}
	}
	if (_n != 4 && GetLineIntersect(_line2h, line1v, _pt) == LG_ERR_HAVEINTERSECT)
	{
		_n++;
		if (map.size() >= _n)
		{
			map[_n - 1]->isAssign = true;
			map[_n - 1]->pt.x = _pt.x;
			map[_n - 1]->pt.y = _pt.y;
		}
		else
		{
			assert(0);
		}
	}
	if (_n != 4 && GetLineIntersect(_line2h, _line1v, _pt) == LG_ERR_HAVEINTERSECT)
	{
		_n++;
		if (map.size() >= _n)
		{
			map[_n - 1]->isAssign = true;
			map[_n - 1]->pt.x = _pt.x;
			map[_n - 1]->pt.y = _pt.y;
		}
		else
		{
			assert(0);
		}
	}
	if (_n == 4)
	{

	}
	else if (_n == 2 || _n == 0)
	{
		if (_n != 4 && IsPtInRect(inRect1, inRect2.nLeftDownPt) == true)
		{
			_n++;
			if (map.size() >= _n)
			{
				map[_n - 1]->isAssign = true;
				map[_n - 1]->pt.x = inRect2.nLeftDownPt.x;
				map[_n - 1]->pt.y = inRect2.nLeftDownPt.y;
			}
			else
			{
				assert(0);
			}
		}
		if (_n != 4 && IsPtInRect(inRect1, inRect2.nLefTopPt) == true)
		{
			_n++;
			if (map.size() >= _n)
			{
				map[_n - 1]->isAssign = true;
				map[_n - 1]->pt.x = inRect2.nLefTopPt.x;
				map[_n - 1]->pt.y = inRect2.nLefTopPt.y;
			}
			else
			{
				assert(0);
			}
		}
		if (_n != 4 && IsPtInRect(inRect1, inRect2.nRightDownPt) == true)
		{
			_n++;
			if (map.size() >= _n)
			{
				map[_n - 1]->isAssign = true;
				map[_n - 1]->pt.x = inRect2.nRightDownPt.x;
				map[_n - 1]->pt.y = inRect2.nRightDownPt.y;
			}
			else
			{
				assert(0);
			}
		}
		if (_n != 4 && IsPtInRect(inRect1, inRect2.nRightTopPt) == true)
		{
			_n++;
			if (map.size() >= _n)
			{
				map[_n - 1]->isAssign = true;
				map[_n - 1]->pt.x = inRect2.nRightTopPt.x;
				map[_n - 1]->pt.y = inRect2.nRightTopPt.y;
			}
			else
			{
				assert(0);
			}
		}
		if (_n != 4 && IsPtInRect(inRect2, inRect1.nLeftDownPt) == true)
		{
			_n++;
			if (map.size() >= _n)
			{
				map[_n - 1]->isAssign = true;
				map[_n - 1]->pt.x = inRect1.nLeftDownPt.x;
				map[_n - 1]->pt.y = inRect1.nLeftDownPt.y;
			}
			else
			{
				assert(0);
			}
		}
		if (_n != 4 && IsPtInRect(inRect2, inRect1.nLefTopPt) == true)
		{
			_n++;
			if (map.size() >= _n)
			{
				map[_n - 1]->isAssign = true;
				map[_n - 1]->pt.x = inRect1.nLefTopPt.x;
				map[_n - 1]->pt.y = inRect1.nLefTopPt.y;
			}
			else
			{
				assert(0);
			}
		}
		if (_n != 4 && IsPtInRect(inRect2, inRect1.nRightDownPt) == true)
		{
			_n++;
			if (map.size() >= _n)
			{
				map[_n - 1]->isAssign = true;
				map[_n - 1]->pt.x = inRect1.nRightDownPt.x;
				map[_n - 1]->pt.y = inRect1.nRightDownPt.y;
			}
			else
			{
				assert(0);
			}
		}
		if (_n != 4 && IsPtInRect(inRect2, inRect1.nRightTopPt) == true)
		{
			_n++;
			if (map.size() >= _n)
			{
				map[_n - 1]->isAssign = true;
				map[_n - 1]->pt.x = inRect1.nRightTopPt.x;
				map[_n - 1]->pt.y = inRect1.nRightTopPt.y;
			}
			else
			{
				assert(0);
			}
		}
		if (_n != 4)
		{
			assert(0);
		}
	}
	else
	{
		if (_n == 1)
		{
			assert(0);
		}
		return LG_ERR_NOINTERSECT;
	}
	//检查四个点的情况，并且返回参数
	CollatingData(map[0]->pt, map[1]->pt, map[2]->pt, map[3]->pt, outRect);
}

LGErrorStates LGGeometryControl::GetLineIntersect(LGLine & line1, LGLine & line2, LGPoint & point)
{
	/********************************************************************/
	/***	//y = kx +b
	/***	//公式1
	/***	//line方程中的两个参数为(k , b)，经过的点为x1y1 , x2y2
	/***	//k = (y1-y2)/(x1-x2)
	/***	//b = ((x1*y2)-(x2*y1))/(x1-x2)
	/***	//公式2
	/***	//已知两条线求交点，交点为(x,y)，两条线参数分别为k,b和m,n
	/***	//x = (b-n)/(m-k)
	/***	//y = (kn-bm)/(k-m)
	/********************************************************************/
	bool isOrthogonal = false;
	float x = 0;
	float y = 0;
	if (line1.point1.x == line1.point2.x && line2.point1.y == line2.point2.y)
	{
		isOrthogonal = true;
	}
	else if (line1.point1.y == line1.point2.y && line2.point1.x == line2.point2.x)
	{
		isOrthogonal = true;
	}
	if (isOrthogonal == true)
	{
		LGPoint point;
		GetOrthogonalLineIntersect(line1, line2, point);
		x = point.x;
		y = point.y;
	}
	else
	{
		float k, b, m, n;
		float x1 = line1.point1.x;
		float x2 = line1.point2.x;
		float y1 = line1.point1.y;
		float y2 = line1.point2.y;
		//这里判断一下除0的错误
		if (x1 == x2)
		{
			x1 = x1 + TINYNUM;//
		}
		k = (y1 - y2) / (x1 - x2);
		b = ((x1*y2) - (x2*y1)) / (x1 - x2);

		x1 = line2.point1.x;
		x2 = line2.point2.x;
		y1 = line2.point1.y;
		y2 = line2.point2.y;
		if (x1 == x2)
		{
			x1 = x1 + TINYNUM;//
		}
		m = (y1 - y2) / (x1 - x2);
		n = ((x1*y2) - (x2*y1)) / (x1 - x2);
		//x,y为求出两条线段延长线的交点
		x = (b - n) / (m - k);
		y = (k*n - b*m) / (k - m);
	}
	//@note 判断点是否在两条线段上

	float hStart = 0;
	float hEnd = 0;
	float vStart = 0;
	float vEnd = 0;
	if ((line1.point1.x - line1.point2.x) != 0)
	{
		hStart = min(line1.point1.x, line1.point2.x);
		hEnd = max(line1.point1.x, line1.point2.x);
	}
	if ((line2.point1.x - line2.point2.x) != 0)
	{
		hStart = min(line2.point1.x, line2.point2.x);
		hEnd = max(line2.point1.x, line2.point2.x);
	}
	if ((line1.point1.y - line1.point2.y) != 0)
	{
		vStart = min(line1.point1.y, line1.point2.y);
		vEnd = max(line1.point1.y, line1.point2.y);
	}
	if ((line2.point1.y - line2.point2.y) != 0)
	{
		vStart = min(line2.point1.y, line2.point2.y);
		vEnd = max(line2.point1.y, line2.point2.y);
	}
	if (x > hStart && x < hEnd && y > vStart && y < vEnd)
	{
		point.x = x;
		point.y = y;
		return LG_ERR_HAVEINTERSECT;
	}
	else
	{
		return LG_ERR_NOINTERSECT;
	}
}

bool LGGeometryControl::IsPtInRect(LGRECT & rect, LGPoint & point)
{
	//不考虑点落在矩形边上
	if (point.x > rect.nLefTopPt.x && point.x < rect.nRightTopPt.x)
	{
		if (point.y > rect.nLeftDownPt.y && point.y < rect.nLefTopPt.y)
		{
			return true;
		}
	}
	return false;
}

LGErrorStates LGGeometryControl::GetOrthogonalLineIntersect(LGLine & line1, LGLine & line2, LGPoint & point)
{
	//这里并没有判断两条直线是否是正交的，默认是正交的
	float x = 0;
	float y = 0;
	if (line1.point1.x == line1.point2.x)
	{
		x = line1.point1.x;
		y = line2.point1.y;
	}
	else
	{
		y = line1.point1.y;
		x = line2.point1.x;
	}
	point.x = x;
	point.y = y;
	return LG_ERR_OTHER;
}

bool LGGeometryControl::CollatingData(LGPoint & point1, LGPoint & point2, LGPoint & point3, LGPoint & point4, LGRECT & rect)
{
	float maxx = 0;
	float maxy = 0;
	float minx = 0;
	float miny = 0;
	if (point1.x != point2.x)
	{
		maxx = max(point1.x, point2.x);
		minx = min(point1.x, point2.x);
	}
	else
	{
		maxx = max(point1.x, point3.x);
		minx = min(point1.x, point3.x);
	}
	if (point1.y != point2.y)
	{
		maxy = max(point1.y, point2.y);
		miny = min(point1.y, point2.y);
	}
	else
	{
		maxy = max(point1.y, point3.y);
		miny = min(point1.y, point3.y);
	}
	rect.nLeftDownPt.x = minx;
	rect.nLeftDownPt.y = miny;
	rect.nLefTopPt.x = minx;
	rect.nLefTopPt.y = maxy;
	rect.nRightDownPt.x = maxx;
	rect.nRightDownPt.y = miny;
	rect.nRightTopPt.x = maxx;
	rect.nRightTopPt.y = maxy;
	return true;
}

bool LGGeometryControl::IsPointOnLine(float * k, float * b, LGPoint & point, LGRANGE & hRange, LGRANGE & vRange)
{
	if (k || b)
	{
		return false;
	}

	if (point.y == ((point.x)*(*k)) + (*b))
	{
		return true;
	}
	return false;
}

bool LGGeometryControl::GetDistance(LGPoint & point1, LGPoint & point2, double & distance)
{
	distance = sqrt(pow(point1.x - point2.x , 2) + pow(point1.y - point2.y , 2));
	return true;
}

bool LGGeometryControl::GetTwoPointSlope(LGPoint & point1, LGPoint & point2, double & slope)
{
	if (point2.x == point1.x)
	{
		slope = (point2.y - point1.y) / (point2.x - (point1.x + TINYNUM));
		return true;
		//assert(0);
	}
	slope = (point2.y - point1.y) / (point2.x - point1.x);
	return true;
}

bool LGGeometryControl::GetPoint(LGPoint & point1, LGPoint & point2, Direction & direction, double & step, LGPoint & point3)
{
	double k = 0.0;

	GetTwoPointSlope(point1 , point2 , k);

	double x = sqrt(pow(step , 2) / (1 + pow(k , 2)));

	double y = sqrt(pow(step , 2) / (1 + (1 / pow(k , 2))));

	double _x = 0.0;
	double _y = 0.0;
	if (direction.origin.x < direction.destination.x)
	{
		_x = x;
	}
	else
	{
		_x = -x;
	}
	if (direction.origin.y < direction.destination.y)
	{
		_y = y;
	}
	else
	{
		_y = -y;
	}
	point3.x = direction.origin.x + _x;
	point3.y = direction.origin.y + _y;
	return true;
}

LGErrorStates LGBitMap::LGConvolutionOperation(LGBitMapId imgInId, LGBitMapId & imgOutId, CONVOLUTIONKERNEL * pKernel)
{
	if (pKernel == 0 || m_mapColorData.count(imgInId) == 0)
	{
		return LG_ERR_PARAM;
	}
	LGBitMapId outId;
	outId = imgInId;
	if (m_mapColorData.at(imgInId).img == IMGBMP1 || m_mapColorData.at(imgInId).img == IMGBMP4 || m_mapColorData.at(imgInId).img == IMGBMP8)
	{
		Convert(imgInId, outId);
	}

	if (outId != -1)
	{
		LGBitMapId OutExId = 0;
		BITMAPCOLORDATA data;
		memset(&data, 0, sizeof(BITMAPCOLORDATA));
		LGErrorStates err = LGGetColorData(outId, data);
		if (err != LG_ERR_ERRID)
		{
			LGErrorStates _err = ProcessBoundary2(pKernel->nHeight, pKernel->nWidth, &data, OutExId, outId);
			if (_err != LG_ERR_PARAM)
			{
				BITMAPCOLORDATA dataEx;
				memset(&dataEx, 0, sizeof(BITMAPCOLORDATA));
				LGGetColorData(OutExId, dataEx);
				BITMAPCOLORDATA processedData;
				memset(&processedData, 0, sizeof(BITMAPCOLORDATA));
				processedData.nMatrixHeight = data.nMatrixHeight;
				processedData.nMatrixWidth = data.nMatrixWidth;
				processedData.pMatrixColorData = malloc(sizeof(PixelData) * processedData.nMatrixHeight * processedData.nMatrixWidth);
				processedData.img = IMGBMP24;
				for (int i = 0; i < processedData.nMatrixHeight; i++)
				{
					for (int j = 0; j < processedData.nMatrixWidth; j++)
					{
						//计算矩阵乘积
						int x = (pKernel->nWidth / 2) + j;
						int y = (pKernel->nHeight / 2) + i;
						int leftTopX = x - (pKernel->nWidth / 2);
						int leftTopY = y - (pKernel->nHeight / 2);
						double r = 0.0;
						double g = 0.0;
						double b = 0.0;
						for (int k = 0; k < pKernel->nHeight; k++)
						{
							for (int m = 0; m < pKernel->nWidth; m++)
							{
								int processX = leftTopX + m;
								int processY = leftTopY + k;
								int nAddress = (int)dataEx.pMatrixColorData;
								nAddress = nAddress + (((processX * dataEx.nMatrixHeight) + processY) * sizeof(PixelData));
								PixelData * ppd = (PixelData *)nAddress;
								if (pKernel->type == DOUBLENUMBER)
								{
									int _nAddress = (int)pKernel->pData;
									_nAddress = _nAddress + (sizeof(double) * ((m * pKernel->nHeight) + k));
									double * pf = (double *)_nAddress;
									b = b + (ppd->b * (*pf));
									g = g + (ppd->g * (*pf));
									r = r + (ppd->r * (*pf));
								}
							}
						}
						int __nAddress = (int)processedData.pMatrixColorData;
						__nAddress = __nAddress + (sizeof(PixelData) * ((j * processedData.nMatrixHeight) + i));
						PixelData * pOutPixelData = (PixelData *)__nAddress;
						pOutPixelData->b = b;
						pOutPixelData->g = g;
						pOutPixelData->r = r;
						//
					}
				}
				m_id++;
				m_mapColorData.insert(std::pair<LGBitMapId, BITMAPCOLORDATA>(m_id, processedData));
				imgOutId = m_id;
			}
		}
		else
		{
			return LG_ERR_PARAM;
		}

	}
}

void LGBitMap::RGB2Lab(double R, double G, double B, double & L, double & a, double & b)
{
	double X, Y, Z, fX, fY, fZ;

	X = 0.412453*R + 0.357580*G + 0.180423*B;
	Y = 0.212671*R + 0.715160*G + 0.072169*B;
	Z = 0.019334*R + 0.119193*G + 0.950227*B;

	X /= (255 * 0.950456);
	Y /= 255;
	Z /= (255 * 1.088754);

	if (Y > 0.008856)
	{
		fY = pow(Y, 1.0 / 3.0);
		L = 116.0*fY - 16.0;
	}
	else
	{
		fY = 7.787*Y + 16.0 / 116.0;
		L = 903.3*Y;
	}

	if (X > 0.008856)
		fX = pow(X, 1.0 / 3.0);
	else
		fX = 7.787*X + 16.0 / 116.0;

	if (Z > 0.008856)
		fZ = pow(Z, 1.0 / 3.0);
	else
		fZ = 7.787*Z + 16.0 / 116.0;

	a = 500.0*(fX - fY);
	b = 200.0*(fY - fZ);

	if (L < BLACK)
	{
		a *= exp((L - BLACK) / (BLACK / 4));
		b *= exp((L - BLACK) / (BLACK / 4));
		L = BLACK;
	}
	if (b > YELLOW)
		b = YELLOW;
}

void LGBitMap::Lab2RGB(double L, double a, double b, double & R, double & G, double & B)
{
	double X, Y, Z, fX, fY, fZ;
	double RR, GG, BB;
	fY = pow((L + 16.0) / 116.0, 3.0);
	if (fY < 0.008856)
		fY = L / 903.3;
	Y = fY;

	if (fY > 0.008856)
		fY = pow(fY, 1.0 / 3.0);
	else
		fY = 7.787 * fY + 16.0 / 116.0;

	fX = a / 500.0 + fY;
	if (fX > 0.206893)
		X = pow(fX, 3.0);
	else
		X = (fX - 16.0 / 116.0) / 7.787;

	fZ = fY - b / 200.0;
	if (fZ > 0.206893)
		Z = pow(fZ, 3.0);
	else
		Z = (fZ - 16.0 / 116.0) / 7.787;

	X *= (0.950456 * 255);
	Y *= 255;
	Z *= (1.088754 * 255);

	RR = 3.240479*X - 1.537150*Y - 0.498535*Z;
	GG = -0.969256*X + 1.875992*Y + 0.041556*Z;
	BB = 0.055648*X - 0.204043*Y + 1.057311*Z;

	R = (float)(RR < 0 ? 0 : RR > 255 ? 255 : RR);
	G = (float)(GG < 0 ? 0 : GG > 255 ? 255 : GG);
	B = (float)(BB < 0 ? 0 : BB > 255 ? 255 : BB);
}

LGErrorStates LGBitMap::LGRGB2LAB(LGBitMapId imgInId, LGBitMapId & imgOutId)
{

	/*>> %%%  Rgb2Lab
	%%%
	function[L, a, b] = Rgb2Lab(Image)
	Image = double(Image) / 255;
	R = Image(:, : , 1);
	G = Image(:, : , 2);
	B = Image(:, : , 3);
	X = 0.5767309*R + 0.1855540*G + 0.1881852*B;
	Y = 0.2973769*R + 0.6273491*G + 0.0752741*B;
	Z = 0.0270343*R + 0.0706872*G + 0.9911085*B;
	[row, col] = size(R);
	L = R;
	a = R;
	b = R;
	for i = 1:row
	for j = 1 : col
	L(i, j) = 116 * F_Correction(Y(i, j)) - 16;
	a(i, j) = 500 * (F_Correction(X(i, j)) - F_Correction(Y(i, j)));
	b(i, j) = 200 * (F_Correction(Y(i, j)) - F_Correction(Z(i, j)));
	end
	end*/
	LGBitMapId outId;
	outId = imgInId;
	if (m_mapColorData.at(imgInId).img == IMGBMP1 || m_mapColorData.at(imgInId).img == IMGBMP4 || m_mapColorData.at(imgInId).img == IMGBMP8)
	{
		Convert(imgInId, outId);
	}
	BITMAPCOLORDATA data;
	memset(&data, 0, sizeof(BITMAPCOLORDATA));
	LGGetColorData(outId, data);

	LGMathematicalOp::MATRIX matrixR;
	LGMathematicalOp::MATRIX matrixG;
	LGMathematicalOp::MATRIX matrixB;

	void * pR = malloc(sizeof(double) * data.nMatrixHeight * data.nMatrixWidth);
	void * pG = malloc(sizeof(double) * data.nMatrixHeight * data.nMatrixWidth);
	void * pB = malloc(sizeof(double) * data.nMatrixHeight * data.nMatrixWidth);
	assert(pR); assert(pG); assert(pB);

	matrixR.data = pR; matrixR.height = data.nMatrixHeight; matrixR.width = data.nMatrixWidth;
	matrixG.data = pR; matrixG.height = data.nMatrixHeight; matrixG.width = data.nMatrixWidth;
	matrixB.data = pR; matrixB.height = data.nMatrixHeight; matrixB.width = data.nMatrixWidth;

	//求RGB通道矩阵
	for (int i = 0; i < data.nMatrixHeight * data.nMatrixWidth; i++)
	{
		int addressR = (int)pR;
		int addressG = (int)pG;
		int addressB = (int)pB;
		int addressSrc = (int)data.pMatrixColorData;
		addressSrc = addressSrc + (sizeof(PixelData) * i);
		PixelData * pSrc = (PixelData *)addressSrc;
		addressR = addressR + (sizeof(double) * i);
		addressG = addressG + (sizeof(double) * i);
		addressB = addressB + (sizeof(double) * i);
		double * r = (double *)addressR;
		double * g = (double *)addressG;
		double * b = (double *)addressB;
		*r = pSrc->r;
		*g = pSrc->g;
		*b = pSrc->b;
	}
	// *(1/255)
	void * _pR = malloc(sizeof(double) * data.nMatrixHeight * data.nMatrixWidth);
	void * _pG = malloc(sizeof(double) * data.nMatrixHeight * data.nMatrixWidth);
	void * _pB = malloc(sizeof(double) * data.nMatrixHeight * data.nMatrixWidth);
	LGMathematicalOp::MATRIX _matrixR;
	LGMathematicalOp::MATRIX _matrixG;
	LGMathematicalOp::MATRIX _matrixB;
	_matrixR.data = _pR; _matrixR.height = data.nMatrixHeight; _matrixR.width = data.nMatrixWidth;
	_matrixG.data = _pG; _matrixG.height = data.nMatrixHeight; _matrixG.width = data.nMatrixWidth;
	_matrixB.data = _pB; _matrixB.height = data.nMatrixHeight; _matrixB.width = data.nMatrixWidth;

	LGMathematicalOp::LGMathematicalOperation::Multiplication(_matrixR, matrixR, 1.0 / 255.0);
	LGMathematicalOp::LGMathematicalOperation::Multiplication(_matrixG, matrixG, 1.0 / 255.0);
	LGMathematicalOp::LGMathematicalOperation::Multiplication(_matrixB, matrixB, 1.0 / 255.0);
	//

	/*X = 0.5767309*R + 0.1855540*G + 0.1881852*B;
	Y = 0.2973769*R + 0.6273491*G + 0.0752741*B;
	Z = 0.0270343*R + 0.0706872*G + 0.9911085*B;*/
	LGMathematicalOp::MATRIX mX; LGMathematicalOp::MATRIX mY; LGMathematicalOp::MATRIX mZ;
	void * pX = malloc(sizeof(double) * data.nMatrixHeight * data.nMatrixWidth);
	void * pY = malloc(sizeof(double) * data.nMatrixHeight * data.nMatrixWidth);
	void * pZ = malloc(sizeof(double) * data.nMatrixHeight * data.nMatrixWidth);

	mX.height = data.nMatrixHeight; mX.width = data.nMatrixWidth; mX.data = pX;
	mY.height = data.nMatrixHeight; mY.width = data.nMatrixWidth; mY.data = pY;
	mZ.height = data.nMatrixHeight; mZ.width = data.nMatrixWidth; mZ.data = pZ;


	LGMathematicalOp::MATRIX m1; LGMathematicalOp::MATRIX m2; LGMathematicalOp::MATRIX m3;
	void * p1 = malloc(sizeof(double) * data.nMatrixHeight * data.nMatrixWidth);
	void * p2 = malloc(sizeof(double) * data.nMatrixHeight * data.nMatrixWidth);
	void * p3 = malloc(sizeof(double) * data.nMatrixHeight * data.nMatrixWidth);
	m1.height = data.nMatrixHeight; m1.width = data.nMatrixWidth; m1.data = p1;
	m2.height = data.nMatrixHeight; m2.width = data.nMatrixWidth; m2.data = p2;
	m3.height = data.nMatrixHeight; m3.width = data.nMatrixWidth; m3.data = p3;

	LGMathematicalOp::LGMathematicalOperation::Multiplication(m1, _matrixR, 0.5767309);
	LGMathematicalOp::LGMathematicalOperation::Multiplication(m2, _matrixG, 0.1855540);
	LGMathematicalOp::LGMathematicalOperation::Multiplication(m3, _matrixB, 0.1881852);

	LGMathematicalOp::MATRIX mSum1;
	void * pSum1 = malloc(sizeof(double) * data.nMatrixHeight * data.nMatrixWidth);
	mSum1.height = data.nMatrixHeight; mSum1.width = data.nMatrixWidth; mSum1.data = pSum1;
	LGMathematicalOp::LGMathematicalOperation::MatrixAddition(mSum1, m1, m2);
	LGMathematicalOp::LGMathematicalOperation::MatrixAddition(mX, mSum1, m3);


	memset(p1, 0, sizeof(double) * data.nMatrixHeight * data.nMatrixWidth);
	memset(p2, 0, sizeof(double) * data.nMatrixHeight * data.nMatrixWidth);
	memset(p3, 0, sizeof(double) * data.nMatrixHeight * data.nMatrixWidth);
	memset(pSum1, 0, sizeof(double) * data.nMatrixHeight * data.nMatrixWidth);
	LGMathematicalOp::LGMathematicalOperation::Multiplication(m1, _matrixR, 0.2973769);
	LGMathematicalOp::LGMathematicalOperation::Multiplication(m2, _matrixG, 0.6273491);
	LGMathematicalOp::LGMathematicalOperation::Multiplication(m3, _matrixB, 0.0752741);
	LGMathematicalOp::LGMathematicalOperation::MatrixAddition(mSum1, m1, m2);
	LGMathematicalOp::LGMathematicalOperation::MatrixAddition(mY, mSum1, m3);


	memset(p1, 0, sizeof(double) * data.nMatrixHeight * data.nMatrixWidth);
	memset(p2, 0, sizeof(double) * data.nMatrixHeight * data.nMatrixWidth);
	memset(p3, 0, sizeof(double) * data.nMatrixHeight * data.nMatrixWidth);
	memset(pSum1, 0, sizeof(double) * data.nMatrixHeight * data.nMatrixWidth);
	LGMathematicalOp::LGMathematicalOperation::Multiplication(m1, _matrixR, 0.0270343);
	LGMathematicalOp::LGMathematicalOperation::Multiplication(m2, _matrixG, 0.0706872);
	LGMathematicalOp::LGMathematicalOperation::Multiplication(m3, _matrixB, 0.9911085);
	LGMathematicalOp::LGMathematicalOperation::MatrixAddition(mSum1, m1, m2);
	LGMathematicalOp::LGMathematicalOperation::MatrixAddition(mZ, mSum1, m3);

	//
	void * pLABSpace = malloc(sizeof(LABSpace) * data.nMatrixHeight * data.nMatrixWidth);
	BITMAPCOLORDATA colorData; memset(&colorData, 0, sizeof(BITMAPCOLORDATA));
	colorData.colorSpace = LAB;
	colorData.img = OTHERDATA;
	colorData.nMatrixHeight = data.nMatrixHeight;
	colorData.nMatrixWidth = data.nMatrixWidth;
	colorData.pMatrixColorData = pLABSpace;
	m_id++;
	imgOutId = m_id;
	m_mapColorData.insert(std::pair<LGBitMapId, BITMAPCOLORDATA>(m_id, colorData));


	for (int i = 0; i < data.nMatrixHeight * data.nMatrixWidth; i++)
	{
		int nAddressDes = (int)pLABSpace;

		nAddressDes = nAddressDes + (sizeof(LABSpace) * i);

		LABSpace * pDes = (LABSpace *)nAddressDes;


		int nAddressSrcX = (int)mX.data;

		nAddressSrcX = nAddressSrcX + (sizeof(double) * i);

		double * dX = (double *)nAddressSrcX;



		int nAddressSrcY = (int)mY.data;

		nAddressSrcY = nAddressSrcY + (sizeof(double) * i);

		double * dY = (double *)nAddressSrcY;




		int nAddressSrcZ = (int)mZ.data;

		nAddressSrcZ = nAddressSrcZ + (sizeof(double) * i);

		double * dZ = (double *)nAddressSrcZ;

		pDes->l = (116 * (*dY)) - 16;
		pDes->a = 500 * (*dX - *dY);
		pDes->b = 200 * (*dY - *dZ);
	}
	//
	return LG_ERR_OTHER;
}

LGErrorStates LGBitMap::LGLAB2RGB(LGBitMapId imgInId, LGBitMapId & imgOutId)
{
	LGBitMapId outId = imgInId;
	if (m_mapColorData.at(imgInId).img == IMGBMP1 || m_mapColorData.at(imgInId).img == IMGBMP4 || m_mapColorData.at(imgInId).img == IMGBMP8)
	{
		Convert(imgInId, outId);
	}
	BITMAPCOLORDATA data;
	memset(&data, 0, sizeof(BITMAPCOLORDATA));
	LGGetColorData(outId, data);
	assert(data.pMatrixColorData);
	void * pLABSpace = malloc(sizeof(PixelData) * data.nMatrixHeight * data.nMatrixWidth);
	BITMAPCOLORDATA colorData; memset(&colorData, 0, sizeof(BITMAPCOLORDATA));
	colorData.colorSpace = RGB;
	colorData.img = IMGBMP24;
	colorData.nMatrixHeight = data.nMatrixHeight;
	colorData.nMatrixWidth = data.nMatrixWidth;
	colorData.pMatrixColorData = pLABSpace;
	for (int i = 0; i < data.nMatrixWidth * data.nMatrixHeight; i++)
	{
		int addressSrc = (int)data.pMatrixColorData;
		addressSrc = addressSrc + (sizeof(LABSpace) * i);
		LABSpace * pSrc = (LABSpace *)addressSrc;
		double l = 0.0;
		double a = 0.0;
		double b = 0.0;
		l = pSrc->l;
		a = pSrc->a;
		b = pSrc->b;
		int addressDes = (int)colorData.pMatrixColorData;
		addressDes = addressDes + (sizeof(PixelData) * i);
		PixelData * pDes = (PixelData *)addressDes;
		double _r = 0.0;
		double _g = 0.0;
		double _b = 0.0;
		Lab2RGB(l, a, b, _r, _g, _b);
		pDes->b = _b;
		pDes->g = _g;
		pDes->r = _r;
	}
	m_id++;
	imgOutId = m_id;
	m_mapColorData.insert(std::pair<LGBitMapId, BITMAPCOLORDATA>(imgOutId, colorData));
	return LG_ERR_OTHER;
}

LGErrorStates LGBitMap::LGRGB2LAB2(LGBitMapId imgInId, LGBitMapId & imgOutId)
{
	LGBitMapId outId = imgInId;
	if (m_mapColorData.at(imgInId).img == IMGBMP1 || m_mapColorData.at(imgInId).img == IMGBMP4 || m_mapColorData.at(imgInId).img == IMGBMP8)
	{
		Convert(imgInId, outId);
	}
	BITMAPCOLORDATA data;
	memset(&data, 0, sizeof(BITMAPCOLORDATA));
	LGGetColorData(outId, data);
	assert(data.pMatrixColorData);
	void * pLABSpace = malloc(sizeof(LABSpace) * data.nMatrixHeight * data.nMatrixWidth);
	BITMAPCOLORDATA colorData; memset(&colorData, 0, sizeof(BITMAPCOLORDATA));
	colorData.colorSpace = LAB;
	colorData.img = OTHERDATA;
	colorData.nMatrixHeight = data.nMatrixHeight;
	colorData.nMatrixWidth = data.nMatrixWidth;
	colorData.pMatrixColorData = pLABSpace;
	for (int i = 0; i < data.nMatrixWidth * data.nMatrixHeight ; i++)
	{
		int addressSrc = (int)data.pMatrixColorData;
		addressSrc = addressSrc + (sizeof(PixelData) * i);
		PixelData * pSrc = (PixelData *)addressSrc;
		double r = 0.0;
		double g = 0.0;
		double b = 0.0;
		r = pSrc->r;
		g = pSrc->g;
		b = pSrc->b;
		int addressDes = (int)colorData.pMatrixColorData;
		addressDes = addressDes + (sizeof(LABSpace) * i);
		LABSpace * pDes = (LABSpace *)addressDes;
		RGB2Lab(r, g, b, pDes->l, pDes->a, pDes->b);
	}
	m_id++;
	imgOutId = m_id;
	m_mapColorData.insert(std::pair<LGBitMapId, BITMAPCOLORDATA>(imgOutId, colorData));
	return LG_ERR_OTHER;
}

LGErrorStates LGGeometryControl::LGHypotrochoid(double circle1, double circle2, double h, double & x, double & y, double stepSize, double & sum , double & X , double & Y)

{
	//这里将圆的周长变为整数

	double perimeter1 = 2 * PI * circle1;
	double perimeter2 = 2 * PI * circle2;

	int nPerimeter1 = perimeter1;//小圆
	int nPerimeter2 = perimeter2;//大圆 

	if (nPerimeter2 < nPerimeter1)
	{
		//return LG_ERR_PARAM;
	}
	//先求出滚动的总长度
	int result = nPerimeter1;//求出约分后最小的那个数

	int Sum = result * nPerimeter2;

	sum = Sum;

	double dis = (double)stepSize;

	double _m = 2.0 * PI * circle2;
	double m = dis / _m;
	int nm = (int)m;
	double decimals = m - nm;

	double θ = (decimals) * 360;
	double radian = (θ * PI) / 180;

	X = cos(radian) * circle2;
	Y = sin(radian) * circle2;
	double t1 = cos(radian);
	double t2 = sin(radian);
	double X2 = cos(radian) * (circle2 - circle1);
	double Y2 = sin(radian) * (circle2 - circle1);

	/*X = X2;
	Y = Y2;*/

	_m = 2.0 * PI * circle1;
	m = dis / _m;
	nm = (int)m;
	decimals = m - nm;
	double θ1 = (decimals) * 360;
	double θ2 = 360 - θ1 + θ;
	double radian2 = (θ2 * PI) / 180;
	double t3 = sin(radian2) * h;
	double PX = (cos(radian2) * h) + X2;
	double PY = (sin(radian2) * h) + Y2;
	x = PX;
	y = PY;
	return LG_ERR_PARAM;
}
LGErrorStates LGGeometryControl::LGBesselCurve(LGPolygon & polygon, double & proportion , LGPoint & result)
{
	//用递归方法求解
	LGPoint point;
	polygon.GetStartPoint(point);
	LGPoint nextPoint;
	double distance = 0.0;
	//
	polygon.GetNextPoint(point, nextPoint);
	LGPoint rPoint;//计算出来的结果点
	GetDistance(point, nextPoint, distance);
	Direction dt;
	dt.origin.x = point.x; dt.origin.y = point.y; dt.destination.x = nextPoint.x; dt.destination.y = nextPoint.y;
	double step = distance * proportion;
	GetPoint(point, nextPoint, dt, step, rPoint);
	point.x = nextPoint.x;
	point.y = nextPoint.y;
	if (polygon.GetNextPoint(point, nextPoint) == false)														                                                                           
	{
		//说明这是最终的结果
		result.x = rPoint.x;
		result.y = rPoint.y;
		return LG_ERR_OTHER;
	}
	//
	LGPolygon _polygon(rPoint);
	
	while (polygon.GetNextPoint(point , nextPoint) == true)
	{
		GetDistance(point, nextPoint, distance);
		Direction dt;
		dt.origin.x = point.x; dt.origin.y = point.y; dt.destination.x = nextPoint.x; dt.destination.y = nextPoint.y;
		double step = distance * proportion;
		LGPoint point3;
		GetPoint(point, nextPoint, dt, step, point3);
		_polygon.AddPoint(&point3);
		point.x = nextPoint.x;
		point.y = nextPoint.y;
	}
	LGBesselCurve(_polygon, proportion, result);
}

LGErrorStates LGBitMap::ColorQuantization1(LGBitMapId imgInId, LGBitMapId & imgOutId , int quantizationLevel)
{
	/*%颜色量化
		B = im2double(I4);
	%选定量化等级
		quant_levels = x;
	dq = 100 / (quant_levels - 1);
	qB = applycform(B, makecform('srgb2lab'));
	%对L空间进行量化后四舍五入在放入[0, 100]的空间
		qB(:, : , 1) = (1 / dq)*qB(:, : , 1);
	qB(:, : , 1) = dq*round(qB(:, : , 1));
	%量化公式
		qB(:, : , 1) = qB(:, : , 1) + (dq / 2)*tanh((B(:, : , 1) - qB(:, : , 1)));
	if exist('applycform', 'file')
		Q = applycform(qB, makecform('lab2srgb'));
	end*/
	LGRGB2LAB2(imgInId, imgOutId);
	BITMAPCOLORDATA colorData;
	BITMAPCOLORDATA data;
	LGGetColorData(imgOutId, data);
	LGGetColorData(imgInId, colorData);
	double dq = 100 / (quantizationLevel - 1);
	//对L空间进行量化
	for (int i = 0; i < data.nMatrixHeight * data.nMatrixWidth; i++)
	{
		int nSrcAddress = (int)data.pMatrixColorData;
		nSrcAddress = nSrcAddress + sizeof(LABSpace) * i;
		LABSpace * lab = (LABSpace *)nSrcAddress;
		lab->l = lab->l * (1 / dq);
		lab->l = dq * round(lab->l);
		int nAddress = (int)colorData.pMatrixColorData;
		nAddress = nAddress + sizeof(PixelData) * i;
		PixelData * colorData = (PixelData *)nAddress;
		lab->l = (((dq / 2) * tanh(colorData->b - lab->l))) + lab->l;
		//
		//qB(:, : , 1) = qB(:, : , 1) + (dq / 2)*tanh((B(:, : , 1) - qB(:, : , 1)));
		//	
	}
	return LG_ERR_OTHER;
}

std::string LGBitMap::GetFileName(LPCTSTR filePath)
{
	std::string strPath;
	USES_CONVERSION;
	strPath = T2A((LPTSTR)filePath);
	size_t n = strPath.rfind("\\");//从后往前查找返回从前往后的位置
	std::string substring = strPath.substr(n + 1, std::string::npos);
	return substring;
}

std::string LGBitMap::GetSuffixName(LPCTSTR fileName)
{
	std::string strName;
	USES_CONVERSION;
	strName = T2A((LPTSTR)fileName);
	size_t n = strName.rfind(".");//从后往前查找返回从前往后的位置
	std::string substring = strName.substr(n + 1, std::string::npos);
	return substring;
}

void LGBitMap::Convert(LGBitMapId & imgInId, LGBitMapId & imgOutId)
{
	if (m_mapColorData.count(imgInId))
	{
		BITMAPCOLORDATA data;
		data = m_mapColorData[imgInId];
		BITMAPCOLORDATA outdata;
		if (m_mapColorData[imgInId].img == IMGBMP8)
		{
			m_id++;
			void * pColorData = malloc(sizeof(PixelData) * data.nMatrixHeight * data.nMatrixWidth);
			outdata.img = IMGBMP24;
			outdata.pMatrixColorData = pColorData;
			outdata.pMatrixColorTable = 0;
			outdata.nMatrixHeight = data.nMatrixHeight;
			outdata.nMatrixWidth = data.nMatrixWidth;
			for (int i = 0; i < data.nMatrixHeight; i++)
			{
				for (int j = 0; j < data.nMatrixWidth; j++)
				{
					if (data.img == IMGBMP8)
					{
						int address = (int)data.pMatrixColorData;
						address = address + (j*data.nMatrixHeight) + i;
						//一个地址里边存储多大的数据
						int outAddress = (int)outdata.pMatrixColorData;
						outAddress = outAddress + (((j*data.nMatrixHeight) + i) * sizeof(PixelData));
						PixelData * ppd = (PixelData *)outAddress;
						byte* pb = (byte*)address;
						int nIndex = (int)*pb;
						int _address = (int)data.pMatrixColorTable;
						_address = _address + (nIndex * sizeof(RGBQUAD));
						RGBQUAD bgr = *((RGBQUAD*)_address);
						ppd->b = bgr.rgbBlue;
						ppd->g = bgr.rgbGreen;
						ppd->r = bgr.rgbRed;
					}

				}
			}
			m_mapColorData.insert(std::pair<LGBitMapId, BITMAPCOLORDATA>(m_id, outdata));
			imgOutId = m_id;
		}
		else if (m_mapColorData[imgInId].img == IMGBMP4)
		{

		}
		else if (m_mapColorData[imgInId].img == IMGBMP1)
		{

		}
	}
	else
	{
		imgOutId = -1;
	}
}

LGErrorStates LGBitMap::GenerateGaussianFilter(CONVOLUTIONKERNEL * pKernel, double variance)
{
	//应当保证输入的矩阵有中心点，这里不做检查
	if (pKernel && pKernel->pData)
	{
		//
		if (pKernel->type == DOUBLENUMBER)
		{
			if (pKernel->nHeight == pKernel->nWidth)
			{
				int center = ((pKernel->nHeight) / 2);
				for (int i = 0; i < pKernel->nHeight; i++)
				{
					for (int j = 0; j < pKernel->nWidth; j++)
					{
						//左移center，上移center
						double x = j - center;
						double y = (-1 * i) + center;
						int pAddress = (int)(pKernel->pData);
						pAddress = pAddress + (((pKernel->nHeight*j) + i) * sizeof(double));
						double * pFloat = (double *)pAddress;
						TwoDimensionalGaussianFunction(&x, &y, pFloat, &variance);
					}
				}
				//保证矩阵之和为1
				double fSum = 0.0;
				for (int i = 0; i < pKernel->nHeight * pKernel->nWidth; i++)
				{
					int pAddress = (int)(pKernel->pData);
					pAddress = pAddress + (i * sizeof(double));
					double * pf = (double *)pAddress;
					fSum = fSum + *pf;
				}
				for (int i = 0; i < pKernel->nHeight * pKernel->nWidth; i++)
				{
					int pAddress = (int)(pKernel->pData);
					pAddress = pAddress + (i * sizeof(double));
					double * pf = (double *)pAddress;
					*pf = (*pf) / fSum;
				}
			}
			else
			{
				return LG_ERR_PARAM;
			}
		}
		else
		{
			return LG_ERR_PARAM;
		}
	}
	else
	{
		return LG_ERR_PARAM;
	}
	if (pKernel->nHeight == 0 || pKernel->nWidth == 0)
	{
		return LG_ERR_PARAM;
	}
}

LGErrorStates LGBitMap::TwoDimensionalGaussianFunction(double * x, double * y, double * r, double * variance)
{
	if (x && y && r && variance)
	{
		double n = (-1 * (((*x)*(*x)) + ((*y)*(*y)))) / (2 * (*variance)*(*variance));
		*r = (1 / (2 * PI*(*variance)*(*variance)))*(exp(n));
	}
	else
	{
		return LG_ERR_PARAM;
	}

}

LGErrorStates LGBitMap::TwoDimensionalGaussianFunction(double * distance, double * r, double * variance)
{
	if (distance && r && variance)
	{
		double n = (-1 * (*distance) * (*distance)) / (2 * (*variance)*(*variance));
		*r = (1 / (2 * PI*(*variance)*(*variance)))*(exp(n));
	}
	else
	{
		return LG_ERR_PARAM;
	}
}

LGErrorStates LGBitMap::ProcessBoundary1(int nHeight, int nWidth, BITMAPCOLORDATA * data, /*out*/LGBitMapId & imgOutId, LGBitMapId imgInId)
{
	if (nHeight % 2 != 0 && nWidth % 2 != 0)
	{
		if (data->img != IMGTRUECOLOR || data->img != IMGBMP24 || data->img != IMGBMP32 || data->img != IMGBMP16)
		{
			LGBitMapId outId = 0;
			Convert(imgInId, outId);
			if (outId == -1)
			{
				return LG_ERR_PARAM;
			}
			m_id++;
			BITMAPCOLORDATA colorData;
			memset(&colorData, 0, sizeof(BITMAPCOLORDATA));
			colorData.nMatrixHeight = data->nMatrixHeight + ((nHeight / 2) * 2);
			colorData.nMatrixWidth = data->nMatrixWidth + ((nWidth / 2) * 2);
			colorData.img = IMGBMP24;
			colorData.pMatrixColorData = malloc(sizeof(PixelData) * colorData.nMatrixHeight * colorData.nMatrixWidth);
			assert(colorData.pMatrixColorData);




		}
	}
	else
	{
		return LG_ERR_PARAM;
	}
}

LGErrorStates LGBitMap::ProcessBoundary2(int nHeight, int nWidth, BITMAPCOLORDATA * data, LGBitMapId & imgOutId, LGBitMapId imgInId)
{
	if (0 == data)
	{
		return LG_ERR_PARAM;
	}
	if (nHeight % 2 != 0 && nWidth % 2 != 0)
	{
		if (data->img != IMGTRUECOLOR || data->img != IMGBMP24 || data->img != IMGBMP32 || data->img != IMGBMP16)
		{
			LGBitMapId outId = 0;
			Convert(imgInId, outId);
			if (outId == -1)
			{
				return LG_ERR_PARAM;
			}
			m_id++;
			BITMAPCOLORDATA colorData;
			memset(&colorData, 0, sizeof(BITMAPCOLORDATA));
			colorData.nMatrixHeight = data->nMatrixHeight + ((nHeight / 2) * 2);
			colorData.nMatrixWidth = data->nMatrixWidth + ((nWidth / 2) * 2);
			colorData.img = IMGBMP24;
			colorData.pMatrixColorData = malloc(sizeof(PixelData) * colorData.nMatrixHeight * colorData.nMatrixWidth);
			assert(colorData.pMatrixColorData);
			//1.先拷贝原图到目标矩形
			for (int i = 0; i < data->nMatrixHeight; i++)
			{
				for (int j = 0; j < data->nMatrixWidth; j++)
				{
					int xDes = (nWidth / 2) + j;
					int yDes = (nHeight / 2) + i;
					int xSrc = j;
					int ySrc = i;
					int nAddressSrc = (int)data->pMatrixColorData;
					nAddressSrc = nAddressSrc + (sizeof(PixelData) *((xSrc * data->nMatrixHeight) + ySrc));
					int nAddressDes = (int)colorData.pMatrixColorData;
					nAddressDes = nAddressDes + (sizeof(PixelData) *((xDes * colorData.nMatrixHeight) + yDes));
					PixelData * pSrc = (PixelData *)nAddressSrc;
					PixelData * pDes = (PixelData *)nAddressDes;
					*pDes = *pSrc;
				}
			}



			//2.处理顺序上下左右

			//左上角+上边+右上角
			//y轴对称 X-(x2-X)=2X-x2
			//x轴对称 Y - (y2 - Y) = 2Y - y2
			//对称中心(nWidth / 2 , nHeight / 2)
			for (int i = 0; i < nHeight / 2; i++)
			{
				for (int j = 0; j < nWidth / 2; j++)
				{
					int x = (2 * (nWidth / 2)) - j;
					int y = (2 * (nHeight / 2)) - i;
					int nAddress = (int)colorData.pMatrixColorData;
					nAddress = nAddress + ((x *colorData.nMatrixHeight + y) * sizeof(PixelData));
					int _nAddress = (int)colorData.pMatrixColorData;
					_nAddress = _nAddress + ((j*colorData.nMatrixHeight + i) * sizeof(PixelData));
					PixelData * pDes = (PixelData *)_nAddress;
					PixelData * pSrc = (PixelData *)nAddress;
					*pDes = *pSrc;
				}
			}
			for (int i = 0; i <nHeight / 2; i++)
			{
				for (int j = 0; j < data->nMatrixWidth; j++)
				{
					//Y = nHeight / 2
					//x轴对称 Y - (y2 - Y) = 2Y - y2
					int x = (nWidth / 2) + j;
					int y = i;
					int ySrc = (2 * (nHeight / 2)) - y;
					int nAddress = (int)colorData.pMatrixColorData;
					nAddress = nAddress + ((x *colorData.nMatrixHeight + ySrc) * sizeof(PixelData));
					int _nAddress = (int)colorData.pMatrixColorData;
					_nAddress = _nAddress + ((x*colorData.nMatrixHeight + y) * sizeof(PixelData));
					PixelData * pDes = (PixelData *)_nAddress;
					PixelData * pSrc = (PixelData *)nAddress;
					*pDes = *pSrc;

				}
			}
			for (int i = 0; i < nHeight / 2; i++)
			{
				for (int j = 0; j < nWidth / 2; j++)
				{
					int xDes = j + (colorData.nMatrixWidth - (nWidth / 2));
					int yDes = i;
					int xPoint = colorData.nMatrixWidth - (nWidth / 2) - 1;
					int yPoint = nHeight / 2;
					int xSrc = (2 * xPoint) - xDes;
					int ySrc = (2 * yPoint) - yDes;
					int nAddressSrc = (int)colorData.pMatrixColorData;
					nAddressSrc = nAddressSrc + (sizeof(PixelData) *((xSrc * colorData.nMatrixHeight) + ySrc));
					int nAddressDes = (int)colorData.pMatrixColorData;
					nAddressDes = nAddressDes + (sizeof(PixelData) *((xDes * colorData.nMatrixHeight) + yDes));
					PixelData * pSrc = (PixelData *)nAddressSrc;
					PixelData * pDes = (PixelData *)nAddressDes;
					*pDes = *pSrc;
				}
			}
			//左下角+下边+右下角
			for (int i = 0; i < nHeight / 2; i++)
			{
				for (int j = 0; j < nWidth / 2; j++)
				{
					int xDes = j;
					int yDes = (colorData.nMatrixHeight - (nHeight / 2)) + i;
					int xPoint = nWidth / 2;
					int yPoint = (colorData.nMatrixHeight - (nHeight / 2)) - 1;
					int xSrc = (2 * xPoint) - xDes;
					int ySrc = (2 * yPoint) - yDes;
					int nAddressSrc = (int)colorData.pMatrixColorData;
					nAddressSrc = nAddressSrc + (sizeof(PixelData) *((xSrc * colorData.nMatrixHeight) + ySrc));
					int nAddressDes = (int)colorData.pMatrixColorData;
					nAddressDes = nAddressDes + (sizeof(PixelData) *((xDes * colorData.nMatrixHeight) + yDes));
					PixelData * pSrc = (PixelData *)nAddressSrc;
					PixelData * pDes = (PixelData *)nAddressDes;
					*pDes = *pSrc;
				}
			}
			for (int i = 0; i <nHeight / 2; i++)
			{
				for (int j = 0; j < data->nMatrixWidth; j++)
				{
					//Y = nHeight / 2
					//x轴对称 Y - (y2 - Y) = 2Y - y2
					int xDes = (nWidth / 2) + j;
					int yDes = (colorData.nMatrixHeight - (nHeight / 2)) + i;
					int yPoint = (colorData.nMatrixHeight - (nHeight / 2)) - 1;
					int xSrc = xDes;
					int ySrc = (2 * yPoint) - yDes;
					int nAddressSrc = (int)colorData.pMatrixColorData;
					nAddressSrc = nAddressSrc + (sizeof(PixelData) *((xSrc * colorData.nMatrixHeight) + ySrc));
					int nAddressDes = (int)colorData.pMatrixColorData;
					nAddressDes = nAddressDes + (sizeof(PixelData) *((xDes * colorData.nMatrixHeight) + yDes));
					PixelData * pSrc = (PixelData *)nAddressSrc;
					PixelData * pDes = (PixelData *)nAddressDes;
					*pDes = *pSrc;
				}
			}
			for (int i = 0; i < nHeight / 2; i++)
			{
				for (int j = 0; j < nWidth / 2; j++)
				{
					int xDes = j + (colorData.nMatrixWidth - (nWidth / 2));
					int yDes = (colorData.nMatrixHeight - (nHeight / 2)) + i;
					int xPoint = (colorData.nMatrixWidth - (nWidth / 2)) - 1;
					int yPoint = (colorData.nMatrixHeight - (nHeight / 2)) - 1;
					int xSrc = (2 * xPoint) - xDes;
					int ySrc = (2 * yPoint) - yDes;
					int nAddressSrc = (int)colorData.pMatrixColorData;
					nAddressSrc = nAddressSrc + (sizeof(PixelData) *((xSrc * colorData.nMatrixHeight) + ySrc));
					int nAddressDes = (int)colorData.pMatrixColorData;
					nAddressDes = nAddressDes + (sizeof(PixelData) *((xDes * colorData.nMatrixHeight) + yDes));
					PixelData * pSrc = (PixelData *)nAddressSrc;
					PixelData * pDes = (PixelData *)nAddressDes;
					*pDes = *pSrc;
				}
			}
			//左边
			for (int j = 0; j < data->nMatrixHeight; j++)
			{
				for (int i = 0; i < nWidth / 2; i++)
				{
					int xDes = i;
					int yDes = j + (nHeight / 2);
					int xPoint = nWidth / 2;
					int ySrc = yDes;
					int xSrc = (2 * xPoint) - xDes;
					int nAddressSrc = (int)colorData.pMatrixColorData;
					nAddressSrc = nAddressSrc + (sizeof(PixelData) *((xSrc * colorData.nMatrixHeight) + ySrc));
					int nAddressDes = (int)colorData.pMatrixColorData;
					nAddressDes = nAddressDes + (sizeof(PixelData) *((xDes * colorData.nMatrixHeight) + yDes));
					PixelData * pSrc = (PixelData *)nAddressSrc;
					PixelData * pDes = (PixelData *)nAddressDes;
					*pDes = *pSrc;
				}
			}
			//右边
			for (int j = 0; j < data->nMatrixHeight; j++)
			{
				for (int i = 0; i < nWidth / 2; i++)
				{
					int xDes = i + (colorData.nMatrixWidth - (nWidth / 2));
					int yDes = j + (nHeight / 2);
					int xPoint = (colorData.nMatrixWidth - (nWidth / 2)) - 1;
					int ySrc = yDes;
					int xSrc = (2 * xPoint) - xDes;
					int nAddressSrc = (int)colorData.pMatrixColorData;
					nAddressSrc = nAddressSrc + (sizeof(PixelData) *((xSrc * colorData.nMatrixHeight) + ySrc));
					int nAddressDes = (int)colorData.pMatrixColorData;
					nAddressDes = nAddressDes + (sizeof(PixelData) *((xDes * colorData.nMatrixHeight) + yDes));
					PixelData * pSrc = (PixelData *)nAddressSrc;
					PixelData * pDes = (PixelData *)nAddressDes;
					*pDes = *pSrc;
				}
			}
			m_mapColorData.insert(std::pair<LGBitMapId, BITMAPCOLORDATA>(m_id, colorData));
			imgOutId = m_id;
		}
	}
	else
	{
		return LG_ERR_PARAM;
	}
}

LGErrorStates LGBitMap::GenerateBilateralFilter(CONVOLUTIONKERNEL * pKernel, double variance1, double variance2, LGMathematicalOp::MATRIX & matrix)
{
	//应当保证输入的矩阵有中心点，这里不做检查
	if (pKernel && pKernel->pData)
	{
		//
		if (pKernel->type == DOUBLENUMBER)
		{
			if (pKernel->nHeight == pKernel->nWidth)
			{
				int center = ((pKernel->nHeight) / 2);
				//取matrix中心像素的颜色值
				PixelFloatData matrixData;
				if (matrix.dimension != 3)
				{
					assert(0);
				}
				else
				{
					int centerX = matrix.width / 2;
					int centerY = matrix.height / 2;
					int pMatrixData = (int)matrix.data;
					pMatrixData = pMatrixData + ((((matrix.height)*centerX) + centerY) * sizeof(double) * matrix.dimension);//double应该为caltype，没有做检查
					PixelFloatData * pData = (PixelFloatData *)pMatrixData;
					matrixData.b = pData->b;
					matrixData.g = pData->g;
					matrixData.r = pData->r;
				}


				//
				for (int i = 0; i < pKernel->nHeight; i++)
				{
					for (int j = 0; j < pKernel->nWidth; j++)
					{
						//左移center，上移center
						//Spatial Weight http://blog.csdn.net/shenziheng1/article/details/50838970
						double x = j - center;
						double y = (-1 * i) + center;
						int pAddress = (int)(pKernel->pData);
						pAddress = pAddress + (((pKernel->nHeight*j) + i) * sizeof(double));
						double * pFloat = (double *)pAddress;
						TwoDimensionalGaussianFunction(&x, &y, pFloat, &variance1);
						//
						int _pAddress = (int)matrix.data;
						_pAddress = _pAddress + ((((matrix.height)*j) + i) * sizeof(double) * matrix.dimension);
						PixelFloatData * _pData = (PixelFloatData *)_pAddress;
						double param1 = _pData->b - matrixData.b;
						double param2 = _pData->g - matrixData.g;
						double param3 = _pData->r - matrixData.r;
						double param4 = (param1 * param1) + (param2 * param2) + (param3 * param3);
						double distance = pow(param4 , 0.5);
						double r = 0.0;
						TwoDimensionalGaussianFunction(&distance, &r, &variance2);
						*pFloat = (*pFloat) * r;
					}
				}
				//保证矩阵之和为1
				double fSum = 0.0;
				for (int i = 0; i < pKernel->nHeight * pKernel->nWidth; i++)
				{
					int pAddress = (int)(pKernel->pData);
					pAddress = pAddress + (i * sizeof(double));
					double * pf = (double *)pAddress;
					fSum = fSum + *pf;
				}
				for (int i = 0; i < pKernel->nHeight * pKernel->nWidth; i++)
				{
					int pAddress = (int)(pKernel->pData);
					pAddress = pAddress + (i * sizeof(double));
					double * pf = (double *)pAddress;
					*pf = (*pf) / fSum;
				}
			}
			else
			{
				return LG_ERR_PARAM;
			}
		}
		else
		{
			return LG_ERR_PARAM;
		}
	}
	else
	{
		return LG_ERR_PARAM;
	}
	if (pKernel->nHeight == 0 || pKernel->nWidth == 0)
	{
		return LG_ERR_PARAM;
	}
}

LGErrorStates LGBitMap::LGGray255Img(LGBitMapId imgIdIn, LGBitMapId & imgIdOut)
{
	//采用加权平均值法
	//加权平均值法：即新的颜色值R＝G＝B＝(R ＊ Wr＋G＊Wg＋B＊Wb)，一般由于人眼对不同颜色的敏感度不一样，所以三种颜色值的权重不一样，一般来说绿色最高，红色其次，蓝色最低，最合理的取值分别为Wr ＝ 30％，Wg ＝ 59％，Wb ＝ 11％
	if (m_mapColorData.count(imgIdIn))
	{
		LGBitMapId convertedId = imgIdIn;
		if (m_mapColorData[imgIdIn].img != IMGBMP24)
		{
			Convert(imgIdIn, convertedId);
		}
		if (convertedId != -1)
		{
			m_id++;
			BITMAPCOLORDATA data;
			BITMAPCOLORDATA outData;
			memset(&data, 0, sizeof(BITMAPCOLORDATA));
			memset(&outData, 0, sizeof(BITMAPCOLORDATA));
			LGGetColorData(convertedId, data);
			void * p = malloc(data.nMatrixHeight * data.nMatrixWidth * sizeof(PixelData));
			//
			double redWeight, blueWeight, greenWeight;
			redWeight = 0.1;
			blueWeight = 0.1;
			greenWeight = 0.8;
			//
			for (int i = 0; i < data.nMatrixHeight * data.nMatrixWidth; i++)
			{
				int offset = 0;
				offset = sizeof(PixelData) * i;
				int nAddressDes = (int)p;
				nAddressDes = nAddressDes + offset;
				int nAddressSrc = (int)data.pMatrixColorData;
				nAddressSrc = nAddressSrc + offset;
				PixelData * pDes = (PixelData *)nAddressDes;
				PixelData * pSrc = (PixelData *)nAddressSrc;
				double r = pSrc->r;
				double g = pSrc->g;
				double b = pSrc->b;
				double color = (r * redWeight) + (g * greenWeight) + (b * blueWeight);
				pDes->b = color;
				pDes->g = color;
				pDes->r = color;
			}
			outData.img = IMGBMP24;
			outData.nMatrixHeight = data.nMatrixHeight;
			outData.nMatrixWidth = data.nMatrixWidth;
			outData.pMatrixColorData = p;
			m_mapColorData.insert(std::pair<LGBitMapId, BITMAPCOLORDATA>(m_id, outData));
			imgIdOut = m_id;
		}
		else
		{
			return LG_ERR_PARAM;
		}
	}
	else
	{
		return LG_ERR_PARAM;
	}
}

LGErrorStates LGBitMap::LGGaussianBlur(LGBitMapId imgIdIn, LGBitMapId & imgIdOut, int nRange, double variance)
{
	int nMatrixHeight = (2 * nRange) + 1;
	int nMatrixWidth = nMatrixHeight;
	void * pMatrix = malloc(nMatrixHeight * nMatrixWidth * sizeof(double));
	CONVOLUTIONKERNEL kernel;
	memset(&kernel, 0, sizeof(CONVOLUTIONKERNEL));
	kernel.nHeight = nMatrixHeight;
	kernel.nWidth = nMatrixWidth;
	kernel.pData = pMatrix;
	kernel.type = DOUBLENUMBER;
	GenerateGaussianFilter(&kernel, variance);
	LGConvolutionOperation(imgIdIn, imgIdOut, &kernel);
	return LG_ERR_OTHER;
}

LGErrorStates LGBitMap::LGGaussianBlur2(LGBitMapId imgIdIn, LGBitMapId & imgIdOut, double nRange)
{
	//在 3σ 以外的贡献比例非常小，为 0.1 %，因此我们截断模板时，对模板边界定义为 3σ ；
	int r = (3 * nRange) + 0.5;
	return LGGaussianBlur(imgIdIn, imgIdOut, r, nRange);
}

LGErrorStates LGBitMap::LGEmboss(LGBitMapId imgIdIn, LGBitMapId & imgIdOut)
{
	double embossKernel[5][5] = { 0.0 };
	embossKernel[5][5];
	embossKernel[0][0] = 1 / 4;
	embossKernel[1][1] = 1 / 4;
	embossKernel[2][2] = 1 / 4;
	embossKernel[3][3] = 1 / 4;
	embossKernel[4][4] = -1;
	CONVOLUTIONKERNEL kernel;
	kernel.nHeight = 5;
	kernel.nWidth = 5;
	kernel.pData = embossKernel;
	kernel.type = DOUBLENUMBER;

	LGConvolutionOperation(imgIdIn, imgIdOut, &kernel);
	BITMAPCOLORDATA data;
	memset(&data, 0, sizeof(BITMAPCOLORDATA));
	LGGetColorData(imgIdOut, data);
	for (int i = 0; i < data.nMatrixHeight * data.nMatrixWidth; i++)
	{
		int offset = 0;
		offset = sizeof(PixelData) * i;
		int nAddressSrc = (int)data.pMatrixColorData;
		nAddressSrc = nAddressSrc + offset;
		PixelData * pSrc = (PixelData *)nAddressSrc;
		pSrc->r = pSrc->r + 128;
		pSrc->g = pSrc->g + 128;
		pSrc->b = pSrc->b + 128;
	}
	LGGray255Img(imgIdOut, imgIdOut);
	return LG_ERR_OTHER;
}

LGErrorStates LGBitMap::LGNegative(LGBitMapId imgIdIn, LGBitMapId & imgIdOut)
{

	if (m_mapColorData.count(imgIdIn))
	{
		LGBitMapId convertedId = imgIdIn;
		if (m_mapColorData[imgIdIn].img != IMGBMP24)
		{
			Convert(imgIdIn, convertedId);
		}
		if (convertedId != -1)
		{
			m_id++;
			BITMAPCOLORDATA data;
			BITMAPCOLORDATA outData;
			memset(&data, 0, sizeof(BITMAPCOLORDATA));
			memset(&outData, 0, sizeof(BITMAPCOLORDATA));
			LGGetColorData(convertedId, data);
			void * p = malloc(data.nMatrixHeight * data.nMatrixWidth * sizeof(PixelData));
			for (int i = 0; i < data.nMatrixHeight * data.nMatrixWidth; i++)
			{
				int offset = 0;
				offset = sizeof(PixelData) * i;
				int nAddressDes = (int)p;
				nAddressDes = nAddressDes + offset;
				int nAddressSrc = (int)data.pMatrixColorData;
				nAddressSrc = nAddressSrc + offset;
				PixelData * pDes = (PixelData *)nAddressDes;
				PixelData * pSrc = (PixelData *)nAddressSrc;
				double r = pSrc->r;
				double g = pSrc->g;
				double b = pSrc->b;

				pDes->b = 255 - b;
				pDes->g = 255 - g;
				pDes->r = 255 - r;
			}
			outData.img = IMGBMP24;
			outData.nMatrixHeight = data.nMatrixHeight;
			outData.nMatrixWidth = data.nMatrixWidth;
			outData.pMatrixColorData = p;
			m_mapColorData.insert(std::pair<LGBitMapId, BITMAPCOLORDATA>(m_id, outData));
			imgIdOut = m_id;
		}
		else
		{
			return LG_ERR_PARAM;
		}
	}
	else
	{
		return LG_ERR_PARAM;
	}
}

LGErrorStates LGBitMap::LGSketch1(LGBitMapId imgInId, LGBitMapId & imgIdOut)
{
	if (m_mapColorData.count(imgInId))
	{
		LGBitMapId convertedId = imgInId;
		if (m_mapColorData[imgInId].img != IMGBMP24)
		{
			Convert(imgInId, convertedId);
			imgInId = convertedId;
		}
	}
	else
	{
		return LG_ERR_PARAM;
	}
	BITMAPCOLORDATA dataSrc;
	memset(&dataSrc, 0, sizeof(BITMAPCOLORDATA));
	LGGetColorData(imgInId, dataSrc);
	LGGray255Img(imgInId, imgInId);
	LGNegative(imgInId, imgInId);
	LGGaussianBlur2(imgInId, imgInId, 10);
	//1、去色；2、复制去色图层，并且反色；3、对反色图像进行高斯模糊；4、模糊后的图像叠加模式选择颜色减淡效果。
	//颜色减淡的算法是这样的：C =MIN( A +（A×B）/（255-B）,255)，其中C为混合结果，A为源像素点，B为目标像素点。
	m_id++;
	BITMAPCOLORDATA dataGauss;
	BITMAPCOLORDATA outData;
	memset(&dataGauss, 0, sizeof(BITMAPCOLORDATA));
	memset(&outData, 0, sizeof(BITMAPCOLORDATA));
	LGGetColorData(imgInId, dataGauss);
	void * p = malloc(dataSrc.nMatrixHeight * dataSrc.nMatrixWidth * sizeof(PixelData));
	for (int i = 0; i < dataSrc.nMatrixHeight * dataSrc.nMatrixWidth; i++)
	{
		int offset = 0;
		offset = sizeof(PixelData) * i;
		int nAddressDes = (int)p;
		nAddressDes = nAddressDes + offset;
		int nAddressSrc = (int)dataSrc.pMatrixColorData;
		nAddressSrc = nAddressSrc + offset;
		int nAddressGauss = (int)dataGauss.pMatrixColorData;
		nAddressGauss = nAddressGauss + offset;
		PixelData * pDes = (PixelData *)nAddressDes;
		PixelData * pSrc = (PixelData *)nAddressSrc;
		PixelData * pGauss = (PixelData *)nAddressGauss;


		pDes->b = min((pSrc->b + ((pSrc->b * pGauss->b) / (255 - pGauss->b))), 255);
		pDes->g = min((pSrc->g + ((pSrc->g * pGauss->g) / (255 - pGauss->g))), 255);
		pDes->r = min((pSrc->r + ((pSrc->r * pGauss->r) / (255 - pGauss->r))), 255);
	}
	outData.img = IMGBMP24;
	outData.nMatrixHeight = dataSrc.nMatrixHeight;
	outData.nMatrixWidth = dataSrc.nMatrixWidth;
	outData.pMatrixColorData = p;
	m_mapColorData.insert(std::pair<LGBitMapId, BITMAPCOLORDATA>(m_id, outData));
	imgIdOut = m_id;
}

LGErrorStates LGBitMap::LGSketch2(LGBitMapId imgInId, LGBitMapId & imgIdOut)
{
	///////////////////////////////////////matlab源程序
	/*%%% Sketch
	clc;
	clear all;
	Image = imread('4.jpg');
	Image = double(Image);
	[row, col, layer] = size(Image);

	Filter_size = 30;
	sigma = Filter_size / 6;
	F_gaussian = fspecial('gaussian', Filter_size, sigma);

	for i = 1:row
	for j = 1 : col
	I(i, j) = max(Image(i, j));
	end
	end
	I = I / 255;
	p = 1;
	Gradient_X = [-1 - p - 1
	0   0  0
	1   p  1];

	Gradient_Y = [-1 0 1
	- p 0 p
	- 1 0 1];
	Image_y = imfilter(I, Gradient_Y, 'conv');
	Image_x = imfilter(I, Gradient_X, 'conv');

	alpha = 0.5;
	% Image_edge = (abs(Image_x)*alpha + (1 - alpha)*abs(Image_y));
	Image_edge = (abs(Image_x) + abs(Image_y));

	F_image = imfilter(Image, F_gaussian, 'conv');
	[L, a, b] = Rgb2Lab(F_image);
	I = sqrt(L.*L + a.*a + b.*b);
	I_mean = mean(mean(I));
	F_S = I;
	for i = 1:row
	for j = 1 : col
	if (I(i, j) < I_mean)
	F_S(i, j) = (1 - I(i, j) / I_mean). ^ 2;
	else
	F_S(i, j) = 0;
	end
	end
	end

	Image_out = (1 - Image_edge).*(0.8 - F_S);*/
	BITMAPCOLORDATA dataSrc;
	memset(&dataSrc, 0, sizeof(BITMAPCOLORDATA));
	LGGetColorData(imgInId, dataSrc);
	//原图像数据24
	BITMAPCOLORDATA dataSrcConvert;
	LGBitMapId idSrcConvert = 0;
	//
	memset(&dataSrcConvert, 0, sizeof(BITMAPCOLORDATA));
	if (dataSrc.img != IMGBMP24)
	{
		Convert(imgInId, idSrcConvert);
		LGGetColorData(idSrcConvert, dataSrcConvert);
	}
	else
	{
		dataSrcConvert = dataSrc;
		idSrcConvert = imgInId;
	}
	LGBitMapId idGaussian = 0;
	//	BITMAPCOLORDATA ;
	//	memset(&data, 0, sizeof(BITMAPCOLORDATA));
	LGGaussianBlur2(idSrcConvert, idGaussian, 5);
	void * p = malloc(sizeof(byte) * dataSrcConvert.nMatrixHeight * dataSrcConvert.nMatrixWidth);
	assert(p);
	for (int i = 0; i < dataSrcConvert.nMatrixHeight * dataSrcConvert.nMatrixWidth; i++)
	{
		//对应位置拷贝
		int nAddressDes = (int)p;
		int nAddressSrc = (int)dataSrcConvert.pMatrixColorData;
		nAddressDes = nAddressDes + (sizeof(byte) * i);
		nAddressSrc = nAddressSrc + (sizeof(PixelData) * i);
		PixelData * pData = (PixelData *)nAddressSrc;
		byte * pDataDes = (byte *)nAddressDes;
		*pDataDes = max(max(pData->r, pData->r), pData->r);
	}


	//
	void * _p = malloc(sizeof(double) * dataSrcConvert.nMatrixHeight * dataSrcConvert.nMatrixWidth);

	assert(_p);

	for (int i = 0; i < dataSrcConvert.nMatrixHeight * dataSrcConvert.nMatrixWidth; i++)
	{
		int nAddressDes = (int)_p;
		int nAddressSrc = (int)p;
		nAddressDes = nAddressDes + (sizeof(double) * i);
		nAddressSrc = nAddressSrc + (sizeof(byte) * i);
		byte * pData = (byte *)nAddressSrc;
		double * pDataDes = (double *)nAddressDes;
		double d = *pData;
		*pDataDes = d / 255;
	}

	//
	//对p进行卷积操作，梯度滤波
	LGMathematicalOp::MATRIX matrixSrc;
	matrixSrc.data = _p;
	matrixSrc.height = dataSrcConvert.nMatrixHeight;
	matrixSrc.width = dataSrcConvert.nMatrixWidth;
	LGMathematicalOp::MATRIX Gradient_X;
	LGMathematicalOp::MATRIX Gradient_Y;
	double dx[3][3] = { { -1 , -1 , -1 } ,{ 0 , 0 , 0 } ,{ 1 , 1 , 1 } };

	double dy[3][3] = { { -1 , 0 , 1 } ,{ -1 , 0 , 1 } ,{ -1 , 0  ,1 } };
	Gradient_X.data = dx;
	Gradient_X.height = 3;
	Gradient_X.width = 3;
	Gradient_Y.data = dy;
	Gradient_Y.height = 3;
	Gradient_Y.width = 3;



	void * px = malloc(sizeof(double) * dataSrcConvert.nMatrixHeight * dataSrcConvert.nMatrixWidth);
	assert(px);
	void * py = malloc(sizeof(double) * dataSrcConvert.nMatrixHeight * dataSrcConvert.nMatrixWidth);
	assert(py);
	LGMathematicalOp::MATRIX matrixX;
	LGMathematicalOp::MATRIX matrixY;
	matrixX.data = px;
	matrixX.height = dataSrcConvert.nMatrixHeight;
	matrixX.width = dataSrcConvert.nMatrixWidth;

	matrixY.data = py;
	matrixY.height = dataSrcConvert.nMatrixHeight;
	matrixY.width = dataSrcConvert.nMatrixWidth;
	LGMathematicalOp::LGMathematicalOperation::Convolution(matrixX, matrixSrc, Gradient_X);//
	LGMathematicalOp::LGMathematicalOperation::Convolution(matrixY, matrixSrc, Gradient_Y);
	//求边缘
	//double alpha = 0.5;
	/*Image_edge = (abs(Image_x) + abs(Image_y));

	F_image = imfilter(Image, F_gaussian, 'conv');*/

	void * pImageEdge = malloc(sizeof(double) * dataSrcConvert.nMatrixHeight * dataSrcConvert.nMatrixWidth);
	void * pImageAbsX = malloc(sizeof(double) * dataSrcConvert.nMatrixHeight * dataSrcConvert.nMatrixWidth);
	void * pImageAbsY = malloc(sizeof(double) * dataSrcConvert.nMatrixHeight * dataSrcConvert.nMatrixWidth);
	assert(pImageAbsX);
	assert(pImageAbsY);
	assert(pImageEdge);
	LGMathematicalOp::MATRIX matrixEdge;
	LGMathematicalOp::MATRIX matrixAbsX;
	LGMathematicalOp::MATRIX matrixAbsY;
	matrixEdge.data = pImageEdge; matrixEdge.height = dataSrcConvert.nMatrixHeight; matrixEdge.width = dataSrcConvert.nMatrixWidth;
	matrixAbsX.data = pImageAbsX; matrixAbsX.height = dataSrcConvert.nMatrixHeight; matrixAbsX.width = dataSrcConvert.nMatrixWidth;
	matrixAbsY.data = pImageAbsY; matrixAbsY.height = dataSrcConvert.nMatrixHeight; matrixAbsY.width = dataSrcConvert.nMatrixWidth;
	LGMathematicalOp::LGMathematicalOperation::AbsoluteValue(matrixAbsX, matrixX);
	LGMathematicalOp::LGMathematicalOperation::AbsoluteValue(matrixAbsY, matrixY);
	LGMathematicalOp::LGMathematicalOperation::MatrixAddition(matrixEdge, matrixAbsX, matrixAbsY);
	//高斯模糊

	//
	//I = sqrt(L.*L + a.*a + b.*b);


	BITMAPCOLORDATA labData; memset(&labData, 0, sizeof(BITMAPCOLORDATA));
	LGBitMapId labId = 0;
	LGRGB2LAB2(idGaussian, labId);
	LGGetColorData(labId, labData);
	LGMathematicalOp::MATRIX matrixI;
	void * pI = malloc(sizeof(double) * dataSrcConvert.nMatrixHeight * dataSrcConvert.nMatrixWidth);
	if (labData.colorSpace != LAB)
	{
		assert(0);
	}
	for (int i = 0; i < dataSrcConvert.nMatrixHeight * dataSrcConvert.nMatrixWidth; i++)
	{
		int nAddressLAB = (int)labData.pMatrixColorData;

		int nAddressDes = (int)pI;
		nAddressDes = nAddressDes + (sizeof(double) * i);


		nAddressLAB = nAddressLAB + (sizeof(LABSpace) * i);

		LABSpace * plab = (LABSpace *)nAddressLAB;

		double d = (plab->l * plab->l) + (plab->a * plab->a) + (plab->b * plab->b);

		double * pDes = (double *)nAddressDes;

		*pDes = sqrt(d);
	}
	//I_mean = mean(mean(I));
	LGMathematicalOp::MATRIX I;
	I.data = pI;//上面已经求出
	I.height = dataSrcConvert.nMatrixHeight;
	I.width = dataSrcConvert.nMatrixWidth;
	LGMathematicalOp::MATRIX I_MEAN;
	I_MEAN.data = malloc(sizeof(double) * dataSrcConvert.nMatrixWidth);
	I_MEAN.height = 1;
	I_MEAN.width = dataSrcConvert.nMatrixWidth;
	LGMathematicalOp::LGMathematicalOperation::Mean(I_MEAN, I);
	double AverageValue = 0.0;
	LGMathematicalOp::LGMathematicalOperation::RowVectorMean(AverageValue, I_MEAN);
	//
	//for i = 1:row
	/*for j = 1 : col
	if (I(i, j) < I_mean)
	F_S(i, j) = (1 - I(i, j) / I_mean). ^ 2;
	else
	F_S(i, j) = 0;*/
	LGMathematicalOp::MATRIX F_S;
	F_S.data = malloc(sizeof(double) * dataSrcConvert.nMatrixHeight * dataSrcConvert.nMatrixWidth);
	F_S.height = dataSrcConvert.nMatrixHeight;
	F_S.width = dataSrcConvert.nMatrixWidth;
	for (int i = 0; i < dataSrcConvert.nMatrixHeight * dataSrcConvert.nMatrixWidth; i++)
	{
		int addressI = (int)I.data;
		addressI = addressI + (sizeof(double) * i);
		int addressF_S = (int)F_S.data;
		addressF_S = addressF_S + (sizeof(double) * i);
		double * valueI = (double *)addressI;
		double * valueF_S = (double *)addressF_S;
		if (*valueI < AverageValue)
		{
			double d = (1 - (*valueI)) / AverageValue;
			*valueF_S = d * d;
		}
		else
		{
			*valueF_S = 0;
		}
	}
	//Image_out=(1-Image_edge).*(0.8-F_S);
	LGMathematicalOp::MATRIX Coefficient;//这个矩阵每个元素的值都是-1
	LGMathematicalOp::LGMathematicalOperation::InitializationMATRIX(Coefficient, dataSrcConvert.nMatrixHeight, dataSrcConvert.nMatrixWidth, sizeof(double));
	LGMathematicalOp::LGMathematicalOperation::InitializationMATRIX(-1, Coefficient);

	LGMathematicalOp::MATRIX _Coefficient;//这个矩阵每个元素的值都是1
	LGMathematicalOp::LGMathematicalOperation::InitializationMATRIX(_Coefficient, dataSrcConvert.nMatrixHeight, dataSrcConvert.nMatrixWidth, sizeof(double));
	LGMathematicalOp::LGMathematicalOperation::InitializationMATRIX(1, _Coefficient);

	LGMathematicalOp::MATRIX VALUE1;//-Image_edge
	LGMathematicalOp::LGMathematicalOperation::InitializationMATRIX(VALUE1, dataSrcConvert.nMatrixHeight, dataSrcConvert.nMatrixWidth, sizeof(double));
	LGMathematicalOp::LGMathematicalOperation::HadamardMultiplication(matrixEdge, Coefficient, VALUE1);

	LGMathematicalOp::MATRIX VALUE2;//1-Image_edge
	LGMathematicalOp::LGMathematicalOperation::InitializationMATRIX(VALUE2, dataSrcConvert.nMatrixHeight, dataSrcConvert.nMatrixWidth, sizeof(double));
	LGMathematicalOp::LGMathematicalOperation::MatrixAddition(VALUE2, VALUE1, _Coefficient);

	LGMathematicalOp::MATRIX VALUE3;//-F_S
	LGMathematicalOp::LGMathematicalOperation::InitializationMATRIX(VALUE3, dataSrcConvert.nMatrixHeight, dataSrcConvert.nMatrixWidth, sizeof(double));
	LGMathematicalOp::LGMathematicalOperation::HadamardMultiplication(Coefficient, F_S, VALUE3);

	LGMathematicalOp::MATRIX VALUE4;//这个矩阵每个元素的值都是0.8
	LGMathematicalOp::LGMathematicalOperation::InitializationMATRIX(VALUE4, dataSrcConvert.nMatrixHeight, dataSrcConvert.nMatrixWidth, sizeof(double));
	LGMathematicalOp::LGMathematicalOperation::InitializationMATRIX(0.8, VALUE4);

	LGMathematicalOp::MATRIX VALUE5; //0.8 - F_S
	LGMathematicalOp::LGMathematicalOperation::InitializationMATRIX(VALUE5, dataSrcConvert.nMatrixHeight, dataSrcConvert.nMatrixWidth, sizeof(double));
	LGMathematicalOp::LGMathematicalOperation::MatrixAddition(VALUE5, VALUE4, VALUE3);

	LGMathematicalOp::MATRIX Image_out; //Image_out = (1 - Image_edge).*(0.8 - F_S)
	LGMathematicalOp::LGMathematicalOperation::InitializationMATRIX(Image_out, dataSrcConvert.nMatrixHeight, dataSrcConvert.nMatrixWidth, sizeof(double));
	LGMathematicalOp::LGMathematicalOperation::HadamardMultiplication(VALUE5, VALUE2, Image_out);
	/*%% 设置底色层
	Image_Dodge(:,:,1)=Image_out;
	Image_Dodge(:,:,2)=Image_out;
	Image_Dodge(:,:,3)=Image_out;
	Base_layer=Image_out;
	Base_layer(:,:,1)=210/255;
	Base_layer(:,:,2)=225/255;
	Base_layer(:,:,3)=105/255;
	alpha=0.7;
	Image1=alpha*Image_Dodge+(1-alpha)*Base_layer;
	Image3=Image_Dodge.*Image1;
	figure, imshow(Image3);*/
	double * pTemp1 = (double *)Image_out.data;
	double temp1 = *pTemp1;
	LGMathematicalOp::MATRIX Image_Dodge;
	//先转换为double计算，最后转换成byte输出
	LGMathematicalOp::LGMathematicalOperation::InitializationMATRIX(Image_Dodge, dataSrcConvert.nMatrixHeight, dataSrcConvert.nMatrixWidth, sizeof(PixelFloatData));
	for (int i = 0; i < Image_Dodge.height * Image_Dodge.width; i++)
	{
		int n = (int)Image_Dodge.data;
		n = n + (sizeof(PixelFloatData) * i);
		PixelFloatData * p = (PixelFloatData *)n;
		int nImage_outDes = (int)Image_out.data;
		nImage_outDes = nImage_outDes + (sizeof(double) * i);
		double * pd = (double *)nImage_outDes;
		p->b = *pd;
		p->g = *pd;
		p->r = *pd;
	}
	PixelFloatData pfData;
	pfData.b = 105.0 / 255.0;
	pfData.r = 210.0 / 255.0;
	pfData.g = 225.0 / 255.0;
	LGMathematicalOp::MATRIX Base_layer;
	LGMathematicalOp::LGMathematicalOperation::InitializationMATRIX(Base_layer, dataSrcConvert.nMatrixHeight, dataSrcConvert.nMatrixWidth, sizeof(PixelFloatData));
	LGMathematicalOp::LGMathematicalOperation::InitializationMATRIX(&pfData, sizeof(PixelFloatData), Base_layer);
	double alpha = 0.7;
	//Image1=alpha*Image_Dodge+(1-alpha)*Base_layer;
	//Image3 = Image_Dodge.*Image1;

	//alpha
	PixelFloatData alphaData;
	alphaData.b = alpha;
	alphaData.r = alpha;
	alphaData.g = alpha;
	LGMathematicalOp::MATRIX alphaMatrix;
	LGMathematicalOp::LGMathematicalOperation::InitializationMATRIX(alphaMatrix, dataSrcConvert.nMatrixHeight, dataSrcConvert.nMatrixWidth, sizeof(PixelFloatData));
	LGMathematicalOp::LGMathematicalOperation::InitializationMATRIX(&alphaData, sizeof(PixelFloatData), alphaMatrix);

	//alpha*Image_Dodge
	LGMathematicalOp::MATRIX add1;
	LGMathematicalOp::LGMathematicalOperation::InitializationMATRIX(add1, dataSrcConvert.nMatrixHeight, dataSrcConvert.nMatrixWidth, sizeof(PixelFloatData));
	LGMathematicalOp::LGMathematicalOperation::HadamardMultiplicationEx(alphaMatrix, Image_Dodge, add1);

	//(1-alpha)
	PixelFloatData _alphaData;
	_alphaData.b = 1 - alpha;
	_alphaData.r = 1 - alpha;
	_alphaData.g = 1 - alpha;
	LGMathematicalOp::MATRIX _alphaMatrix;
	LGMathematicalOp::LGMathematicalOperation::InitializationMATRIX(_alphaMatrix, dataSrcConvert.nMatrixHeight, dataSrcConvert.nMatrixWidth, sizeof(PixelFloatData));
	LGMathematicalOp::LGMathematicalOperation::InitializationMATRIX(&_alphaData, sizeof(PixelFloatData), _alphaMatrix);

	//(1-alpha)*Base_layer
	LGMathematicalOp::MATRIX add2;
	LGMathematicalOp::LGMathematicalOperation::InitializationMATRIX(add2, dataSrcConvert.nMatrixHeight, dataSrcConvert.nMatrixWidth, sizeof(PixelFloatData));
	LGMathematicalOp::LGMathematicalOperation::HadamardMultiplicationEx(_alphaMatrix, Base_layer, add2);

	//Image1=alpha*Image_Dodge+(1-alpha)*Base_layer
	LGMathematicalOp::MATRIX Image1;
	LGMathematicalOp::LGMathematicalOperation::InitializationMATRIX(Image1, dataSrcConvert.nMatrixHeight, dataSrcConvert.nMatrixWidth, sizeof(PixelFloatData));
	LGMathematicalOp::LGMathematicalOperation::MatrixAdditionEx(Image1, add2, add1);

	//Image3 = Image_Dodge.*Image1;
	LGMathematicalOp::MATRIX Image3;
	LGMathematicalOp::LGMathematicalOperation::InitializationMATRIX(Image3, dataSrcConvert.nMatrixHeight, dataSrcConvert.nMatrixWidth, sizeof(PixelFloatData));
	LGMathematicalOp::LGMathematicalOperation::HadamardMultiplicationEx(Image_Dodge, Image1, Image3);
	//将double转化为byte
	//figure, imshow(Image3)
	BITMAPCOLORDATA colorDataOut;
	colorDataOut.colorSpace = RGB;
	colorDataOut.img = IMGBMP24;
	colorDataOut.nMatrixHeight = dataSrcConvert.nMatrixHeight;
	colorDataOut.nMatrixWidth = dataSrcConvert.nMatrixWidth;
	colorDataOut.pMatrixColorData = malloc(sizeof(PixelData) * colorDataOut.nMatrixHeight * colorDataOut.nMatrixWidth);
	for (int i = 0; i < colorDataOut.nMatrixHeight * colorDataOut.nMatrixWidth; i++)
	{
		int nSrcOut = (int)Image3.data;
		nSrcOut = nSrcOut + (sizeof(PixelFloatData) * i);
		int nDesOut = (int)colorDataOut.pMatrixColorData;
		nDesOut = nDesOut + (sizeof(PixelData) * i);
		PixelFloatData * pfd = (PixelFloatData *)nSrcOut;
		PixelData * pd = (PixelData *)nDesOut;
		/*if (pfd->b > 1 || pd->g > 1 || pd->r > 1)
		{
			assert(0);
		}*/
		double dr = pfd->r * 255;
		double dg = pfd->g * 255;
		double db = pfd->b * 255;
		pd->b = abs(db);
		pd->g = abs(dg);
		pd->r = abs(dr);
	}
	m_id++;
	imgIdOut = m_id;
	m_mapColorData.insert(std::pair<LGBitMapId, BITMAPCOLORDATA>(m_id, colorDataOut));



	//free memory
	/*free(p);
	free(_p);
	free(px);
	free(py);*/
	free(p);
	free(_p);
	free(px);
	free(py);
	free(pImageEdge);
	free(pImageAbsX);
	free(pImageAbsY);
	free(pI);
	free(I_MEAN.data);
	free(F_S.data);
	free(Coefficient.data);
	free(_Coefficient.data);
	free(VALUE1.data);
	free(VALUE2.data);
	free(VALUE3.data);
	free(VALUE4.data);
	free(VALUE5.data);
	free(Image_out.data);
	free(Image_Dodge.data);
	free(Base_layer.data);
	free(alphaMatrix.data);
	free(add1.data);
	free(_alphaMatrix.data);
	free(add2.data);
	free(Image1.data);
	free(Image3.data);
	return LG_ERR_OTHER;
}

LGErrorStates LGBitMap::LGBilateralFiltering(LGBitMapId imgInId, LGBitMapId & imgIdOut , double nRange , double nRange2)
{
	//在 3σ 以外的贡献比例非常小，为 0.1 %，因此我们截断模板时，对模板边界定义为 3σ ；
	int _r = (3 * nRange) + 0.5;

	if (m_mapColorData.count(imgInId) == 0)
	{
		return LG_ERR_PARAM;
	}
	LGBitMapId outId;
	outId = imgInId;
	if (m_mapColorData.at(imgInId).img == IMGBMP1 || m_mapColorData.at(imgInId).img == IMGBMP4 || m_mapColorData.at(imgInId).img == IMGBMP8)
	{
		Convert(imgInId, outId);
	}

	if (outId != -1)
	{
		LGBitMapId OutExId = 0;
		BITMAPCOLORDATA data;
		memset(&data, 0, sizeof(BITMAPCOLORDATA));
		LGErrorStates err = LGGetColorData(outId, data);
		if (err != LG_ERR_ERRID)
		{
			LGErrorStates _err = ProcessBoundary2((2 * _r) + 1, (2 * _r) + 1, &data, OutExId, outId);
			if (_err != LG_ERR_PARAM)
			{
				BITMAPCOLORDATA dataEx;
				memset(&dataEx, 0, sizeof(BITMAPCOLORDATA));
				LGGetColorData(OutExId, dataEx);
				BITMAPCOLORDATA processedData;
				memset(&processedData, 0, sizeof(BITMAPCOLORDATA));
				processedData.nMatrixHeight = data.nMatrixHeight;
				processedData.nMatrixWidth = data.nMatrixWidth;
				processedData.pMatrixColorData = malloc(sizeof(PixelData) * processedData.nMatrixHeight * processedData.nMatrixWidth);
				processedData.img = IMGBMP24;
				for (int i = 0; i < processedData.nMatrixHeight; i++)
				{
					for (int j = 0; j < processedData.nMatrixWidth; j++)
					{
						//计算矩阵乘积
						int x = (((2 * _r) + 1) / 2) + j;
						int y = (((2 * _r) + 1) / 2) + i;
						int leftTopX = x - (((2 * _r) + 1) / 2);
						int leftTopY = y - (((2 * _r) + 1) / 2);
						double r = 0.0;
						double g = 0.0;
						double b = 0.0;
						//为计算双边滤波提供一个颜色矩阵
						int nMatrixHeight = (2 * _r) + 1;
						int nMatrixWidth = nMatrixHeight;
						void * pMatrix = malloc(nMatrixHeight * nMatrixWidth * sizeof(double));
						CONVOLUTIONKERNEL kernel;
						memset(&kernel, 0, sizeof(CONVOLUTIONKERNEL));
						kernel.nHeight = nMatrixHeight;
						kernel.nWidth = nMatrixWidth;
						kernel.pData = pMatrix;
						kernel.type = DOUBLENUMBER;
						
						LGMathematicalOp::MATRIX matrix;
						int DataSize = sizeof(PixelFloatData) / sizeof(double);
						LGMathematicalOp::LGMathematicalOperation::InitializationMATRIX(matrix , kernel.nHeight , kernel.nWidth , sizeof(double) * DataSize);
						CONVOLUTIONKERNEL * pKernel = &kernel;
						for (int k = 0; k < pKernel->nHeight; k++)
						{
							for (int m = 0; m < pKernel->nWidth; m++)
							{
								int processX = leftTopX + m;
								int processY = leftTopY + k;
								int nAddress = (int)dataEx.pMatrixColorData;
								nAddress = nAddress + (((processX * dataEx.nMatrixHeight) + processY) * sizeof(PixelData));
								PixelData * ppd = (PixelData *)nAddress;
								int _nAddress = (int)matrix.data;
								_nAddress = _nAddress + (((m * matrix.height) + k) * sizeof(PixelFloatData));
								PixelFloatData * pfd = (PixelFloatData *)_nAddress;
								pfd->b = ppd->b;
								pfd->g = ppd->g;
								pfd->r = ppd->r;
							}
						}
						GenerateBilateralFilter(&kernel , nRange , nRange2, matrix);

						


						//
						for (int k = 0; k < pKernel->nHeight; k++)
						{
							for (int m = 0; m < pKernel->nWidth; m++)
							{
								int processX = leftTopX + m;
								int processY = leftTopY + k;
								int nAddress = (int)dataEx.pMatrixColorData;
								nAddress = nAddress + (((processX * dataEx.nMatrixHeight) + processY) * sizeof(PixelData));
								PixelData * ppd = (PixelData *)nAddress;
								if (pKernel->type == DOUBLENUMBER)
								{
									int _nAddress = (int)pKernel->pData;
									_nAddress = _nAddress + (sizeof(double) * ((m * pKernel->nHeight) + k));
									double * pf = (double *)_nAddress;
									b = b + (ppd->b * (*pf));
									g = g + (ppd->g * (*pf));
									r = r + (ppd->r * (*pf));
								}
							}
						}
						int __nAddress = (int)processedData.pMatrixColorData;
						__nAddress = __nAddress + (sizeof(PixelData) * ((j * processedData.nMatrixHeight) + i));
						PixelData * pOutPixelData = (PixelData *)__nAddress;
						pOutPixelData->b = b;
						pOutPixelData->g = g;
						pOutPixelData->r = r;
						//
						free(matrix.data);
						free(pMatrix);
					}
				}
				m_id++;
				m_mapColorData.insert(std::pair<LGBitMapId, BITMAPCOLORDATA>(m_id, processedData));
				imgIdOut = m_id;
			}
		}
		else
		{
			return LG_ERR_PARAM;
		}

	}
}

LGErrorStates LGMathematicalOp::LGMathematicalOperation::HadamardMultiplication(_m_in_ MATRIX & matrix1, _m_in_ MATRIX & matrix2, _m_out_ MATRIX & matrix3)
{
	HadamardMultiplicationEx(matrix1, matrix2, matrix3);
	return LGErrorStates();
}

LGErrorStates LGMathematicalOp::LGMathematicalOperation::MatrixAddition(_m_out_ MATRIX & matrix, _m_in_ MATRIX & matrix1, _m_in_ MATRIX & matrix2)
{

	if (matrix1.width == matrix2.width && matrix1.height == matrix2.height)
	{
		for (int i = 0; i < matrix1.height * matrix1.width; i++)
		{
			int address1 = (int)matrix1.data;
			int address2 = (int)matrix2.data;
			address1 = address1 + (sizeof(caltype) * i);
			address2 = address2 + (sizeof(caltype) * i);
			int addressDes = (int)matrix.data;
			addressDes = addressDes + (sizeof(caltype) * i);
			caltype * pData1 = (caltype *)address1;
			caltype * pData2 = (caltype *)address2;
			caltype * pDes = (caltype *)addressDes;
			*pDes = (*pData1) + (*pData2);
		}
	}
	else
	{
		return LG_ERR_PARAM;
	}
}

LGErrorStates LGMathematicalOp::LGMathematicalOperation::Convolution(_m_out_ MATRIX & matrix3, _m_in_ MATRIX & matrix1, _m_in_ MATRIX & kernel)
{

	int nHeight = kernel.height;
	int nWidth = kernel.width;
	//扩展矩形边缘
	if (nHeight % 2 != 0 && nWidth % 2 != 0)
	{
		//保证kernel为奇数
	}
	else
	{
		return LG_ERR_PARAM;
	}
	int nMatrixHeight = matrix1.height + ((nHeight / 2) * 2);
	int nMatrixWidth = matrix1.width + ((nWidth / 2) * 2);
	void * pExpand = malloc(sizeof(caltype) * nMatrixHeight * nMatrixWidth);
	for (int i = 0; i < matrix1.height; i++)
	{
		for (int j = 0; j < matrix1.width; j++)
		{
			int xDes = (nWidth / 2) + j;
			int yDes = (nHeight / 2) + i;
			int xSrc = j;
			int ySrc = i;
			int nAddressSrc = (int)matrix1.data;
			nAddressSrc = nAddressSrc + (sizeof(caltype) *((xSrc * matrix1.height) + ySrc));
			int nAddressDes = (int)pExpand;
			nAddressDes = nAddressDes + (sizeof(caltype) *((xDes * nMatrixHeight) + yDes));
			caltype * pSrc = (caltype *)nAddressSrc;
			caltype * pDes = (caltype *)nAddressDes;
			*pDes = *pSrc;
		}
	}
	//2.处理顺序上下左右

	//左上角+上边+右上角
	//y轴对称 X-(x2-X)=2X-x2
	//x轴对称 Y - (y2 - Y) = 2Y - y2
	//对称中心(nWidth / 2 , nHeight / 2)
	for (int i = 0; i < nHeight / 2; i++)
	{
		for (int j = 0; j < nWidth / 2; j++)
		{
			int x = (2 * (nWidth / 2)) - j;
			int y = (2 * (nHeight / 2)) - i;
			int nAddress = (int)pExpand;
			nAddress = nAddress + ((x *nMatrixHeight + y) * sizeof(caltype));
			int _nAddress = (int)pExpand;
			_nAddress = _nAddress + ((j*nMatrixHeight + i) * sizeof(caltype));
			caltype * pDes = (caltype *)_nAddress;
			caltype * pSrc = (caltype *)nAddress;
			*pDes = *pSrc;
		}
	}
	for (int i = 0; i < nHeight / 2; i++)
	{
		for (int j = 0; j < matrix1.width; j++)
		{
			//Y = nHeight / 2
			//x轴对称 Y - (y2 - Y) = 2Y - y2
			int x = (nWidth / 2) + j;
			int y = i;
			int ySrc = (2 * (nHeight / 2)) - y;
			int nAddress = (int)pExpand;
			nAddress = nAddress + ((x *nMatrixHeight + ySrc) * sizeof(caltype));
			int _nAddress = (int)pExpand;
			_nAddress = _nAddress + ((x*nMatrixHeight + y) * sizeof(caltype));
			caltype * pDes = (caltype *)_nAddress;
			caltype * pSrc = (caltype *)nAddress;
			*pDes = *pSrc;

		}
	}
	for (int i = 0; i < nHeight / 2; i++)
	{
		for (int j = 0; j < nWidth / 2; j++)
		{
			int xDes = j + (nMatrixHeight - (nWidth / 2));
			int yDes = i;
			int xPoint = nMatrixHeight - (nWidth / 2) - 1;
			int yPoint = nHeight / 2;
			int xSrc = (2 * xPoint) - xDes;
			int ySrc = (2 * yPoint) - yDes;
			int nAddressSrc = (int)pExpand;
			nAddressSrc = nAddressSrc + (sizeof(caltype) *((xSrc * nMatrixHeight) + ySrc));
			int nAddressDes = (int)pExpand;
			nAddressDes = nAddressDes + (sizeof(caltype) *((xDes * nMatrixHeight) + yDes));
			caltype * pSrc = (caltype *)nAddressSrc;
			caltype * pDes = (caltype *)nAddressDes;
			*pDes = *pSrc;
		}
	}
	//左下角+下边+右下角
	for (int i = 0; i < nHeight / 2; i++)
	{
		for (int j = 0; j < nWidth / 2; j++)
		{
			int xDes = j;
			int yDes = (nMatrixHeight - (nHeight / 2)) + i;
			int xPoint = nWidth / 2;
			int yPoint = (nMatrixHeight - (nHeight / 2)) - 1;
			int xSrc = (2 * xPoint) - xDes;
			int ySrc = (2 * yPoint) - yDes;
			int nAddressSrc = (int)pExpand;
			nAddressSrc = nAddressSrc + (sizeof(caltype) *((xSrc * nMatrixHeight) + ySrc));
			int nAddressDes = (int)pExpand;
			nAddressDes = nAddressDes + (sizeof(caltype) *((xDes * nMatrixHeight) + yDes));
			caltype * pSrc = (caltype *)nAddressSrc;
			caltype * pDes = (caltype *)nAddressDes;
			*pDes = *pSrc;
		}
	}
	for (int i = 0; i < nHeight / 2; i++)
	{
		for (int j = 0; j < nWidth; j++)
		{
			//Y = nHeight / 2
			//x轴对称 Y - (y2 - Y) = 2Y - y2
			int xDes = (nWidth / 2) + j;
			int yDes = (nMatrixHeight - (nHeight / 2)) + i;
			int yPoint = (nMatrixHeight - (nHeight / 2)) - 1;
			int xSrc = xDes;
			int ySrc = (2 * yPoint) - yDes;
			int nAddressSrc = (int)pExpand;
			nAddressSrc = nAddressSrc + (sizeof(caltype) *((xSrc * nMatrixHeight) + ySrc));
			int nAddressDes = (int)pExpand;
			nAddressDes = nAddressDes + (sizeof(caltype) *((xDes * nMatrixHeight) + yDes));
			caltype * pSrc = (caltype *)nAddressSrc;
			caltype * pDes = (caltype *)nAddressDes;
			*pDes = *pSrc;
		}
	}
	for (int i = 0; i < nHeight / 2; i++)
	{
		for (int j = 0; j < nWidth / 2; j++)
		{
			int xDes = j + (nMatrixWidth - (nWidth / 2));
			int yDes = (nMatrixHeight - (nHeight / 2)) + i;
			int xPoint = (nMatrixWidth - (nWidth / 2)) - 1;
			int yPoint = (nMatrixHeight - (nHeight / 2)) - 1;
			int xSrc = (2 * xPoint) - xDes;
			int ySrc = (2 * yPoint) - yDes;
			int nAddressSrc = (int)pExpand;
			nAddressSrc = nAddressSrc + (sizeof(caltype) *((xSrc * nMatrixHeight) + ySrc));
			int nAddressDes = (int)pExpand;
			nAddressDes = nAddressDes + (sizeof(caltype) *((xDes * nMatrixHeight) + yDes));
			caltype * pSrc = (caltype *)nAddressSrc;
			caltype * pDes = (caltype *)nAddressDes;
			*pDes = *pSrc;
		}
	}
	//左边
	for (int j = 0; j < nHeight; j++)
	{
		for (int i = 0; i < nWidth / 2; i++)
		{
			int xDes = i;
			int yDes = j + (nHeight / 2);
			int xPoint = nWidth / 2;
			int ySrc = yDes;
			int xSrc = (2 * xPoint) - xDes;
			int nAddressSrc = (int)pExpand;
			nAddressSrc = nAddressSrc + (sizeof(caltype) *((xSrc * nMatrixHeight) + ySrc));
			int nAddressDes = (int)pExpand;
			nAddressDes = nAddressDes + (sizeof(caltype) *((xDes * nMatrixHeight) + yDes));
			caltype * pSrc = (caltype *)nAddressSrc;
			caltype * pDes = (caltype *)nAddressDes;
			*pDes = *pSrc;
		}
	}
	//右边
	for (int j = 0; j < nHeight; j++)
	{
		for (int i = 0; i < nWidth / 2; i++)
		{
			int xDes = i + (nMatrixWidth - (nWidth / 2));
			int yDes = j + (nHeight / 2);
			int xPoint = (nMatrixWidth - (nWidth / 2)) - 1;
			int ySrc = yDes;
			int xSrc = (2 * xPoint) - xDes;
			int nAddressSrc = (int)pExpand;
			nAddressSrc = nAddressSrc + (sizeof(caltype) *((xSrc * nMatrixHeight) + ySrc));
			int nAddressDes = (int)pExpand;
			nAddressDes = nAddressDes + (sizeof(caltype) *((xDes * nMatrixHeight) + yDes));
			caltype * pSrc = (caltype *)nAddressSrc;
			caltype * pDes = (caltype *)nAddressDes;
			*pDes = *pSrc;
		}
	}


	for (int i = 0; i < matrix1.height; i++)
	{
		for (int j = 0; j < matrix1.width; j++)
		{
			//计算矩阵乘积
			int x = (kernel.width / 2) + j;
			int y = (kernel.height / 2) + i;
			int leftTopX = x - (kernel.width / 2);
			int leftTopY = y - (kernel.height / 2);
			caltype result = 0.0;
			for (int k = 0; k < kernel.height; k++)
			{
				for (int m = 0; m < kernel.width; m++)
				{
					int processX = leftTopX + m;
					int processY = leftTopY + k;
					int nAddress = (int)pExpand;
					nAddress = nAddress + (((processX * nMatrixHeight) + processY) * sizeof(caltype));
					caltype * ppd = (caltype *)nAddress;
					int _nAddress = (int)kernel.data;
					_nAddress = _nAddress + (sizeof(caltype) * ((m * kernel.height) + k));
					caltype * pf = (caltype *)_nAddress;
					result = result + (*ppd) * (*pf);
				}
			}
			int __nAddress = (int)matrix3.data;
			__nAddress = __nAddress + (sizeof(caltype) * ((j * matrix1.height) + i));
			caltype * pOutPixelData = (caltype *)__nAddress;
			*pOutPixelData = result;
			//
		}
	}
	//free memory
	free(pExpand);
	return LG_ERR_OTHER;
}

LGErrorStates LGMathematicalOp::LGMathematicalOperation::AbsoluteValue(_m_out_ MATRIX & matrix, _m_in_ MATRIX & _matrix)
{
	if (matrix.data && _matrix.data)
	{
		for (int i = 0; i < _matrix.height * _matrix.width; i++)
		{
			int address = (int)_matrix.data;
			int addressDes = (int)matrix.data;
			address = address + (sizeof(caltype) * i);
			addressDes = addressDes + (sizeof(caltype) * i);
			caltype * pData = (caltype *)address;
			caltype * pDataDes = (caltype *)addressDes;
			*pDataDes = abs(*pData);
		}
	}
	else
	{
		return LG_ERR_PARAM;
	}
}

LGErrorStates LGMathematicalOp::LGMathematicalOperation::Multiplication(_m_out_ MATRIX & matrix, _m_in_ MATRIX & _matrix, caltype lmd)
{
	if (matrix.data && _matrix.data)
	{
		for (int i = 0; i < _matrix.height * _matrix.width; i++)
		{
			int address = (int)_matrix.data;
			int addressDes = (int)matrix.data;
			address = address + (sizeof(caltype) * i);
			addressDes = addressDes + (sizeof(caltype) * i);
			caltype * pData = (caltype *)address;
			caltype * pDataDes = (caltype *)addressDes;
			*pDataDes = lmd * (*pData);
		}
	}
	else
	{
		return LG_ERR_PARAM;
	}
}

LGErrorStates LGMathematicalOp::LGMathematicalOperation::Mean(_m_out_ MATRIX & matrix, _m_in_ MATRIX & _matrix)
{
	int nDes = (int)matrix.data;
	for (int i = 0; i < _matrix.width; i++)
	{
		double fSum = 0.0;
		double fAverage = 0.0;
		for (int j = 0; j < _matrix.height; j++)
		{
			int x = i;
			int y = j;
			int nAddressSrc = (int)_matrix.data;
			nAddressSrc = nAddressSrc + (((x * _matrix.height) + y) * sizeof(double));
			double * pF = (double *)nAddressSrc;
			fSum = fSum + (*pF);
		}

		fAverage = fSum / _matrix.height;
		int nAddressDes = nDes + (i * sizeof(double));
		double * pDes = (double *)nAddressDes;
		*pDes = fAverage;
	}
	return LG_ERR_OTHER;
}

LGErrorStates LGMathematicalOp::LGMathematicalOperation::RowVectorMean(_m_out_ caltype & r, _m_in_ MATRIX & _matrix)
{
	int nDes = (int)_matrix.data;
	double nSum = 0.0;
	for (int i = 0; i < _matrix.width; i++)
	{
		int pDes = nDes + (sizeof(double) * i);
		caltype * n = (caltype *)pDes;
		nSum = nSum + (*n);
	}
	r = nSum / _matrix.width;
	return LG_ERR_OTHER;
}

LGErrorStates LGMathematicalOp::LGMathematicalOperation::HadamardMultiplicationEx(_m_in_ MATRIX & matrix1, _m_in_ MATRIX & matrix2, _m_out_ MATRIX & matrix3)
{
	//应该保证三个矩阵dimension，height，width一致
	int dimension = matrix1.dimension;
	if (matrix1.dimension != matrix2.dimension)
	{
		assert(0);
	}
	if (matrix1.dimension <= 0)
	{
		assert(0);
	}
	for (int i = 0; i < matrix1.height * matrix1.width; i++)
	{
		int nDes = (int)matrix3.data;
		int nSrc1 = (int)matrix1.data;
		int nSrc2 = (int)matrix2.data;
		nDes = nDes + (sizeof(caltype) * dimension * i);
		nSrc1 = nSrc1 + (sizeof(caltype) * dimension * i);
		nSrc2 = nSrc2 + (sizeof(caltype) * dimension * i);
		for (int j = 0; j < dimension; j++)
		{
			int n1 = (sizeof(caltype) * j) + nSrc1;
			int n2 = (sizeof(caltype) * j) + nSrc2;
			int n3 = (sizeof(caltype) * j) + nDes;
			caltype * p1 = (caltype *)n1;
			caltype * p2 = (caltype *)n2;
			caltype * p3 = (caltype *)n3;
			*p3 = (*p1) * (*p2);
		}
	}
	return LG_ERR_OTHER;
}

LGErrorStates LGMathematicalOp::LGMathematicalOperation::MatrixAdditionEx(_m_out_ MATRIX & matrix, _m_in_ MATRIX & matrix1, _m_in_ MATRIX & matrix2)
{
	//应该保证三个矩阵dimension，height，width一致
	int dimension = matrix1.dimension;
	if (matrix1.dimension != matrix2.dimension)
	{
		assert(0);
	}
	if (matrix1.dimension <= 0)
	{
		assert(0);
	}
	for (int i = 0; i < matrix1.height * matrix1.width; i++)
	{
		int nDes = (int)matrix.data;
		int nSrc1 = (int)matrix1.data;
		int nSrc2 = (int)matrix2.data;
		nDes = nDes + (sizeof(caltype) * dimension * i);
		nSrc1 = nSrc1 + (sizeof(caltype) * dimension * i);
		nSrc2 = nSrc2 + (sizeof(caltype) * dimension * i);
		for (int j = 0; j < dimension; j++)
		{
			int n1 = (sizeof(caltype) * j) + nSrc1;
			int n2 = (sizeof(caltype) * j) + nSrc2;
			int n3 = (sizeof(caltype) * j) + nDes;
			caltype * p1 = (caltype *)n1;
			caltype * p2 = (caltype *)n2;
			caltype * p3 = (caltype *)n3;
			*p3 = (*p1) + (*p2);
		}
	}
	return LG_ERR_OTHER;
}

LGErrorStates LGMathematicalOp::LGMathematicalOperation::InitializationMATRIX(caltype value, _m_in_ _m_out_ MATRIX & matrix)
{
	for (int i = 0; i < matrix.height * matrix.width; i++)
	{
		int address = (int)matrix.data;
		address = address + (sizeof(caltype) * i);
		double * d = (double *)address;
		*d = value;
	}
	return LG_ERR_OTHER;
}

LGErrorStates LGMathematicalOp::LGMathematicalOperation::InitializationMATRIX(_m_in_ _m_out_ MATRIX & matrix, int height, int width, int DataSize)
{
	matrix.height = height;
	matrix.width = width;
	matrix.data = malloc(DataSize * height * width);
	assert(matrix.data);
	matrix.dimension = DataSize / sizeof(caltype);
	return LG_ERR_OTHER;
}

LGErrorStates LGMathematicalOp::LGMathematicalOperation::InitializationMATRIX(void * p, int DataSize, _m_in_ _m_out_ MATRIX & matrix)
{
	///这里的算法速度可能会慢
	for (int i = 0; i < matrix.height * matrix.width; i++)
	{
		int address = (int)matrix.data;
		address = address + (DataSize * i);
		void * pDes = (void *)address;
		memcpy(pDes, p, DataSize);
	}
	return LG_ERR_OTHER;
}
bool LGPolygon::AddPoint(LGPoint * point)
{
	
	if (m_StartPoint == 0)
	{
		return false;
	}
	//测试发现当加入的两个点坐标完全一致的时候，会在找点的时候死循环
	//这里判断新加入的点与存在的点是否一致
	for (auto iter = memory.begin(); iter != memory.end(); iter++)
	{
		if (point->x == (*iter)->x && point->y == (*iter)->y)
		{
			return false;
		}
	}
	//SetEndPoint(*point);
	LGPoint * _point = GetNewPoint(point->x, point->y);
	m_EndPoint = _point;
	assert(m_CurrentPoint);
	map.insert(std::pair<LGPoint *, LGPoint *>(m_CurrentPoint, _point));
	m_CurrentPoint = _point;
	return true;
}

bool LGPolygon::GetPolygon(AllPointPath & path)
{
	//这里的path由外部分配
	path.currentPoint->x = m_StartPoint->x;
	path.currentPoint->y = m_StartPoint->y;
	AllPointPath * _path = &path;
	LGPoint * point = m_StartPoint;
	for (int i = 0; i < map.size(); i++)
	{
		if(map.count(point))
		{
			_path->currentPoint->x = map.at(point)->x;

			_path->currentPoint->y = map.at(point)->y;

			_path = _path->path;

			point = map.at(point);
		}		
	}
	_path->currentPoint->x = point->x;
	_path->currentPoint->y = point->y;
	_path->path = 0;
	return true;
}

bool LGPolygon::GetNextPoint(LGPoint & point , LGPoint & nextPoint)
{
	//算法速度可能会慢
	for (auto iter = map.begin(); iter != map.end(); iter++)
	{
		if (iter->first->x == point.x && iter->first->y == point.y)
		{
			nextPoint.x = iter->second->x;
			nextPoint.y = iter->second->y;
			return true;
		}
	}
	return false;
}

bool LGPolygon::GetLastPoint(LGPoint & point, LGPoint & lastPoint)
{
	//算法速度可能会慢
	for (auto iter = map.begin(); iter != map.end(); iter++)
	{
		if (iter->second->x == point.x && iter->second->y == point.y)
		{
			lastPoint.x = iter->first->x;
			lastPoint.y = iter->first->y;
			return true;
		}
	}
	return false;
}

bool LGPolygon::GetStartPoint(LGPoint & startPoint)
{
	startPoint.x = m_StartPoint->x;
	startPoint.y = m_StartPoint->y;
	return false;
}

bool LGPolygon::GetEndPoint(LGPoint & endPoint)
{
	endPoint.x = m_EndPoint->x;
	endPoint.y = m_EndPoint->y;
	return false;
}

void LGPolygon::SetStartPoint(LGPoint & point)
{
	m_StartPoint = GetNewPoint(point.x , point.y);
	m_CurrentPoint = m_StartPoint;
}

void LGPolygon::SetEndPoint(LGPoint & point)
{
	m_EndPoint = GetNewPoint(point.x, point.y);
}

LGPoint * LGPolygon::GetNewPoint(double x, double y)
{
	LGPoint * pStart = (LGPoint *)malloc(sizeof(LGPoint));
	if (pStart == 0)
	{
		assert(0);
	}
	pStart->x = x;
	pStart->y = y;
	memory.push_back(pStart);
	return pStart;
}

void LGPolygon::Clear()
{
	for (auto iter = memory.begin(); iter != memory.end(); iter++)
	{
		free(*iter);
		*iter = 0;
	}
}


