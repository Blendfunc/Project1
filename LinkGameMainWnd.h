#pragma once
#include "D:\\duilib-master\\DuiLib\\UIlib.h"
#include "LogicalProcess.h"
#include <map>
#include <set>
using namespace DuiLib;
class CLLKMainWnd : public WindowImplBase
{
	friend class CLogicalProcess;
public:
	CLLKMainWnd();
	~CLLKMainWnd();
protected:
	virtual void InitWindow();
	virtual void Notify(TNotifyUI& msg);
	virtual void OnClick(TNotifyUI& msg);
	virtual CDuiString GetSkinFolder();
	virtual CDuiString GetSkinFile();
	virtual LPCTSTR GetWindowClassName(void) const;
	HRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
private:
	CVerticalLayoutUI * m_Page;
	CVerticalLayoutUI * m_PageAni;
	CLogicalProcess m_LogicalManager;//main logical manager
	CLinkGameGenerate m_GenerateManager;//main generTE manager
	std::set<CControlUI *> m_setBlock;
	std::vector<BlockInfomationST *> m_vecReleaseBuffer;
	void CreateBlock(CHorizontalLayoutUI * ph);
	//@note-----µ÷ÓÃË³Ðò 1.InitGameData 2.InitGameDisplay
	void InitGameData(LINKGAMEGAMEMODE mode);
	void InitGameDisplay(CVerticalLayoutUI * pVer);

	void GetBlockInformation(BlockInfomationST * pBlock);
	CControlUI * m_pCtrl;
	//modi at 20170910 zy
	void DrawBorder();
	std::set<CControlUI *>m_setCurrentBlock;
	//test code
	int m_nTotalBlock;

};
class CGameBlockUI : public CControlUI
{

};