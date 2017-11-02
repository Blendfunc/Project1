#pragma once
#include "atlconv.h"
#include "..\..\..\DuiLib\UIlib.h"
#include ".\RenderWnd.h"
#include "..\..\..\LGImgCommon.h"
using namespace DuiLib;
class CImageProcessApp : public WindowImplBase
{
public:
	CImageProcessApp();
	~CImageProcessApp();
protected:
	virtual void InitWindow();
	virtual void Notify(TNotifyUI& msg);
	virtual void OnClick(TNotifyUI& msg);
	virtual CDuiString GetSkinFolder();
	virtual CDuiString GetSkinFile();
	virtual LPCTSTR GetWindowClassName(void) const;
	HRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);


protected:
	std::string GetEXEPath();

protected:
	CRenderWnd  * m_RenderWnd;
	CHorizontalLayoutUI * m_Ctrl;//�������m_RenderWnd��
	CVerticalLayoutUI * m_Tool;//Ч��չʾ������
	//
	CButtonUI * m_BtSketch1;
	LGBitMap m_LgBitmap;
};

//class 