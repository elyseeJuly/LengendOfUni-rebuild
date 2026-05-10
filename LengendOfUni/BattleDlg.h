#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include <map>
#include "ComboListCtrl.h"
#include "ToolTipDlg.h"

// CBattleDlg 对话框

class CBattleDlg : public CDialog
{
	DECLARE_DYNAMIC(CBattleDlg)
	enum BATTLE_RESULT
	{
		BR_WIN,
		BR_LOSE,
		BR_INBATTLE,
		BR_DRAW,
	};
	enum BATTLE_SIDE
	{
		BS_EARTH,
		BS_ENEMY,
	};
	enum ATTACK_TYPE
	{
		AT_UNITS,
		AT_BOMB,
		AT_GENERAL_SKILL,
	};
	struct SSimpleWeapon
	{
		CString			m_sOriginalName;		// 原始名字
		CString			m_sName;				// 名字(前面可能加了编号，用于方便部队识别)
		int				m_iAttack;				// 攻击力
		int				m_iHp;					// hp值
		int				m_iPriority;			// 优先级
		WEAPON_TYPE		m_Type;					// 类型
		CString			m_sEnemy;				// 攻击的敌人目标名
		BATTLE_SIDE		m_iOwner;				// 0为我方， 1为敌方
	};
	typedef std::map<CString, SSimpleWeapon*> SimpleWeaponMap;

	class CompareFunObj
	{
	public:
		bool operator()(const SSimpleWeapon *pX,const SSimpleWeapon* pY)   
		{   
			return  pX->m_iPriority > pY->m_iPriority;
		};   
	};  

public:
	CBattleDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CBattleDlg();

// 对话框数据
	enum { IDD = IDD_BATTLE_DLG };

protected:
	void InitTitle();
	void ShowBattleResult(BATTLE_RESULT battleRes);
	BOOL DoGeneralSkill();
	void UpdateBothSidesPopulation();
	void SetWeaponToBarback(BATTLE_SIDE iType);
	BATTLE_RESULT GetBattleResult();
	void DoBattleResult(BATTLE_RESULT battleRes);
	void OutputBattleInfo(CString sInfo);
	void AttackProc(SSimpleWeapon *pWeapon, std::list<SSimpleWeapon*>* pLstWeapon = NULL);
	void BattleProc(ATTACK_TYPE iAttackType);
	void UpdateEnemyUI();
	void UpdateMyUI();
	void InitGeneralSkill();
	void InsertArmyListCtrl(SSimpleWeapon* pSimpleWeapon);
	void InitArmyListCtrl();
	void InsertEnemyListCtrl(SSimpleWeapon* pSimpleWeapon);
	void InitEnemyListCtrl();
	void InitMyUI();
	void InitEnemyUI();
	void InitOutputBox();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	virtual void OnOK();
	virtual void OnCancel();
public:
	void Clear();
	void Init(int iMyStarIndex, int iEnemyStarIndex, BATTLE_TYPE battleType);
	virtual BOOL OnInitDialog();

	LRESULT PopTargetComboList(WPARAM wParam, LPARAM lParam);

private:
	CToolTipDlg					m_toolTipDlg;
	void InitToolTip();
	BOOL			m_bUseGeneralSkill;		// 是否已使用将军技
	SimpleWeaponMap	m_mapMyArmy;
	SimpleWeaponMap	m_mapEnemyArmy;
	int		m_iMyStarIndex;
	int		m_iEnemyStarIndex;
	BATTLE_TYPE m_battleType;
public:
	CString m_sLeader;
	CStatic m_ctrFace;
	CComboListCtrl m_ctrlMyArmyListCtrl;
	CComboBox m_ctrMyWeaponCombo;
	CListCtrl m_ctrlEnemyListCtrl;
	CString m_sEnemyName;
	CStatic m_ctrEnemyFace;
	CEdit m_ctrInfoEdit;
	CString m_sInfoEdit;
	afx_msg void OnBnClickedButtonArmyAttack();
	afx_msg void OnBnClickedButtonWeaponAttack();
	afx_msg void OnBnClickedButtonGenskill();
	CComboBox m_ctrGeneralSkill;
	afx_msg void OnCbnSelchangeComboWeapon();
	afx_msg void OnCbnSelchangeCombo2();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
