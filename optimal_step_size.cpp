#include<stdio.h>
#include<math.h>
#include <windows.h>
#define f  (pow(x1,5) + pow(x2,4) - 5*pow(x1,4) + 30*pow(x2,3) + 25*pow(x1,3) + 10*pow(x2,2) + x1*x2 - 10*x1 - 14*x2 + 30)
#define dfx1 (5 * pow(x1,4) - 20 * pow(x1,3) + 75 * pow(x1,2) + x2 - 10)
#define dfx2 (4 * pow(x2,3) + 90 * pow(x2,2) + 20 * x2 + x1 - 14)
#define dfx1x1 (20 * pow(x1,3) - 60 * pow(x1,2) + 150 * x1)
#define dfx2x2 (12 * pow(x2,2) + 180 * x2 + 20)
#define dfx1x2 1.00
int main()
{
	double analysismethod();
	double golddivision(double a1, double a4);
	double quadratic(double a1, double a2, double a3, double f1, double f2, double f3);

	//-------------------进退法计算区间----------------------//
	
	double x, x01, x02,x11,x12, s, s1, s2;
	double a,a0 = 0.01, a1, a2, a3, f0, f1, f2, f3;
	double x1, x2;
	x01 = 3;
	x02 = 7;									//确定初始点
	s1 = 0.7;
	s2 = -0.714;							//确定搜索方向
	x1 = x01;
	x2 = x02;
	a2 = 0; a = a0; f2 = f;
	while (1)
	{
		a1 = a2 + a;
		x11 = x01 + a1 * s1;
		x12 = x02 + a1 * s2;
		x1 = x11;
		x2 = x12;
		f1 = f;
		if (f1 > f2)
		{
			if (a == a0)
			{
				a3 = a1;
				f3 = f1;
				a = -a;
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
		}
	}
	if (a1 > a3)
	{
		s = a1; f0 = f1;
		a1 = a3; f1 = f3;
		a3 = s; f3 = f0;
	}

	printf("进退法确定搜索区间：\na1 = %6f a2 = %6f a3 = %6f\nf1 = %6f f2 = %6f f3 = %6f\n",a1,a2,a3,f1,f2,f3);		//输出
	
	//-----------------------解析法确定最优步长-------------------//
	x = analysismethod();
	printf("解析法最优步长：    x = %6f \n", x);

	//---------------------黄金分割法确定最优步长-----------------//
	x = golddivision(a1, a3);
	printf("黄金分割法最优步长：x = %6f \n", x);

	//---------------------二次插值法确定最优步长-----------------//
	x = quadratic(a1, a2, a3, f1, f2, f3);
	printf("二次插值法最优步长：x = %6f \n", x);
}

double analysismethod()    //解析法函数
{
	double x1, x2, a, s1, s2;
	LARGE_INTEGER start;
	LARGE_INTEGER end;
	LARGE_INTEGER freq;

	QueryPerformanceFrequency(&freq);
	QueryPerformanceCounter(&start);
	x1 = 3;
	x2 = 7;
	s1 = 0.7;
	s2 = -0.714;
	a = -(s1 * dfx1 + s2 * dfx2) / (s1 * (s1 * dfx1x1 + s2 * dfx1x2) + s2 * (s1 * dfx1x2 + s2 * dfx2x2));
	QueryPerformanceCounter(&end);
	printf("\n运行时间为：%10fs\n", (double)(end.QuadPart - start.QuadPart) / (double)freq.QuadPart);
	return (a);
}

double golddivision(double a1,double a4)		//黄金分割
{
	double eps = 1e-6;
	double x1,x2;
	double a0, a2, a3, x0, x01, x02, x11, x12, x21, x22, x31, x32, f0, f1, f2, f3, s1, s2;
	int k = 0;
	LARGE_INTEGER start;
	LARGE_INTEGER end;
	LARGE_INTEGER freq;

	QueryPerformanceFrequency(&freq);
	QueryPerformanceCounter(&start);

	x01 = 3; x02 = 7;

	s1 = 0.7; s2 = -0.714;

	a2 = a1 + 0.382 * (a4 - a1);
	x21 = x01 + a2 * s1;
	x22 = x02 + a2 * s2;
	x1 = x21; x2 = x22;
	f2 = f;
	
	a3 = a1 + 0.618 * (a4 - a1);
	x31 = x01 + a3 * s1;
	x32 = x02 + a3 * s2;
	x1 = x31; x2 = x32;
	f3 = f;

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
		}
		else
		{
			a1 = a2; a2 = a3; f2 = f3;
			a3 = a1 + 0.618 * (a4 - a1);
			x31 = x01 + a3 * s1;
			x32 = x02 + a3 * s2;
			x1 = x31; x2 = x32;
			f3 = f;
		}
		k++;
	} while ((abs(a4 - a1))> eps);
	a0 = (a1 + a4) / 2;
	x11 = x01 + a0 * s1;
	x12 = x02 + a0 * s2;
	x1 = x11; x2 = x12;
	f0 = f;
	printf("\nGolddivision Step Count: %d\n", k);
	QueryPerformanceCounter(&end);
	printf("运行时间为：%10fs\n", (double)(end.QuadPart - start.QuadPart) / (double)freq.QuadPart);
	return (a0);
}

double quadratic(double a1, double a2, double a3, double f1, double f2, double f3)	//二次插值法
{
	double S1, S2, s01, s02, x01, x02, eps, x11, x12, astar, fstar, xstar1, xstar2;
	double x1, x2;	//用于替换宏定义中函数中的变量值//
	int k = 0;
	LARGE_INTEGER start;
	LARGE_INTEGER end;
	LARGE_INTEGER freq;

	QueryPerformanceFrequency(&freq);
	QueryPerformanceCounter(&start);
	s01 = 0.7; s02 = -0.714; x01 = 3; x02 = 7; eps = 1e-6;
	while (1)
	{
		S1 = (f3 - f1) / (a3 - a1);
		S2 = ((f2 - f1) / (a2 - a1) - S1) / (a2 - a3);
		
		//printf("%6f	%6f\n", S1,S2);
		if (S2 == 0)
		{
			f1 = f2;
			x11 = x01 + a2 * s01; 
			x12 = x02 + a2 * s02;
			break;
		}
		else
		{
			astar = (a1 + a3 - (S1 / S2)) / 2;
			if ((astar - a1) * (a3 - astar) <= 0)
			{
				f1 = f2;
				x11 = x01 + a2 * s01;
				x12 = x02 + a2 * s02;
				break;
			}
			else
			{
				xstar1 = x01 + astar * s01;
				xstar2 = x02 + astar * s02;
				x1 = xstar1; 
				x2 = xstar2;
				fstar = f;
				if ((abs(astar - a2)) <= eps)
				{
					if (fstar < f2)
					{
						f1 = fstar;
						x11 = xstar1;
						x12 = xstar2;
						break;
					}
					else
					{
						f1 = f2;
						x11 = x01 + a2 * s01;
						x12 = x02 + a2 * s02;
						break;
					}
				}
				else
				{
					if ((astar - a2) * (a3 - a1) < 0)
					{
						if (f2 < fstar)
						{
							a1 = astar;
							f1 = fstar;
						}
						else
						{
							a3 = a2;
							f3 = f2;
							a2 = astar;
							f2 = fstar;
						}
					}
					else
					{
						if (f2 < fstar)
						{
							a3 = astar;
							f3 = fstar;
						}
						else
						{
							a1 = a2;
							f1 = f2;
							a2 = astar;
							f2 = fstar;
						}
					}
				}
			}
		}
		k++;
	}
	//printf("%f	%f	%f	%f \n", a1, a2, a3, astar);
	printf("\nQuadratic Step Count: %d\n", k);
	QueryPerformanceCounter(&end);
	printf("运行时间为：%10fs\n", (double)(end.QuadPart - start.QuadPart) / (double)freq.QuadPart);
	return (astar);
}