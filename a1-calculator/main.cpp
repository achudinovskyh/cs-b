#include "a1-calculator.h"
#include <iostream>

int main(int argc, char* argv[])
{
    CalcLogic calculator;
    cout << "This program will calculate expression." << endl << endl;
    string expression = "";

    if(argc > 1){
        for(int i = 1; i < argc; i++){
            expression = argv[i];
        }
    }else{
        char ch = 0;
        while(1){
            cout << "Press 1 if you want to calculate expression, or q to quit." << endl;
            cin >> ch;
            if(ch == '1'){
                cout << "Enter expression!" << endl;
                cin >> expression;
                cout << "The calculation result of: " << expression << " = " << calculator.calculate(expression) << endl << endl;
                continue;
            }else if(ch == 'q'){
                break;
            }
        }
    }
}
