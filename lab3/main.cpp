#include <iostream>
#include "classes.h"

using namespace std;

int main(){
    cout << "Creating classes." << endl;
    ComplexPoint deriative1(4, 5, 1, 3, "F00");
    ComplexPoint deriative2(3, 1, 4, 5, "FBB");
    FractionPoint deriative3(7, 6, 1, 3, "BAR");
    FractionPoint deriative4(3, 2, 3, 3, "B0R");
    point exampleBase(1, 2, 3, "Test");

    cout << "\nUsing virtual functions:" << endl;
    point *pointPtr[5];
    pointPtr[0] = &deriative1;
    pointPtr[1] = &deriative2;
    pointPtr[2] = &deriative3;
    pointPtr[3] = &deriative4;
    pointPtr[4] = &exampleBase;
    for(int i = 0; i<5; i++) pointPtr[i]->print();

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

    cout << "\nNew cin stuff for fractions: enter x, y, value, name and denominator:" << endl;
    cin >> deriative3;
    cout << deriative3;

    cout << "\nNew cin stuff for complex numbers: enter x, y, value, name and complex part:" << endl;
    cin >> deriative1;
    cout << deriative1;
    return 0;
}
