//#include "LogicalProcess.h"
#include "LinkGameMainWnd.h"
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



void main()
{
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
}