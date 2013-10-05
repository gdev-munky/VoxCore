#ifndef _MY_INC_
#define _MY_INC_
#include<Windows.h> //WIN API
#include<gl\GL.h>//GL LIB
#include<gl\glut.h>//GLU LIB
#include<math.h>//����������
#include<complex>//����������� �����(��� ���������� ��������)
#include<functional>//����� ������, ��� plus � ptr_fun
#include<algorithm>//����� ���������, ��� copy, fill_n
#include<string>//����������� ����� �����
#include<numeric>//����� ���������, ��� accumulate
#include<vector>//����� �������
#include<stdio.h>//�������� ����/����� � �
#include<ctime>
using namespace std;
//��������������� ����� �� ���������
typedef unsigned int uint;
typedef unsigned char uchar;
typedef signed short sshort;
typedef unsigned short ushort;
typedef unsigned long ulong;
typedef complex<float> complexf;
template<class T, class T1, class T2>
inline void minmax(T&src, T1 mi, T2 ma)
 {
	 src=(src<mi)?mi:((src>ma)?ma:src);
 }

//#define NULL nullptr
//#define null nullptr

#include "array2d.h"//������ � ���������� ��������
#include "mtrans.h"//������� �������������� 
#include "glmain.h"
#include "winput.h"//���� � ���������
#include "vertex.h"//�������
#include "model.h"//����� �������
#include "globj.h"//������
#include "image.h"//��������/���������� �����������
#include "fimage.h"//��������� �����������
#include "fractset.h"//����������� ���������
#endif