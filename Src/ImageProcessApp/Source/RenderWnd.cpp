#include "..\Header\RenderWnd.h"
#include "..\..\..\LGImgCommon.h"
CRenderWnd::CRenderWnd()
{
	m_DC.status = dc_nodo;
	HWND hDesktop = ::GetDesktopWindow();
	HDC desktopDC = ::GetDC(hDesktop);
	m_DC.dc = ::CreateCompatibleDC(desktopDC);
	HBITMAP bitmap = ::CreateCompatibleBitmap(desktopDC, 0, 0);
	::SelectObject(m_DC.dc, bitmap);
	::DeleteObject(desktopDC);
//	BITMAP bitmap = ::CreateCompatibleBitmap(desktopDC, data->nMatrixWidth, data->nMatrixHeight);
//	::SelectObject(m_DC.dc, bitmap);
}

CRenderWnd::~CRenderWnd()
{
	::DeleteObject(m_DC.dc);
}

void CRenderWnd::RefreshRenderWnd(void * pData)
{
	BITMAPCOLORDATA * data = (BITMAPCOLORDATA *)pData;
	//1.将pData的数据存放到内存DC中
	HWND hDesktop = ::GetDesktopWindow();
	HDC desktopDC = ::GetDC(hDesktop);
	HBITMAP bitmap = ::CreateCompatibleBitmap(desktopDC, data->nMatrixWidth, data->nMatrixHeight);
	HDC dcMem = ::CreateCompatibleDC(desktopDC);

	::SelectObject(dcMem, bitmap);
	/*::SetDIBits()*/
	for (int i = 0; i < data->nMatrixHeight; i++)
	{
		for (int j = 0; j < data->nMatrixWidth; j++)
		{
			int address = (int)data->pMatrixColorData;
			address = address + (((j*data->nMatrixHeight) + i) * sizeof(PixelData));
			PixelData * ppd = (PixelData *)address;
			SetPixelV(dcMem, j, i, RGB(ppd->r, ppd->g, ppd->b));
		}
	}
	

	this->SetBkImage(_T(""));
	//
	//2.拷贝DC到m_DC
	m_DC.status = dc_needupdate;
	m_DC.height = this->GetHeight();
	m_DC.width = this->GetWidth();
	HBITMAP _bitmap = ::CreateCompatibleBitmap(desktopDC, m_DC.width, m_DC.height);
	HBITMAP _old = (HBITMAP)::SelectObject(m_DC.dc, _bitmap);
	::StretchBlt(m_DC.dc, 0, 0, m_DC.width, m_DC.height, dcMem, 0, 0, data->nMatrixWidth, data->nMatrixHeight, SRCCOPY);

	//...

	//
	//3.通过Invalidate调用DoPaint
	Invalidate();
	//

	::DeleteObject(dcMem);
	::DeleteObject(bitmap);
	::DeleteObject(desktopDC);
	::DeleteObject(_old);
}

bool CRenderWnd::DoPaint(HDC hDC, const RECT & rcPaint, CControlUI * pStopControl)
{
	bool b = true;
	b = __super::DoPaint(hDC, rcPaint, pStopControl);
	//判断当前m_DC，拷贝内存DC到屏幕DC上
	if (m_DC.status == dc_needupdate)
	{
		/*BITMAP bitmap;
		memset(&bitmap, 0, sizeof(BITMAP));
		::GetObject(m_DC.dc, sizeof(BITMAP), &bitmap);
		int nWidth = this->GetWidth();
		int nHeight = this->GetHeight();*/
		//::StretchBlt(hDC, rcPaint.left, rcPaint.top, rcPaint.right- rcPaint.left, rcPaint.bottom - rcPaint.top, m_DC.dc, 0 , 0 , 800, 600 , SRCCOPY);
		::BitBlt(hDC, /*rcPaint.left*/1, /*rcPaint.top*/40, /*rcPaint.right - rcPaint.left*/m_DC.width, /*rcPaint.bottom - rcPaint.top*/m_DC.height, m_DC.dc, /*rcPaint.left*/0, /*rcPaint.top*/0, SRCCOPY);
	}
	return b;
}

void CRenderWnd::CopyMemoryDC(HDC desDC, HDC srcDC)
{

}
