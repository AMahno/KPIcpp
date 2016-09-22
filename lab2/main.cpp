#include <iostream>
#include "classes.h"

using namespace std;

int main(){
    cout << "Creating deriative class." << endl;
    ComplexPoint deriative1(4, 5, 6, 7, "BAR");
    cout << "Printing:" << endl;
    deriative1.print();
    cout << "Setting complexPart using method:" << endl;
    deriative1.setComplexPart(-10);
    cout << "Printing:" << endl;
    deriative1.print();
    cout << "Creating another deriative class." << endl;
    FractionPoint deriative2(4, 5, -6, 7, "BAR");
    cout << "Printing:" << endl;
    deriative2.print();
    cout << "Setting fraction using method:" << endl;
    deriative2.setDenominator(20);
    deriative2.print();
    return 0;
}
