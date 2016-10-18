#include <iostream>
#include "classes.h"

using namespace std;

int main(){
    cout << "Creating deriative class." << endl;
    ComplexPoint deriative1(4, 5, 6, 7, "FOO");
    cout << "Printing:" << endl;
    deriative1.print();
    cout << "Setting complexPart using method:" << endl;
    deriative1.setComplexPart(-10);
    cout << "Printing:" << endl;
    deriative1.print();
    cout << "Creating another deriative class." << endl;
    FractionPoint deriative2(4, 5, -6, 7, "BAR");
    cout << deriative2.getX() << endl;
    cout << "Printing:" << endl;
    deriative2.print();
    cout << "Setting denominator using method:" << endl;
    deriative2.setDenominator(20);
    cout << "Printing:" << endl;
    deriative2.print();
    cout << "Using reloaded function:" << endl;
    deriative2.printData();

    return 0;
}
