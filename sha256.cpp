//zwh
//zwh
//zwh

#define _CRT_SECURE_NO_WARNINGS


#include "stdio.h"
#include "windows.h"


typedef unsigned int uint;
typedef unsigned char uchar;

//void showbin(uint x)
//{
//	uint s = 0x80000000;
//	for (int i = 0; i < 32; i++)
//	{
//		printf("%d", x / s);
//		x = x % s;
//		s = s / 2;
//		if (i % 4 == 3)
//		{
//			printf("  ");
//		}
//	}
//
//}


#define MAX 100



uint Ch(uint x, uint y, uint z)
{
	return (x&y) ^ ((~x)&z);
}

uint Ma(uint x, uint y, uint z)
{

	return (x&y) ^ (x&z) ^ (y&z);
}

uint S0(uint x,int n)
{
	uint a = 1, b = 1;
	for (int i = 0; i < n; i++)
	{
		a = 2 * a;
	}
	b = 128  * 256 * 256 * 256 / a*2;
	uint sc= (x % a * b + x / a);

	return sc;
}

uint R0(uint x, int n)
{
	uint a = 1;
	for (int i = 0; i < n; i++)
	{
		a = 2 * a;
	}
	uint rc = x / a;

	return rc;
}

uint E0(uint x)
{
	return (S0(x, 2)) ^ (S0(x, 13)) ^ (S0(x, 22));
}

uint E1(uint x)
{
	return (S0(x, 6)) ^ (S0(x, 11)) ^ (S0(x, 25));
}

uint D0(uint x)
{
	return (S0(x, 7)) ^ (S0(x, 18)) ^ (R0(x, 3));
}

uint D1(uint x)
{
	return (S0(x, 17)) ^ (S0(x, 19)) ^ (R0(x, 10));
}




int shuru(uchar preData[])
{
	uint num=0;
	for (int i = 0; i < MAX; i++)
	{
		if ((preData[i] = getchar()) == '\n')
		{
			preData[i] = (uchar)128;//填充128 （1000 0000）=（0x80）
			num = (uint)i;
			break;
		}

	}

	return num;

}

int tianchong(uchar preData[],uint num)
{

	uchar waitforenter[8] = { 0 };//待填充的长度8字节
	uint tem = num * 8;//字符串所占的位（即一个字符占8位）
	//计算填充字节
	for (int i = 7; i > 0; i--)
	{

		//循环到四次之后 得到的结果都为零 理由是sha256用64比特存储长度
		//而C语言中整数所占的比特为32位
		waitforenter[i] = (uchar)tem % 256;
		tem = tem / 256;
		//printf("%d", waitforenter[i]);
	}

	/*printf("\n");*/

	int Mnlength = 0;//n块的字节数

	if ((num + 1) % 64 <= 56)
	{
		int i = 0;
		for (i = num + 1; i < (num + 1) / 64 * 64 + 56; i++)
		{
			preData[i] = (uchar)0;
		}
		for (int j = 0; i < (num + 1) / 64 * 64 + 64; i++, j++)
		{
			preData[i] = waitforenter[j];
		}


		Mnlength = (num + 1) / 64 * 64 + 64;



	}
	else {
		int i = 0;
		for (i = num + 1; i < (num + 1) / 64 * 64 + 64 + 56; i++)
		{
			preData[i] = (uchar)0;
		}
		for (int j = 0; i < (num + 1) / 64 * 64 + 64 + 64; i++, j++)
		{
			preData[i] = waitforenter[j];
		}
		Mnlength = (num + 1) / 64 * 64 + 64 + 64;
	}

	return Mnlength;

}




void trans(uchar preData[],uint traData[],int Mnlength)
{
	for (int i = 0; i < Mnlength / 4; i++)
	{
		traData[i] = preData[4 * i] * 256 * 256 * 256
			+ preData[4 * i + 1] * 256 * 256 + preData[4 * i + 2] * 256 + preData[4 * i + 3];
		/*printf("tra[%d]=%x\t\t",i, traData[i]);
		if (i % 2 != 0) {
			printf("\n");
		}*/
	}


}


void jiami(uint ha[],uint traData[],int Mnlength)
{
	uint h[8] = {
	0x6a09e667,
	0xbb67ae85,
	0x3c6ef372,
	0xa54ff53a,
	0x510e527f,
	0x9b05688c,
	0x1f83d9ab,
	0x5be0cd19
	};

	uint k[64] = {

	0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,

	0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,

	0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,

	0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,

	0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,

	0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,

	0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,

	0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2

	};

	uint hash[8] = { 0 };
	int n = Mnlength / 64;//将Mn分为n块
	uint w[64] = { 0 };

	uint temCh, temE1, temMa, temE0;
	uint t1, t2;
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < 16; j++)
		{
			w[j] = traData[i * 16 + j];

		}
		for (int j = 16; j < 64; j++)
		{
			w[j] = D1(w[j - 2]) + w[j - 7] + D0(w[j - 15]) + w[j - 16];

		}


		for (int j = 0; j < 8; j++)
		{
			hash[j] = h[j];

		}


		for (int j = 0; j < 64; j++)
		{

			temCh = Ch(hash[4], hash[5], hash[6]);
			temE1 = E1(hash[4]);
			temMa = Ma(hash[0], hash[1], hash[2]);
			temE0 = E0(hash[0]);


			t1 = temCh + hash[7] + k[j] + w[j] + temE1;
			t2 = temMa + temE0;



			hash[7] = hash[6];
			hash[6] = hash[5];
			hash[5] = hash[4];
			hash[4] = hash[3] + t1;
			hash[3] = hash[2];
			hash[2] = hash[1];
			hash[1] = hash[0];
			hash[0] = t1 + t2;


			/*printf("\n*****第%d次循环加密*******\n", j + 1);
			printf("temMa==%x,temCh==%x,temE0==%x,temE1==%x\n", temMa, temCh, temE0, temE1);
			printf("t1==%x,t2==%x\n",t1,t2);
			printf("k[j]==%x,w[j]==%x\n", k[j], w[j]);
			for (int k = 0; k < 8; k++)
			{
				printf("hash[%d]==%x \n", k, hash[k]);

			}*/


		}
		for (int j = 0; j < 8; j++)
		{
			h[j] += hash[j];
		}

	}

	for (int i = 0; i < 8; i++)
	{

		ha[i] = h[i];
	}

	
}

int main()
{

	//用preData储存读取的字符，num为字符个数
	printf("Please Input:\n");
	uchar preData[MAX] = { 0 };
	uint num = shuru(preData);//字符串的个数
	

	int Mnlength=tianchong(preData, num);
	
	/*printf("填充 is over!!\n\n");*/

	
	//将得到的Mn输出，这里还是用predate表示
	/*for (int i = 0; i < Mnlength; i++)
	{
		printf("preDate[%d]=%c\t", i, preData[i]);
		if (i % 2 != 0) {
			printf("\n");
		}
	}*/



	//translate uchar to uint
	/*printf("Translating\n");*/
	uint traData[MAX] = { 0 };
	trans(preData, traData, Mnlength);
	/*printf("Translate is over!!\n");*/

	uint ha[8];

	jiami(ha,traData, Mnlength);

	printf("SHA256=");
	
	for (int i = 0; i < 8; i++)
	{
		printf("%x",ha[i]);

	}

	printf("\n");

	system("pause");

	return 0;
}