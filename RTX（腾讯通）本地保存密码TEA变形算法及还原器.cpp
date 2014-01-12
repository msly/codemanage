// lankerr
// http://bbs.pediy.com/showthread.php?t=158505

//TEA test program written by HappyTown   [2006-10-10]
#include <stdio.h>
#include <stdlib.h>
#include "winsock2.h"

#pragma comment(lib,"WS2_32.LIB")
#include "tea.h"
//变形TEADec
void myDecrypt(unsigned char *data,unsigned char *key,unsigned char *out);
//变形TEAEnc
void myEncrypt(unsigned char *data,unsigned char *key,unsigned char *out);
//Ecnrypt
void myEncrypt(unsigned char *data,unsigned char *key,unsigned char *out)
{
  int i;
  unsigned int y=0,z=0,a,b,c,d;
  int  e = 0;
  unsigned int sum = 0x61C88647;

  //设置y和z
//  printf("%08X\n",*(DWORD*)data);
//  printf("%08X\n",*(DWORD*)(data+4));
  y =  ntohl(*(DWORD*)data);
  z =  ntohl(*(DWORD*)(data+4));
//  printf("y=%08X z=%08X\n",y,z);
  //变形key设置a,b,c,d值
  a = ntohl(*(DWORD*)(key+0));
  b = ntohl(*(DWORD*)(key+4));
  c = ntohl(*(DWORD*)(key+8));
  d = ntohl(*(DWORD*)(key+12));

//  printf("a=%08X,b=%08X,c=%08X,d=%08X\n",a,b,c,d);
//  printf("%08X %08X %08X\n",(c+(y<<4)),(d+(y>>5)),(c+(y<<4))^(d+(y>>5)));
//  printf("%08X %08X %08X\n",(delta+y),(delta+y) ^ (c+(y<<4))^(d+(y>>5)),z-((delta+y) ^ (c+(y<<4))^(d+(y>>5))));
  //Decrypt
  for(i=0; i<16; i++)
  {
    e -= sum;
    y += (e+z) ^ (a+(z<<4)) ^ (b+(z>>5));
    z += (e+y) ^ (c+(y<<4)) ^ (d+(y>>5));
  }
//  printf("y=%08X z=%08X \n",y,z);

  //output y
  *(DWORD*)out =  ntohl(y);
  //output z
  *(DWORD*)(out+4) = ntohl(z);
  return;
}
//Decrypt
void myDecrypt(unsigned char *data,unsigned char *key,unsigned char *out)
{
  int i;
  unsigned int y=0,z=0,a,b,c,d;
  int  e = 0;
  unsigned int sum = 0x61C88647;
  unsigned delta = 0xE3779B90;

  //设置y和z
//  printf("%08X\n",*(DWORD*)data);
//  printf("%08X\n",*(DWORD*)(data+4));
  y =  ntohl(*(DWORD*)data);
  z =  ntohl(*(DWORD*)(data+4));
//  printf("y=%08X\n",y);
//  printf("z=%08X\n",z);
  
//  printf("a=%08X,b=%08X,c=%08X,d=%08X\n",a,b,c,d);
  //变形key设置a,b,c,d值
  a = ntohl(*(DWORD*)(key));
  b = ntohl(*(DWORD*)(key+4));
  c = ntohl(*(DWORD*)(key+8));
  d = ntohl(*(DWORD*)(key+12));
//  printf("%08X %08X %08X\n",(c+(y<<4)),(d+(y>>5)),(c+(y<<4))^(d+(y>>5)));
//  printf("%08X %08X %08X\n",(delta+y),(delta+y) ^ (c+(y<<4))^(d+(y>>5)),z-((delta+y) ^ (c+(y<<4))^(d+(y>>5))));
  //Decrypt
  for(i=0; i<16; i++)
  {
    z -= (delta+y) ^ (c+(y<<4)) ^ (d+(y>>5));
    e = (delta+z) ^ (a+(z<<4)) ^ (b+(z>>5));
    delta += sum;
    y -= e;
  }
//  printf("y=%08X z=%08X \n",y,z);

  //output y
  *(DWORD*)out =  ntohl(y);
  //output z
  *(DWORD*)(out+4) = ntohl(z);
  return;
}
int main()
{
  int i;
  int runnum = 0;
  int keylen = 40;//暂定密文长40
  unsigned char k[16] = {0x52,0x00,0x54,0x00,0x58,0x00,0x21,0x00,0x33,0x00,0x00,0x00,0x00,0x00,0x00,0x00};//128bits key
  //变形密钥
  unsigned char k2[16] = {0x00, 0x54, 0x00, 0x52, 0x00, 0x21, 0x00, 0x58, 0x00, 0x00, 
0x00, 0x33, 0x00, 0x00, 0x00, 0x00};//128bits key
  unsigned char data[128] = {0x95, 0x8D, 0x23, 0x06, 0x74, 0xBB, 0x15, 0xDA, 0xC2, 0x6B, 
0x0E, 0xFF, 0xE7, 0x0F, 0x6D, 0xE6, 0x88, 0x26, 0x91, 0x1F, 
0xBE, 0x68, 0xBE, 0xF0, 0x3E, 0x24, 0x65, 0xBB, 0x53, 0xF0, 
0x89, 0x8D, 0xB3, 0xBE, 0xE2, 0xAC, 0xC1, 0x81, 0xBA, 0x17, 
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};//plaintext
  unsigned char outEn[128] = {0};  //cipher
  unsigned char temp[128] = {0};
  unsigned char testdata[]={0xD2,0x90,0x2d,0x48,0xd3,0x73,0x00,0x68};

  //以十六进制输出密钥k
  printf("Key is:");
  for (i=0; i<16; i++)
  {
    printf("%02X", k[i]);
  }
  //以十六进制输出密钥k
  printf("\ndata is:");
  for (i=0; i<8; i++)
  {
    printf("%02X", data[i]);
  }
  printf("\n数据解密为:\n");
/*
  //加密明文data，并输出密文
  TEA_Encrypt(outEn, data, k);
  printf("TEA(");
  for (i=0; i<16;i++)
  {
    printf("%02X ", data[i]);
  }
  printf(") is:");
  for (i=0; i<8; i++)
  {
    printf("%02X", outEn[i]);
  }

  printf("\n");
*/
  
  //解密上一步的密文outEn，并输出明文
  myDecrypt(data, k,outEn);//第一次解密
  for (i=0; i<8;i++)
  {
    printf("%02X ", outEn[i]);
  }//加密
  //开始加密
  printf("\n数据加密为:\n");
  myEncrypt(outEn,k,outEn);
  for (i=0; i<8;i++)
  {
    printf("%02X ", outEn[i]);
  }
  //再解密
  printf("\n数据再解密为:\n");
  myDecrypt(outEn,k,outEn);
  for (i=0; i<8;i++)
  {
    printf("%02X ", outEn[i]);
  }
  if(!(outEn[0] & 7))
    return 0;
//  runnum = (40-(outEn[0] & 7) -10);
//  printf("次数:%d %d",runnum,(outEn[0] & 7));
  printf("\n\n");
  system("pause");
  return 0;
}
RTX对本地代码用了随机数填充，多次加密，造成相同密码，密文不一致(密钥用的是“RTX!3”)。另外处理步骤过于复杂，还原密码就直接调用了RTX的Crypt.Dll，还原和加密代码如下(好像和QQ有些算法一致)，加解密代码用到了上面的变形算法：
//自定义TEA解密
typedef BOOL (*oi_symmetry_decrypt2) (char *mw,int Len,char *key,char *ret,int *num);
oi_symmetry_decrypt2 mydecrypt2;

//自定义TEA加密
//data 是宽字符
//key 是宽字符
//ret 应该是宽字符，长64位
//num 指向长64位的宽字符
typedef BOOL (*oi_symmetry_encrypt2) (wchar_t *data,int Len,char *key,unsigned char *ret,int *num);
oi_symmetry_encrypt2 myencrypt2;

//查找函数地址
  mydecrypt2 = (oi_symmetry_decrypt2)::GetProcAddress(::LoadLibrary("Crypt.dll"), "oi_symmetry_decrypt2");
  myencrypt2 = (oi_symmetry_encrypt2)::GetProcAddress(::LoadLibrary("Crypt.dll"), "oi_symmetry_encrypt2");
还原密码：
void CRtxPwDlg::OnRead() 
{
  // TODO: Add your control notification handler code here
  char key[]={0x52, 0x00, 0x54, 0x00, 0x58, 0x00, 0x21, 0x00, 0x33, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  char ret[1024]={0};
  char buff[1024]={0};
  char filekey[]="strPassword";
  int keylen = 0;
  int num = 1024;

  ::GetPrivateProfileStruct("Default","strPassword",buff,
    64,
    "D:\\rtx.cfg");//注意rtx.cfg在RTX的安装目录下，这里是本机未安装RTX将CFG文件COPY到D盘测试用
  if (2 * wcslen((unsigned short*)buff) >= 64 )
      keylen = 64;
    else
      keylen = 2*wcslen((unsigned short*)buff);

  CString msg;

  msg.Empty();
  //解密
  mydecrypt2(buff,keylen,key,ret,&num);

  for(int j=0;j<64;j++)
  {
    CString temp;
    if(ret[j]!=0x00)
    {
      temp.Format("%c",ret[j]);
      msg+=temp;      
    }
  }
  AfxMessageBox(msg);
}
//加密密码
void CRtxPwDlg::OnWrite() 
{
  // TODO: Add your control notification handler code here
  char key[]={0x52, 0x00, 0x54, 0x00, 0x58, 0x00, 0x21, 0x00, 0x33, 0x00, 
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  char buff[64] = "123456";
  unsigned char ret[64] = {0};
  int temp = 0;


//  AfxMessageBox(msg);

  //第四个参数为-1时,取得转换需要的大小
  int nLen = MultiByteToWideChar(CP_ACP, 0,buff, -1, NULL, NULL);
  //分配空间
  wchar_t *pwText;
  pwText = new wchar_t[nLen];
  if(!pwText)
    delete[]pwText;
  //转换
  MultiByteToWideChar(CP_ACP,0,buff,-1,pwText,nLen);
  
  CString msg,ls;
  m_Show.Empty();
//  for(int k=1;k<nLen*2;k++)
  {
  myencrypt2(pwText,nLen*2-2,key,ret,&temp);
  msg.Empty();
  for(int i=0;i<64;i++)
  {
    ls.Format("%02X",ret[i]);
    m_Show += ls;
  }
//  AfxMessageBox(m_Show);
  }
  delete[]pwText;
  UpdateData(FALSE);
} 


//---------------------------------------------------
TEA和QQTEA 
http://mathswork.blog.163.com/blog/static/1400170282010102264755630/

TEA算法由剑桥大学计算机实验室的David Wheeler和Roger Needham于1994年发明[3]。它是一种分组密码算法，其明文密文块为64比特，密钥长度为128比特。TEA算法利用不断增加的Delta(黄金分割率)值作为变化，使得每轮的加密是不同，该加密算法的迭代次数可以改变，建议的迭代次数为32轮。

代码如下：

void qq_encipher(unsigned long *const plain, const unsigned long *const key, unsigned long *const crypt)

//参数为8字节的明文输入和16字节的密钥，输出8字节密文

{

unsigned long  left  =  plain[0],right  = plain[1],

a  =  key[0], b  =  key[1],

c  =  key[2], d  =  key[3],

n  =  32,   sum = 0,

delta = 0x9E3779B9; 

// 明文输入被分为左右两部分，密钥分为四部分存入寄存器，n表示加密轮数推荐32。Delta为一常数。   

   while (n-- > 0) {

   sum += delta;

   left += ((right << 4) + a) ^ (right + sum) ^ ((right >> 5) + b);

   right += ((left << 4) + c) ^ (left + sum) ^ ((left >> 5) + d);

   }     

   crypt[0] =  left ;

   crypt[1] =  right ;

}        

虽然TEA算法比 DES(Data Encryption Standard) 要简单得多， 但有很强的抗差分分析能力，加密速度也比 DES 快得多，而且对 64 位数据加密的密钥长达 128 位，安全性相当好。其可靠性是通过加密轮数而不是算法的复杂度来保证的。从中可以看到TEA 算法主要运用了移位和异或运算。密钥在加密过程中始终不变。

QQTEA 算法建立在标准 TEA算法的基础上，使用16轮的加密（这是最低限，推荐应该是32轮）。QQ在使用这个算法的时候，由于需要加密不定长的数据，所以使用了一些常规的填充办法和交织算法。


      1 填充算法
如果明文本身的长度不是8的倍数，那么还要进行填充以使其成为8的倍数。以字节为单位，令N=原始字符串+10+填充字节数n，则N应该是8的倍数。

具体的填充方法：第一个字节为：(random()&0xf8)|n，随后填充(n+2)个字节random()&0xff ,后面接原始数据，最后填充7 个字节0x00 。

因为使用了不同的随机数，所以填充的结果使得即使对于相同的明文，密文的结果也会不同。


       2 交织算法
消息被分为多个加密单元，每一个加密单元都是8字节，使用TEA进行加密，加密结果与下一个加密单元做异或运算后再作为待加密的明文。具体加密过程如图所示：

 

3 示例数据

试验数据如下：

随机数：Random（）=0xad；

明文：00 01 01 00 00 00 07 00 00 00 00 3A FB 95 E7 ；

密钥：ef 62 36 e0 60 96 3c 24 d2 c8 53 1b 7c 5e 17 7f；

密文：D7 81 D2 03 79 F2 79 E0 4D 6D B2 B8 C2 B4 5C A2 76 C2 CF E5 99 D6 B5 CE 88 14 55 5A E5 CC 0C 4B

填充过程：

1、计算n，由于明文是15个字节，则（15+10+7）%8==0，所以n=7；

2、计算第一个字节：(0xad&0xf8)|0x07=0xaf;

3、随后的7+2个字节：ad ad ad ad ad ad ad ad ad

4、原始数据：00 01 01 00 00 00 07 00 00 00 00 3A FB 95 E7

5、最后7个字节的填充：00 00 00 00 00 00 00

填充后的数据为：

af ad ad ad ad ad ad ad ad ad 00 01 01 00 00 00

07 00 00 00 00 3A FB 95 E7 00 00 00 00 00 00 00

 

加密过程：

数据同上，可以看出填充后的数据将被分为4组，命名为P1 ,P2 ,P3 ,P4 ，加密结果按8字节组分别为 C1，C2, C3, C4 ，则加密过程如下：

P1= af ad ad ad ad ad ad ad；

P2= ad ad 00 01 01 00 00 00;

P3=  07 00 00 00 00 3A FB 95;

P4= E7 00 00 00 00 00 00 00;

Sessionkey= ef 62 36 e0 60 96 3c 24 d2 c8 53 1b 7c 5e 17 7f;

实现步骤如下：

C1  = E(SessionKey)(P1) = D7 81 D2 03 79 F2 79 E0；

 

P2’= P2 Xor C1=7A 2C D2 02 78 F2 79 E0;

C2’= E(SessionKey)( P2’)= E2 C0 1F 15 6F 19 F1 0F;

C2 = C2’Xor P1=4D 6D B2 B8 C2 B4 5C A2；       

 

P3’= P3 Xor C2=4A 6D B2 B8 C2 8E A7 37;

C3’= E(SessionKey)( P3’)= 0C EE 1D E7 E1 24 CC 2E;

C3 = C3’Xor P2’=76 C2 CF E5 99 D6 B5 CE；

 

P4’= P4 Xor C3=91 C2 CF E5 99 D6 B5 CE;

C4’= E(SessionKey)( P4’)= C2 79 E7 E2 27 42 AB 7C;

C4 = C4’Xor P3’=88 14 55 5A E5 CC 0C 4B；

经过以上计算，将C1, C2, C3, C4这四个数据串联起来后即是最终的加密结果。

大家还可以用以下数据进行验证

密钥：ae 27 0f 52 c9 14 d0 b5 e7 21 b6 1c a8 3b 8a 7c

明文（15个0）：00 00 00 00 00 00 00 00 00 00 00 00 00 00 00

结果：ee 07 4a 41 99 c0 16 f3 1a ba 55 51 0c ef c1 29 66 70 9d 8b e9 7a 22 51 f0 f0 45 4c 50 8a 51 df 7d 08 6d 6f a0 7f 5a b9 99 5c 6d 34 cb be db bc

 

作业：

1、根据资料写出QQTEA的解密程序

2、分析QQ登录过程的pcap包，分析数据包的格式特点，并尝试解密。