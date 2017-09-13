#include "LGImgCommon.h"

HANDLE LGBitMap::GetWindowsObjHandle(LGObjId objId)
{
	if (m_ImgSet.count(objId))
	{
		m_ImgSet.erase(objId);
	}
	else if (m_MemDCId.count(objId))
	{
		m_MemDCId.erase(objId);
	}
	return 0;
}
