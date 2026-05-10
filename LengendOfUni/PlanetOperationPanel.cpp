// PlanetOperationPanel.cpp : 实现文件
//

#include "stdafx.h"
#include "LengendOfUni.h"
#include "PlanetOperationPanel.h"


// CPlanetOperationPanel 对话框

IMPLEMENT_DYNAMIC(CPlanetOperationPanel, CDialog)

CPlanetOperationPanel::CPlanetOperationPanel(CWnd* pParent /*=NULL*/)
	: CDialog(CPlanetOperationPanel::IDD, pParent)
{

}

CPlanetOperationPanel::~CPlanetOperationPanel()
{
}

void CPlanetOperationPanel::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPlanetOperationPanel, CDialog)
	ON_BN_CLICKED(IDOK, &CPlanetOperationPanel::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CPlanetOperationPanel::OnBnClickedCancel)
END_MESSAGE_MAP()


// CPlanetOperationPanel 消息处理程序

void CPlanetOperationPanel::OnBnClickedOk()
{
	return;
}

void CPlanetOperationPanel::OnBnClickedCancel()
{
	return;
}
