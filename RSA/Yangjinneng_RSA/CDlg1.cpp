// CDlg1.cpp: 实现文件
//

#include "stdafx.h"
#include "Yangjinneng_RSA.h"
#include "CDlg1.h"
#include "afxdialogex.h"


// CDlg1 对话框

IMPLEMENT_DYNAMIC(CDlg1, CDialogEx)

CDlg1::CDlg1(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{

}

CDlg1::~CDlg1()
{
}

void CDlg1::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT6, mw);
}


BEGIN_MESSAGE_MAP(CDlg1, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CDlg1::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CDlg1::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CDlg1::OnBnClickedButton3)
	
END_MESSAGE_MAP()











