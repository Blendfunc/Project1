#pragma once
/*****************************************************************************
*                                                  *
*  Copyright (C) 2017 zhaozhiyuan  1457321461@qq.com                         *
*                                                                            *
*  This file is part of OST.                                                 *
*                                                                            *
*  This program is free software; you can redistribute it and/or modify      *
*  it under the terms of the GNU General Public License version 3 as         *
*  published by the Free Software Foundation.                                *
*                                                                            *
*  You should have received a copy of the GNU General Public License         *
*  along with OST. If not, see <http://www.gnu.org/licenses/>.               *
*                                                                            *
*  Unless required by applicable law or agreed to in writing, software       *
*  distributed under the License is distributed on an "AS IS" BASIS,         *
*  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  *
*  See the License for the specific language governing permissions and       *
*  limitations under the License.                                            *
*                                                                            *
*  @file     LinkGamgeMainWnd.h                                                       *
*  @brief    对文件的简述                                                      *
*  Details.                                                                  *
*                                                                            *
*  @author   zhaozhiyuan                                                       *
*  @email    1457321461@qq.com                                              *
*  @version  1.0.0.1(版本号)                                                  *
*  @date     2017.08.31                                              *
*  @license  GNU General Public License (GPL)                                *
*                                                                            *
*----------------------------------------------------------------------------*
*  Remark         : Description                                              *
*----------------------------------------------------------------------------*
*  Change History :                                                          *
*  <Date>     | <Version> | <Author>       | <Description>                   *
*----------------------------------------------------------------------------*
*  2017/08/30 | 1.0.0.1   | zhaozhiyuan      | Create file                     *
*----------------------------------------------------------------------------*
*                                                                            *
*****************************************************************************/
#include "LinkGameCommonBusinessData.h"
/**
* @brief CLinkGameMainWnd
* 程序主窗口
*/
class CLinkGameMainWnd : public INotify   /*final*/
{
public:
	CLinkGameMainWnd();
	~CLinkGameMainWnd();

	void										SetWindowInformation(LGMainWndInfoSt * pInfo);
	void										CreatLinkGameWnd();
	void										SetNotifyCallBack();
protected:

private:
	INotify *									m_pNotify;
	int											m_WndWidth;
	int											m_WndHeight;
	LGRGPosion									m_CilentArea;
	LGRGPosion									m_TitleArea;





};