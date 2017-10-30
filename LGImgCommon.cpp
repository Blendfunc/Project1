#define _CRT_SECURE_NO_WARNINGS
#define _LG_IMG_COMMON_
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
			float redWeight, blueWeight, greenWeight;
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
	for (int i = 0 ; i < dataSrcConvert.nMatrixHeight * dataSrcConvert.nMatrixWidth ; i++)
	{
		//对应位置拷贝
		int nAddressDes = (int)p;
		int nAddressSrc = (int)dataSrcConvert.pMatrixColorData;
		nAddressDes = nAddressDes + (sizeof(byte) * i);
		nAddressSrc = nAddressSrc + (sizeof(PixelData) * i);
		PixelData * pData = (PixelData *)nAddressSrc;
		byte * pDataDes = (byte *)nAddressDes;
		*pDataDes = max(max(pData->b, pData->g), pData->r);
	}


	//
	void * _p = malloc(sizeof(double) * dataSrcConvert.nMatrixHeight * dataSrcConvert.nMatrixWidth);

	assert(_p);

	for (int i = 0 ; i < dataSrcConvert.nMatrixHeight * dataSrcConvert.nMatrixWidth ; i++)
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
	double alpha = 0.5;
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





	//



	//
	/*if (data.img == IMGBMP24)
	{

		LGGetColorData(idOut, data);


	}
	else
	{

	}*/







	//free memory
	free(p);
	free(_p);
	free(px);
	free(py);
	return LG_ERR_OTHER;
}

LGErrorStates LGMathematicalOp::LGMathematicalOperation::MatrixAddition(_m_out_ MATRIX & matrix, _m_in_ MATRIX & matrix1, _m_in_ MATRIX & matrix2)
{

	if (matrix1.width == matrix2.width && matrix1.height == matrix2.height)
	{
		for (int i = 0; i < matrix1.height * matrix1.width ; i++)
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
			int __nAddress = (int)matrix1.data;
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
