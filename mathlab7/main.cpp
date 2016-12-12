#include "fparser.hh"

#include <iostream>
#include <string>

using namespace std;

int main(){
    double eps = 0.001;
    FunctionParser fp;
    FunctionParser fpd;
    string function;
    string derivative;
    cout << "Enter the function y(x): y=";
    cin >> function;
    cout << "Enter y'(x): y'=";
    cin >> derivative;
    fp.Parse(function, "x");
    fpd.Parse(derivative, "x");
    cout << "Enter x1: ";
    double variables[] = {0};
    cin >> variables[0];
    cout << "Enter epsilon: ";
    cin >> eps;

    double x, fx, fdx, prev;
	do{
        prev = x;
		fx = fp.Eval(variables);
		fdx = fpd.Eval(variables);
		x = variables[0] - fx / fdx;
		cout << "x: " << x << endl;
		variables[0] = x;
	} while (fabs(x-prev) > eps);

    cout << "\nResult: " << x << endl;

    return 0;
}
