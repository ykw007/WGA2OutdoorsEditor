//## 2 ���������� üũ�ϴ� ��ũ��
//#define ISPOW2(x)  (x && !(x & (x-1)))

#define ISPOW2(n)  (!( n & (n-1)))

//#define ISPOW2(x)  (!((~(~0U >> 1) | x) & x - 1))

