#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include<iostream>

using namespace std;

inline float q(float x, float x0, float h){ return (x - x0) / h; }

float** init(int size);
float** T(float** a, int size);
float** getDiffTable(float *a, int size);
long long int fact(int);
float* getExpTable(float, float, int);
float Q(float, float, float, int);
void showDiffTable(float, float, float** a, int size);
void approximate(float argument);


int main(void){
    float x;
	cout << "Enter the argument ";
	cin >> x;
	approximate(x);
	return 0;
}

void approximate(float x){
    bool flag;
    if(x < 0){
        flag = true;
        x *= (-1);
    }else{
        flag = false;
    }
    int size = 8;
	float h = 1;
	float first = 0.07;
	float *a = new float[size];
	a = getExpTable(first, h, size);
	float **y;
	y = getDiffTable(a, size);
	showDiffTable(first, h, y, size);
	int mid = ceil(size / 2);
	int x0 = first + h*mid;
	float P = y[mid][0];
	float temp1, temp2, temp3;
	for (int n = 1; n <= mid; n++)
	{
		temp1 = Q(x, x0, h, n) / fact(2 * n - 1);
		temp2 = (y[-n + mid][2 * n - 1] + y[-(n - 1) + mid][2 * n - 1]) / 2;
		temp3 = (Q(x, x0, h, n)*q(x, x0, h)) / fact(2 * n);
		P += (temp1)*(temp2)+(temp3)*(y[-n + mid][2 * n]);
	}
    if(!flag){
    cout << "Calculated by own function:  " << P << "\n";
	cout << "Real value:  " << 1/tan(x) << "\n";
	cout << "Absolute error:  " << fabs(P - 1/tan(x)) << "\n";
	cout << "Relative error:  " << fabs(P - 1/tan(x)) / P * 100 << "%\n";
    }else{
    cout << "Calculated by own function:  " << (-1)*P << "\n";
	cout << "Real value:  " << (-1)*1/tan(x) << "\n";
	cout << "Absolute error:  " << fabs(P - 1/tan(x)) << "\n";
	cout << "Relative error:  " << fabs(P - 1/tan(x)) / P * 100 << "%\n";
    }
}

long long int fact(int a)
{
	long long int r = 1;
	for (int i = 1; i <= a; i++) r *= i;
	return r;
}


float** init(int size)
{
	float** res;
	res = new float*[size];
	for (int i = 0; i<size; i++)
	{
		res[i] = new float[size - i];
	}
	return res;
}

float** T(float** a, int size)
{
	float** res;
	res = init(size);
	for (int i = 0; i<size; i++)
	{
		for (int j = 0; j<size - i; j++)
		{
			res[i][j] = a[j][i];
		}
	}
	return res;
}

float** getDiffTable(float *a, int size)
{
	float** res;
	res = init(size);
	res[0] = a;
	for (int i = 1; i<size; i++)
	{
		for (int j = 0; j<size - i; j++)
		{
			res[i][j] = res[i - 1][j + 1] - res[i - 1][j];
		}
	}
	return T(res, size);
}

float Q(float x, float x0, float h, int n)
{
	float res = q(x, x0, h);
	for (int i = 1; i<n; i++)
	{
		res = res*(q(x, x0, h)*q(x, x0, h) - i*i);
	}
	return res;
}

float* getExpTable(float first, float h, int n)
{
	float *res = new float[n];
	float x = first;
	for (int i = 0; i<n; i++)
	{
		res[i] = 1/tan(x);
		x += h;
	}
	return res;
}

void showDiffTable(float first, float h, float** a, int size)
{
	for (int i = 0; i<size; i++)
	{
		printf("%3.2f\t", first + h*i);
		for (int j = 0; j<size - i; j++)
		{
			printf("%3.2f\t", a[i][j]);
		}
		cout << "\n";
	}
}
