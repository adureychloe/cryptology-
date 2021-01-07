#pragma once

#define WM_MyMessage WM_USER+100
// CDlg1 对话框

class CDlg1 : public CDialogEx
{
	DECLARE_DYNAMIC(CDlg1)

public:
	CDlg1(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDlg1();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	CEdit mw;
	
};
