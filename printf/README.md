# printf() format experiment

```
nabeshin@iMacNabeshin printf % ./a.out 
RAND_MAX =2147483647
INT_MAX  =2147483647
UINT_MAX =4294967295
LONG_MAX =9223372036854775807
ULONG_MAX=18446744073709551615

int  p=0x7ff7bed0f598, a=0 
char p=0x7ff7bed0f594, c=abc size=4

sock p=0x7ff7bed0f590 
&sock+2   p=0x7ff7bed0f598 
&sock+133 p=0x7ff7bed0f7a4 
&sock+133 - &sock+2 =131 
sock_size=4, &sock_size=8 

name=0x7ff7bed0f590.txt
name=7ff7bed0f590.txt
FILE p=0x7ff8560eae00 
ptr_len=8 

nabeshin@iMacNabeshin printf % cat ./7ff7bed0f590.txt 
this is abc
nabeshin@iMacNabeshin printf % 
```