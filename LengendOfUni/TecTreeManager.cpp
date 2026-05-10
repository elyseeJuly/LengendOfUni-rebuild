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
	for (int i = TT_ASTROSOCIOLOGY; i < TT_COUNT; i ++)
	{
		CTecTree *tecTree = new CTecTree;
		switch (i)
		{
		case TT_ASTROSOCIOLOGY:
			tecTree->AddNode(_T("root"), _T("宇宙社会学公理"), FALSE, 100, 10, _T("用于与外星文明的谈判。\n1.生存是文明的第一需求。2.文明不断增长和扩张，但宇宙中的物质总量保持不变。"));
			tecTree->AddNode(_T("宇宙社会学公理"), _T("技术爆炸理论"), FALSE, 150, 50, _T("用于与外星文明的谈判。\n文明可能在短期内出现技术的巨大突破。"));
			tecTree->AddNode(_T("技术爆炸理论"), _T("猜疑链"), FALSE, 230, 100, _T("用于与外星文明的谈判。\n文明之间无法确定对方是否善意。"));
			tecTree->AddNode(_T("猜疑链"), _T("黑暗森林理论"), FALSE, 300, 20, _T("用于与外星文明的谈判。\n宇宙就是一座黑暗森林，每个文明都是带枪的猎人。"));
			tecTree->AddNode(_T("猜疑链"), _T("种族沟通"), FALSE, 500, 300, _T("用于与外星文明的谈判。\n与其它外星文明沟通的通用技巧。"));
			break;
		case TT_NUCLEAR:
			tecTree->AddNode(_T("root"), _T("小行星级氢弹"), FALSE, 100, 30, _T("生产小行星级氢弹的技术。"));
			tecTree->AddNode(_T("root"), _T("宏原子聚变"), FALSE, 100, 10, _T("使宏观原子发生聚变的技术。"));
			tecTree->AddNode(_T("小行星级氢弹"), _T("行星级氢弹"), FALSE, 180, 60, _T("生产行星级氢弹的技术。"));
			tecTree->AddNode(_T("行星级氢弹"), _T("恒星级氢弹"), FALSE, 250, 120, _T("生产恒星级氢弹的技术。"));
			tecTree->AddNode(_T("宏原子聚变"), _T("球状闪电"), FALSE, 300, 150, _T("生产球状闪电的技术。"));
			tecTree->AddNode(_T("球状闪电"), _T("宏化部队"), FALSE, 500, 200, _T("生产宏化部队的技术"));
			break;
		case TT_SPACEFIGHT:
			tecTree->AddNode(_T("root"), _T("10%光速飞船"), FALSE, 100, 30, _T("登陆50光年空间范围的技术，并能生产小型战舰。"));
			tecTree->AddNode(_T("root"), _T("行星开发Ⅰ"), FALSE, 100, 20, _T("在行星上建造采矿工厂的技术"));
			tecTree->AddNode(_T("root"), _T("行星建设Ⅰ"), FALSE, 100, 20, _T("在行星上建造加工工厂的技术"));
			tecTree->AddNode(_T("root"), _T("行星殖民Ⅰ"), FALSE, 100, 20, _T("在行星上建造城市的技术"));
			tecTree->AddNode(_T("10%光速飞船"), _T("50%光速飞船"), FALSE, 150, 50, _T("登陆1万光年空间范围的技术，并能生产中型战舰。"));
			tecTree->AddNode(_T("50%光速飞船"), _T("99%光速飞船"), FALSE, 230, 100, _T("登陆银河系空间范围的技术，并能生产巨型战舰。"));
			tecTree->AddNode(_T("50%光速飞船"), _T("恒星开发Ⅰ"), FALSE, 200, 100, _T("在恒星上建造采矿工厂的技术"));
			tecTree->AddNode(_T("行星开发Ⅰ"), _T("行星开发Ⅱ"), FALSE, 120, 50, _T("提高建造行星采矿工厂的速度。"));
			tecTree->AddNode(_T("行星开发Ⅱ"), _T("行星开发Ⅲ"), FALSE, 150, 80, _T("进一步提高建造行星采矿工厂的速度。"));
			tecTree->AddNode(_T("行星建设Ⅰ"), _T("行星建设Ⅱ"), FALSE, 120, 50, _T("提高建造加工工厂的速度。"));
			tecTree->AddNode(_T("行星建设Ⅱ"), _T("行星建设Ⅲ"), FALSE, 150, 80, _T("进一步提高建造加工工厂的速度。"));
			tecTree->AddNode(_T("行星殖民Ⅰ"), _T("行星殖民Ⅱ"), FALSE, 120, 50, _T("提高建造城市的速度。"));
			tecTree->AddNode(_T("行星殖民Ⅱ"), _T("行星殖民Ⅲ"), FALSE, 150, 80, _T("进一步提高建造城市的速度。"));
			tecTree->AddNode(_T("恒星开发Ⅰ"), _T("恒星开发Ⅱ"), FALSE, 220, 120, _T("提高建造恒星采矿工厂的速度。"));
			tecTree->AddNode(_T("恒星开发Ⅱ"), _T("恒星开发Ⅲ"), FALSE, 250, 150, _T("进一步提高建造恒星采矿工厂的速度。"));
			break;
		case TT_PROTON:
			tecTree->AddNode(_T("root"), _T("质子3维展开"), FALSE, 150, 50, _T("将质子在3个维度内展开的技术。"));
			tecTree->AddNode(_T("质子3维展开"), _T("质子6维展开"), FALSE, 180, 80, _T("将质子在6个维度内展开的技术。"));
			tecTree->AddNode(_T("质子6维展开"), _T("质子9维展开"), FALSE, 230, 150, _T("将质子在9个维度内展开的技术。"));
			tecTree->AddNode(_T("质子6维展开"), _T("智子"), FALSE, 250, 170, _T("生产智子的技术。智子可以监测出其它文明星球的军事实力。"));
			tecTree->AddNode(_T("质子9维展开"), _T("质子11维展开"), FALSE, 300, 200, _T("将质子在11个维度内展开的技术。"));
			tecTree->AddNode(_T("质子9维展开"), _T("奇点炸弹"), FALSE, 300, 500, _T("生产奇点炸弹的技术。奇点炸弹可以直接将一颗星球摧毁。"));
			tecTree->AddNode(_T("质子11维展开"), _T("质能转换"), FALSE, 500, 400, _T("掌握该技术，生产经济将不消耗资源。"));
			break;
		case TT_ASTROPHYSICS:
			tecTree->AddNode(_T("root"), _T("50光年望远镜"), FALSE, 100, 50, _T("查看50光年空间范围星图的技术"));
			tecTree->AddNode(_T("root"), _T("太阳电波放大(50光年)"), FALSE, 100, 20, _T("在星图上用颜色标识50光年空间范围内星球属性的技术"));
			tecTree->AddNode(_T("50光年望远镜"), _T("1万光年望远镜"), FALSE, 200, 100, _T("查看1万光年空间范围星图的技术"));
			tecTree->AddNode(_T("太阳电波放大(50光年)"), _T("太阳电波放大(1万光年)"), FALSE, 200, 50, _T("在星图上用颜色标识1万光年空间范围内星球属性的技术"));
			tecTree->AddNode(_T("1万光年望远镜"), _T("银河系望远镜"), FALSE, 300, 150, _T("查看银河系空间范围星图的技术"));
			tecTree->AddNode(_T("太阳电波放大(1万光年)"), _T("太阳电波放大(银河系)"), FALSE, 300, 100, _T("在星图上用颜色标识银河系空间范围内星球属性的技术"));
			break;
		case TT_CULTURETEC:
			tecTree->AddNode(_T("root"), _T("思想钢印Ⅰ"), FALSE, 100, 30, _T("一种通过思想控制提高文化增长率的技术"));
			tecTree->AddNode(_T("root"), _T("人口Ⅰ"), FALSE, 100, 30, _T("提高人口出生率的技术"));
			tecTree->AddNode(_T("思想钢印Ⅰ"), _T("思想钢印Ⅱ"), FALSE, 200, 70, _T("一种通过思想控制提高文化增长率的技术（强化）"));
			tecTree->AddNode(_T("思想钢印Ⅱ"), _T("思想钢印Ⅲ"), FALSE, 300, 140, _T("一种通过思想控制提高文化增长率的技术（超强化）"));
			tecTree->AddNode(_T("人口Ⅰ"), _T("人口Ⅱ"), FALSE, 200, 70, _T("提高人口出生率的技术（强化）"));
			tecTree->AddNode(_T("人口Ⅱ"), _T("人口Ⅲ"), FALSE, 300, 140, _T("提高人口出生率的技术（超强化）"));
			break;
		case TT_ECONOMYTEC:
			tecTree->AddNode(_T("root"), _T("采矿技术Ⅰ"), FALSE, 100, 30, _T("提高采矿效率的技术"));
			tecTree->AddNode(_T("root"), _T("生产技术Ⅰ"), FALSE, 100, 30, _T("提高经济生产效率的技术"));
			tecTree->AddNode(_T("采矿技术Ⅰ"), _T("采矿技术Ⅱ"), FALSE, 200, 70, _T("进一步提高采矿效率的技术"));
			tecTree->AddNode(_T("采矿技术Ⅱ"), _T("采矿技术Ⅲ"), FALSE, 300, 140, _T("更进一步提高采矿效率的技术"));
			tecTree->AddNode(_T("生产技术Ⅰ"), _T("生产技术Ⅱ"), FALSE, 200, 70, _T("进一步提高经济生产效率的技术"));
			tecTree->AddNode(_T("生产技术Ⅱ"), _T("生产技术Ⅲ"), FALSE, 300, 140, _T("更一步提高经济生产效率的技术"));
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