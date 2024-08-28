#include<stdio.h>
#include<math.h>
#include<windows.h>
//#define f  (pow(x1,2) + pow(x2,2) - x1 * x2 - 10 * x1 - 4 * x2 + 60)	//目标函数1宏定义，初始点（0，0），计算稳定，与初始点选取关系不大
//#define dfx1 (2 * x1 - x2 - 10)
//#define dfx2 (2 * x2 - x1 - 4)
#define f ((pow(2.718281828,x1)) * (4 * pow(x1,2) + 2 * pow(x2,2) + 4 * x1 * x2 + 2 * x2 + 1))	//目标函数2宏定义，初始点（-1,1），算不出来，可能因为e的指数缘故.(0,0)点可以算出最小值0！！
#define dfx1 ((pow(2.718281828,x1)) * (4 * pow(x1,2) + 2 * pow(x2,2) + 4 * x1 * x2 + 8 * x1 + 6 * x2 + 1))
#define dfx2 ((pow(2.718281828,x1)) * (4 * x2 + 4 * x1 + 2))
int main()		//主函数
{
	double *fb(double x01, double x02, double s1, double s2);		//调用函数声明
	double *golddivision(double a1, double a4, double x01, double x02, double s1, double s2);
	double quadratic(double a1, double a2, double a3, double f1, double f2, double f3, double x01, double x02, double s01, double s02);
	double rot(double x01, double x02);
	double powell(double x01, double x02);
	double dfp(double x01, double x02);

	double a1, a2, a3, f1, f2, f3;	//进退法给出的步长区间和对应函数值
	double x;						 //输出值
	double *p0;						//替换用指针
	double x01, x02;				//初始迭代点
	double s11, s12, s21, s22;		//坐标轮换法搜索方向（固定不变）
	
/*	x01 = 0, x02 = 0;*/							//目标函数1初始迭代点初始化
	x01 = -1, x02 = 1;							//目标函数2初始迭代点初始化

	x = rot(x01,x02);
	x = powell(x01, x02);
	x = dfp(x01,x02);
	return 0;
}
double *fb(double x01,double x02,double s1,double s2)			//进退法函数
{
	double out[7];
	double x, x11, x12, s;
	double a, a0 = 0.1, a1, a2, a3, f0, f1, f2, f3;
	double x1, x2,K;
	double *p;
	int j;
	K = 0;
	j = 0;
	x1 = x01;
	x2 = x02;
	a2 = 0; a = a0; f2 = f;
	j++;
	while (1)
	{
		a1 = a2 + a;
		x11 = x01 + a1 * s1;
		x12 = x02 + a1 * s2;
		x1 = x11;
		x2 = x12;
		f1 = f;
		j++;
		if (f1 > f2)
		{
			if (a == a0)
			{
				a3 = a1;
				f3 = f1;
				a = -a;
				x01 = x11;
				x02 = x12;
			}
			else
			{
				break;
			}
		}
		else
		{
			a3 = a2; f3 = f2;
			a2 = a1; f2 = f1;
			a = 2 * a;
			x01 = x11;
			x02 = x12;
		}
		
		K = K + 1;
	}

	if (a1 > a3)
	{
		s = a1; f0 = f1;
		a1 = a3; f1 = f3;
		a3 = s; f3 = f0;
	}
	out[0] = a1;
	out[1] = a2;
	out[2] = a3;
	out[3] = f1;
	out[4] = f2;
	out[5] = f3;
	out[6] = j;
	p = out;
	return p;
}
double *golddivision(double a1, double a4,double x01,double x02,double s1,double s2)		//黄金分割
{
	double eps = 1e-6;
	double x1, x2,out[2];
	double a0, a2, a3, x0 , x11, x12, x21, x22, x31, x32, f0, f1, f2, f3, *p, j;
	int k = 0;
	LARGE_INTEGER start;
	LARGE_INTEGER end;
	LARGE_INTEGER freq;

	QueryPerformanceFrequency(&freq);
	QueryPerformanceCounter(&start);

	j = 0;
	a2 = a1 + 0.382 * (a4 - a1);
	x21 = x01 + a2 * s1;
	x22 = x02 + a2 * s2;
	x1 = x21; x2 = x22;
	f2 = f;
	j++;

	a3 = a1 + 0.618 * (a4 - a1);
	x31 = x01 + a3 * s1;
	x32 = x02 + a3 * s2;
	x1 = x31; x2 = x32;
	f3 = f;
	j++;

	do
	{
		if (f2<f3)
		{
			a4 = a3; a3 = a2; f3 = f2;
			a2 = a1 + 0.382 * (a4 - a1);
			x21 = x01 + a2 * s1;
			x22 = x02 + a2 * s2;
			x1 = x21; x2 = x22;
			f2 = f;
			j++;
		}
		else
		{
			a1 = a2; a2 = a3; f2 = f3;
			a3 = a1 + 0.618 * (a4 - a1);
			x31 = x01 + a3 * s1;
			x32 = x02 + a3 * s2;
			x1 = x31; x2 = x32;
			f3 = f;
			j++;
		}
		k++;
		//printf("%f	huang",a1);//test point
	} while ((abs(a4 - a1))> eps);
	a0 = (a1 + a4) / 2;
	x11 = x01 + a0 * s1;
	x12 = x02 + a0 * s2;
	x1 = x11; x2 = x12;
	f0 = f;
	j++;
	out[0] = a0;
	out[1] = j;
	p = out;
	//printf("\nGolddivision Step Count: %d\n", k);
	QueryPerformanceCounter(&end);
	return (p);
}
double dfp(double x01,double x02)			//DFP法函数
{
	double *fb(double x01, double x02, double s1, double s2);
	double *golddivision(double a1, double a4, double x01, double x02, double s1, double s2);//调用的一维搜索函数

	double x11, x12, H11, H12, H21, H22, B11, B12, B21, B22, C11, C12, C21, C22, s01, s02, eps = 1e-6;
	double *p0, *p1;
	double a0, a1, a2, a3, f1, f2, f3, x1, x2;
	double NF01, NF02,NF11,NF12, Dg1, Dg2, Dx1, Dx2, j;
	int K;
	K = 1;
	j = 0;
	H11 = 1.0, H12 = 0.0, H21 = 0.0, H22 = 1.0;
	x1 = x01;
	x2 = x02;
	NF01 = dfx1;
	NF02 = dfx2;
	s01 = -NF01;
	s02 = -NF02;
	while (1)
	{
		p0 = fb(x01, x02, s01, s02);
		a1 = *p0;
		a2 = *(p0 + 1);
		a3 = *(p0 + 2);
		f1 = *(p0 + 3);
		f2 = *(p0 + 4);
		f3 = *(p0 + 5);		
		j = j + *(p0 + 6);//输出进退法给出的六个值
		p1 = golddivision(a1, a3, x01, x02, s01, s02);
		a0 = *p1;
		j = j + *(p1 + 1);
		
		x11 = x01 + a0 * s01;
		x12 = x02 + a0 * s02;
		x1 = x11;
		x2 = x12;
		NF11 = dfx1;
		NF12 = dfx2;
		if (sqrt(pow(NF11, 2) + pow(NF12, 2)) <= eps)
			break;
		if (K == 1000)
			break;
		Dx1 = x11 - x01;
		Dx2 = x12 - x02;
		Dg1 = NF11 - NF01;
		Dg2 = NF12 - NF02;
		B11 = (Dx1 * Dx1) / (Dx1 * Dg1 + Dx2 * Dg2);
		B12 = (Dx1 * Dx2) / (Dx1 * Dg1 + Dx2 * Dg2);
		B21 = (Dx1 * Dx2) / (Dx1 * Dg1 + Dx2 * Dg2);
		B22 = (Dx2 * Dx2) / (Dx1 * Dg1 + Dx2 * Dg2);
		C11 = (H11 * H11 * Dg1 * Dg1 + H11 * H12 * Dg1 * Dg2 + H11 * H21 * Dg1 * Dg2 + H12 * H21 * Dg2 * Dg2) / (Dg1 * Dg1 * H11 + Dg1 * Dg2 * H21 + Dg1 * Dg2 * H12 + Dg2 * Dg2 * H22);
		C12 = (H11 * H12 * Dg1 * Dg1 + H12 * H12 * Dg1 * Dg2 + H11 * H22 * Dg1 * Dg2 + H12 * H22 * Dg2 * Dg2) / (Dg1 * Dg1 * H11 + Dg1 * Dg2 * H21 + Dg1 * Dg2 * H12 + Dg2 * Dg2 * H22);
		C21 = (H11 * H21 * Dg1 * Dg1 + H11 * H22 * Dg1 * Dg2 + H21 * H21 * Dg1 * Dg2 + H21 * H22 * Dg2 * Dg2) / (Dg1 * Dg1 * H11 + Dg1 * Dg2 * H21 + Dg1 * Dg2 * H12 + Dg2 * Dg2 * H22);
		C22 = (H12 * H21 * Dg1 * Dg1 + H12 * H22 * Dg1 * Dg2 + H22 * H21 * Dg1 * Dg2 + H22 * H22 * Dg2 * Dg2) / (Dg1 * Dg1 * H11 + Dg1 * Dg2 * H21 + Dg1 * Dg2 * H12 + Dg2 * Dg2 * H22);
		H11 = H11 + B11 - C11;
		H12 = H12 + B12 - C12;
		H21 = H21 + B21 - C21;
		H22 = H22 + B22 - C22;
		s01 = -(H11 * NF11 + H12 * NF12);
		s02 = -(H21 * NF11 + H22 * NF12);
		x01 = x11;
		x02 = x12;
		NF01 = NF11;
		NF02 = NF12;
		K = K + 1;
	}
	printf("DFP方法给出的最优值点及最优值为：\nx1 = %f	x2 = %f	f = %f\n", x11, x12, f);
	printf("迭代次数为：%d\n", K);
	printf("调用原函数次数为：%d\n\n",(int)j);
	return 0;
}
double rot(double x01,double x02)			//坐标轮换法函数
{
	double *fb(double x01, double x02, double s1, double s2);
	double *golddivision(double a1, double a4, double x01, double x02, double s1, double s2);//调用的一维搜索函数

	double s11 = 0.0, s12 = 1.0, s21 = 1.0, s22 = 0.0;	//搜索方向初始化
	double x11, x12, x010, x020;
	double *p0, *p1;
	double a0 ,a1, a2, a3, f1, f2, f3;
	double x1, x2, j;
	int K;

	K = 0;
	j = 0;
	while (1)
	{
		x010 = x01;
		x020 = x02;
		p0 = fb(x01, x02, s11, s12);
		a1 = *p0;
		a2 = *(p0 + 1);
		a3 = *(p0 + 2);
		f1 = *(p0 + 3);
		f2 = *(p0 + 4);
		f3 = *(p0 + 5);	
		j = j + *(p0 + 6);//输出进退法给出的六个值
		p1 = golddivision(a1,a3,x01,x02,s11,s12);
		a0 = *p1;
		j = j + *(p1 + 1);
		x12 = x02 + a0 * s12;

		x02 = x12;
		
		p0 = fb(x01, x02, s21, s22);
		a1 = *p0;
		a2 = *(p0 + 1);
		a3 = *(p0 + 2);
		f1 = *(p0 + 3);
		f2 = *(p0 + 4);
		f3 = *(p0 + 5);
		j = j + *(p0 + 6);//输出进退法给出的六个值
		p1 = golddivision(a1,a3,x01,x02,s21,s22);
		a0 = *p1;
		j = j + *(p1 + 1);
		x11 = x01 + a0 * s21;

		x01 = x11;

		K = K + 1;
		if (sqrt((x11 - x010) * (x11 - x010) + (x12 - x020) * (x12 - x020)) <= 1e-6)
			break;
	}
	x1 = x01;
	x2 = x02;
	
	printf("坐标轮换法给出的最优值点及最优值为：\nx1 = %f	x2 = %f	f = %f\n",x01,x02,f);
	printf("迭代次数为：%d\n",K);
	printf("原函数调用次数为：%d\n\n",(int)j);
	return 0;
}
double powell(double x01,double x02)			//共轭方向（Powell）法函数
{
	double *fb(double x01, double x02, double s1, double s2);
	double *golddivision(double a1, double a4, double x01, double x02, double s1, double s2);//调用的一维搜索函数

	double x11, x12, x21, x22, x31, x32;
	double a0, a1, a2, a3, f1, f2, f3;	//代换用变量，不作为基本变量
	double *p0;
	double eps1 = 1e-6, eps2 = 1e-6;
	double s11, s12, s21, s22;	//搜索方向初始化
	double s31, s32;
	double x1, x2, j, *p1;
	double delta1, delta2, max;
	double F1, F2, F3, F4,F01,F02;
	double x010, x020;
	int K;

	s11 = 0.0, s12 = 1.0, s21 = 1.0, s22 = 0.0;
	K = 0;
	j = 0;
	while (1)
	{
		x010 = x01;
		x020 = x02;
		p0 = fb(x01, x02, s11, s12);	//一维搜索第一步
		a1 = *p0;
		a2 = *(p0 + 1);
		a3 = *(p0 + 2);
		f1 = *(p0 + 3);
		f2 = *(p0 + 4);
		f3 = *(p0 + 5);	
		j = j + *(p0 + 6);//输出进退法给出的六个值
		p1 = golddivision(a1,a3,x01,x02,s11,s12);
		a0 = *p1;
		j = j + *(p1 + 1);
		x11 = x01 + a0 * s11;
		x12 = x02 + a0 * s12;

		p0 = fb(x11, x12, s21, s22);	//一维搜索第二步
		a1 = *p0;
		a2 = *(p0 + 1);
		a3 = *(p0 + 2);
		f1 = *(p0 + 3);
		f2 = *(p0 + 4);
		f3 = *(p0 + 5);		
		j = j + *(p0 + 6);//输出进退法给出的六个值
		p1 = golddivision(a1, a3, x11, x12, s21, s22);
		a0 = *p1;
		j = j + *(p1 + 1);
		x21 = x11 + a0 * s21;
		x22 = x12 + a0 * s22;

		s31 = x21 - x01;
		s32 = x22 - x02;
		x31 = 2 * x21 - x01;
		x32 = 2 * x22 - x02;

		x1 = x01;
		x2 = x02;
		F1 = f;
		j++;
		x1 = x11;
		x2 = x12;
		F2 = f;
		j++;
		x1 = x21;
		x2 = x22;
		F3 = f;
		j++;
		x1 = x31;
		x2 = x32;
		F4 = f;
		j++;

		delta1 = F1 - F2;
		delta2 = F2 - F3;
		if (delta1 > delta2)
			max = delta1;
		else
			max = delta2;

		if ((F4 < F1) && (((F1 + F4 - 2 * F3) * (F1 - F3 - max) * (F1 - F3 - max))<(0.5 * max * (F1 - F4) * (F1 - F4))))
		{
			p0 = fb(x21, x22, s31, s32);	//一维搜索
			a1 = *p0;
			a2 = *(p0 + 1);
			a3 = *(p0 + 2);
			f1 = *(p0 + 3);
			f2 = *(p0 + 4);
			f3 = *(p0 + 5);		
			j = j + *(p0 + 6);//输出进退法给出的六个值
			p1 = golddivision(a1, a3, x21, x22, s31, s32);
			a0 = *p1;
			j = j + *(p1 + 1);
			x01 = x21 + a0 * s31;
			x02 = x22 + a0 * s32;

			if (delta1 > delta2)
			{
				s11 = s21;
				s12 = s22;
				s21 = s31;
				s22 = s32;
			}
			else
			{
				s21 = s31;
				s22 = s32;
			}
		}
		else
		{
			if (F3 < F4)
			{
				x01 = x21;
				x02 = x22;
			}
			else
			{
				x01 = x31;
				x02 = x32;
			}

		}
		x1 = x01;
		x2 = x02;
		F01 = f;
		j++;
		x1 = x010;
		x2 = x020;
		F02 = f;
		j++;
		K = K + 1;
		//printf("%f	%f",x01,x02);//test
		if ((sqrt((x01 - x010) * (x01 - x010) + (x02 - x020) * (x02 - x020)) <= eps1) || (abs((F01 - F02) / F02) <= eps2))
			break;
		if (K > 1e5)
			break;
	}
	printf("Powell法给出的最优值点及最优值为：\nx1 = %f	x2 = %f	f = %f\n", x01, x02, f);
	printf("迭代次数为：%d\n",K);
	printf("原函数调用次数为：%d\n\n",(int)j);
	return 0;
}