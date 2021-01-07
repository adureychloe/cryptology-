#pragma once
#include "stdafx.h"
#include "Resource.h"
#include<fstream>
#include<bitset>
using namespace std;
using std::bitset;


int block = 0;//分组,每组64bit
bool SubKey[16][48];    //16轮子密钥
static char Temp[256], deskey[16];      //暂存字符串，密钥串

//初始置换IP
const static char IP_Table[64] = {
	58,50,42,34,26,18,10,2,
	60,52,44,36,28,20,12,4,
	62,54,46,38,30,22,14,6,
	64,56,48,40,32,24,16,8,
	57,49,41,33,25,17,9,1,
	59,51,43,35,27,19,11,3,
	61,53,45,37,29,21,13,5,
	63,55,47,39,31,23,15,7 };


//逆初始置换
const static char IPInv_Table[64] = {
	40,8,48,16,56,24,64,32,
	39,7,47,15,55,23,63,31,
	38,6,46,14,54,22,62,30,
	37,5,45,13,53,21,61,29,
	36,4,44,12,52,20,60,28,
	35,3,43,11,51,19,59,27,
	34,2,42,10,50,18,58,26,
	33,1,41,9,49,17,57,25
};

//扩展表E
static const char E_Table[48] = {
	32,1,2,3,4,5,
	4,5,6,7,8,9,
	8,9,10,11,12,13,
	12,13,14,15,16,17,
	16,17,18,19,20,21,
	20,21,22,23,24,25,
	24,25,26,27,28,29,
	28,29,30,31,32,1
};

//P盒
static const char P_Table[32] = {
	16,7,20,21,29,12,28,17,1,15,23,26,5,18,31,10,
	2,8,24,14,32,27,3,9,19,13,30,6,22,11,4,25
};

// PC_1
const static char PC1_Table[56] =
{
	57, 49, 41, 33, 25, 17,  9,  1, 58, 50, 42, 34, 26, 18,
	10,  2, 59, 51, 43, 35, 27, 19, 11,  3, 60, 52, 44, 36,
	63, 55, 47, 39, 31, 23, 15,  7, 62, 54, 46, 38, 30, 22,
	14,  6, 61, 53, 45, 37, 29, 21, 13,  5, 28, 20, 12,  4
};


// PC_2
const static char PC2_Table[48] =
{
	14, 17, 11, 24,  1,  5,  3, 28, 15,  6, 21, 10,
	23, 19, 12,  4, 26,  8, 16,  7, 27, 20, 13,  2,
	41, 52, 31, 37, 47, 55, 30, 40, 51, 45, 33, 48,
	44, 49, 39, 56, 34, 53, 46, 42, 50, 36, 29, 32
};

// 每轮移动的位数
const static char LS_Table[16] =
{
	1,1,2,2,2,2,2,2,1,2,2,2,2,2,2,1
};

// S盒
const static char S_Box[8][4][16] =  //8个S盒
{
	// S盒1
	14,	 4,	13,	 1,  2, 15, 11,  8,  3, 10,  6, 12,  5,  9,  0,  7,
	 0, 15,  7,  4, 14,  2, 13,  1, 10,  6, 12, 11,  9,  5,  3,  8,
	 4,  1, 14,  8, 13,  6,  2, 11, 15, 12,  9,  7,  3, 10,  5,  0,
	15, 12,  8,  2,  4,  9,  1,  7,  5, 11,  3, 14, 10,  0,  6, 13,
	// S盒2
	15,  1,  8, 14,  6, 11,  3,  4,  9,  7,  2, 13, 12,  0,  5, 10,
	 3, 13,  4,  7, 15,  2,  8, 14, 12,  0,  1, 10,  6,  9, 11,  5,
	 0, 14,  7, 11, 10,  4, 13,  1,  5,  8, 12,  6,  9,  3,  2, 15,
	13,  8, 10,  1,  3, 15,  4,  2, 11,  6,  7, 12,  0,  5, 14,  9,
	// S盒3
	10,  0,  9, 14,  6,  3, 15,  5,  1, 13, 12,  7, 11,  4,  2,  8,
	13,  7,  0,  9,  3,  4,  6, 10,  2,  8,  5, 14, 12, 11, 15,  1,
	13,  6,  4,  9,  8, 15,  3,  0, 11,  1,  2, 12,  5, 10, 14,  7,
	 1, 10, 13,  0,  6,  9,  8,  7,  4, 15, 14,  3, 11,  5,  2, 12,
	 // S盒4
	   7, 13, 14,  3,  0,  6,  9, 10,  1,  2,  8,  5, 11, 12,  4, 15,
	  13,  8, 11,  5,  6, 15,  0,  3,  4,  7,  2, 12,  1, 10, 14,  9,
	  10,  6,  9,  0, 12, 11,  7, 13, 15,  1,  3, 14,  5,  2,  8,  4,
		3, 15,  0,  6, 10,  1, 13,  8,  9,  4,  5, 11, 12,  7,  2, 14,
		// S盒5
		2, 12,  4,  1,  7, 10, 11,  6,  8,  5,  3, 15, 13,  0, 14,  9,
		14, 11,  2, 12,  4,  7, 13,  1,  5,  0, 15, 10,  3,  9,  8,  6,
		4,  2,  1, 11, 10, 13,  7,  8, 15,  9, 12,  5,  6,  3,  0, 14,
		11,  8, 12,  7,  1, 14,  2, 13,  6, 15,  0,  9, 10,  4,  5,  3,
		// S盒6
		12,  1, 10, 15,  9,  2,  6,  8,  0, 13,  3,  4, 14,  7,  5, 11,
		10, 15,  4,  2,  7, 12,  9,  5,  6,  1, 13, 14,  0, 11,  3,  8,
		 9, 14, 15,  5,  2,  8, 12,  3,  7,  0,  4, 10,  1, 13, 11,  6,
		4,  3,  2, 12,  9,  5, 15, 10, 11, 14,  1,  7,  6,  0,  8, 13,
		// S盒7
		4, 11,  2, 14, 15,  0,  8, 13,  3, 12,  9,  7,  5, 10,  6,  1,
		13,  0, 11,  7,  4,  9,  1, 10, 14,  3,  5, 12,  2, 15,  8,  6,
		1,  4, 11, 13, 12,  3,  7, 14, 10, 15,  6,  8,  0,  5,  9,  2,
		 6, 11, 13,  8,  1,  4, 10,  7,  9,  5,  0, 15, 14,  2,  3, 12,
		 // S盒8
		  13,  2,  8,  4,  6, 15, 11,  1, 10,  9,  3, 14,  5,  0, 12,  7,
		 1, 15, 13,  8, 10,  3,  7,  4, 12,  5,  6, 11,  0, 14,  9,  2,
		 7, 11,  4,  1,  9, 12, 14,  2,  0,  6, 10, 13, 15,  3,  5,  8,
		 2,  1, 14,  7,  4, 10,  8, 13, 15, 12,  9,  0,  3,  5,  6, 11
};

static void DES_Yangjinneng(char Out[8], char In[8], bool flag);//标准DES加/解密
static void SetSubKey(char Key[8]);// 设置子密钥
static void F_func(bool In[32], bool Ki[48]);// 轮函数f
static void S_func(bool Out[32], bool In[48]);// S 盒代替
static void Transform(bool *Out, bool *In, const char *Table, int len);// 表置换
static void Xor(bool *InA, const bool *InB, int len);// 异或
static void RotateL(bool *In, int len, int loop);// 循环左移
static void ByteToBit(bool *Out, const char *In, int bits);// 字节组转换成位组
static void HalfByteToBit(bool *Out, const char *In, int bits);// 半字节组转换成位组
static void BitToByte(char *Out, const bool *In, int bits);// 位组转换成字节组

static void DES_Yangjinneng(char Out[8], char In[8], bool flag) { //flag表示加密或解密
	static bool M[64];
	static bool Temp[32];
	static bool* Li = &M[0];
	static bool* Ri = &M[32];//64 bits明文 经过IP置换后，分成左右两份
	ByteToBit(M, In, 64); //8个字符转换成64比特
	if (flag) {              //加密
		Transform(M, M, IP_Table, 64);//M进行IP置换
		for (int i = 0; i < 16; i++) {
			memcpy(Temp, Ri, 32);     //暂存右半边
			F_func(Ri, SubKey[i]);    //上一轮的右半边和第二轮子密钥轮变换
			Xor(Ri, Li, 32);          //异或
			memcpy(Li, Temp, 32);     //下一轮左半边等于上一轮右半边
		}
		memcpy(Temp, Li, 32);
		memcpy(Li, Ri, 32);
		memcpy(Ri, Temp, 32);  //保证加密和解密的一致性，交换Ri和Li

		Transform(M, M, IPInv_Table, 64);
	}
	else {
		Transform(M, M, IP_Table, 64);
		for (int i = 15; i >= 0; i--) {  //子密钥使用顺序和加密相反
			memcpy(Temp, Ri, 32);
			F_func(Ri, SubKey[i]);
			Xor(Ri, Li, 32);
			memcpy(Li, Temp, 32);
		}
		memcpy(Temp, Li, 32);
		memcpy(Li, Ri, 32);
		memcpy(Ri, Temp, 32);  //保证加密和解密的一致性，交换Ri和Li

		Transform(M, M, IPInv_Table, 64);
	}
	BitToByte(Out, M, 64);
}

static void SetSubKey(char Key[8]) {
	static bool K[64];
	static bool * KL = &K[0];
	static bool * KR = &K[28];//将64位密钥串去掉8位奇偶校验位后，分成两份
	ByteToBit(K, Key, 64);
	Transform(K, K, PC1_Table, 56);//PC_1置换
	for (int i = 0; i < 16; i++) {
		RotateL(KL, 28, LS_Table[i]);//循环左移
		RotateL(KR, 28, LS_Table[i]);
		Transform(SubKey[i], K, PC2_Table, 48);//PC_2置换
	}
}

static void F_func(bool In[32], bool Ki[48]) {
	static bool MR[48];
	Transform(MR, In, E_Table, 48);  //E变换
	Xor(MR, Ki, 48);//异或
	S_func(In, MR);//S盒代替
	Transform(In, In, P_Table, 32);//P盒置换
}

static void S_func(bool Out[32], bool In[48]) {   //将8组，每组6 bits的串，转化为8组，每组4 bits
	for (char i = 0, j, k; i < 8; i++) {         //分成8组，每组6比特
		j = (In[0 + 6 * i] << 1) + In[5 + 6 * i];//第一个比特和第六个比特
		k = (In[1 + 6 * i] << 3) + (In[2 + 6 * i] << 2) + (In[3 + 6 * i] << 1) + In[4 + 6 * i];//中间4个比特
		ByteToBit(Out + 4 * i, &S_Box[i][j][k], 4);//取出第j行第k列元素
	}
}

static void Transform(bool *Out, bool *In, const char *Table, int len) {
	static char Temp[256];
	for (int i = 0; i < len; i++)
		Temp[i] = In[Table[i] - 1];//取Table[i]个元素
	memcpy(Out, Temp, len);
}

static void Xor(bool *InA, const bool *InB, int len) {
	for (int i = 0; i < len; i++) {
		InA[i] = InB[i] ^ InA[i];
	}
}

static void RotateL(bool *In, int len, int loop) {
	static char Temp[256];
	memcpy(Temp, In, loop);
	memcpy(In, In + loop, len - loop);
	memcpy(In + len - loop, Temp, loop);
}

static void ByteToBit(bool *Out, const char *In, int bits) {
	for (int i = 0; i < bits; ++i)
		Out[i] = (In[i >> 3] >> (i & 7)) & 1;//确定每一位来自哪个字符；确定是哪一位，移动至末尾；与1相与，取出结果
}

static void HalfByteToBit(bool *Out, const char *In, int bits) {
	for (int i = 0; i < bits; ++i)
		Out[i] = (In[i >> 2] >> (i & 3)) & 1;
}

static void BitToByte(char *Out, const bool *In, int bits) {
	memset(Out, 0, bits >> 3);//把每个字节都初始化为0
	for (int i = 0; i < bits; i++)
		Out[i >> 3] |= In[i] << (i & 7);//i>>3位运算，按位右移三位等于i除以8，i&7按位与运算等于i求余8；
}