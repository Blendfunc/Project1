#include"stdio.h"
#include"windows.h"
#include "TlHelp32.h"
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
#define XLENGTH 16
#define YLENGTH 9
#define BLANK 0
#endif // Excelso
//定义连连看棋牌的桌面位置
#define firstpointx 349
#define firstpointy 202
//#define pid 12924
//#define hwnd 71968198
#define baseaddress 4480460
#define stepx 45
#define stepy 45
//

void click(int x1, int y1, int x2, int y2, int num , HWND llkwnd)
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
		SendMessage((HWND)llkwnd, WM_LBUTTONDOWN, NULL, MAKELPARAM(clickx, clicky));
		//	SendMessage((HWND)hwnd, WM_LBUTTONUP, NULL, MAKELPARAM(clickx, clicky));
		Sleep(300);
		///
		int _clickx = firstpointx + ((x2 - 1) * stepx);
		int _clicky = firstpointy + ((y2 - 1) * stepy);
		PostMessage((HWND)llkwnd, WM_LBUTTONDOWN, NULL, MAKELPARAM(_clickx, _clicky));
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
		int clickx = firstpointx + ((x1) * stepx);
		int clicky = firstpointy + ((y1) * stepy);
		//SetCursorPos(clickx, clicky);
		//mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
		SendMessage((HWND)llkwnd, WM_LBUTTONDOWN, NULL, MAKELPARAM(clickx, clicky));
		//	SendMessage((HWND)hwnd, WM_LBUTTONUP, NULL, MAKELPARAM(clickx, clicky));
		//Sleep(1);
		///
		int _clickx = firstpointx + ((x2) * stepx);
		int _clicky = firstpointy + ((y2) * stepy);
		SendMessage((HWND)llkwnd, WM_LBUTTONDOWN, NULL, MAKELPARAM(_clickx, _clicky));
		//	PostMessage((HWND)hwnd, WM_LBUTTONUP, NULL, MAKELPARAM(_clickx, _clicky));
		//SetCursorPos(_clickx, _clicky);
		//mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
		//Sleep(1000);
		//Sleep(100);
	}
}
void readdata(CLogicalProcess & lp , DWORD LLKID)
{
	int matrix[XLENGTH][YLENGTH];
	//将最外层元素赋值-1(将第一行，第一列，最后一行，最后一列元素赋值-1)
	//for (int i = 0; i < XLENGTH; i++)
	//{
	//	//第一行
	//	matrix[i][0] = 88888;
	//	//
	//}
	//for (int i = 0; i < XLENGTH; i++)
	//{
	//	//最后一行
	//	matrix[i][YLENGTH - 1] = 88888;
	//	//
	//}
	//for (int i = 0; i < YLENGTH; i++)
	//{
	//	//第一列
	//	matrix[0][i] = 88888;
	//	//
	//}
	//for (int i = 0; i < YLENGTH; i++)
	//{
	//	//最后一列
	//	matrix[XLENGTH - 1][i] = 88888;
	//	//
	//}

	LINKGAMEGAMEMODE lggm = userdefined;
	lp.SetGameMode(lggm);



	/*DWORD LLKID = pid;*/
	HANDLE hLLK = OpenProcess(PROCESS_VM_READ, FALSE, LLKID);
	if (hLLK == NULL)
	{
		//error
	}


	//我只是读内存，应该默认都有读的权限了吧，不用这个应该可以VirtualProtectEx


	LPCVOID lpBaseAddress = (LPCVOID)baseaddress;
	int nMem = ((XLENGTH ) * (YLENGTH)) + ((XLENGTH - 2 - 1) * BLANK);
	LPVOID lpBuffer = malloc(nMem);

	SIZE_T nSize = nMem;
	SIZE_T *lpNumberOfBytesRead = NULL;
	ReadProcessMemory(hLLK, lpBaseAddress, lpBuffer, nSize, lpNumberOfBytesRead);

	//开始读取内存并初始化数据
	LPVOID address = lpBuffer;
	for (int i = 0; i < XLENGTH; i++)
	{
		for (int j = 0; j < YLENGTH; j++)
		{

			byte * pData = (byte *)address;
			byte data = *pData;
			*pData = 1;
			matrix[i + 0][j + 0] = (int)data;
			int nextAddress = int(address);
			nextAddress = nextAddress + 1;
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
			/*if (block.nIndex == 88888)
			{
				block.nIndex = -1;
				block.isExist = false;
			}*/
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
DWORD GetProcessIDByName(const char* pName)
{

	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (INVALID_HANDLE_VALUE == hSnapshot) {
		return NULL;
	}
	PROCESSENTRY32 pe = { sizeof(pe) };
	for (BOOL ret = Process32First(hSnapshot, &pe); ret; ret = Process32Next(hSnapshot, &pe)) {
		if (strcmp(pe.szExeFile, pName) == 0) {
			CloseHandle(hSnapshot);
			return pe.th32ProcessID;
		}
		//printf("%-6d %s\n", pe.th32ProcessID, pe.szExeFile);
	}
	CloseHandle(hSnapshot);
	return 0;
}
void main()
{
	HWND mainwnd = FindWindow(NULL, "连连看");
	HWND childwnd = FindWindowEx(mainwnd, NULL, "AfxMDIFrame42", NULL);
	HWND llkwnd = FindWindowEx(childwnd, NULL, "#32770", NULL);
	DWORD id = GetProcessIDByName("llkk.exe");
	CLogicalProcess lp;
	readdata(lp , id);
	//开始消除
	//从（0，0）到（XLENGTH - 1， YLENGTH - 1）
	int x = 0; int y = 0;
	int _x = 1; int _y = 0;
	int num = (XLENGTH) * (YLENGTH);//不确定
	LG_Path path;
	while (true)
	{
		x = -1; y = 0;
		while (x != XLENGTH - 1 || y != YLENGTH - 1)
		{
			if (x == XLENGTH - 1)
			{
				x = 0; y++;
				_y = y; _x = x;
				while (_x != XLENGTH - 1 || _y != YLENGTH - 1)
				{
					if (_x == XLENGTH - 1)
					{
						_x = 0; _y++;
						if (lp.m_UDMode[x][y].nIndex == lp.m_UDMode[_x][_y].nIndex)
						{
							if (lp.GetConnectInfo(path, x, y, _x, _y) != LG_ERR_NOPATHCONNECT)
							{
								//消除，跳出循环
								click(x, y, _x, _y, num , llkwnd);
								num = num - 2;
								if (num == 0)
								{
									/*x = XLENGTH - 2; y = YLENGTH - 2;
									num = (XLENGTH - 2) * (YLENGTH - 2);*/
								}
								system("cls");
								for (int i = 0; i < YLENGTH; i++)
								{
								for (int j = 0; j < XLENGTH; j++)
								{
								printf("%d  ", (lp.m_UDMode)[j][i].nIndex);
								}
								printf("\n");
								}
								//readdata(lp);
								break;
							}
						}

					}
					else if (_y == YLENGTH - 1)
					{
						_x++;
						if (lp.m_UDMode[x][y].nIndex == lp.m_UDMode[_x][_y].nIndex)
						{
							if (lp.GetConnectInfo(path, x, y, _x, _y) != LG_ERR_NOPATHCONNECT)
							{
								//消除，跳出循环
								click(x, y, _x, _y, num , llkwnd);
								num = num - 2;
								if (num == 0)
								{
									/*x = XLENGTH - 2; y = YLENGTH - 2;
									num = (XLENGTH - 2) * (YLENGTH - 2);*/
								}
								//readdata(lp);
								system("cls");
								for (int i = 0; i < YLENGTH; i++)
								{
									for (int j = 0; j < XLENGTH; j++)
									{
										printf("%d  ", (lp.m_UDMode)[j][i].nIndex);
									}
									printf("\n");
								}
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
								click(x, y, _x, _y, num , llkwnd);
								num = num - 2;
								if (num == 0)
								{
									/*x = XLENGTH - 2; y = YLENGTH - 2;
									num = (XLENGTH - 2) * (YLENGTH - 2);*/
								}
								//readdata(lp);
								system("cls");
								for (int i = 0; i < YLENGTH; i++)
								{
									for (int j = 0; j < XLENGTH; j++)
									{
										printf("%d  ", (lp.m_UDMode)[j][i].nIndex);
									}
									printf("\n");
								}
								break;
							}
						}
					}
				}
			}
			else if (y == YLENGTH - 1)
			{
				x++;
				_y = y; _x = x;
				while (_x != XLENGTH - 1 || _y != YLENGTH - 1)
				{
					if (_x == XLENGTH - 1)
					{
						_x = 0; _y++;
						if (lp.m_UDMode[x][y].nIndex == lp.m_UDMode[_x][_y].nIndex)
						{
							if (lp.GetConnectInfo(path, x, y, _x, _y) != LG_ERR_NOPATHCONNECT)
							{
								//消除，跳出循环
								click(x, y, _x, _y, num , llkwnd);
								num = num - 2;
								if (num == 0)
								{
									/*x = XLENGTH - 2; y = YLENGTH - 2;
									num = (XLENGTH - 2) * (YLENGTH - 2);*/
								}
								//readdata(lp);
								system("cls");
								for (int i = 0; i < YLENGTH; i++)
								{
									for (int j = 0; j < XLENGTH; j++)
									{
										printf("%d  ", (lp.m_UDMode)[j][i].nIndex);
									}
									printf("\n");
								}
								break;
							}
						}

					}
					else if (_y == YLENGTH - 1)
					{
						_x++;
						if (lp.m_UDMode[x][y].nIndex == lp.m_UDMode[_x][_y].nIndex)
						{
							if (lp.GetConnectInfo(path, x, y, _x, _y) != LG_ERR_NOPATHCONNECT)
							{
								//消除，跳出循环
								click(x, y, _x, _y, num , llkwnd);
								num = num - 2;
								if (num == 0)
								{
									/*x = XLENGTH - 2; y = YLENGTH - 2;
									num = (XLENGTH - 2) * (YLENGTH - 2);*/
								}
								system("cls");
								for (int i = 0; i < YLENGTH; i++)
								{
									for (int j = 0; j < XLENGTH; j++)
									{
										printf("%d  ", (lp.m_UDMode)[j][i].nIndex);
									}
									printf("\n");
								}
								//readdata(lp);
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
								click(x, y, _x, _y, num , llkwnd);
								num = num - 2;
								if (num == 0)
								{
									/*x = XLENGTH - 2; y = YLENGTH - 2;
									num = (XLENGTH - 2) * (YLENGTH - 2);*/
								}
								system("cls");
								for (int i = 0; i < YLENGTH; i++)
								{
									for (int j = 0; j < XLENGTH; j++)
									{
										printf("%d  ", (lp.m_UDMode)[j][i].nIndex);
									}
									printf("\n");
								}
								//readdata(lp);
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
				while (_x != XLENGTH - 1 || _y != YLENGTH - 1)
				{
					if (_x == XLENGTH - 1)
					{
						_x = 0; _y++;
						if (lp.m_UDMode[x][y].nIndex == lp.m_UDMode[_x][_y].nIndex)
						{
							if (lp.GetConnectInfo(path, x, y, _x, _y) != LG_ERR_NOPATHCONNECT)
							{
								//消除，跳出循环
								click(x, y, _x, _y, num , llkwnd);
								num = num - 2;
								if (num == 0)
								{
									/*x = XLENGTH - 2; y = YLENGTH - 2;
									num = (XLENGTH - 2) * (YLENGTH - 2);*/
								}
								system("cls");
								for (int i = 0; i < YLENGTH; i++)
								{
									for (int j = 0; j < XLENGTH; j++)
									{
										printf("%d  ", (lp.m_UDMode)[j][i].nIndex);
									}
									printf("\n");
								}
								//readdata(lp);
								break;
							}
						}

					}
					else if (_y == YLENGTH - 1)
					{
						_x++;
						if (lp.m_UDMode[x][y].nIndex == lp.m_UDMode[_x][_y].nIndex)
						{
							if (lp.GetConnectInfo(path, x, y, _x, _y) != LG_ERR_NOPATHCONNECT)
							{
								//消除，跳出循环
								click(x, y, _x, _y, num , llkwnd);
								num = num - 2;
								if (num == 0)
								{
									/*x = XLENGTH - 2; y = YLENGTH - 2;
									num = (XLENGTH - 2) * (YLENGTH - 2);*/
								}
								system("cls");
								for (int i = 0; i < YLENGTH; i++)
								{
									for (int j = 0; j < XLENGTH; j++)
									{
										printf("%d  ", (lp.m_UDMode)[j][i].nIndex);
									}
									printf("\n");
								}
								//readdata(lp);
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
								click(x, y, _x, _y, num , llkwnd);
								num = num - 2;
								if (num == 0)
								{
									/*x = XLENGTH - 2; y = YLENGTH - 2;
									num = (XLENGTH - 2) * (YLENGTH - 2);*/

								}
								system("cls");
								for (int i = 0; i < YLENGTH; i++)
								{
									for (int j = 0; j < XLENGTH; j++)
									{
										printf("%d  ", (lp.m_UDMode)[j][i].nIndex);
									}
									printf("\n");
								}
								//readdata(lp);
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