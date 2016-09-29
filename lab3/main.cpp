#include <iostream>
#include "classes.h"

using namespace std;

int main(){
    cout << "Creating deriative classes." << endl;
    ComplexPoint deriative1(4, 5, 1, 3, "FOO");
    ComplexPoint deriative2(3, 1, 4, 5, "FBB");
    FractionPoint deriative3(7, 6, 2, 3, "BAR");
    FractionPoint deriative4(3, 2, 7, 6, "B@R");

    cout << "\nUsing new cout stuff:" << endl;
    cout << deriative1;
    cout << deriative2;
    cout << deriative3;
    cout << deriative4;

    cout << "\nComparing fractions: " << endl;
    cout << deriative3.getName() << " Is ";
    if(deriative3<deriative4) cout << "Less than ";
    else cout << "Bigger than ";
    cout << deriative4.getName() << endl;

    cout << "\nComparing complex numbers (not really): " << endl;
    cout << deriative1.getName() << " Is ";
    if(deriative1<deriative2) cout << "Less than ";
    else cout << "Bigger than ";
    cout << deriative2.getName() << endl;

    cout << "\nAdding and assigning fractions:" << endl;
    deriative3 = deriative3+deriative4;
    cout << deriative3;

    cout << "\nAdding and assigning complex numbers:" << endl;
    deriative1 = deriative1+deriative2;
    cout << deriative1;
    return 0;
}
