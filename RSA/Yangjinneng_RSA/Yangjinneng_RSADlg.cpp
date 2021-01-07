
// Yangjinneng_RSADlg.cpp: 实现文件
//

#include "stdafx.h"
#include "Yangjinneng_RSA.h"
#include "Yangjinneng_RSADlg.h"
#include "afxdialogex.h"
#include"Bigint.h"
#include"DES.h"
#include"SM3.h"
#include"CDlg1.h"
#include"CDlg2.h"
#define WM_MyMessage WM_USER+100
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

char cipher[8];
char temp[8];

CYangjinnengRSADlg* plogin;
// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();
	CDlg1 cdlg1;
	CDlg2 cdlg2;
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CYangjinnengRSADlg 对话框



CYangjinnengRSADlg::CYangjinnengRSADlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_YANGJINNENG_RSA_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CYangjinnengRSADlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_tabctrl);
}

BEGIN_MESSAGE_MAP(CYangjinnengRSADlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CYangjinnengRSADlg::OnTcnSelchangeTab1)
END_MESSAGE_MAP()


// CYangjinnengRSADlg 消息处理程序

BOOL CYangjinnengRSADlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	TCITEM item;

	item.mask = TCIF_TEXT;

	item.pszText = _T("RSA");

	m_tabctrl.InsertItem(0, &item);

	item.pszText = _T("DES,SM3");

	m_tabctrl.InsertItem(1, &item);

	page1.Create(IDD_DIALOG1, &m_tabctrl);

	page2.Create(IDD_DIALOG2, &m_tabctrl);

	CRect rect;

	m_tabctrl.GetClientRect(&rect);

	rect.top += 22;

	rect.bottom -= 1;

	rect.left += 1;

	rect.right -= 1;

	page1.MoveWindow(&rect);

	page2.MoveWindow(&rect);

	page1.ShowWindow(TRUE);

	m_tabctrl.SetCurSel(0);


	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CYangjinnengRSADlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CYangjinnengRSADlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CYangjinnengRSADlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CYangjinnengRSADlg::OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	switch (m_tabctrl.GetCurSel())
	{
	case 0:
		page1.ShowWindow(SW_SHOW);
		page2.ShowWindow(SW_HIDE);
		break;
	case 1:
		page1.ShowWindow(SW_HIDE);
		page2.ShowWindow(SW_SHOW);
		break;
	default:break;
	}


}

void CDlg1::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码

	srand((unsigned int)time(NULL));
	int t1, t2,t3;
	t1 = GetTickCount();
	p = GenPrime(16);//128bits
	q = GenPrime(16);
	N = Narrow(Mul(p, q));

	int n = Length(p);
	int len = 0;
	unsigned char out1[2000] = { 0 };
	unsigned char out2[2000] = { 0 };
	unsigned char out3[2000] = { 0 };
	unsigned char out4[2000] = { 0 };
	unsigned char out5[2000] = { 0 };
	CString temp;

	//输出P
	CString temp1;
	PrintNum(out1, len, p);
	temp.Format("%s", out1);
	SetDlgItemText(IDC_EDIT1, temp);
	
	//输出Q
	
	PrintNum(out2, len, p);
	temp.Format(_T("%s"), out2);
	SetDlgItemText(IDC_EDIT2, temp);

	//输出N
	PrintNum(out3, len, N);
	temp.Format(_T("%s"), out3);
	temp1.Format(_T("%d"), len);
	SetDlgItemText(IDC_EDIT3, temp);
	SetDlgItemText(IDC_EDIT9, temp1);

	//求φ(N)
	Bigint one = { 1 };
	PhiN = Narrow(Mul(Sub(p, one), Sub(q, one)));

	//求公钥e与私钥d
	e = GenE(PhiN);
	Inverse(e, PhiN, d);

	//输出公钥e
	PrintNum(out4, len, e);
	temp.Format(_T("%s"), out4);
	SetDlgItemText(IDC_EDIT4, temp);

	//输出私钥d
	PrintNum(out5, len, d);
	temp.Format(_T("%s"), out5);
	SetDlgItemText(IDC_EDIT5, temp);

	t2 = GetTickCount();
	t3 = t2 - t1;
	temp.Format(("%d ms"), t3);
	SetDlgItemText(IDC_EDIT12, temp);
}

void CDlg1::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	CString message;
	//读入明文
	GetDlgItemText(IDC_EDIT6, message);
	int len = message.GetLength();
	char* message1 = message.GetBuffer(0);
	for (int i = 0; i < len; i++)
	{
		m.num[i] = message1[i]; //明文对应到大整数
	}
	c = Encrypt(m, e, N);  //加密
	CString temp;
	//输出密文
	temp.Format(_T("%s"), c.num);
	SetDlgItemText(IDC_EDIT7, temp);
}


void CDlg1::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	CString message;
	//读入密文
	GetDlgItemText(IDC_EDIT7, message);
	int len = message.GetLength();
	char* message1 = message.GetBuffer(0);
	for (int i = 0; i < len; i++)
	{
		c.num[i] = message1[i]; //明文对应到大整数
	}
	m1 = Decrypt(c, d, N);//解密
	CString temp;
	//输出解密后的明文
	temp.Format(_T("%s"), m1.num);
	SetDlgItemText(IDC_EDIT8, temp);
}

void CDlg2::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog dlg(TRUE/*这个参数为TRUE就是“打开”对话框，为FALSE就是“保存”对话框*/, NULL/*默认文件类型*/, NULL/*默认文件名*/, OFN_HIDEREADONLY/*样式，这里设置为“隐藏只读”*/, _T("所有文件(*.*)|*.*||")/*文件类型列表*/, NULL, NULL, FALSE/*指定文件打开对话框是否为Vista样式*/);
	if (dlg.DoModal() == IDOK)
	{
		CString strPathName = dlg.GetPathName();
		way1.SetWindowText(strPathName);
	}
}

void CDlg2::OnBnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码
	
	CFileDialog dlg(FALSE/*这个参数为TRUE就是“打开”对话框，为FALSE就是“保存”对话框*/, NULL/*默认文件类型*/, NULL/*默认文件名*/, OFN_HIDEREADONLY/*样式，这里设置为“隐藏只读”*/, _T("所有文件(*.*)|*.*||")/*文件类型列表*/, NULL, NULL, FALSE/*指定文件打开对话框是否为Vista样式*/);
	if (dlg.DoModal() == IDOK)
	{
		CString strPathName = dlg.GetPathName();
		way2.SetWindowText(strPathName);
	}
	
}

void CDlg2::OnBnClickedButton6()
{
	// TODO: 在此添加控件通知处理程序代码
	//读入密钥并设置子密钥
	CString k;
	GetDlgItemText(IDC_EDIT11, k);
	char* key = k.GetBuffer(0);//转换成字符数组类型

	if (strlen(key) < 8) {
		AfxMessageBox(_T("密钥长度必须8位！"));
	}
	else 
		SetSubKey(key);

	CString path, path1;
	GetDlgItemText(IDC_EDIT9, path);
	GetDlgItemText(IDC_EDIT10, path1);
	ifstream in;
	ofstream out;
	in.open(path, ios::binary);
	out.open(path1, ios::binary);
	char plain[8];

	while (in.read(plain, sizeof(plain))) {
		DES_Yangjinneng(cipher, plain, 1);
		out.write(cipher, sizeof(cipher));
	}
	in.close();
	out.close();
}

void CDlg2::OnBnClickedButton7()
{
	// TODO: 在此添加控件通知处理程序代码

	CString path1, m, temp;
	GetDlgItemText(IDC_EDIT10, path1);
	CFile file;
	file.Open(path1, CFile::modeRead, NULL);
	DWORD len = file.GetLength();
	unsigned char* buf = (unsigned char*)malloc(len * sizeof(unsigned char)), buf1[32] = { 0 };
	buf[len] = 0;
	file.Read(buf, len);
	file.Close();
	SM3Calc_Yangjinneng(buf, len, buf1);
	for (int i = 0; i < 32; i++) {
		if ((i + 1) % 4 == 0)
			temp.Format("%02x  ", buf1[i]);
		else
			temp.Format("%02x", buf1[i]);
		m += temp;
	}
	SetDlgItemText(IDC_EDIT12, m);
}
