#pragma once

#define TINYNUM 0.000001
typedef struct tagLGRANGE
{
	float left;
	float right;
}LGRANGE;
enum LGErrorStates
{
	LG_ERR_ILLEGALiMG = 0,/*!< 非法图像 */
	LG_ERR_MEMORYERR,/*!< 内存错误 */
	LG_ERR_NOPATHCONNECT,/*!< 给出的坐标没有路径可以连接 */
	LG_ERR_ONEPATHCONNECT,/*!< 一条路径连接 */
	LG_ERR_TWOPATHCONNECT,/*!< 两条路径连接 */
	LG_ERR_THREEPATHCONNECT,/*!< 三条路径连接 */
	LG_ERR_ILLEGALPOINT,/*!< 坐标非法 */
	LG_ERR_SQUARESNUMILLEGALiMG,/*!< 棋盘方块个数不正确 */
	LG_ERR_NULLPOINTER,/*!< 空指针 */
	LG_ERR_VALUETOOSMALL,/*!< 值过小 */
	LG_ERR_NOINTERSECT,/*!< 没有交点 */
	LG_ERR_HAVEINTERSECT,/*!< 有交点 */
	LG_ERR_ERRID,/*!< 错误id */
	LG_ERR_PARAM,/*!< 参数错误 */
	LG_ERR_OTHER/*!< 其他 */
};

/**
* @brief LG_Point，描述一条路径
*/
typedef struct tagLG_Point
{
	int x;
	int y;
	tagLG_Point * pNext;/*!< 头节点为0，尾节点为1 */
	tagLG_Point()
	{
		x = 0;
		y = 0;
		pNext = 0;
	}
}LG_Point;

typedef struct tagLG_Path
{
	LG_Point pointBegin;
	LG_Point pointEnd;
	int nPoint;
}LG_Path;