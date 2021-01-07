#pragma once
#include<iostream>
using namespace std;

#define SIZE 33
#define MULSIZE 2*SIZE

typedef struct Bigint
{
	unsigned char num[SIZE];//无符号字符数组
}Bigint;

typedef struct Bigint2//实现乘法需要
{
	unsigned char num[2 * SIZE];
}Bigint2;

Bigint p = { 0 };
Bigint q = { 0 };
Bigint N = { 0 };
Bigint PhiN = { 0 };//φ(N)
Bigint e = { 0 };
Bigint d = { 0 };
Bigint m = { 0 };//明文
Bigint c = { 0 };//密文
Bigint m1 = { 0 };//解密文

void Copy(Bigint &a, Bigint b);//拷贝
void Print(Bigint a);//把a打印成十进制输出
void PrintNum(unsigned char *out, int &len, Bigint a);//打印输出并储存数组
int Length(Bigint a);//计算字节长度
int Length(Bigint2 a);//
int Compare(Bigint a, Bigint b);//比较大小，a>b,a=b,a<b分别输出1,0，-1
int Compare(Bigint2 a, Bigint2 b);
Bigint ByteMoveLeft(Bigint a, int loop);//左移loop个字节(每左移一位乘以256)
Bigint2 ByteMoveLeft(Bigint2 a, int loop);
void BitMoveRight(Bigint &a);//右移一个比特(每右移一位除以2)
Bigint2 Extend(Bigint a);//扩充数组
Bigint Narrow(Bigint2 a);//截断数组
//生成随机数
Bigint BigRand(Bigint n);  //生成[0,n）内的随机数
Bigint BigRandOdd(int bytes); //生成字节数为bytes的随机奇数
bool MillerRabinKnl(Bigint &n); //单次MillerRabin素性检测
bool MillerRabin(Bigint &n, long loop);  //多次MillerRabin素性检测
Bigint GenPrime(int bytes);  //生成字节数为bytes的大素数

//基本运算
Bigint Add(Bigint a, Bigint b);//加法
Bigint Sub(Bigint a, Bigint b);//减法
Bigint2 Sub(Bigint2 a, Bigint2 b);
Bigint2 Mul(Bigint a, Bigint b);//乘法
Bigint Div(Bigint a, Bigint b);//除法
Bigint Mod(Bigint a, Bigint b);//求余
Bigint2 Mod(Bigint2 a, Bigint2 b);
Bigint AddMod(Bigint a, Bigint b, Bigint n);//模加，计算a+bmodn
Bigint SubMod(Bigint a, Bigint b, Bigint n);//模加，计算a-bmodn,a>=b
Bigint Sub2Mod(Bigint a, Bigint b, Bigint n);//模加，计算a-bmodn
Bigint MulMod(Bigint a, Bigint b, Bigint n);//模乘，计算a*bmodn
Bigint PowMod(Bigint a, Bigint b, Bigint n);//模幂，计算a^bmodn
Bigint GCD(Bigint a, Bigint b);//求最大公因数

Bigint GenE(Bigint PhiN);//生成公私钥
bool Inverse(Bigint e, Bigint N, Bigint &res);
//加密
Bigint Encrypt(Bigint m, Bigint e, Bigint n);
//解密
Bigint Decrypt(Bigint c, Bigint d, Bigint n);
int uctoi(unsigned char str);

void Copy(Bigint &a, Bigint b) {
	for (int i = 0; i < SIZE; i++)
		a.num[i] = b.num[i];
}

void Print(Bigint a)  //把a打印成十进制输出
{
	int res[2000];
	int i = 0;
	Bigint b = { 0 };
	Bigint c = { 10 };
	while (Compare(a, b) == 1) {
		res[i] = Mod(a, c).num[0];
		a = Div(a, c);
		i++;
	}

}

void PrintNum(unsigned char *out, int &len, Bigint a)  //打印输出并储存数组
{
	int i = 0;

	Bigint b = { 0 };
	Bigint c = { 10 };
	while (Compare(a, b) == 1)
	{
		out[i] = Mod(a, c).num[0];
		a = Div(a, c);
		i++;
	}
	len = i;
	unsigned char temp = 0;
	for (int k = 0; k < len / 2; k++)
	{
		temp = out[k];
		out[k] = out[len - 1 - k];
		out[len - 1 - k] = temp;
	}
	for (i = 0; i < len; i++)
	{
		out[i] = out[i] + '0';
	}
}

int uctoi(unsigned char str)
{
	int res = 0;
	for (int i = 0; i < 8; i++)
	{
		res = res + (((str >> i) & 1) << i);
	}
	return res;
}

int Length(Bigint a) {//默认length为size
	int i = SIZE - 1;
	while (a.num[i] == 0 && i >= 0) {//从高往低找，直到字节不为0
		i--;
	}
	return i + 1;
}

int Length(Bigint2 a) {//默认length为size
	int i = 2 * SIZE - 1;
	while (a.num[i] == 0 && i >= 0) {//从高往低找，直到字节不为0
		i--;
	}
	return i + 1;
}

int Compare(Bigint a, Bigint b) {
	int a_len = Length(a);
	int b_len = Length(b);
	int max;
	if (a_len > b_len)
		max = a_len;
	else
		max = b_len;
	if (max == 0)
		return 0;
	else
	{
		for (int i = max - 1; i >= 0; i--)
		{
			if (a.num[i] > b.num[i])
				return 1;
			if (a.num[i] < b.num[i])
				return -1;
		}
	}
	return 0;
}

int Compare(Bigint2 a, Bigint2 b) {
	int a_len = Length(a);
	int b_len = Length(b);
	int max;
	if (a_len > b_len)
		max = a_len;
	else
		max = b_len;
	if (max == 0)
		return 0;
	else
	{
		for (int i = max - 1; i >= 0; i--)
		{
			if (a.num[i] > b.num[i])
				return 1;
			if (a.num[i] < b.num[i])
				return -1;
		}
	}
	return 0;
}

Bigint ByteMoveLeft(Bigint a, int loop) {
	for (int i = Length(a) - 1; i >= 0; i--)
	{
		if (i + loop >= SIZE)//如果超过最大长度
			continue;
		a.num[i + loop] = a.num[i];
	}
	for (int i = loop - 1; i >= 0; i--)//移位后补0
		a.num[i] = 0;
	return a;
}

Bigint2 ByteMoveLeft(Bigint2 a, int loop) {
	for (int i = Length(a) - 1; i >= 0; i--)
	{
		if (i + loop >= 2 * SIZE)
			continue;
		a.num[i + loop] = a.num[i];
	}
	for (int i = loop - 1; i >= 0; i--)
		a.num[i] = 0;
	return a;
}

void BitMoveRight(Bigint &a) {
	a.num[0] = a.num[0] >> 1;
	int flag;
	for (int i = 1; i < SIZE; i++)
	{
		flag = (a.num[i] & 0x01) << 7;
		a.num[i - 1] = a.num[i - 1] + flag;
		a.num[i] = a.num[i] >> 1;
	}
}

Bigint2 Extend(Bigint a) {
	Bigint2 res = { 0 };
	for (int i = 0; i < SIZE; i++)
		res.num[i] = a.num[i];
	return res;
}

Bigint Narrow(Bigint2 a) {
	Bigint res = { 0 };
	for (int i = 0; i < SIZE; i++)
		res.num[i] = a.num[i];
	return res;
}

Bigint Add(Bigint a, Bigint b) { //加法
	Bigint c;//运算结果
	unsigned short temp;//16位
	unsigned char carry = 0;//进位
	for (int i = 0; i < SIZE; i++)
	{
		temp = a.num[i] + b.num[i] + carry;
		c.num[i] = temp & 0x00ff;//把低八位取出来
		carry = (temp >> 8) & 0xff;//看高8位有没有进位
	}
	return c;
}

Bigint Sub(Bigint a, Bigint b) { //减法
	if (Compare(a, b) == -1) {
		cout << "subtract error";
		return a;
	}
	Bigint c;
	short temp;
	unsigned char carry = 0;//借位
	for (int i = 0; i < SIZE; i++) {
		temp = a.num[i] - b.num[i] - carry;
		c.num[i] = temp & 0x00ff;//结果
		carry = (temp >> 15) & 0x01;//有无借位
	}
	return c;
}

Bigint2 Sub(Bigint2 a, Bigint2 b) { //减法
	if (Compare(a, b) == -1) {
		cout << "subtract error";
		return a;
	}
	Bigint2 c;
	short temp;
	unsigned char carry = 0;//借位
	for (int i = 0; i < 2 * SIZE; i++) {
		temp = a.num[i] - b.num[i] - carry;
		c.num[i] = temp & 0x00ff;//结果
		carry = (temp >> 15) & 0x01;//有无借位
	}
	return c;
}

Bigint2 Mul(Bigint a, Bigint b)//乘法
{
	Bigint2 c = { 0 };
	unsigned short temp;
	unsigned char carry;
	for (int i = 0; i < SIZE; i++)
	{
		carry = 0;
		for (int j = 0; j < SIZE; j++)
		{
			temp = a.num[i] * b.num[j] + c.num[i + j] + carry;
			c.num[i + j] = temp & 0x000ff;//取低八位
			carry = (temp >> 8) & 0xff;
		}
	}
	c.num[2 * SIZE - 1] = carry;
	return c;
}

Bigint Div(Bigint a, Bigint b) //除法
{
	Bigint B = { 0 };
	Bigint c = { 0 };
	int len = Length(a) - Length(b);//商的数组长度
	while (len >= 0) {
		B = ByteMoveLeft(b, len);//除数b左移len个字节，作为临时除数B
		while (Compare(a, B) >= 0)
		{
			a = Sub(a, B);//当a>=B时，不断减去B
			c.num[len]++;//商不断自增
		}
		len--;
	}
	return c;
}

Bigint Mod(Bigint a, Bigint b)  //求模
{
	if (Compare(a, b) < 0)
		return a;
	else
	{
		Bigint B = { 0 };
		int len = Length(a) - Length(b);//len用来试除
		while (len >= 0)
		{
			B = ByteMoveLeft(b, len); //除数b左移len个字节，作为临时除数B,B是b的倍数;希望扩大后的B与a接近			
			while (Compare(a, B) >= 0)
				a = Sub(a, B);
			len--;
		}
		return a;//当a<b时得到结果	
	}
}

Bigint2 Mod(Bigint2 a, Bigint2 b)  //求模
{
	if (Compare(a, b) < 0)
		return a;
	else
	{
		Bigint2 B = { 0 };
		int len = Length(a) - Length(b);//len用来试除
		while (len >= 0)
		{
			B = ByteMoveLeft(b, len); //除数b左移len个字节，作为临时除数B,B是b的倍数;希望扩大后的B与a接近			
			while (Compare(a, B) >= 0)
				a = Sub(a, B);
			len--;
		}
		return a;//当a<b时得到结果	
	}
}

Bigint AddMod(Bigint a, Bigint b, Bigint n) //模加，计算a+bmodn
{
	Bigint res;
	res = Add(a, b);
	return Mod(res, n);
}

Bigint SubMod(Bigint a, Bigint b, Bigint n)  //模加，计算a-bmodn,a>=b
{
	Bigint res;
	res = Sub(a, b);
	return Mod(res, n);
}

Bigint Sub2Mod(Bigint a, Bigint b, Bigint n)  //模减，计算a-bmodn
{
	while (Compare(a, b) < 0)
		a = Add(a, n);
	return Sub(a, b);
}

Bigint MulMod(Bigint a, Bigint b, Bigint n)  //模乘，计算a*bmodn
{
	Bigint2 res;
	res = Mul(a, b);
	return Narrow(Mod(res, Extend(n)));
}

Bigint PowMod(Bigint a, Bigint b, Bigint n)  //模幂，计算a^bmodn
{
	Bigint c = { 1 };
	Bigint temp = { 1 };
	while (Length(b) > 0)
	{
		while (!(b.num[0] & 1))
		{
			BitMoveRight(b);
			a = MulMod(a, a, n);
		}
		b = Sub(b, temp);
		c = MulMod(a, c, n);
	}
	return c;
}

Bigint GCD(Bigint a, Bigint b)  //求最大公因数
{
	Bigint c = { 0 };
	while (Length(a) > 0)
	{
		Copy(c, Mod(b, a));
		Copy(b, a);
		Copy(a, c);  //辗转相除法
	}
	return b;
}

Bigint BigRand(Bigint n)  //生成[0,n）内的随机数
{
	Bigint res = { 0 };
	for (int i = 0; i < SIZE; i++)
	{
		res.num[i] = rand() % 256;  //每个字节都取成[0,255]内随机数
	}
	res = Mod(res, n);  //利用求模函数使其落到[0,n)中
	return res;
}

Bigint BigRandOdd(int bytes) //生成字节数为bytes的随机奇数
{
	Bigint res = { 0 };
	for (int i = 0; i < bytes - 1; i++)
	{
		res.num[i] = rand() % 256;
	}
	res.num[bytes - 1] = 128 + rand() % 128;  //保证最高字节的最高比特数是1

	if (!(res.num[0] & 0x01))
		res.num[0] = res.num[0] + 1;

	return res;
}

bool MillerRabinKnl(Bigint &n) //单次MillerRabin素性检测
{
	Bigint b, m, v, temp;
	Bigint j = { 0 };
	Bigint one = { 1 };
	Bigint two = { 2 };
	Bigint three = { 3 };
	m = Sub(n, one);
	while (!(m.num[0] & 0x01))  //计算m,j,使得n-1=2^j*m
	{
		j = Add(j, one);
		BitMoveRight(m);
	}
	b = Add(two, BigRand(Sub(n, three)));//随机选取b在[2,N-1]
	v = PowMod(b, m, n);   //计算v=b^mmodn
	if (Compare(v, one) == 0)  //若v=1,通过测试
		return 1;

	Bigint i = { 1 };
	temp = Sub(n, one);
	while (Compare(v, temp) < 0)  //若v<n-1,不通过
	{
		if (Compare(i, j) == 0)  //若i=j，是合数，不通过
			return 0;
		v = MulMod(v, v, n);  //v=v^2modn,i=i+1
		i = Add(i, one);
	}
	return 1;  //若v=n-1,通过检测
}

bool MillerRabin(Bigint &n, long loop)  //多次MillerRabin素性检测
{
	for (int i = 0; i < loop; i++)
	{
		if (!MillerRabinKnl(n))
			return 0;
	}
	return 1;
}

Bigint GenPrime(int bytes)  //生成字节数为bytes的大素数
{
	Bigint res = BigRandOdd(bytes);
	int loop = 20;
	while (!MillerRabin(res, loop))
	{
		res = BigRandOdd(bytes);
	}
	return res;
}

Bigint GenE(Bigint PhiN)
{
	Bigint e = BigRand(PhiN); //随机生成e
	Bigint g = GCD(PhiN, e);
	while (Length(g) != 1 || g.num[0] != 1)//如果最大公因数不为1
	{
		e = BigRand(PhiN);
		g = GCD(PhiN, e);
	}
	return e;
}

bool Inverse(Bigint e, Bigint N, Bigint &d) //大整数模逆，求e模N的逆，结果存入d
{
	Bigint r1 = { 0 };
	Bigint r2 = { 0 };
	Copy(r1, e);
	Copy(r2, N);
	Bigint s1 = { 1 };
	Bigint s2 = { 0 };
	Bigint s = { 0 };
	Bigint r = { 0 };

	while (1)
	{
		if (Length(r1) == 0)  //若r1=0，求模逆失败
			return 0;
		if (Length(r1) == 1 && r1.num[0] == 1)
		{
			Copy(d, s1);     //若r1=1,求模逆成功，将结果存入d
			return 1;
		}
		q = Div(r1, r2);     //商q=r1/r2
		s = Sub2Mod(s1, MulMod(q, s2, N), N);//s=s1-q*s2,为了结果非负，使用模N运算
		r = Sub(r1, Narrow(Mul(q, r2)));  //r=r1-q*r2
		Copy(r1, r2);
		Copy(s1, s2);
		Copy(s2, s);
		Copy(r2, r);
	}
}

Bigint Encrypt(Bigint m, Bigint e, Bigint n)
{
	return PowMod(m, e, n);
}

Bigint Decrypt(Bigint c, Bigint d, Bigint n)
{
	return PowMod(c, d, n);
}