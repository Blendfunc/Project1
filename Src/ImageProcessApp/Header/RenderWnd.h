#pragma once
#include "windows.h"
#include "..\..\..\DuiLib\UIlib.h"
using namespace DuiLib;
enum dcStatus
{
	dc_needupdate = 0 ,
	dc_nodo
};
typedef struct RENDERDC
{
	HDC dc;
	dcStatus status;
	int height;
	int width;
};
class CRenderWnd : public CControlUI
{
public:
	CRenderWnd();
	~CRenderWnd();
	void RefreshRenderWnd(void* pData);//pData��24λ��ɫ����
protected:
	virtual bool DoPaint(HDC hDC, const RECT& rcPaint, CControlUI* pStopControl);

protected:
	void CopyMemoryDC(HDC desDC, HDC srcDC);
protected:
	RENDERDC m_DC;
};