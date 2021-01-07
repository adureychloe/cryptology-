
// Yangjinneng_RSADlg.h: 头文件
//

#pragma once
#include"CDlg1.h"
#include"CDlg2.h"

// CYangjinnengRSADlg 对话框
class CYangjinnengRSADlg : public CDialogEx
{
// 构造
public:
	CYangjinnengRSADlg(CWnd* pParent = nullptr);	// 标准构造函数


// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_YANGJINNENG_RSA_DIALOG };
#endif

	CDlg1 page1;
	CDlg2 page2;
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CTabCtrl m_tabctrl;
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
};
