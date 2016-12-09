#include <iostream>
using namespace std;

int main ()
{
    int n;
    double sum = 0;

    cout << "Enter matrix size\n n = ";
    cin >> n;

    double A [ n ][ n ];
    double L [ n ][ n ];
    double U [ n ][ n ];

//задаем матрицу A[][] ...

    for (int i  = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            cout << "\na[" << i << "][" << j << "] = ";
            cin >> A [i][j];

            L [i][j] = 0;
            U [i][j] = 0;

            if (i == j)
                U [i][j] = 1;
        }
    }

//==============================================

//находим первый столбец L[][] и первую строку U[][]

    for (int i = 0; i < n; i++)
    {
        L [i][0] = A [i][0];
        U [0][i] = A [0][i] / L [0][0];
    }

//дальше вычисляем L[][], U[][] по формуле

    for (int i = 1; i < n; i++)
    {
        for (int j = 1; j < n; j++)
        {
            if (i >= j) //нижний треугольник
            {
                sum = 0;
                for (int k = 0; k < j; k++)
                    sum += L [i][k] * U [k][j];

                L [i][j] = A [i][j] - sum;
            }
            else // верхний
            {
                sum = 0;
                for (int k = 0; k < i; k++)
                    sum += L [i][k] * U [k][j];

                U [i][j] = (A [i][j] - sum) / L [i][i];
            }
        }
    }

//====================================================
    cout << "\n\n";
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
            cout << "  " << L [i][j] << "   ";
        cout << "\n\n";
    }

    cout << "\n\n";

    for (int i  = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
            cout << "  " << U [i][j] << "   ";
        cout << "\n\n";
    }
//====================================================
    int i, j;
    double b[n];
    double y[n];
    double x[n];
    cout << "Enter b:\n";
    for(int z = 0; z < n; z++){
        cout << "b[" << z << "]: ";
        cin >> b[z];
    }
    y[0] = b[0] / L[0][0];

    for (i = 1; i < n; i++)
    {
        sum = 0;
        for (j = 0; j < i; j++)
            sum += L[i][j] * y[j];
        y[i] = (b[i] - sum) / L[i][i];
    }

    x[n - 1] = y[n - 1];
    for (i = n - 2; i >= 0; i--)
    {
        sum = 0;
        for (j = n - 1; j >= i + 1; j--)
            sum += x[j] * U[i][j];
        x[i] = y[i] - sum;
    }
    cout << endl;
    for(int z = 0; z<n; z++) cout << "x[" << z << "]: " << x[z] << endl;
    return 0;
}
