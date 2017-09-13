#include "LinkGameMainWnd.h"
#include "atlconv.h"

CLLKMainWnd::CLLKMainWnd()
{
	m_Page = NULL;
	m_pCtrl = NULL;
}

CLLKMainWnd::~CLLKMainWnd()
{
}

void CLLKMainWnd::InitWindow()
{
	m_Page = (CVerticalLayoutUI *)m_PaintManager.FindControl(_T("hpage"));
	InitGameData(LINKGAMEGAMEMODE::easy);
	InitGameDisplay(m_Page);
}

void CLLKMainWnd::Notify(TNotifyUI & msg)
{
	__super::Notify(msg);
}

void CLLKMainWnd::OnClick(TNotifyUI & msg)
{
	__super::OnClick(msg);
}

CDuiString CLLKMainWnd::GetSkinFolder()
{
	/*return _T("C:\\Users\\Blendfunc\\Desktop\\Project1");*/
	char chpath[MAX_PATH];
	GetModuleFileName(NULL, chpath, sizeof(chpath));
	USES_CONVERSION;
	(_tcsrchr(chpath, _T('\\')))[1] = 0;//删除文件名，只获得路径 字串
	return (A2T(chpath));
}

CDuiString CLLKMainWnd::GetSkinFile()
{
	return _T("LLKMainWnd.xml");
}

LPCTSTR CLLKMainWnd::GetWindowClassName(void) const
{
	return _T("llkgamewnd");
}

HRESULT CLLKMainWnd::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (uMsg == WM_LBUTTONUP)
	{
		POINT point;
		point.x = (short)LOWORD(lParam);
		point.y = (short)HIWORD(lParam);
		CControlUI * pCtrl = m_PaintManager.FindControl(point);
		if (pCtrl)
		{
			CDuiString strUserData = pCtrl->GetUserData();
			//////////////////////
			BlockInfomationST * pBlock0 = (BlockInfomationST *)pCtrl->GetTag();
			//////////////////////
			if (strUserData == _T("linkgameblock"))
			{
				m_setCurrentBlock.insert(pCtrl);
				if (m_setCurrentBlock.size() == 2)
				{
					BlockInfomationST * pBlock0 = NULL;
					BlockInfomationST * pBlock1 = NULL;
					CControlUI * pCtrl0 = NULL;
					CControlUI * pCtrl1 = NULL;
					for (auto iter = m_setCurrentBlock.begin(); iter != m_setCurrentBlock.end(); iter++)
					{
						if (iter != --m_setCurrentBlock.end())
						{
							pCtrl0 = *iter;
							pBlock0 = (BlockInfomationST *)(*iter)->GetTag();
						}
						else
						{
							pCtrl1 = *iter;
							pBlock1 = (BlockInfomationST *)(*iter)->GetTag();
						}
					}
					
					if (pBlock0 && pBlock1 && pBlock0->nIndex == pBlock1->nIndex && pCtrl0 && pCtrl1)
					{
						LG_Path path;
						LGErrorStates error = m_LogicalManager.GetConnectInfo(path, pBlock0->tlcX, pBlock0->tlcY, pBlock1->tlcX, pBlock1->tlcY);
						if (error != LG_ERR_NOPATHCONNECT)
						{
							/*pCtrl0->SetVisible(false);
							pCtrl1->SetVisible(false);*/
							pCtrl0->SetBkImage(_T(""));
							pCtrl1->SetBkImage(_T(""));
							pBlock0->isExist = false;
							pBlock1->isExist = false;
						}
						else if (error == LG_ERR_NOPATHCONNECT)
						{
							m_setCurrentBlock.clear();
						}
					}
					m_setCurrentBlock.clear();
				}
			}
		}
	}
return __super::HandleMessage(uMsg, wParam, lParam);
}

void CLLKMainWnd::CreateBlock(CHorizontalLayoutUI * ph)
{
	if(ph)
	{
		for (int i = 0; i < 10; i++)
		{
			CControlUI * pCtrl = new CControlUI;
			pCtrl->SetBkImage(_T("C:\\Users\\Blendfunc\\Desktop\\block.bmp"));
			pCtrl->SetAttribute("bordersize", "1");
			pCtrl->SetAttribute("bordercolor", "#ff000000");
			ph->Add(pCtrl);
		}
	}
}

void CLLKMainWnd::InitGameData(LINKGAMEGAMEMODE mode)
{
	if (mode == LINKGAMEGAMEMODE::easy)
	{
		int matrix [EASYXLENGTH][EASYYLENGTH];
		m_GenerateManager.Methods2(EASYXLENGTH, EASYYLENGTH, matrix , 10);
		m_LogicalManager.SetGameMode(LINKGAMEGAMEMODE::easy);
		for (int i = 0; i < EASYYLENGTH; i++)
		{
			for (int j = 0; j < EASYXLENGTH; j++)
			{
				BlockInfomationST block;
				memset(&block , 0 , sizeof(BlockInfomationST));
				//这里先设置每个方块的id，对应的图片，坐标
				block.nIndex = matrix[j][i];
				block.tlcX = j;
				block.tlcY = i;
				GetBlockInformation(&block);
				block.isExist = true;
				m_LogicalManager.SetDataEx(j, i, &block);
			}
		}
	}
	else if (mode == LINKGAMEGAMEMODE::hard)
	{
		;
	}
}

void CLLKMainWnd::InitGameDisplay(CVerticalLayoutUI * pVer)
{
	ASSERT(pVer);
	if (pVer)
	{
		LINKGAMEGAMEMODE mode = m_LogicalManager.GetGameMode();
		int i = 0;
		int j = 0;
		if (mode == LINKGAMEGAMEMODE::easy)
		{
			i = EASYXLENGTH;
			j = EASYYLENGTH;
		}
		else if (mode == LINKGAMEGAMEMODE::hard)
		{
			i = HARDXLENGTH;
			j = HARDYLENGTH;
		}
		for (int k = 0; k < j; k++)
		{
			CHorizontalLayoutUI * pHor = new CHorizontalLayoutUI;
			for (int l = 0; l < i; l++)
			{
				CControlUI * pCtrl = new CControlUI;
				m_setBlock.insert(pCtrl);
				BlockInfomationST * p = NULL;
				BlockInfomationST ** _p = &p;
				m_LogicalManager.GetBlockInformation(_p , l , k);
				if (p)
				{
					m_vecReleaseBuffer.push_back(p);
					USES_CONVERSION;
					pCtrl->SetBkImage(A2T((LPSTR)(p->lpszNormalImg)));
					pCtrl->SetTag(UINT_PTR(p));
					pCtrl->SetUserData(_T("linkgameblock"));
					pCtrl->SetFixedHeight(90);
					pCtrl->SetFixedWidth(90);
					pHor->SetFixedHeight(90);
					pHor->SetFixedWidth(90 * i);
					pHor->Add(pCtrl);
				}
				ASSERT(p);
			}
			pVer->Add(pHor);
		}
	}
}

void CLLKMainWnd::GetBlockInformation(BlockInfomationST * pBlock)
{
	if (pBlock)
	{
		int nIndex = pBlock->nIndex;
		std::string strfile;
		char chpath[MAX_PATH];
		GetModuleFileName(NULL, chpath, sizeof(chpath));
		USES_CONVERSION;
		(_tcsrchr(chpath, _T('\\')))[1] = 0;//删除文件名，只获得路径 字串
		strfile = chpath;
		std::string strName;
		std::string strFileName;
		switch (nIndex)
		{
		case 0: 
			strName = "block0.bmp";
			strFileName = strfile + strName;
			pBlock->lpszNormalImg = A2T((LPSTR)strFileName.data());
			break;
		case 1:
			strName = "block1.bmp";
			strFileName = strfile + strName;
			pBlock->lpszNormalImg = A2T((LPSTR)strFileName.data());
			break;
		case 2: 
			strName = "block2.bmp";
			strFileName = strfile + strName;
			pBlock->lpszNormalImg = A2T((LPSTR)strFileName.data());
			break;
		case 3: 
			strName = "block3.bmp";
			strFileName = strfile + strName;
			pBlock->lpszNormalImg = A2T((LPSTR)strFileName.data());
			break;
		case 4: 
			strName = "block4.bmp";
			strFileName = strfile + strName;
			pBlock->lpszNormalImg = A2T((LPSTR)strFileName.data()); 
			break;
		case 5: 
			strName = "block5.bmp";
			strFileName = strfile + strName;
			pBlock->lpszNormalImg = A2T((LPSTR)strFileName.data());
			break;
		case 6: 
			strName = "block6.bmp";
			strFileName = strfile + strName;
			pBlock->lpszNormalImg = A2T((LPSTR)strFileName.data());
			break;
		case 7: 
			strName = "block7.bmp";
			strFileName = strfile + strName;
			pBlock->lpszNormalImg = A2T((LPSTR)strFileName.data());
			break;
		case 8: 
			strName = "block8.bmp";
			strFileName = strfile + strName;
			pBlock->lpszNormalImg = A2T((LPSTR)strFileName.data());
			break;
		case 9: 
			strName = "block9.bmp";
			strFileName = strfile + strName;
			pBlock->lpszNormalImg = A2T((LPSTR)strFileName.data());
			break;
		case 10: 
			strName = "block10.bmp";
			strFileName = strfile + strName;
			pBlock->lpszNormalImg = A2T((LPSTR)strFileName.data());
			break;
		}
	}
}

void CLLKMainWnd::DrawBorder()
{
	/*m_pCtrl->SetAttribute(_T("bordersize"), _T("1"));
	m_pCtrl->SetAttribute(_T("bordercolor"), _T("#ff6c717a"));
}
pCtrl->SetAttribute(_T("bordercolor"), _T("#ff2d87ff"));
pCtrl->SetAttribute(_T("bordersize"), _T("1"));*/
}
