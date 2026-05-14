#include "StdAfx.h"
#include "TecTree.h"

CTecTreeNode::CTecTreeNode(void)
{
	m_sName.Empty();
	m_bFinish = FALSE;
	m_bInReseach = FALSE;
	m_iTotalWorkload = m_iCurrentWorkload = 0;
	m_sTip.Empty();
	m_pParentNode = NULL;
	m_iCost = 0;
}

CTecTreeNode::~CTecTreeNode(void)
{
}

CString CTecTreeNode::GetName()
{
	return m_sName;
}

void CTecTreeNode::SetName(CString sName)
{
	m_sName = sName;
}

BOOL CTecTreeNode::GetFinish()
{
	return m_bFinish;
}
void CTecTreeNode::SetFinish(BOOL bVal)
{
	m_bFinish = bVal;
	if (bVal)
		m_iCurrentWorkload = m_iTotalWorkload;
}

BOOL CTecTreeNode::GetInResearch()
{
	return m_bInReseach;
}
void CTecTreeNode::SetInResearch(BOOL bVal)
{
	m_bInReseach = bVal;
}
int CTecTreeNode::GetTotalWorkload()
{
	return m_iTotalWorkload;
}
void CTecTreeNode::SetTotalWorkload(int iVal)
{
	m_iTotalWorkload = iVal;
}

int CTecTreeNode::GetCurrentWorkload()
{
	return m_iCurrentWorkload;
}
void CTecTreeNode::SetCurrentWorkload(int iVal)
{
	m_iCurrentWorkload = (iVal < m_iTotalWorkload ? iVal : m_iTotalWorkload);
	if (m_iCurrentWorkload == m_iTotalWorkload && m_iCurrentWorkload != 0)
	{
		m_bFinish = TRUE;
	}
}

CString CTecTreeNode::GetTip()
{
	return m_sTip;
}
void CTecTreeNode::SetTip(CString sTip)
{
	m_sTip = sTip;
}


void CTecTreeNode::Research()
{
}

CTecTreeNode* CTecTreeNode::GetChildNode(CString sName, CTecTreeNode* pRoot)
{
	if (pRoot->m_sName.Compare(sName) == 0)
		return pRoot;

	if (pRoot->m_mapChildrenNode.size() == 0)
		return NULL;

	if (pRoot->m_mapChildrenNode.find(sName) == pRoot->m_mapChildrenNode.end())
	{
		std::map<CString, CTecTreeNode*>::iterator itr = pRoot->m_mapChildrenNode.begin();
		while (itr != pRoot->m_mapChildrenNode.end())
		{
			CTecTreeNode* pNode = GetChildNode(sName, itr->second);
			if (pNode)
				return pNode;
			itr ++;
		}
	}
	else
		return pRoot->m_mapChildrenNode[sName];

	return NULL;
}

CTecTreeNode* CTecTreeNode::GetParent()
{
	return m_pParentNode;
}
void CTecTreeNode::SetParent(CTecTreeNode* pNode)
{
	m_pParentNode = pNode;
}


int CTecTreeNode::GetCost()
{
	return m_iCost;
}

void CTecTreeNode::SetCost(int iVal)
{
	m_iCost = iVal;
}
//-----------------------------------------------------------------------

CTecTree::CTecTree(void)
{
	m_pRoot = NULL;
	m_pResearchTecNode = NULL;
	Init();
}

CTecTree::~CTecTree(void)
{
	Clear(m_pRoot);
}

void CTecTree::AddNode(CString sParentName, CString sName, bool bFinish, int iTotalWorkload, int iCost, CString sTip)
{
	CTecTreeNode* pNode = m_pRoot->GetChildNode(sParentName, m_pRoot);
	if (!pNode)
		return;

	CTecTreeNode* pNewNode = new CTecTreeNode;
	pNewNode->SetName(sName);
	pNewNode->SetFinish(bFinish);
	pNewNode->SetTotalWorkload(iTotalWorkload);
	pNewNode->SetCurrentWorkload(0);
	pNewNode->SetCost(iCost);
	CString sInfo;
	sInfo.Format(_T("消耗经济：%d\n%s"), iCost, sTip);
	pNewNode->SetTip(sInfo);
	pNewNode->SetParent(pNode);
	std::map<CString, CTecTreeNode*>::iterator itr;
	if ((itr = pNode->m_mapChildrenNode.find(sName)) != pNode->m_mapChildrenNode.end())
	{
		delete pNode->m_mapChildrenNode[sName];
		pNode->m_mapChildrenNode[sName] = NULL;
		pNode->m_mapChildrenNode.erase(itr);
	}
	pNode->m_mapChildrenNode[sName] = pNewNode;

}

void CTecTree::Init()
{
	m_pRoot = new CTecTreeNode;
	m_pRoot->SetName(_T("root"));		// 所有技能树的根都叫"root"
	m_pRoot->SetFinish(TRUE);
}

void CTecTree::Clear(CTecTreeNode* pRoot)
{
	if (pRoot->m_mapChildrenNode.size() != 0)
	{
		std::map<CString, CTecTreeNode*>::iterator itr = pRoot->m_mapChildrenNode.begin();
		while (itr != pRoot->m_mapChildrenNode.end())
		{
			Clear(itr->second);
			itr ++;
		}
		pRoot->m_mapChildrenNode.clear();
		SAFE_DELETE(pRoot)
	}
	else
	{
		SAFE_DELETE(pRoot)
	}
}

CTecTreeNode* CTecTree::GetResearchTecNode()
{
	return m_pResearchTecNode;
}

CTecTreeNode* CTecTree::GetChildNode(CString sName, CTecTreeNode* pRoot)
{
	if (pRoot == NULL)
	{
		pRoot = m_pRoot;
	}
	return pRoot->GetChildNode(sName, pRoot);
}

void CTecTree::SetResearchTecNode(CTecTreeNode* pNode)
{
	m_pResearchTecNode = pNode;
}

void CTecTree::GetFinishedTecName(std::vector<CString> &vecName, CTecTreeNode* pRoot)
{
	if (pRoot == NULL)
	{
		pRoot = m_pRoot;
	}

	if (pRoot->m_mapChildrenNode.size() != 0)
	{
		std::map<CString, CTecTreeNode*>::iterator itr = pRoot->m_mapChildrenNode.begin();
		while (itr != pRoot->m_mapChildrenNode.end())
		{
			GetFinishedTecName(vecName, itr->second);
			itr ++;
		}
		if (pRoot->GetFinish())
			vecName.push_back(pRoot->GetName());
	}
	else
	{
		if (pRoot->GetFinish())
			vecName.push_back(pRoot->GetName());
	}
}

void CTecTree::SetAllTecFinish(CTecTreeNode* pRoot, BOOL bFinish)
{
	if (pRoot == NULL)
	{
		pRoot = m_pRoot;
	}

	if (pRoot->m_mapChildrenNode.size() != 0)
	{
		std::map<CString, CTecTreeNode*>::iterator itr = pRoot->m_mapChildrenNode.begin();
		while (itr != pRoot->m_mapChildrenNode.end())
		{
			SetAllTecFinish(itr->second, bFinish);
			itr ++;
		}
		pRoot->SetFinish(bFinish);
	}
	else
	{
		pRoot->SetFinish(bFinish);
	}
}