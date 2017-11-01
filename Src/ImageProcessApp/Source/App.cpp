#include "..\Header\ImageProcessApp.h"
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	CImageProcessApp app;
	app.CreateDuiWindow(NULL, _T(""), 0, 0);
	app.CenterWindow();
	app.ShowModal();

	return 0;
}