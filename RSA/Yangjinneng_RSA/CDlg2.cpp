// CDlg2.cpp: 实现文件
//

#include "stdafx.h"
#include "Yangjinneng_RSA.h"
#include "CDlg2.h"
#include "afxdialogex.h"


// CDlg2 对话框

IMPLEMENT_DYNAMIC(CDlg2, CDialogEx)

CDlg2::CDlg2(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG2, pParent)
{

}

CDlg2::~CDlg2()
{
}

void CDlg2::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT9, way1);
	DDX_Control(pDX, IDC_EDIT10, way2);
	DDX_Control(pDX, IDC_EDIT12, m_name);
}


BEGIN_MESSAGE_MAP(CDlg2, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON4, &CDlg2::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CDlg2::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &CDlg2::OnBnClickedButton6)
	ON_EN_CHANGE(IDC_EDIT11, &CDlg2::OnEnChangeEdit11)
	ON_BN_CLICKED(IDC_BUTTON7, &CDlg2::OnBnClickedButton7)
END_MESSAGE_MAP()


// CDlg2 消息处理程序








void CDlg2::OnEnChangeEdit11()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}


