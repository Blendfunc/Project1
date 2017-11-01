#include "..\Header\ImageProcessApp.h"
#ifdef DEBUG
#pragma comment(lib, "DuiLib_d.lib")
#else
#pragma comment(lib, "DuiLib_d.lib")
#endif
void CImageProcessApp::InitWindow()
{
	__super::InitWindow();
}

void CImageProcessApp::Notify(TNotifyUI & msg)
{
	__super::Notify(msg);
}

void CImageProcessApp::OnClick(TNotifyUI & msg)
{
	__super::OnClick(msg);
}

CDuiString CImageProcessApp::GetSkinFolder()
{
	std::string path = GetEXEPath();
	USES_CONVERSION;
	CDuiString skin(A2T((LPSTR)path.c_str()));
	return skin;
}

CDuiString CImageProcessApp::GetSkinFile()
{
	return _T("ImageProcessApp.xml");
	
}

LPCTSTR CImageProcessApp::GetWindowClassName(void) const
{
	return _T("ImageProcessApp");
}

HRESULT CImageProcessApp::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return __super::HandleMessage(uMsg , wParam , lParam);
}

std::string CImageProcessApp::GetEXEPath()
{
	char pBuf[MAX_PATH];
	/*::GetCurrentDirectory(MAX_PATH, pBuf);*/
	::GetModuleFileName(NULL, pBuf, MAX_PATH);
	std::string path;
	path = pBuf;
	std::string _path = path.substr(0, path.rfind("\\"));
	return _path;
}
