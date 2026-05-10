#pragma once
#include <vector>
#include "TecTree.h"

class CTecTreeManager : public CObject 
{
	DECLARE_SERIAL(CTecTreeManager)
private:
	std::vector<CTecTree*>		m_vecTecTree;
public:
	void SetAllTecTreeFinish(BOOL bFinish = TRUE);
	void Serialize(CArchive &ar);
	BOOL IsTecFinished(TEC_TREE_TYPE ttType, CString sTecName);
	void Clear();
	void Init();
	CTecTree* GetTecTree(TEC_TREE_TYPE tecTreeType);
	CTecTreeNode* GetTecTreeNode(TEC_TREE_TYPE ttType, CString sTecName);
	CTecTreeManager(void);
	~CTecTreeManager(void);
};
