#pragma once
#include "atlconv.h"
#include "..\..\..\DuiLib\UIlib.h"
using namespace DuiLib;
class CImageProcessApp : public WindowImplBase
{
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

};