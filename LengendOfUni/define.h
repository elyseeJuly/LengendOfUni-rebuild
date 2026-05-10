#pragma once

#define SAFE_DELETE(x)	if( (x)!=NULL ) { delete (x); (x)=NULL; }
#define EARTH_CIVI_NAME _T("地球")


enum DEPARTMENT_TYPE
{
	DT_ECONOMY = 0,			// 经济部
	DT_ARMY,				// 军事部
	DT_CULTURE,				// 文化部
	DT_HUMANRES,			// 人力资源
	DT_ASTROSOCIOLOGY,		// 宇宙社会学
	DT_NUCLEAR,				// 核技术
	DT_SPACEFIGHT,			// 航天技术
	DT_PROTON,				// 质子技术
	DT_ASTROPHYSICS,		// 天体物理
	DT_CULTURETEC,			// 地球文化研究所
	DT_ECONOMYTEC,			// 经济学研究所
	DT_COUNT,				// 特殊值，用以标识panel type的数量
};

/*enum ALIEN_TYPE
{
	AT_DINOSAUR = 0,		// 吞食帝国
	AT_THREEBODY,			// 三体
	AT_LIBAI,				// 李白文明
	AT_BROKENSTAR,			// 碎星者
	AT_GALAXYSAFER,			// 排险者
	AT_CARBONUNITS,			// 碳基联邦
	AT_GOD,					// 上帝文明
	AT_BROTHER,				// 哥哥文明
	AT_ARTIST,				// 低温艺术家
	AT_ERIDANUS,			// 波江座文明
	AT_GREENSEASON,			// 绿洋星
	AT_COUNT,				// 特殊值，用以标识alien type的数量
};*/

enum STAR_AREA
{
	SA_SOLARSYSTEM = 0,		// 太阳系
	SA_50LIGHTYEAR,			// 50光年
	SA_1WLIGHTYEAR,			// 1万光年
	SA_GALAXY,				// 银河系
	SA_COUNT,				// 特殊值，用以标识star area的数量
};

enum TEC_TREE_TYPE
{
	TT_ASTROSOCIOLOGY = 0,	// 宇宙社会学
	TT_NUCLEAR,				// 核技术
	TT_SPACEFIGHT,			// 航天技术
	TT_PROTON,				// 质子技术
	TT_ASTROPHYSICS,		// 天体物理
	TT_CULTURETEC,			// 地球文化研究所
	TT_ECONOMYTEC,			// 经济学研究所
	TT_COUNT,				// 特殊值，用以标识panel type的数量
};

enum WEAPON_TYPE
{
	WT_UNIT,				// 作战单元
	WE_EXPENDABLE,			// 消耗品
	WE_SPY,					// 间谍
	WE_SUPERBOMB,			// 超级炸弹，可以炸星球的
};

enum BATTLE_TYPE
{
	BT_ATTACK,
	BT_DEFEND,
};

enum FRIENDSHIP_TYPE
{
	FT_VERYANGRY = 0,
	FT_ANGRY,
	FT_NORMAL,
	FT_FRIEND,
	FT_VERYFRIEND,
};

enum EVENT_TYPE
{
	ET_INYEAR,			// 以年份标记
	ET_STRINGINDEX,		// 以名称标记
	ET_RANDOM,			// 随机事件
};

enum EVENT_EFFECT
{
	EE_NONE,
	EE_ADDECONEMY,
	EE_ADDCULTURE,
	EE_ADDPOP,
	EE_ADDTREACHERY,
	EE_WAR,
};