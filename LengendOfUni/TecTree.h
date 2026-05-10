#pragma once
#include <map>
#include <vector>

class CTecTreeNode
{
private:
	CString		m_sName;				// 科技名
	BOOL		m_bFinish;				// 是否掌握
	BOOL		m_bInReseach;			// 是否正在研究中
	int			m_iTotalWorkload;		// 总共工作量
	int			m_iCurrentWorkload;		// 当前工作量
	int			m_iCost;				// 发展需要的花销
	CString		m_sTip;					// 科技介绍

	CTecTreeNode*							m_pParentNode;
public:
	std::map<CString, CTecTreeNode*>		m_mapChildrenNode;

	CString GetName();
	void SetName(CString sName);
	BOOL GetFinish();
	void SetFinish(BOOL bVal);
	BOOL GetInResearch();
	void SetInResearch(BOOL bVal);
	int GetTotalWorkload();
	void SetTotalWorkload(int iVal);
	int GetCurrentWorkload();
	void SetCurrentWorkload(int iVal);
	int GetCost();
	void SetCost(int iVal);
	CString GetTip();
	void SetTip(CString sTip);
	CTecTreeNode* GetParent();
	void SetParent(CTecTreeNode* pNode);

	CTecTreeNode* GetChildNode(CString sName, CTecTreeNode* pRoot);


	void Research();
	CTecTreeNode(void);
	~CTecTreeNode(void);
};

class CTecTree
{
	CTecTreeNode*		m_pRoot;
	CTecTreeNode*		m_pResearchTecNode;
public:
	CTecTree(void);
	~CTecTree(void);
	void Init();
	void Clear(CTecTreeNode* pRoot);
	void AddNode(CString sParentName, CString sName, bool bFinish, int iTotalWorkload, int iCost, CString sTip);
	CTecTreeNode* GetResearchTecNode();
	void SetResearchTecNode(CTecTreeNode* pNode);
	CTecTreeNode* GetChildNode(CString sName, CTecTreeNode* pRoot = NULL);
	void GetFinishedTecName(std::vector<CString> &vecName, CTecTreeNode* pRoot = NULL);
	void SetAllTecFinish(CTecTreeNode* pRoot = NULL, BOOL bFinish = TRUE);
};
