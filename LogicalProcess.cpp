#include "LogicalProcess.h"

CLogicalProcess::CLogicalProcess()
{
}

CLogicalProcess::~CLogicalProcess()
{
}

LGErrorStates CLogicalProcess::InitData(int x, int y)
{
	return LGErrorStates();
}

LGErrorStates CLogicalProcess::SetData(int x, int y, int nIndex, void * pUserData)
{
	if (GetGameMode() == easy)
	{
		m_easyMode[x][y].isExist = true;
		m_easyMode[x][y].pUserData = pUserData;
		m_easyMode[x][y].nIndex = nIndex;
	}
	else if (GetGameMode() == hard)
	{
		m_hardMode[x][y].isExist = true;
		m_hardMode[x][y].pUserData = pUserData;
		m_hardMode[x][y].nIndex = nIndex;
	}
	return LG_ERR_OTHER;
}

LGErrorStates CLogicalProcess::SetDataEx(int x, int y, BlockInfomationST * pInfo)
{
	if (GetGameMode() == easy && NULL != pInfo)
	{
		m_easyMode[x][y] = *pInfo;
	}
	else if (GetGameMode() == hard && NULL != pInfo)
	{
		m_hardMode[x][y] = *pInfo;
	}
	return LG_ERR_OTHER;
}

LGErrorStates CLogicalProcess::GetConnectInfo(LG_Path & path, int x1, int y1, int x2, int y2)
{
	if (IsOneLoadConnect(x1, y1, x2, y2) == true)
	{
		ClearPoint(x1, y1, x2, y2);
		path.nPoint = 0;
		return LGErrorStates::LG_ERR_ONEPATHCONNECT;
	}
	if (IsTwoLoadConnect(x1, y1, x2, y2 , path) == true)
	{
		ClearPoint(x1, y1, x2, y2);
		path.nPoint = 1;
		return LGErrorStates::LG_ERR_TWOPATHCONNECT;
	}
	if (IsThreeLoadConnect(x1, y1, x2, y2, path) == true)
	{
		ClearPoint(x1, y1, x2, y2);
		path.nPoint = 2;
		return LGErrorStates::LG_ERR_THREEPATHCONNECT;
	}
	return LGErrorStates::LG_ERR_NOPATHCONNECT;
}

LGErrorStates CLogicalProcess::ConvertPath(LG_Path & logicPath, LG_Path & realPath, LG_ConvertInfo info)
{
	return LGErrorStates();
}

void CLogicalProcess::SetGameMode(LINKGAMEGAMEMODE lggm)
{
	m_GameMode = lggm;
}

LINKGAMEGAMEMODE CLogicalProcess::GetGameMode()
{
	return m_GameMode;
}

bool CLogicalProcess::GetBlockInformation(BlockInfomationST ** pInfo, int x, int y)
{
	bool bRtn = true;
	if (NULL != *pInfo)
	{
		*pInfo = NULL;
	}
	if (IsBlockExist(x, y) == false)
	{
		return false;
	}
	BlockInfomationST * pbi = new BlockInfomationST;
	//ASSERT(pbi);
	*pInfo = pbi;
	if (GetGameMode() == LINKGAMEGAMEMODE::easy)
	{
		*pbi = m_easyMode[x][y];
	}
	else if (GetGameMode() == LINKGAMEGAMEMODE::hard)
	{
		*pbi = m_hardMode[x][y];
	}
	else
	{
		bRtn = false;
		delete pbi;
	}
	return true;
}

bool CLogicalProcess::IsBlockExist(int x, int y)
{
	if (GetGameMode() == LINKGAMEGAMEMODE::easy)
	{
		return m_easyMode[x][y].isExist;
	}
	else if (GetGameMode() == LINKGAMEGAMEMODE::hard)
	{
		return m_hardMode[x][y].isExist;
	}
	else
	{
		return false;
	}
}

bool CLogicalProcess::IsOneLoadConnect(int x1, int y1, int x2, int y2)
{
	if (IsBlockExist(x1, y1) && IsBlockExist(x2, y2))
	{
		if (x1 == x2 || y1 == y2)
		{
			if (x1 == x2)
			{
				bool bRtn = IsHaveobstacle(x1, y1, x2, y2, Orientation::vertical);
				return bRtn;
			}
			else if (y1 == y2)
			{
				bool bRtn = IsHaveobstacle(x1, y1, x2, y2, Orientation::horizontal);
				return bRtn;
			}
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

bool CLogicalProcess::IsTwoLoadConnect(int x1, int y1, int x2, int y2, LG_Path & path)
{
	bool bRtn = false;
	if (IsBlockExist(x1, y1) == false || IsBlockExist(x2, y2) == false)
		return false;
	if (IsBlockExist(x1, y2) == false)
	{
		SetPointInformation(x1, y2, true);
		if (IsOneLoadConnect(x1, y2, x1, y1) == true && IsOneLoadConnect(x1, y2, x2, y2) == true)
		{
			memset(&path, 0, sizeof(LG_Path));
			path.pointBegin.x = x1;
			path.pointBegin.y = y2;
			path.pointBegin.pNext = 0;
			//恢复点的属性
			SetPointInformation(x1, y2, false);
			//
			/*SetPointInformation(x1, y1, false);
			SetPointInformation(x2, y2, false);*/
			return true;
		}
		SetPointInformation(x1, y2, false);
	}
	if (IsBlockExist(x2, y1) == false)
	{
		SetPointInformation(x2, y1, true);
		if (IsOneLoadConnect(x2, y1, x2, y2) == true && IsOneLoadConnect(x2, y1, x1, y1) == true)
		{
			memset(&path, 0, sizeof(LG_Path));
			path.pointBegin.x = x2;
			path.pointBegin.y = y1;
			path.pointBegin.pNext = 0;
			//恢复点的属性
			SetPointInformation(x2, y1, false);
			//
			/*SetPointInformation(x1, y1, false);
			SetPointInformation(x2, y2, false);*/
			return true;
		}
		SetPointInformation(x2, y1, false);
	}
	return bRtn;
}

bool CLogicalProcess::IsTwoLoadConnect(int x1, int y1, int x2, int y2, std::vector<LG_Path>* vec)
{
	bool bRtn = false;
	if (IsBlockExist(x1, y1) == false || IsBlockExist(x2, y2) == false)
		return false;
	if (NULL == vec)
		return false;
	if (IsBlockExist(x1, y2) == false)
	{
		SetPointInformation(x1, y2, true);
		if (IsOneLoadConnect(x1, y2, x1, y1) == true && IsOneLoadConnect(x1, y2, x2, y2) == true)
		{
			LG_Path path;
			memset(&path, 0, sizeof(LG_Path));
			path.pointBegin.x = x1;
			path.pointBegin.y = y2;
			path.pointBegin.pNext = 0;
			vec->push_back(path);
			//恢复点的属性
			SetPointInformation(x1, y2, false);
			//
			/*SetPointInformation(x1, y1, false);
			SetPointInformation(x2, y2, false);*/
			bRtn = true;
		}
		SetPointInformation(x1, y2, false);
	}
	if (IsBlockExist(x2, y1) == false)
	{
		SetPointInformation(x2, y1, true);
		if (IsOneLoadConnect(x2, y1, x2, y2) == true && IsOneLoadConnect(x2, y1, x1, y1) == true)
		{
			LG_Path path;
			memset(&path, 0, sizeof(LG_Path));
			path.pointBegin.x = x2;
			path.pointBegin.y = y1;
			path.pointBegin.pNext = 0;
			vec->push_back(path);
			//恢复点的属性
			SetPointInformation(x2, y1, false);
			bRtn = true;
			//
			/*SetPointInformation(x1, y1, false);
			SetPointInformation(x2, y2, false);*/
		}
		SetPointInformation(x2, y1, false);
	}
	return bRtn;
}

bool CLogicalProcess::IsThreeLoadConnect(int x1, int y1, int x2, int y2, LG_Path & path)
{
	//判断中途的第二个点的坐标范围（分为两种情况，一种是在(x2,y2)点的水平方向上另一种是在垂直方向上）
	if (IsBlockExist(x1, y1) == false || IsBlockExist(x2, y2) == false)
		return false;
	int xLength = 0;
	int yLength = 0;
	if (GetGameMode() == LINKGAMEGAMEMODE::easy)
	{
		xLength = EASYXLENGTH;
		yLength = EASYYLENGTH;
	}
	else if (GetGameMode() == LINKGAMEGAMEMODE::hard)
	{
		xLength = HARDXLENGTH;
		yLength = HARDYLENGTH;
	}

	for (int i = 0; i < xLength; i++)
	{
		if (i != x2 && i != x1)
		{
			if (IsBlockExist(i, y2) == false)
			{
				SetPointInformation(i, y2, true);
				/*LG_Path _path;
				memset(&_path, 0 , sizeof(LG_Path));*/
				std::vector<LG_Path> vecPath;
				if (IsTwoLoadConnect(i, y2, x1, y1, &vecPath) == true)
				{
					for (auto iter = vecPath.begin(); iter != vecPath.end(); iter++)
					{
						if ((*iter).pointBegin.y != y2 && IsOneLoadConnect(i, y2, x2, y2) == true)
						{
							SetPointInformation(i, y2, false);
							//拼装路径
							memset(&path, 0, sizeof(LG_Path));
							path.nPoint = 2;
							path.pointBegin.x = iter->pointBegin.x;
							path.pointBegin.y = iter->pointBegin.y;
							path.pointBegin.pNext = &(path.pointEnd);
							path.pointEnd.x = i;
							path.pointEnd.y = y2;
							path.pointEnd.pNext = 0;
							return true;
						}
					}
				}
				//if (IsTwoLoadConnect(i, y2, x1, y1, _path) == true && _path.pointBegin.y != y2 && IsOneLoadConnect(i , y2 , x2 , y2) == true)
				//{
				//	SetPointInformation(i, y2, false);
				//	//拼装路径
				//	memset(&path, 0, sizeof(LG_Path));
				//	path.nPoint = 2;
				//	path.pointBegin.x = _path.pointBegin.x;
				//	path.pointBegin.y = _path.pointBegin.y;
				//	path.pointBegin.pNext = &(path.pointEnd);
				//	path.pointEnd.x = 1;
				//	path.pointEnd.y = y2;
				//	path.pointEnd.pNext = 0;
				//	return true;
				//}
				SetPointInformation(i, y2, false);
			}
		}
	}
	for (int i = 0; i < yLength; i++)
	{
		if (i != y1 && i != y2)
		{
			if (IsBlockExist(x2, i) == false)
			{
				SetPointInformation(x2, i, true);
				std::vector<LG_Path> vecPath;
				if (IsTwoLoadConnect(x2, i, x1, y1, &vecPath) == true)
				{
					for (auto iter = vecPath.begin(); iter != vecPath.end(); iter++)
					{
						if (iter->pointBegin.x != x2 && IsOneLoadConnect(x2, i, x2, y2) == true)
						{
							SetPointInformation(x2, i, false);
							//拼装路径
							memset(&path, 0, sizeof(LG_Path));
							path.nPoint = 2;
							path.pointBegin.x = iter->pointBegin.x;
							path.pointBegin.y = iter->pointBegin.y;
							path.pointBegin.pNext = &(path.pointEnd);
							path.pointEnd.x = x2;
							path.pointEnd.y = i;
							path.pointEnd.pNext = 0;
							return true;
						}
					}
				}
				SetPointInformation(x2, i, false);
			}
		}
	}
	return false;
}

void CLogicalProcess::SetPointInformation(int x, int y, bool isExist)
{
	if (GetGameMode() == LINKGAMEGAMEMODE::easy)
	{
		m_easyMode[x][y].isExist = isExist;
	}
	else if (GetGameMode() == LINKGAMEGAMEMODE::hard)
	{
		m_hardMode[x][y].isExist = isExist;
	}
	else
	{
		;
	}
}

bool CLogicalProcess::IsHaveobstacle(int x1, int y1, int x2, int y2, Orientation ori)
{
	if (ori == horizontal && y1 == y2 && x1 != x2)
	{
		//遍历查找
		for (int i = min(x1, x2) + 1; i < max(x1, x2) - 1; i++)
		{
			if (IsBlockExist(i, y2) == true)
			{
				return false;
			}
		}
	}
	if (ori == vertical && x1 == x2 && y1 != y2)
	{
		for (int i = min(y1, y2) + 1; i < max(y1, y2); i++)
		{
			if (IsBlockExist(x2, i) == true)
			{
				return false;
			}
		}
	}
	return true;
}

void CLogicalProcess::ClearPoint(int x1, int y1, int x2, int y2)
{
	SetPointInformation(x1, y1, false);
	SetPointInformation(x2, y2, false);
}
int ToRandNum(int iStart, int iEnd)//provide by shangji
{
	srand((unsigned)time(NULL));
	return (rand() % (iEnd - iStart + 1)) + iStart;
}
LGErrorStates CLinkGameGenerate::Methods2(int nx, int ny , void * p , int nTypes)
{
	if (nx*ny % 2 != 0)
		return LG_ERR_SQUARESNUMILLEGALiMG;
	int *pMatrix = (int *)p;

	enum { enNotIntilize = -1 };

	memset(pMatrix, -1, nx*ny * sizeof(int));

	int iStartType = 0, iEndType = nTypes - 1;
	int iMiddle = nx*ny / 2;

	int iType = 0;

	for (int i = 0; i<iMiddle; i++, iType++)
	{
		iType %= nTypes;
		//iType = ToRandNum(iStartType, iEndType);
		pMatrix[i] = pMatrix[i + iMiddle] = iType;
	}


	for (int j = 0; j < 10; j++)
	{
		for (int i = 0; i<nx*ny; i++)
		{
			int iResult = ToRandNum(0, nx*ny - 1);
			if (i == iResult)
				continue;
			int iTemp = pMatrix[iResult];
			pMatrix[iResult] = pMatrix[i];
			pMatrix[i] = iTemp;
		}
	}


	return LG_ERR_OTHER;
}

LGErrorStates CLinkGameGenerate::Methods3(int nx, int ny, void * p, RangeST & _range)
{
	if (nx*ny % 2 != 0)
		return LG_ERR_SQUARESNUMILLEGALiMG;
	if (NULL == p)
	{
		return LG_ERR_NULLPOINTER;
	}
	int MAXX = nx;
	int MAXY = ny;
	int** matrix = (int**)p;
	RangeST range;
	range.n = _range.n;
	range.m = _range.m;
	int nTime = time(0);//精度丢失
	bool b = false;
	int * database = (int *)malloc(((MAXX * MAXY) /2) * sizeof(int));
	for (int i = 0; i < (MAXX * MAXY) / 2; i++)
	{
		int n = GenerateRandNum(&nTime, b, range);
		// if(b == true)
		// {
		// 	printf("%d\n" , nTime) ;	
		// }
		database[i] = nTime;
		nTime = n + 1;
	}
	//int matrix[MAXX][MAXY];
	int index = 0;
	for (int i = 0; i < MAXY / 2; i++)
	{
		for (int j = 0; j < MAXX; j++)
		{
			matrix[j][i] = database[index];
			// matrix[MAXX - 1 - j][MAXY - 1 - i] = database[((MAXX * MAXY) / 2) - 1 - index] ; one way
			matrix[MAXX - 1 - j][MAXY - 1 - i] = database[index];//another way
			index++;
		}

	}
	RandomDisturb1(p);
	for (int i = 0; i < MAXY; i++)
	{
		for (int j = 0; j < MAXX; j++)
		{
			printf("%d  ", matrix[j][i]);
		}
		printf("\n");
	}
	free(database);
	return LG_ERR_OTHER;
}

LGErrorStates CLinkGameGenerate::RandomDisturb1(void * p , int nx , int ny)
{
	int* matrix = (int*)p;
	
}

int CLinkGameGenerate::GenerateRandNum(int * pInt, bool & b, RangeST & range)
{
	if (pInt == 0)
	{
		b = false;
		return 0;
	}
	else
	{
		srand((unsigned)(*pInt));
		int M = rand();
		int nRtn = *pInt;
		*pInt = M % (range.m - range.n + 1) + range.n;
		b = true;
		return nRtn;
	}
}
