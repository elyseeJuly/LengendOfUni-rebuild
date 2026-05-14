#include "StdAfx.h"
#include "TecTreeManager.h"

IMPLEMENT_SERIAL(CTecTreeManager, CObject, 1)

CTecTreeManager::CTecTreeManager(void)
{
}

CTecTreeManager::~CTecTreeManager(void)
{
	Clear();
}

void CTecTreeManager::Init()
{
	for (int i = TT_PHYSICS; i < TT_COUNT; i ++)
	{
		CTecTree *tecTree = new CTecTree;
		switch (i)
		{
		case TT_PHYSICS:
			tecTree->AddNode(_T("root"), _T("粒子对撞实验"), FALSE, 80, 20, _T("基础物理研究。"));
			tecTree->AddNode(_T("粒子对撞实验"), _T("质子3维展开"), FALSE, 150, 50, _T("将质子在3个维度内展开的技术。"));
			tecTree->AddNode(_T("质子3维展开"), _T("质子6维展开"), FALSE, 180, 80, _T("将质子在6个维度内展开的技术。"));
			tecTree->AddNode(_T("质子6维展开"), _T("质子9维展开"), FALSE, 230, 150, _T("将质子在9个维度内展开的技术。"));
			tecTree->AddNode(_T("质子9维展开"), _T("质子11维展开"), FALSE, 300, 200, _T("将质子在11个维度内展开的技术。"));
			tecTree->AddNode(_T("质子6维展开"), _T("智子工程"), FALSE, 250, 170, _T("生产智子的技术。智子可以监测出其它文明星球的军事实力并锁死其科技。"));
			tecTree->AddNode(_T("质子3维展开"), _T("强相互作用力材料"), FALSE, 200, 120, _T("生产极度坚固的材料，防御水滴攻击并作为行星发动机的基础。"));
			tecTree->AddNode(_T("强相互作用力材料"), _T("行星发动机基础"), FALSE, 250, 150, _T("流浪地球计划的前置科技。"));
			tecTree->AddNode(_T("粒子对撞实验"), _T("反物质实验"), FALSE, 200, 100, _T("研究反物质的技术。"));
			tecTree->AddNode(_T("反物质实验"), _T("反物质弹"), FALSE, 300, 500, _T("生产反物质弹，可以直接将一颗星球摧毁。"));
			tecTree->AddNode(_T("root"), _T("天文观测"), FALSE, 60, 10, _T("基础天文探测。"));
			tecTree->AddNode(_T("天文观测"), _T("50光年远镜"), FALSE, 100, 50, _T("查看50光年空间范围星图的技术。"));
			tecTree->AddNode(_T("50光年远镜"), _T("1万光年远镜"), FALSE, 200, 100, _T("查看1万光年空间范围星图的技术。"));
			tecTree->AddNode(_T("1万光年远镜"), _T("银河系远镜"), FALSE, 300, 150, _T("查看银河系空间范围星图的技术。"));
			tecTree->AddNode(_T("天文观测"), _T("太阳波放大器50光年"), FALSE, 100, 20, _T("在星图上标识50光年范围内星球属性。"));
			tecTree->AddNode(_T("太阳波放大器50光年"), _T("太阳波放大器1万光年"), FALSE, 200, 50, _T("在星图上标识1万光年范围内星球属性。"));
			tecTree->AddNode(_T("太阳波放大器1万光年"), _T("太阳波放大器银河系"), FALSE, 300, 100, _T("在星图上标识银河系范围内星球属性。"));
			tecTree->AddNode(_T("root"), _T("维度物理"), FALSE, 150, 80, _T("研究空间维度的技术。"));
			tecTree->AddNode(_T("维度物理"), _T("曲率驱动理论"), FALSE, 350, 300, _T("掌握空间曲率驱动的原理。"));
			tecTree->AddNode(_T("曲率驱动理论"), _T("光速飞船原型"), FALSE, 500, 400, _T("维德计划的核心，解锁光速飞船。"));
			tecTree->AddNode(_T("维度物理"), _T("二向箔防御学"), FALSE, 400, 350, _T("研究防御降维打击的方法。"));
			tecTree->AddNode(_T("二向箔防御学"), _T("黑域发生器"), FALSE, 500, 500, _T("制造低光速黑洞，发布宇宙安全声明。"));
			break;
		case TT_AEROSPACE:
			tecTree->AddNode(_T("root"), _T("化学推进"), FALSE, 60, 10, _T("基础航天推进技术。"));
			tecTree->AddNode(_T("化学推进"), _T("10%光速飞船"), FALSE, 100, 30, _T("解锁50光年星域，可生产小型战舰。"));
			tecTree->AddNode(_T("10%光速飞船"), _T("50%光速飞船"), FALSE, 150, 50, _T("解锁1万光年星域，可生产中型战舰。"));
			tecTree->AddNode(_T("50%光速飞船"), _T("99%光速飞船"), FALSE, 230, 100, _T("解锁银河系星域，可生产巨型战舰。"));
			tecTree->AddNode(_T("99%光速飞船"), _T("光速飞船"), FALSE, 500, 400, _T("需曲率驱动前置，可生产光速飞船逃离。"));
			tecTree->AddNode(_T("化学推进"), _T("太空电梯"), FALSE, 150, 80, _T("所有星际建设成本降低30%。"));
			tecTree->AddNode(_T("太空电梯"), _T("轨道空间站"), FALSE, 200, 120, _T("大型武器建造时间减少50%。"));
			tecTree->AddNode(_T("轨道空间站"), _T("太空船坞"), FALSE, 300, 200, _T("解锁星际方舟等超级单位的建造。"));
			tecTree->AddNode(_T("太空电梯"), _T("地月转运系统"), FALSE, 180, 100, _T("月球基地建设效率提升100%。"));
			tecTree->AddNode(_T("root"), _T("核聚变推进"), FALSE, 120, 50, _T("大推力航天技术。"));
			tecTree->AddNode(_T("核聚变推进"), _T("重元素聚变"), FALSE, 200, 120, _T("流浪地球行星发动机的核心动力源。"));
			tecTree->AddNode(_T("重元素聚变"), _T("行星发动机Ⅰ型"), FALSE, 250, 150, _T("需强相互作用力材料，解锁地球推进。"));
			tecTree->AddNode(_T("行星发动机Ⅰ型"), _T("行星发动机Ⅱ型"), FALSE, 300, 200, _T("发动机推力提升50%。"));
			tecTree->AddNode(_T("行星发动机Ⅱ型"), _T("行星发动机Ⅲ型"), FALSE, 400, 300, _T("推力提升100%，可启动流浪地球计划。"));
			tecTree->AddNode(_T("行星发动机Ⅰ型"), _T("转向发动机"), FALSE, 200, 120, _T("解锁地球变轨功能，规避二向箔打击。"));
			tecTree->AddNode(_T("重元素聚变"), _T("星际方舟"), FALSE, 350, 250, _T("建造可搭载大量人口的逃亡飞船。"));
			tecTree->AddNode(_T("核聚变推进"), _T("月球发动机"), FALSE, 180, 100, _T("月球危机事件链的必要科技。"));
			tecTree->AddNode(_T("月球发动机"), _T("月球推离系统"), FALSE, 250, 150, _T("永久解决月球坠落危机。"));
			tecTree->AddNode(_T("root"), _T("行星工程"), FALSE, 80, 30, _T("行星地表改造技术。"));
			tecTree->AddNode(_T("行星工程"), _T("地下城Ⅰ"), FALSE, 100, 50, _T("星球人口容量增加200。"));
			tecTree->AddNode(_T("地下城Ⅰ"), _T("地下城Ⅱ"), FALSE, 150, 80, _T("星球人口容量增加500。"));
			tecTree->AddNode(_T("地下城Ⅱ"), _T("地下城Ⅲ"), FALSE, 200, 120, _T("星球人口容量增加1000。"));
			tecTree->AddNode(_T("地下城Ⅰ"), _T("地下城农业系统"), FALSE, 120, 60, _T("粮食自给，经济每回合增加20。"));
			tecTree->AddNode(_T("行星工程"), _T("地表冷却防护"), FALSE, 150, 80, _T("流浪期间保护地表建筑不被损坏。"));
			tecTree->AddNode(_T("root"), _T("星矿Ⅰ"), FALSE, 100, 20, _T("在行星上建造采矿工厂的技术。"));
			tecTree->AddNode(_T("星矿Ⅰ"), _T("星矿Ⅱ"), FALSE, 120, 50, _T("提高采矿效率。"));
			tecTree->AddNode(_T("星矿Ⅱ"), _T("星矿Ⅲ"), FALSE, 150, 80, _T("进一步提高采矿效率。"));
			tecTree->AddNode(_T("root"), _T("星厂Ⅰ"), FALSE, 100, 20, _T("在行星上建造加工工厂的技术。"));
			tecTree->AddNode(_T("星厂Ⅰ"), _T("星厂Ⅱ"), FALSE, 120, 50, _T("提高加工效率。"));
			tecTree->AddNode(_T("星厂Ⅱ"), _T("星厂Ⅲ"), FALSE, 150, 80, _T("进一步提高加工效率。"));
			tecTree->AddNode(_T("root"), _T("殖民城Ⅰ"), FALSE, 100, 20, _T("在行星上建造城市的技术。"));
			tecTree->AddNode(_T("殖民城Ⅰ"), _T("殖民城Ⅱ"), FALSE, 120, 50, _T("提高城市建造速度。"));
			tecTree->AddNode(_T("殖民城Ⅱ"), _T("殖民城Ⅲ"), FALSE, 150, 80, _T("进一步提高城市建造速度。"));
			break;
		case TT_MILITARY:
			tecTree->AddNode(_T("root"), _T("小行星级氢弹"), FALSE, 100, 30, _T("生产小行星级氢弹的技术。"));
			tecTree->AddNode(_T("小行星级氢弹"), _T("行星级氢弹"), FALSE, 180, 60, _T("生产行星级氢弹的技术。"));
			tecTree->AddNode(_T("行星级氢弹"), _T("恒星级氢弹"), FALSE, 250, 120, _T("生产恒星级氢弹的技术。"));
			tecTree->AddNode(_T("root"), _T("宏原子聚变"), FALSE, 100, 10, _T("使宏观原子发生聚变的技术。"));
			tecTree->AddNode(_T("宏原子聚变"), _T("球状闪电"), FALSE, 300, 150, _T("生产球状闪电的技术。"));
			tecTree->AddNode(_T("球状闪电"), _T("宏化部队"), FALSE, 500, 200, _T("生产宏化部队的技术。"));
			tecTree->AddNode(_T("root"), _T("黑暗森林威慑"), FALSE, 150, 80, _T("建立威慑体系的基础。"));
			tecTree->AddNode(_T("黑暗森林威慑"), _T("天体社会学Ⅰ"), FALSE, 100, 30, _T("理解黑暗森林法则的基础。"));
			tecTree->AddNode(_T("天体社会学Ⅰ"), _T("引力波广播系统"), FALSE, 300, 200, _T("解锁执剑人机制，建立威慑。"));
			tecTree->AddNode(_T("引力波广播系统"), _T("万有引力号"), FALSE, 400, 300, _T("建造具有宇宙广播能力的终极战舰。"));
			tecTree->AddNode(_T("天体社会学Ⅰ"), _T("黑暗森林打击"), FALSE, 350, 250, _T("向宇宙广播某文明坐标的技术。"));
			tecTree->AddNode(_T("root"), _T("降维打击"), FALSE, 400, 300, _T("需维度物理前置，研究降维武器。"));
			tecTree->AddNode(_T("降维打击"), _T("二向箔武器化"), FALSE, 500, 500, _T("生产二向箔，终极毁灭武器。"));
			break;
		case TT_INFORMATION:
			tecTree->AddNode(_T("root"), _T("思想钢印Ⅰ"), FALSE, 100, 30, _T("通过思想控制提高文化增长率。"));
			tecTree->AddNode(_T("思想钢印Ⅰ"), _T("思想钢印Ⅱ"), FALSE, 200, 70, _T("强化思想控制，进一步提高文化增长率。"));
			tecTree->AddNode(_T("思想钢印Ⅱ"), _T("思想钢印Ⅲ"), FALSE, 300, 140, _T("超强化思想控制，极大提高文化增长率。"));
			tecTree->AddNode(_T("root"), _T("数字文明"), FALSE, 120, 60, _T("研究数字生命的前置科技。"));
			tecTree->AddNode(_T("数字文明"), _T("数字生命研究"), FALSE, 180, 100, _T("研究意识数字化的可行性。"));
			tecTree->AddNode(_T("数字生命研究"), _T("意识上传"), FALSE, 250, 150, _T("解锁数字人格保存，角色阵亡后保留。"));
			tecTree->AddNode(_T("意识上传"), _T("数字复活"), FALSE, 200, 120, _T("角色可以通过消耗经济复活。"));
			tecTree->AddNode(_T("意识上传"), _T("550W量子计算机"), FALSE, 350, 250, _T("全局科研速度增加30%。"));
			tecTree->AddNode(_T("550W量子计算机"), _T("全域AI监控网"), FALSE, 200, 150, _T("每回合自动侦察所有已知星球。"));
			tecTree->AddNode(_T("550W量子计算机"), _T("MOSS协议"), FALSE, 300, 200, _T("AI自主防御，被攻击时额外减伤20%。"));
			tecTree->AddNode(_T("数字文明"), _T("数字方舟"), FALSE, 400, 350, _T("触发数字永生胜利条件。"));
			tecTree->AddNode(_T("数字文明"), _T("量子通信"), FALSE, 200, 120, _T("外交谈判不再受距离限制。"));
			tecTree->AddNode(_T("量子通信"), _T("超光速通信"), FALSE, 300, 200, _T("全域即时通讯系统。"));
			tecTree->AddNode(_T("root"), _T("面壁者心理学"), FALSE, 150, 80, _T("解锁面壁者系统，制定秘密战略。"));
			tecTree->AddNode(_T("root"), _T("冬眠技术"), FALSE, 120, 60, _T("保存角色生命，跨越纪元。"));
			break;
		case TT_INTERSTELLAR:
			tecTree->AddNode(_T("root"), _T("宇宙社会学"), FALSE, 150, 80, _T("星际文明交往的前置科技。"));
			tecTree->AddNode(_T("宇宙社会学"), _T("猜疑链理论"), FALSE, 200, 120, _T("深刻理解黑暗森林的运行规律。"));
			tecTree->AddNode(_T("猜疑链理论"), _T("技术爆炸预判"), FALSE, 250, 150, _T("使AI威胁评估准确度提升50%。"));
			tecTree->AddNode(_T("技术爆炸预判"), _T("宇宙文明图谱"), FALSE, 300, 200, _T("自动显示宇宙中所有文明的详细信息。"));
			tecTree->AddNode(_T("宇宙社会学"), _T("安全声明理论"), FALSE, 300, 200, _T("黑域发生器的前置理论。"));
			tecTree->AddNode(_T("安全声明理论"), _T("黑域生成"), FALSE, 500, 500, _T("达成黑域胜利条件。"));
			tecTree->AddNode(_T("宇宙社会学"), _T("宇宙道德学"), FALSE, 250, 150, _T("使得与其他文明的外交获得加成。"));
			tecTree->AddNode(_T("宇宙道德学"), _T("银河共同体"), FALSE, 400, 350, _T("达成外交胜利条件。"));
			tecTree->AddNode(_T("root"), _T("归零者研究"), FALSE, 350, 250, _T("研究宇宙终极命运。"));
			tecTree->AddNode(_T("归零者研究"), _T("宇宙重启理论"), FALSE, 500, 400, _T("触发隐藏结局：重启宇宙。"));
			tecTree->AddNode(_T("root"), _T("流浪地球计划"), FALSE, 200, 150, _T("需行星发动机Ⅲ型前置。"));
			tecTree->AddNode(_T("流浪地球计划"), _T("新家园选址"), FALSE, 300, 200, _T("到达新家园，触发流浪胜利。"));
			break;
		}
		m_vecTecTree.push_back(tecTree);
	}
}

CTecTree* CTecTreeManager::GetTecTree(TEC_TREE_TYPE tecTreeType)
{
	return m_vecTecTree[tecTreeType];
}

void CTecTreeManager::Clear()
{
	for (int i = 0; i < m_vecTecTree.size(); i ++)
	{
		SAFE_DELETE(m_vecTecTree[i])
	}
	m_vecTecTree.clear();
}

BOOL CTecTreeManager::IsTecFinished(TEC_TREE_TYPE ttType, CString sTecName)
{
	if (ttType < 0 || ttType >= TT_COUNT)
		return FALSE;

	CTecTreeNode* pNode = m_vecTecTree[ttType]->GetChildNode(sTecName);
	if (!pNode)
		return FALSE;
	
	return pNode->GetFinish();
}

CTecTreeNode* CTecTreeManager::GetTecTreeNode(TEC_TREE_TYPE ttType, CString sTecName)
{
	if (ttType < 0 || ttType >= TT_COUNT)
		return FALSE;

	CTecTreeNode* pNode = m_vecTecTree[ttType]->GetChildNode(sTecName);
	return pNode;
}

void CTecTreeManager::Serialize(CArchive &ar) 
{ 
	CObject::Serialize(ar); 

	if(ar.IsStoring()) 
	{ 
		// 只存正在研究的科技的信息，及已完成的科技的名称。
		for (int i = 0; i < m_vecTecTree.size(); i ++)
		{
			CTecTreeNode* pNode = m_vecTecTree[i]->GetResearchTecNode();
			if (pNode == NULL)	// 写入当前是否有科技在研究
				ar << 0;
			else
			{
				ar << 1 << pNode->GetName() << pNode->GetCurrentWorkload();
			}

			// 写入已完成科技名称
			std::vector<CString> vecFinishTec;
			m_vecTecTree[i]->GetFinishedTecName(vecFinishTec);
			ar << vecFinishTec.size();
			for (int j = 0; j < vecFinishTec.size(); j ++)
			{
				ar << vecFinishTec[j];
			}
		
		}
	} 
	else 
	{ 
		Init();
		for (int i = 0; i < m_vecTecTree.size(); i ++)
		{
			int iVal = 0;
			CString sVal;
			ar >> iVal;	// 是否有科技在研究中
			if (iVal == 1)
			{
				ar >> sVal >> iVal;
				CTecTreeNode* pNode = GetTecTreeNode((TEC_TREE_TYPE)i, sVal);
				pNode->SetCurrentWorkload(iVal);
				pNode->SetInResearch(TRUE);
				GetTecTree((TEC_TREE_TYPE)i)->SetResearchTecNode(pNode);
			}

			// 读取已完成科技名称
			ar >> iVal;
			for (int j = 0; j < iVal; j ++)
			{
				ar >> sVal;
				CTecTreeNode* pNode = GetTecTreeNode((TEC_TREE_TYPE)i, sVal);
				pNode->SetFinish(TRUE);
			}
		}
	} 
}  

void CTecTreeManager::SetAllTecTreeFinish(BOOL bFinish)
{
	for (int i = 0; i < m_vecTecTree.size(); i ++)
	{
		m_vecTecTree[i]->SetAllTecFinish();
		m_vecTecTree[i]->SetResearchTecNode(NULL);
	}
}