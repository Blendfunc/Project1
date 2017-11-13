
#define WINDOWSMODE
//#define USEPALETTE
#define USECONVERTFUNC
//#define USENOPALETTE
//#define USE
#ifdef LLKMODE
#include "LinkGameMainWnd.h"
#include "LGImgCommon.h"
#endif
#ifdef WINDOWSMODE
#include "LGImgCommon.h"
#endif
//void InitGameData(CLogicalProcess & lp)
//{
//	lp.SetData(1, 1, 1, 0);
//	lp.SetData(2, 1, 2, 0);
//	lp.SetData(3, 1, 3, 0);
//	lp.SetData(4, 1, 4, 0);
//	lp.SetData(1, 2, 2, 0);
//	lp.SetData(3, 2, 3, 0);
//	lp.SetData(4, 2, 5, 0);
//	lp.SetData(1, 3, 1, 0);
//	lp.SetData(4, 3, 6, 0);
//	lp.SetData(5, 0, 1, 0);
//}
static CONVOLUTIONKERNEL kernel;
static double f[7][7];

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);        //声明用来处理消息的函数
LGBitMap lgBitmap;
LGErrorStates err;
LGErrorStates _err;
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;                //设备环境句柄
	PAINTSTRUCT ps;         //绘制结构
	RECT rect;               //矩形结构
	LGBitMapId id = 0;
	LGBitMapId outid = 0;
	LGBitMapId outid2 = 0;
	LGBitMapId outid3 = 0;
	BITMAPCOLORDATA data;
	BITMAPCOLORDATA data2;
	kernel.nHeight = 210;
	kernel.nWidth = 210;
	kernel.pData = f;
	kernel.type = DOUBLENUMBER;
	int nV = 50;
	int nR = (2 * nV) + 1;
	//memset(&data, 0, sizeof(BITMAPCOLORDATA));
	double x = 0.0;
	double y = 0.0;
	double sum = 0.0;
	double stepSize = 1.0;
	double X = 0.0;
	double Y = 0.0;
	switch (message)        //处理得到的消息
	{
	case WM_CREATE:          //窗口创建完成时发来的消息
		MessageBox(hwnd, TEXT("窗口已创建完成!"), TEXT("我的窗口"), MB_OK | MB_ICONINFORMATION);
		return 0;

	case WM_PAINT:           //处理窗口区域无效时发来的消息
		hdc = BeginPaint(hwnd, &ps);
		GetClientRect(hwnd, &rect);
		DrawText(hdc, TEXT("Hello, 这是我自己的窗口!"), -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		
		err = lgBitmap.LGLoadBitMap(TEXT("C:\\Users\\Public\\Pictures\\Sample Pictures\\scenery1.bmp") , id);
		
		//lgBitmap.LGGetColorData(id, data);
		/*lgBitmap.Convert(id, outid);
		lgBitmap.LGGetColorData(outid, data);*/
//		lgBitmap.ProcessBoundary2(3, 3, &data, outid2, outid);
//		lgBitmap.LGGetColorData(out%id2, data);
		
		
//		lgBitmap.LGGaussianBlur2(id, outid, 13);
		
		lgBitmap.LGGray255Img(id, outid);
//		lgBitmap.LGGetColorData(outid, data);
//		lgBitmap.LGSketch1(id, outid);
		lgBitmap.LGGetColorData(outid, data);
		
		if (err != LG_ERR_ILLEGALiMG)
		{
			//_err = lgBitmap.LGGetColorData(id, data);
			
////			lgBitmap.Convert(id, outid);
//			CONVOLUTIONKERNEL kernel;
//			float ff [3][3];
//			kernel.nHeight = 3;
//			kernel.nWidth = 3;
//			kernel.type = DOUBLENUMBER;
//			kernel.pData = ff;
////			lgBitmap.GenerateGaussianFilter(&kernel , 1.5);
//			_err = lgBitmap.LGGetColorData(outid, data);
			if (_err != LG_ERR_OTHER)
			{
				if (data.img == IMGBMP24 && outid != -1)
				{

					//开始画像素
					for (int i = 0; i < data.nMatrixHeight; i++)
					{
						for (int j = 0; j < data.nMatrixWidth; j++)
						{
#ifdef USEPALETTE
							//有调色板画图
							int address = (int)data.pMatrixColorData;
							address = address + (j*data.nMatrixHeight) + i;
							byte* pb = (byte*)address;
							int nIndex = (int)*pb;
							int _address = (int)data.pMatrixColorTable;
							_address = _address + (nIndex * sizeof(RGBQUAD));
							RGBQUAD bgr = *((RGBQUAD*)_address);
							SetPixelV(hdc, j, i, RGB(bgr.rgbRed, bgr.rgbGreen, bgr.rgbBlue));
#else
#ifdef USENOPALETTE
							//无调色板画图
							int address = (int)data.pMatrixColorData;
							address = address + (j*data.nMatrixHeight) + i;
							byte* pb = (byte*)address;
							COLORREF color = (COLORREF)(*pb) + 65536;
							SetPixel(hdc, j, i, color);
							//...
#endif
#ifdef USECONVERTFUNC
							int address = (int)data.pMatrixColorData;
							address = address + (((j*data.nMatrixHeight) + i)*sizeof(PixelData));
							PixelData * ppd = (PixelData *)address;

							while (true)
							{
								lgBitmap.LGHypotrochoid(33.0, 100.0, 33.0, x, y, stepSize, sum , X , Y);
								stepSize= stepSize+1;
								int nx = x;
								int ny = y;
								int nX = X;
								int nY = Y;
								SetPixelV(hdc, nx + 500, 500 - ny, RGB(0, 0, 0));
								nX = nX + 500;
								nY = nY - 500;
								SetPixelV(hdc, nX, -nY, RGB(120, 120, 120));
								if (stepSize > sum)                                                                                                                                                                                
								{
									break;
								}
								//Sleep(1);
							}
							//SetPixelV(hdc, j, i, RGB(ppd->r, ppd->g, ppd->b));
#endif
#endif				
							//
						}
					}
					//
				}
				else if (data.img == IMGBMP24)
				{
					//...
				}

			}
		}
		EndPaint(hwnd, &ps);
		return 0;

	case WM_LBUTTONDOWN:     //处理鼠标左键被按下的消息
		MessageBox(hwnd, TEXT("鼠标左键被按下。"), TEXT("单击"), MB_OK | MB_ICONINFORMATION);
		return 0;

	case WM_DESTROY:         //处理窗口关闭时的消息
		MessageBox(hwnd, TEXT("关闭程序!"), TEXT("结束"), MB_OK | MB_ICONINFORMATION);
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);        //DefWindowProc处理我们自定义的消息处理函数没有处理到的消息

}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
#ifdef LLKMODE
	LGGeometryControl gc;
	LGRECT r1, r2 , r3;
	r1.nLefTopPt.x = 2.99;
	r1.nLefTopPt.y = 7.14;
	r1.nLeftDownPt.x = 2.99;
	r1.nLeftDownPt.y = 2.01;
	r1.nRightTopPt.x = 11.99;
	r1.nRightTopPt.y = 7.14;
	r1.nRightDownPt.x = 11.99;
	r1.nRightDownPt.y = 2.01;



	r2.nLefTopPt.x = 6.75;
	r2.nLefTopPt.y = 11.22;
	r2.nLeftDownPt.x = 6.75;
	r2.nLeftDownPt.y = 3.97;
	r2.nRightTopPt.x = 20.74;
	r2.nRightTopPt.y = 11.22;
	r2.nRightDownPt.x = 20.74;
	r2.nRightDownPt.y = 3.97;


	LGBitMap lbm;
	lbm.Test();
	

	/*LARGE_INTEGER BegainTime;

	LARGE_INTEGER EndTime;

	LARGE_INTEGER Frequency;

	QueryPerformanceFrequency(&Frequency);

	QueryPerformanceCounter(&BegainTime);*/
	gc.GetRectIntersect(r1, r2, r3); //待测试函数
	/*QueryPerformanceCounter(&EndTime);

	double runtime = (double)(EndTime.QuadPart - BegainTime.QuadPart)

		/ Frequency.QuadPart;*/
	CLLKMainWnd wnd;
	wnd.CreateDuiWindow(NULL, _T(""), 0, 0);
	wnd.CenterWindow();
	wnd.ShowModal();
	/*CLogicalProcess lp;
	lp.SetGameMode(LINKGAMEGAMEMODE::easy);
	InitGameData(lp);
	LG_Path path;
	lp.GetConnectInfo(path , 5 , 0 , 1 , 1);
	LG_Path path1;
	lp.GetConnectInfo(path1, 2, 1, 1, 2);
	LG_Path path2;
	lp.GetConnectInfo(path2, 3, 1, 3, 2);
	LG_Path path3;
	lp.GetConnectInfo(path3, 3, 1, 3, 2);*/
#endif
#ifdef WINDOWSMODE
	static TCHAR szAppName[] = TEXT("MyWindow");
	HWND hwnd;
	MSG msg;
	WNDCLASS wndclass;        //声明一个窗口类对象

							  //以下为窗口类对象wndclass的属性
	wndclass.style = CS_HREDRAW | CS_VREDRAW;                         //窗口样式
	wndclass.lpszClassName = szAppName;                               //窗口类名
	wndclass.lpszMenuName = NULL;                                     //窗口菜单:无
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);    //窗口背景颜色
	wndclass.lpfnWndProc = WndProc;                                   //窗口处理函数
	wndclass.cbWndExtra = 0;                                          //窗口实例扩展:无
	wndclass.cbClsExtra = 0;                                          //窗口类扩展:无
	wndclass.hInstance = hInstance;                                   //窗口实例句柄
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);               //窗口最小化图标:使用缺省图标
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);                 //窗口采用箭头光标

	if (!RegisterClass(&wndclass))
	{    //注册窗口类, 如果注册失败弹出错误提示
		MessageBox(NULL, TEXT("窗口注册失败!"), TEXT("错误"), MB_OK | MB_ICONERROR);
		return 0;
	}

	hwnd = CreateWindow(                   //创建窗口
		szAppName,                 //窗口类名
		TEXT("我的窗口"),           //窗口标题
		WS_OVERLAPPEDWINDOW,       //窗口的风格
		CW_USEDEFAULT,             //窗口初始显示位置x:使用缺省值
		CW_USEDEFAULT,             //窗口初始显示位置y:使用缺省值
		CW_USEDEFAULT,             //窗口的宽度:使用缺省值
		CW_USEDEFAULT,             //窗口的高度:使用缺省值
		NULL,                      //父窗口:无
		NULL,                      //子菜单:无
		hInstance,                 //该窗口应用程序的实例句柄 
		NULL                       //
	);

	ShowWindow(hwnd, iCmdShow);        //显示窗口
	UpdateWindow(hwnd);                //更新窗口

	while (GetMessage(&msg, NULL, 0, 0))        //从消息队列中获取消息
	{
		TranslateMessage(&msg);                 //将虚拟键消息转换为字符消息
		DispatchMessage(&msg);                  //分发到回调函数(过程函数)
	}
	//return msg.wParam;
#endif
}
