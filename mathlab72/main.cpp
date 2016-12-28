#include <iostream>
#include <cmath>
using namespace std;

float f( float x)
{
    //return ((pow(x,3)+2*pow(x,2)+3*x+5));
    //return (tan(x));
    return (sin(x));
}
int main()
{
float a, b, fa, fb,an,bn, e, last, h, x;

last = 0;
cout <<"Enter a "<< endl;
cin>>a;
cout <<"Enter b "<< endl;
cin>>b;
cout << f(a) << " " << f(b) << endl;
while (f(a) * f(b) > 0)
{
    cout <<"error"<<endl;
    cout <<"Enter a "<< endl;
    cin>>a;
    cout <<"Enter b "<< endl;
    cin>>b;
}

cout <<"Enter E "<< endl;
cin>>e;

x = a;
float x0 = x + e;
float d1_a = (f(x) - f(x0))/(x - x0);
float Numerator_a = (f(x + e) - f(x0 + e))/((x + e) - (x0 + e)) - (f(x) - f(x0))/(x - x0);
float d2_a = Numerator_a/e;
//cout << "f(a) = " << f(a) << endl;
//cout << "f''(x) = " << d2_a << endl;

x = b;
x0 = x + e;
float d1_b = (f(x) - f(x0))/(x - x0);
float Numerator_b = (f(x + e) - f(x0 + e))/((x + e) - (x0 + e)) - (f(x) - f(x0))/(x - x0);
float d2_b = Numerator_b/e;
//cout << "f(b) = " << f(b) << endl;
//cout << "f''(a) = " << d2_b << endl;

if (f(a)*d2_a>0 & f(b)*d2_b<0)
{
    cout << "Neruhoma tochka a"<<endl;
do
{
    last = b;
    bn = b - (a-b)*f(b)/(f(a)-f(b));
    cout<<"x "<< bn<< endl;
    b=bn;
} while(abs(last-bn)>e);
}else {
    cout << "Neruhoma tochka b"<<endl;
do
{
    last = a;
    an = a - (b-a)*f(a)/(f(b)-f(a));
    cout<<"x "<< an<< endl;
    cout<<"f(x) "<<f(an)<<endl;
    a=an;
} while(abs(f(an))>e);
}
}

