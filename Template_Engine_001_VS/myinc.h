#ifndef _MY_INC_
#define _MY_INC_
#include<Windows.h> //WIN API
#include<gl\GL.h>//GL LIB
#include<gl\glut.h>//GLU LIB
#include<math.h>//математика
#include<complex>//комплексные числа(для построения фрактала)
#include<functional>//такие классы, как plus и ptr_fun
#include<algorithm>//такие алгоритмы, как copy, fill_n
#include<string>//стандартный класс строк
#include<numeric>//такие алгоритмы, как accumulate
#include<vector>//класс вектора
#include<stdio.h>//файловый ввод/вывод в С
#include<ctime>
using namespace std;
//переопределение типов по краткости
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

#include "array2d.h"//работа с двухмерным массивом
#include "mtrans.h"//матрица преоброзований 
#include "glmain.h"
#include "winput.h"//ввод с устройств
#include "vertex.h"//вершина
#include "model.h"//класс моделей
#include "globj.h"//камера
#include "image.h"//загрузка/сохранение изображений
#include "fimage.h"//обработка ихоброжения
#include "fractset.h"//фрактальные алгоритмы
#endif