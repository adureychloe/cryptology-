
// SM3_YangjinnengDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "SM3_Yangjinneng.h"
#include "SM3_YangjinnengDlg.h"
#include "afxdialogex.h"
#include <stdio.h>
#include <memory.h>
#include "sm3.h"
#include<fstream>
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//判断运行环境是否为小端
static const int endianTest = 1;
#define IsLittleEndian() (*(char *)&endianTest == 1)

//向左循环移位
#define LeftRotate(word, bits) ( (word) << (bits) | (word) >> (32 - (bits)) )

//反转四字节整型字节序
unsigned int *ReverseWord(unsigned int *word)
{
	unsigned char *byte, temp;

	byte = (unsigned char *)word;
	temp = byte[0];
	byte[0] = byte[3];
	byte[3] = temp;

	temp = byte[1];
	byte[1] = byte[2];
	byte[2] = temp;
	return word;
}

//常量T
unsigned int T(int i)
{
	if (i >= 0 && i <= 15)
		return 0x79CC4519;
	else if (i >= 16 && i <= 63)
		return 0x7A879D8A;
	else
		return 0;
}

//FF
unsigned int FF(unsigned int X, unsigned int Y, unsigned int Z, int i)
{
	if (i >= 0 && i <= 15)
		return X ^ Y ^ Z;
	else if (i >= 16 && i <= 63)
		return (X & Y) | (X & Z) | (Y & Z);
	else
		return 0;
}

//GG
unsigned int GG(unsigned int X, unsigned int Y, unsigned int Z, int i)
{
	if (i >= 0 && i <= 15)
		return X ^ Y ^ Z;
	else if (i >= 16 && i <= 63)
		return (X & Y) | (~X & Z);
	else
		return 0;
}

//P0
unsigned int P0(unsigned int X)
{
	return X ^ LeftRotate(X, 9) ^ LeftRotate(X, 17);
}

//P1
unsigned int P1(unsigned int X)
{
	return X ^ LeftRotate(X, 15) ^ LeftRotate(X, 23);
}

//初始化函数
void SM3Init(SM3Context *context)
{
	context->Hash[0] = 0x7380166F;
	context->Hash[1] = 0x4914B2B9;
	context->Hash[2] = 0x172442D7;
	context->Hash[3] = 0xDA8A0600;
	context->Hash[4] = 0xA96F30BC;
	context->Hash[5] = 0x163138AA;
	context->Hash[6] = 0xE38DEE4D;
	context->Hash[7] = 0xB0FB0E4E;
}

//处理消息块
void SM3Process_Yanjinneng(SM3Context *context)
{
	int i;
	unsigned int W[68];
	unsigned int W1[64];
	unsigned int A, B, C, D, E, F, G, H, SS1, SS2, TT1, TT2;

	/* 消息扩展 */
	for (i = 0; i < 16; i++)
	{
		W[i] = *(unsigned int *)(context->messageBlock + i * 4);//将消息分组划分为16个字
		if (IsLittleEndian())
			ReverseWord(W + i);
		  
	}
	for (i = 16; i < 68; i++)
	{
		W[i] = P1(W[i - 16] ^ W[i - 9] ^ LeftRotate(W[i - 3], 15))
			^ LeftRotate(W[i - 13], 7)
			^ W[i - 6];
		  
	}
	for (i = 0; i < 64; i++)
	{
		W1[i] = W[i] ^ W[i + 4];
  
	}

	/* 消息压缩 */
	A = context->Hash[0];
	B = context->Hash[1];
	C = context->Hash[2];
	D = context->Hash[3];
	E = context->Hash[4];
	F = context->Hash[5];
	G = context->Hash[6];
	H = context->Hash[7];
	for (i = 0; i < 64; i++)
	{
		SS1 = LeftRotate((LeftRotate(A, 12) + E + LeftRotate(T(i), i)), 7);
		SS2 = SS1 ^ LeftRotate(A, 12);
		TT1 = FF(A, B, C, i) + D + SS2 + W1[i];
		TT2 = GG(E, F, G, i) + H + SS1 + W[i];
		D = C;
		C = LeftRotate(B, 9);
		B = A;
		A = TT1;
		H = G;
		G = LeftRotate(F, 19);
		F = E;
		E = P0(TT2);
	}
	context->Hash[0] ^= A;
	context->Hash[1] ^= B;
	context->Hash[2] ^= C;
	context->Hash[3] ^= D;
	context->Hash[4] ^= E;
	context->Hash[5] ^= F;
	context->Hash[6] ^= G;
	context->Hash[7] ^= H;
}

//sm3算法主函数
unsigned char *SM3Calc_Yangjinneng(const unsigned char *message,
	unsigned int messageLen, unsigned char digest[SM3_HASH_SIZE]) {
	SM3Context context;
	unsigned int i, remainder, bitLen;

	//初始化
	SM3Init(&context);

	//对前面的消息分组进行处理
	for (i = 0; i < messageLen / 64; i++)//分组
	{
		memcpy(context.messageBlock, message + i * 64, 64);//把每一个分组内容分别传入消息块中
		SM3Process_Yanjinneng(&context);
	}

	//填充消息分组并处理
	bitLen = messageLen * 8;
	if (IsLittleEndian())
		ReverseWord(&bitLen);
	remainder = messageLen % 64;//余数
	memcpy(context.messageBlock, message + i * 64, remainder);//余数存到下一个消息块
	context.messageBlock[remainder] = 0x80;//后补1
	if (remainder < 56)//如果不足448比特
	{
		memset(context.messageBlock + remainder + 1, 0, 64 - remainder - 1 - 8 + 4);
		memcpy(context.messageBlock + 64 - 4, &bitLen, 4);
		SM3Process_Yanjinneng(&context);
	}
	else
	{
		memset(context.messageBlock + remainder + 1, 0, 64 - remainder - 1);
		SM3Process_Yanjinneng(&context);
		memset(context.messageBlock, 0, 64 - 4);
		memcpy(context.messageBlock + 64 - 4, &bitLen, 4);
		SM3Process_Yanjinneng(&context);
	}

	/* 返回结果 */
	if (IsLittleEndian())
		for (i = 0; i < 8; i++)
			ReverseWord(context.Hash + i);
	memcpy(digest, context.Hash, SM3_HASH_SIZE);

	return digest;
}







// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

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


// CSM3YangjinnengDlg 对话框



CSM3YangjinnengDlg::CSM3YangjinnengDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SM3_YANGJINNENG_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSM3YangjinnengDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//  DDX_Control(pDX, IDC_EDIT3, way);
	DDX_Control(pDX, IDC_EDIT4, way1);
}

BEGIN_MESSAGE_MAP(CSM3YangjinnengDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CSM3YangjinnengDlg::OnBnClickedButton1)
	ON_EN_CHANGE(IDC_EDIT1, &CSM3YangjinnengDlg::OnEnChangeEdit1)
	ON_BN_CLICKED(IDC_BUTTON3, &CSM3YangjinnengDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON2, &CSM3YangjinnengDlg::OnBnClickedButton2)
	ON_EN_CHANGE(IDC_EDIT4, &CSM3YangjinnengDlg::OnEnChangeEdit4)
//	ON_EN_CHANGE(IDC_EDIT3, &CSM3YangjinnengDlg::OnChangeEdit3)
//ON_NOTIFY(NM_CUSTOMDRAW, IDC_PROGRESS1, &CSM3YangjinnengDlg::OnNMCustomdrawProgress1)
//ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DATETIMEPICKER1, &CSM3YangjinnengDlg::OnDtnDatetimechangeDatetimepicker1)
END_MESSAGE_MAP()


// CSM3YangjinnengDlg 消息处理程序

BOOL CSM3YangjinnengDlg::OnInitDialog()
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

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CSM3YangjinnengDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CSM3YangjinnengDlg::OnPaint()
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
HCURSOR CSM3YangjinnengDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CSM3YangjinnengDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	CString message1,message2,temp;
	GetDlgItemText(IDC_EDIT1, message1);
	const unsigned char* m1=(unsigned char*)message1.GetBuffer(0);
	unsigned char m2[32] = { 0 };
	int size = message1.GetLength();
	SM3Calc_Yangjinneng(m1, size, m2);
	for (int i = 0; i < 32; i++) {
		if((i+1)%4==0)
			temp.Format("%02x  ", m2[i]);
		else
			temp.Format("%02x", m2[i]);
		message2 += temp;
	}
	
	SetDlgItemText(IDC_EDIT2, message2);

}


void CSM3YangjinnengDlg::OnEnChangeEdit1()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}


void CSM3YangjinnengDlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	
	CString path1,m,temp;
	GetDlgItemText(IDC_EDIT4, path1);
	CFile file;
	file.Open(path1, CFile::modeRead, NULL);
	DWORD len = file.GetLength();
	unsigned char* buf = (unsigned char*)malloc(len*sizeof(unsigned char)), buf1[32] = { 0 };
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

	SetDlgItemText(IDC_EDIT2, m);
}


void CSM3YangjinnengDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog dlg(TRUE/*这个参数为TRUE就是“打开”对话框，为FALSE就是“保存”对话框*/, NULL/*默认文件类型*/, NULL/*默认文件名*/, OFN_HIDEREADONLY/*样式，这里设置为“隐藏只读”*/, _T("所有文件(*.*)|*.*||")/*文件类型列表*/, NULL, NULL, FALSE/*指定文件打开对话框是否为Vista样式*/);
	if (dlg.DoModal() == IDOK)
	{
		CString strPathName = dlg.GetPathName();
		way1.SetWindowText(strPathName);
	}

}


void CSM3YangjinnengDlg::OnEnChangeEdit4()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}






