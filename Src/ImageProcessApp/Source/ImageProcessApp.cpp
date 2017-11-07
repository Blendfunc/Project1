#include "..\Header\ImageProcessApp.h"
#ifdef DEBUG
#pragma comment(lib, "DuiLib_d.lib")
#else
#pragma comment(lib, "DuiLib_d.lib")
#endif
#pragma comment(lib,"Project1.lib")
CImageProcessApp::CImageProcessApp()
{
	m_RenderWnd = NULL;
	m_Ctrl = NULL;
	m_Tool = NULL;
	m_BtSketch1 = NULL;
	m_RenderWnd = new CRenderWnd;
}
CImageProcessApp::~CImageProcessApp()
{
}
void CImageProcessApp::InitWindow()
{
	m_Ctrl = (CHorizontalLayoutUI *)m_PaintManager.FindControl(_T("c1"));
	m_Tool = (CVerticalLayoutUI *)m_PaintManager.FindControl(_T("tool"));
	m_BtSketch1 = (CButtonUI *)m_PaintManager.FindControl(_T("Sketch1"));
	m_RenderWnd->SetAttribute(_T("padding"), _T("1,1,1,1"));
	m_Ctrl->Add((CControlUI *)m_RenderWnd);
	m_RenderWnd->SetBkImage(_T("C:\\Users\\Public\\Pictures\\Sample Pictures\\img18.bmp"));
	__super::InitWindow();
}

void CImageProcessApp::Notify(TNotifyUI & msg)
{
	__super::Notify(msg);
}

void CImageProcessApp::OnClick(TNotifyUI & msg)
{
	if (msg.pSender == m_BtSketch1)
	{
		LGBitMapId id;
		LGBitMapId outid;
		BITMAPCOLORDATA data;
		m_LgBitmap.LGLoadBitMap(_T("C:\\Users\\Public\\Pictures\\Sample Pictures\\img18.bmp") , id);
		m_LgBitmap.LGSketch1(id, outid);
		m_LgBitmap.LGGetColorData(outid, data);
		m_RenderWnd->RefreshRenderWnd(&data);
	}
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
