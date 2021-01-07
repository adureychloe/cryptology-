
// Yangjinneng_DESDlg.h: 头文件
//

#pragma once


// CYangjinnengDESDlg 对话框
class CYangjinnengDESDlg : public CDialogEx
{
// 构造
public:
	CYangjinnengDESDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_YANGJINNENG_DES_DIALOG };
#endif

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
	afx_msg void OnEnChangeEdit7();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	CEdit way1;
	CEdit way2;
	CEdit way3;
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton6();
};
