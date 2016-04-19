#include "a1-calculator.h"

int main()
{
    CalcLogic calculator = CalcLogic();
    cout << "This program will calculate expression." << endl;
    cout << "Enter expression!" << endl;
    string expression = "";
    getline(cin,expression);
    double result = calculator.calculate(expression);
    cout << "Result: " << expression << "=" << result;

}
