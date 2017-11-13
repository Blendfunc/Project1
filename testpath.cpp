
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

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);        //��������������Ϣ�ĺ���
LGBitMap lgBitmap;
LGErrorStates err;
LGErrorStates _err;
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;                //�豸�������
	PAINTSTRUCT ps;         //���ƽṹ
	RECT rect;               //���νṹ
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
	switch (message)        //����õ�����Ϣ
	{
	case WM_CREATE:          //���ڴ������ʱ��������Ϣ
		MessageBox(hwnd, TEXT("�����Ѵ������!"), TEXT("�ҵĴ���"), MB_OK | MB_ICONINFORMATION);
		return 0;

	case WM_PAINT:           //������������Чʱ��������Ϣ
		hdc = BeginPaint(hwnd, &ps);
		GetClientRect(hwnd, &rect);
		DrawText(hdc, TEXT("Hello, �������Լ��Ĵ���!"), -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		
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

					//��ʼ������
					for (int i = 0; i < data.nMatrixHeight; i++)
					{
						for (int j = 0; j < data.nMatrixWidth; j++)
						{
#ifdef USEPALETTE
							//�е�ɫ�廭ͼ
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
							//�޵�ɫ�廭ͼ
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

	case WM_LBUTTONDOWN:     //���������������µ���Ϣ
		MessageBox(hwnd, TEXT("�����������¡�"), TEXT("����"), MB_OK | MB_ICONINFORMATION);
		return 0;

	case WM_DESTROY:         //�����ڹر�ʱ����Ϣ
		MessageBox(hwnd, TEXT("�رճ���!"), TEXT("����"), MB_OK | MB_ICONINFORMATION);
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, message, wParam, lParam);        //DefWindowProc���������Զ������Ϣ������û�д�������Ϣ

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
	gc.GetRectIntersect(r1, r2, r3); //�����Ժ���
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
	WNDCLASS wndclass;        //����һ�����������

							  //����Ϊ���������wndclass������
	wndclass.style = CS_HREDRAW | CS_VREDRAW;                         //������ʽ
	wndclass.lpszClassName = szAppName;                               //��������
	wndclass.lpszMenuName = NULL;                                     //���ڲ˵�:��
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);    //���ڱ�����ɫ
	wndclass.lpfnWndProc = WndProc;                                   //���ڴ�����
	wndclass.cbWndExtra = 0;                                          //����ʵ����չ:��
	wndclass.cbClsExtra = 0;                                          //��������չ:��
	wndclass.hInstance = hInstance;                                   //����ʵ�����
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);               //������С��ͼ��:ʹ��ȱʡͼ��
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);                 //���ڲ��ü�ͷ���

	if (!RegisterClass(&wndclass))
	{    //ע�ᴰ����, ���ע��ʧ�ܵ���������ʾ
		MessageBox(NULL, TEXT("����ע��ʧ��!"), TEXT("����"), MB_OK | MB_ICONERROR);
		return 0;
	}

	hwnd = CreateWindow(                   //��������
		szAppName,                 //��������
		TEXT("�ҵĴ���"),           //���ڱ���
		WS_OVERLAPPEDWINDOW,       //���ڵķ��
		CW_USEDEFAULT,             //���ڳ�ʼ��ʾλ��x:ʹ��ȱʡֵ
		CW_USEDEFAULT,             //���ڳ�ʼ��ʾλ��y:ʹ��ȱʡֵ
		CW_USEDEFAULT,             //���ڵĿ��:ʹ��ȱʡֵ
		CW_USEDEFAULT,             //���ڵĸ߶�:ʹ��ȱʡֵ
		NULL,                      //������:��
		NULL,                      //�Ӳ˵�:��
		hInstance,                 //�ô���Ӧ�ó����ʵ����� 
		NULL                       //
	);

	ShowWindow(hwnd, iCmdShow);        //��ʾ����
	UpdateWindow(hwnd);                //���´���

	while (GetMessage(&msg, NULL, 0, 0))        //����Ϣ�����л�ȡ��Ϣ
	{
		TranslateMessage(&msg);                 //���������Ϣת��Ϊ�ַ���Ϣ
		DispatchMessage(&msg);                  //�ַ����ص�����(���̺���)
	}
	//return msg.wParam;
#endif
}
