#include"stdio.h"
#include"windows.h"
#include"../../../../LogicalProcess.h"
//#define QQLLK
#define excelso
#pragma comment(lib,"Project1.lib")

#ifdef primary
#define XLENGTH
#define YLENGTH
#define BLANK 31
#endif // primary
#ifdef middlerank
#define XLENGTH
#define YLENGTH
#define BLANK 27
#endif // middlerank
#ifdef advanced
#define XLENGTH
#define YLENGTH
#define BLANK 23
#endif // advanced
#ifdef excelso
#define XLENGTH 20
#define YLENGTH 12
#define BLANK 16
#endif // Excelso
//定义连连看棋牌的桌面位置
#define firstpointx 60
#define firstpointy 43
#define pid 2248
#define hwnd 658490
#define stepx 40
#define stepy 50
//

void click(int x1 , int y1 , int x2 , int y2 , int num)
{
	///
	if (num == 2)
	{
		/*SetCursorPos(569, 472);
		mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);*/
		int clickx = firstpointx + ((x1 - 1) * stepx);
		int clicky = firstpointy + ((y1 - 1) * stepy);
		//SetCursorPos(clickx, clicky);
		//mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
		SendMessage((HWND)hwnd, WM_LBUTTONDOWN, NULL, MAKELPARAM(clickx, clicky));
		//	SendMessage((HWND)hwnd, WM_LBUTTONUP, NULL, MAKELPARAM(clickx, clicky));
		Sleep(300);
		///
		int _clickx = firstpointx + ((x2 - 1) * stepx);
		int _clicky = firstpointy + ((y2 - 1) * stepy);
		PostMessage((HWND)hwnd, WM_LBUTTONDOWN, NULL, MAKELPARAM(_clickx, _clicky));
		//SendMessage((HWND)hwnd, WM_LBUTTONDOWN, NULL, MAKELPARAM(413, 335));
		//SendMessage((HWND)hwnd, WM_LBUTTONUP, NULL, MAKELPARAM(413, 335));
		
		Sleep(500);
		//SendMessage(FindWindow("#32770", "成功"), WM_LBUTTONDOWN, NULL, MAKELPARAM(95, 129));
		//SendMessage(FindWindow("#32770", "成功"), WM_LBUTTONUP, NULL, MAKELPARAM(95, 129));
		HWND h = FindWindowEx(FindWindow("#32770", "成功"), NULL, NULL, NULL);
		//h = FindWindow("#32770", "成功");
		SendMessage(h, WM_LBUTTONDOWN, NULL, MAKELPARAM(10, 10));
		SendMessage(h, WM_LBUTTONUP, NULL, MAKELPARAM(10, 10));
		Sleep(500);
	}
	else
	{
		int clickx = firstpointx + ((x1 - 1) * stepx);
		int clicky = firstpointy + ((y1 - 1) * stepy);
		//SetCursorPos(clickx, clicky);
		//mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
		SendMessage((HWND)hwnd, WM_LBUTTONDOWN, NULL, MAKELPARAM(clickx, clicky));
		//	SendMessage((HWND)hwnd, WM_LBUTTONUP, NULL, MAKELPARAM(clickx, clicky));
		Sleep(1);
		///
		int _clickx = firstpointx + ((x2 - 1) * stepx);
		int _clicky = firstpointy + ((y2 - 1) * stepy);
		SendMessage((HWND)hwnd, WM_LBUTTONDOWN, NULL, MAKELPARAM(_clickx, _clicky));
		//	PostMessage((HWND)hwnd, WM_LBUTTONUP, NULL, MAKELPARAM(_clickx, _clicky));
		//SetCursorPos(_clickx, _clicky);
		//mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
		//Sleep(1000);
		//Sleep(100);
	}
}
void readdata(CLogicalProcess & lp)
{
	int matrix[XLENGTH][YLENGTH];
	//将最外层元素赋值-1(将第一行，第一列，最后一行，最后一列元素赋值-1)
	for (int i = 0; i < XLENGTH; i++)
	{
		//第一行
		matrix[i][0] = 88888;
		//
	}
	for (int i = 0; i < XLENGTH; i++)
	{
		//最后一行
		matrix[i][YLENGTH - 1] = 88888;
		//
	}
	for (int i = 0; i < YLENGTH; i++)
	{
		//第一列
		matrix[0][i] = 88888;
		//
	}
	for (int i = 0; i < YLENGTH; i++)
	{
		//最后一列
		matrix[XLENGTH - 1][i] = 88888;
		//
	}
	
	LINKGAMEGAMEMODE lggm = userdefined;
	lp.SetGameMode(lggm);



	DWORD LLKID = pid;
	HANDLE hLLK = OpenProcess(PROCESS_VM_READ, FALSE, LLKID);
	if (hLLK == NULL)
	{
		//error
	}


	//我只是读内存，应该默认都有读的权限了吧，不用这个应该可以VirtualProtectEx


	LPCVOID lpBaseAddress = (LPCVOID)1702420;
	int nMem = ((XLENGTH - 2) * (YLENGTH - 2) * 4) + ((XLENGTH - 2 - 1) * BLANK);
	LPVOID lpBuffer = malloc(nMem);
	
	SIZE_T nSize = nMem;
	SIZE_T *lpNumberOfBytesRead = NULL;
	ReadProcessMemory(hLLK, lpBaseAddress, lpBuffer, nSize, lpNumberOfBytesRead);
	
	//开始读取内存并初始化数据
	LPVOID address = lpBuffer;
	for (int i = 0; i < XLENGTH - 2; i++)
	{
		for (int j = 0; j < YLENGTH - 2; j++)
		{

			byte * pData = (byte *)address;
			byte data = *pData;
			*pData = 1;
			matrix[i + 1][j + 1] = (int)data;
			int nextAddress = int(address);
			nextAddress = nextAddress + 4;
			address = (LPVOID)nextAddress;
		}
		int newAddress = (int)address;
		newAddress = newAddress - 1 + BLANK + 1;
		address = (LPVOID)newAddress;
	}
	for (int i = 0; i < YLENGTH; i++)
	{
		for (int j = 0; j < XLENGTH; j++)
		{
			BlockInfomationST block;
			memset(&block, 0, sizeof(BlockInfomationST));
			//这里先设置每个方块的id，对应的图片，坐标
			block.nIndex = matrix[j][i];
			block.tlcX = j;
			block.tlcY = i;
			block.isExist = true;
			if (block.nIndex == 88888)
			{
				block.nIndex = -1;
				block.isExist = false;
			}
			if (block.nIndex == 0)
			{
				block.nIndex = -1;
				block.isExist = false;
			}
			lp.SetDataEx(j, i, &block);
		}
	}


	//
	//测试显示
	/*for (int i = 0; i < YLENGTH; i++)
	{
	for (int j = 0; j < XLENGTH; j++)
	{
	printf("%d  ", (lp.m_UDMode)[j][i].nIndex);
	}
	printf("\n");
	}*/

	//
	CloseHandle(hLLK);
	free(lpBuffer);
	//delete(lpBuffer);
}
void main()
{
	CLogicalProcess lp;
	readdata(lp);
	//开始消除
	//从（1，1）到（XLENGTH - 2 ， YLENGTH - 2）
	int x = 1; int y = 1;
	int _x = 1; int _y = 1;
	int num = (XLENGTH - 2) * (YLENGTH - 2);
	LG_Path path;
	while (true)
	{
		x = 0; y = 1;
		while (x != XLENGTH - 2 || y != YLENGTH - 2)
		{
			if (x == XLENGTH - 2)
			{
				x = 1; y++;
				_y = y; _x = x;
				while (_x != XLENGTH - 2 || _y != YLENGTH - 2)
				{
					if (_x == XLENGTH - 2)
					{
						_x = 1; _y++;
						if (lp.m_UDMode[x][y].nIndex == lp.m_UDMode[_x][_y].nIndex)
						{
							if (lp.GetConnectInfo(path, x, y, _x, _y) != LG_ERR_NOPATHCONNECT)
							{
								//消除，跳出循环
								click(x, y, _x, _y , num);
								num = num - 2;
								if (num == 0)
								{
									x = XLENGTH - 2; y = YLENGTH - 2;
									num = (XLENGTH - 2) * (YLENGTH - 2);
								}
								readdata(lp);
								break;
							}
						}
						
					}
					else if (_y == YLENGTH - 2)
					{
						_x++;
						if (lp.m_UDMode[x][y].nIndex == lp.m_UDMode[_x][_y].nIndex)
						{
							if (lp.GetConnectInfo(path, x, y, _x, _y) != LG_ERR_NOPATHCONNECT)
							{
								//消除，跳出循环
								click(x, y, _x, _y, num);
								num = num - 2;
								if (num == 0)
								{
									x = XLENGTH - 2; y = YLENGTH - 2;
									num = (XLENGTH - 2) * (YLENGTH - 2);
								}
								readdata(lp);
								break;
							}
						}
					}
					else
					{
						_x++;
						if (lp.m_UDMode[x][y].nIndex == lp.m_UDMode[_x][_y].nIndex)
						{
							if (lp.GetConnectInfo(path, x, y, _x, _y) != LG_ERR_NOPATHCONNECT)
							{
								//消除，跳出循环
								click(x, y, _x, _y, num);
								num = num - 2;
								if (num == 0)
								{
									x = XLENGTH - 2; y = YLENGTH - 2;
									num = (XLENGTH - 2) * (YLENGTH - 2);
								}
								readdata(lp);
								break;
							}
						}
					}
				}
			}
			else if (y == YLENGTH - 2)
			{
				x++;
				_y = y; _x = x;
				while (_x != XLENGTH - 2 || _y != YLENGTH - 2)
				{
					if (_x == XLENGTH - 2)
					{
						_x = 1; _y++;
						if (lp.m_UDMode[x][y].nIndex == lp.m_UDMode[_x][_y].nIndex)
						{
							if (lp.GetConnectInfo(path, x, y, _x, _y) != LG_ERR_NOPATHCONNECT)
							{
								//消除，跳出循环
								click(x, y, _x, _y, num);
								num = num - 2;
								if (num == 0)
								{
									x = XLENGTH - 2; y = YLENGTH - 2;
									num = (XLENGTH - 2) * (YLENGTH - 2);
								}
								readdata(lp);
								break;
							}
						}

					}
					else if (_y == YLENGTH - 2)
					{
						_x++;
						if (lp.m_UDMode[x][y].nIndex == lp.m_UDMode[_x][_y].nIndex)
						{
							if (lp.GetConnectInfo(path, x, y, _x, _y) != LG_ERR_NOPATHCONNECT)
							{
								//消除，跳出循环
								click(x, y, _x, _y, num);
								num = num - 2;
								if (num == 0)
								{
									x = XLENGTH - 2; y = YLENGTH - 2;
									num = (XLENGTH - 2) * (YLENGTH - 2);
								}
								readdata(lp);
								break;
							}
						}
					}
					else
					{
						_x++;
						if (lp.m_UDMode[x][y].nIndex == lp.m_UDMode[_x][_y].nIndex)
						{
							if (lp.GetConnectInfo(path, x, y, _x, _y) != LG_ERR_NOPATHCONNECT)
							{
								//消除，跳出循环
								click(x, y, _x, _y, num);
								num = num - 2;
								if (num == 0)
								{
									x = XLENGTH - 2; y = YLENGTH - 2;
									num = (XLENGTH - 2) * (YLENGTH - 2);
								}
								readdata(lp);
								break;
							}
						}
					}
				}
			}
			else
			{
				x++;
				_y = y; _x = x;
				while (_x != XLENGTH - 2 || _y != YLENGTH - 2)
				{
					if (_x == XLENGTH - 2)
					{
						_x = 1; _y++;
						if (lp.m_UDMode[x][y].nIndex == lp.m_UDMode[_x][_y].nIndex)
						{
							if (lp.GetConnectInfo(path, x, y, _x, _y) != LG_ERR_NOPATHCONNECT)
							{
								//消除，跳出循环
								click(x, y, _x, _y, num);
								num = num - 2;
								if (num == 0)
								{
									x = XLENGTH - 2; y = YLENGTH - 2;
									num = (XLENGTH - 2) * (YLENGTH - 2);
								}
								readdata(lp);
								break;
							}
						}

					}
					else if (_y == YLENGTH - 2)
					{
						_x++;
						if (lp.m_UDMode[x][y].nIndex == lp.m_UDMode[_x][_y].nIndex)
						{
							if (lp.GetConnectInfo(path, x, y, _x, _y) != LG_ERR_NOPATHCONNECT)
							{
								//消除，跳出循环
								click(x, y, _x, _y, num);
								num = num - 2;
								if (num == 0)
								{
									x = XLENGTH - 2; y = YLENGTH - 2;
									num = (XLENGTH - 2) * (YLENGTH - 2);
								}
								readdata(lp);
								break;
							}
						}
					}
					else
					{
						_x++;
						if (lp.m_UDMode[x][y].nIndex == lp.m_UDMode[_x][_y].nIndex)
						{
							if (lp.GetConnectInfo(path, x, y, _x, _y) != LG_ERR_NOPATHCONNECT)
							{
								//消除，跳出循环
								click(x, y, _x, _y, num);
								num = num - 2;
								if (num == 0)
								{
									x = XLENGTH - 2; y = YLENGTH - 2;
									num = (XLENGTH - 2) * (YLENGTH - 2);
								}
								readdata(lp);
								break;
							}
						}
					}
				}
			}	
		}
		if (num == 0)
		{
			/*num = (XLENGTH - 2) * (YLENGTH - 2);
			SetCursorPos(569, 472);
			mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);*/
		}
		
	}
	
	//
	getchar();
}