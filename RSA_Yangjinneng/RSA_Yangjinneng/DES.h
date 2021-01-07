#pragma once
#include "stdafx.h"
#include "Resource.h"
#include<fstream>
#include<bitset>
using namespace std;
using std::bitset;


int block = 0;//����,ÿ��64bit
bool SubKey[16][48];    //16������Կ
static char Temp[256], deskey[16];      //�ݴ��ַ�������Կ��

//��ʼ�û�IP
const static char IP_Table[64] = {
	58,50,42,34,26,18,10,2,
	60,52,44,36,28,20,12,4,
	62,54,46,38,30,22,14,6,
	64,56,48,40,32,24,16,8,
	57,49,41,33,25,17,9,1,
	59,51,43,35,27,19,11,3,
	61,53,45,37,29,21,13,5,
	63,55,47,39,31,23,15,7 };


//���ʼ�û�
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

//��չ��E
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

//P��
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

// ÿ���ƶ���λ��
const static char LS_Table[16] =
{
	1,1,2,2,2,2,2,2,1,2,2,2,2,2,2,1
};

// S��
const static char S_Box[8][4][16] =  //8��S��
{
	// S��1
	14,	 4,	13,	 1,  2, 15, 11,  8,  3, 10,  6, 12,  5,  9,  0,  7,
	 0, 15,  7,  4, 14,  2, 13,  1, 10,  6, 12, 11,  9,  5,  3,  8,
	 4,  1, 14,  8, 13,  6,  2, 11, 15, 12,  9,  7,  3, 10,  5,  0,
	15, 12,  8,  2,  4,  9,  1,  7,  5, 11,  3, 14, 10,  0,  6, 13,
	// S��2
	15,  1,  8, 14,  6, 11,  3,  4,  9,  7,  2, 13, 12,  0,  5, 10,
	 3, 13,  4,  7, 15,  2,  8, 14, 12,  0,  1, 10,  6,  9, 11,  5,
	 0, 14,  7, 11, 10,  4, 13,  1,  5,  8, 12,  6,  9,  3,  2, 15,
	13,  8, 10,  1,  3, 15,  4,  2, 11,  6,  7, 12,  0,  5, 14,  9,
	// S��3
	10,  0,  9, 14,  6,  3, 15,  5,  1, 13, 12,  7, 11,  4,  2,  8,
	13,  7,  0,  9,  3,  4,  6, 10,  2,  8,  5, 14, 12, 11, 15,  1,
	13,  6,  4,  9,  8, 15,  3,  0, 11,  1,  2, 12,  5, 10, 14,  7,
	 1, 10, 13,  0,  6,  9,  8,  7,  4, 15, 14,  3, 11,  5,  2, 12,
	 // S��4
	   7, 13, 14,  3,  0,  6,  9, 10,  1,  2,  8,  5, 11, 12,  4, 15,
	  13,  8, 11,  5,  6, 15,  0,  3,  4,  7,  2, 12,  1, 10, 14,  9,
	  10,  6,  9,  0, 12, 11,  7, 13, 15,  1,  3, 14,  5,  2,  8,  4,
		3, 15,  0,  6, 10,  1, 13,  8,  9,  4,  5, 11, 12,  7,  2, 14,
		// S��5
		2, 12,  4,  1,  7, 10, 11,  6,  8,  5,  3, 15, 13,  0, 14,  9,
		14, 11,  2, 12,  4,  7, 13,  1,  5,  0, 15, 10,  3,  9,  8,  6,
		4,  2,  1, 11, 10, 13,  7,  8, 15,  9, 12,  5,  6,  3,  0, 14,
		11,  8, 12,  7,  1, 14,  2, 13,  6, 15,  0,  9, 10,  4,  5,  3,
		// S��6
		12,  1, 10, 15,  9,  2,  6,  8,  0, 13,  3,  4, 14,  7,  5, 11,
		10, 15,  4,  2,  7, 12,  9,  5,  6,  1, 13, 14,  0, 11,  3,  8,
		 9, 14, 15,  5,  2,  8, 12,  3,  7,  0,  4, 10,  1, 13, 11,  6,
		4,  3,  2, 12,  9,  5, 15, 10, 11, 14,  1,  7,  6,  0,  8, 13,
		// S��7
		4, 11,  2, 14, 15,  0,  8, 13,  3, 12,  9,  7,  5, 10,  6,  1,
		13,  0, 11,  7,  4,  9,  1, 10, 14,  3,  5, 12,  2, 15,  8,  6,
		1,  4, 11, 13, 12,  3,  7, 14, 10, 15,  6,  8,  0,  5,  9,  2,
		 6, 11, 13,  8,  1,  4, 10,  7,  9,  5,  0, 15, 14,  2,  3, 12,
		 // S��8
		  13,  2,  8,  4,  6, 15, 11,  1, 10,  9,  3, 14,  5,  0, 12,  7,
		 1, 15, 13,  8, 10,  3,  7,  4, 12,  5,  6, 11,  0, 14,  9,  2,
		 7, 11,  4,  1,  9, 12, 14,  2,  0,  6, 10, 13, 15,  3,  5,  8,
		 2,  1, 14,  7,  4, 10,  8, 13, 15, 12,  9,  0,  3,  5,  6, 11
};

static void DES_Yangjinneng(char Out[8], char In[8], bool flag);//��׼DES��/����
static void SetSubKey(char Key[8]);// ��������Կ
static void F_func(bool In[32], bool Ki[48]);// �ֺ���f
static void S_func(bool Out[32], bool In[48]);// S �д���
static void Transform(bool *Out, bool *In, const char *Table, int len);// ���û�
static void Xor(bool *InA, const bool *InB, int len);// ���
static void RotateL(bool *In, int len, int loop);// ѭ������
static void ByteToBit(bool *Out, const char *In, int bits);// �ֽ���ת����λ��
static void HalfByteToBit(bool *Out, const char *In, int bits);// ���ֽ���ת����λ��
static void BitToByte(char *Out, const bool *In, int bits);// λ��ת�����ֽ���

static void DES_Yangjinneng(char Out[8], char In[8], bool flag) { //flag��ʾ���ܻ����
	static bool M[64];
	static bool Temp[32];
	static bool* Li = &M[0];
	static bool* Ri = &M[32];//64 bits���� ����IP�û��󣬷ֳ���������
	ByteToBit(M, In, 64); //8���ַ�ת����64����
	if (flag) {              //����
		Transform(M, M, IP_Table, 64);//M����IP�û�
		for (int i = 0; i < 16; i++) {
			memcpy(Temp, Ri, 32);     //�ݴ��Ұ��
			F_func(Ri, SubKey[i]);    //��һ�ֵ��Ұ�ߺ͵ڶ�������Կ�ֱ任
			Xor(Ri, Li, 32);          //���
			memcpy(Li, Temp, 32);     //��һ�����ߵ�����һ���Ұ��
		}
		memcpy(Temp, Li, 32);
		memcpy(Li, Ri, 32);
		memcpy(Ri, Temp, 32);  //��֤���ܺͽ��ܵ�һ���ԣ�����Ri��Li

		Transform(M, M, IPInv_Table, 64);
	}
	else {
		Transform(M, M, IP_Table, 64);
		for (int i = 15; i >= 0; i--) {  //����Կʹ��˳��ͼ����෴
			memcpy(Temp, Ri, 32);
			F_func(Ri, SubKey[i]);
			Xor(Ri, Li, 32);
			memcpy(Li, Temp, 32);
		}
		memcpy(Temp, Li, 32);
		memcpy(Li, Ri, 32);
		memcpy(Ri, Temp, 32);  //��֤���ܺͽ��ܵ�һ���ԣ�����Ri��Li

		Transform(M, M, IPInv_Table, 64);
	}
	BitToByte(Out, M, 64);
}

static void SetSubKey(char Key[8]) {
	static bool K[64];
	static bool * KL = &K[0];
	static bool * KR = &K[28];//��64λ��Կ��ȥ��8λ��żУ��λ�󣬷ֳ�����
	ByteToBit(K, Key, 64);
	Transform(K, K, PC1_Table, 56);//PC_1�û�
	for (int i = 0; i < 16; i++) {
		RotateL(KL, 28, LS_Table[i]);//ѭ������
		RotateL(KR, 28, LS_Table[i]);
		Transform(SubKey[i], K, PC2_Table, 48);//PC_2�û�
	}
}

static void F_func(bool In[32], bool Ki[48]) {
	static bool MR[48];
	Transform(MR, In, E_Table, 48);  //E�任
	Xor(MR, Ki, 48);//���
	S_func(In, MR);//S�д���
	Transform(In, In, P_Table, 32);//P���û�
}

static void S_func(bool Out[32], bool In[48]) {   //��8�飬ÿ��6 bits�Ĵ���ת��Ϊ8�飬ÿ��4 bits
	for (char i = 0, j, k; i < 8; i++) {         //�ֳ�8�飬ÿ��6����
		j = (In[0 + 6 * i] << 1) + In[5 + 6 * i];//��һ�����غ͵���������
		k = (In[1 + 6 * i] << 3) + (In[2 + 6 * i] << 2) + (In[3 + 6 * i] << 1) + In[4 + 6 * i];//�м�4������
		ByteToBit(Out + 4 * i, &S_Box[i][j][k], 4);//ȡ����j�е�k��Ԫ��
	}
}

static void Transform(bool *Out, bool *In, const char *Table, int len) {
	static char Temp[256];
	for (int i = 0; i < len; i++)
		Temp[i] = In[Table[i] - 1];//ȡTable[i]��Ԫ��
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
		Out[i] = (In[i >> 3] >> (i & 7)) & 1;//ȷ��ÿһλ�����ĸ��ַ���ȷ������һλ���ƶ���ĩβ����1���룬ȡ�����
}

static void HalfByteToBit(bool *Out, const char *In, int bits) {
	for (int i = 0; i < bits; ++i)
		Out[i] = (In[i >> 2] >> (i & 3)) & 1;
}

static void BitToByte(char *Out, const bool *In, int bits) {
	memset(Out, 0, bits >> 3);//��ÿ���ֽڶ���ʼ��Ϊ0
	for (int i = 0; i < bits; i++)
		Out[i >> 3] |= In[i] << (i & 7);//i>>3λ���㣬��λ������λ����i����8��i&7��λ���������i����8��
}