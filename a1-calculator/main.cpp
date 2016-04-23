#include "a1-calculator.h"
#include <iostream>

int main(int argc, char* argv[])
{
    CalcLogic calculator;
    cout << "This program will calculate expression." << endl << endl;
    string expression = "";

    if(argc > 1){
        for(int i = 1; i < argc; i++){
            if(argv[i] == "/c"){
                i++;
                expression = argv[i];
            }else if(argv[i] == "/v"){
                i++;
                string variable = argv[i];
                char var = variable[0];
                i++;
                string value = argv[i];
                calculator.setVariable(var,value);
            }
        }
    }else{
        char ch = 0;
        while(1){
            cout << "Press" << endl << "1 if you want to calculate expression, or"
                 << endl << "2 if you want to set variables, or" << endl <<  "q to quit." << endl;
            cin >> ch;
            if(ch == '1'){
                cout << "Enter expression!" << endl;
                cin >> expression;
                cout << "The calculation result of: " << expression << " = " << calculator.calculate(expression) << endl << endl;
                continue;
            }else if(ch == '2'){
                char variable = 0;
                string value = "";
                cout << "Enter char for variable" << endl;
                cin >> variable;
                cout << "Enter value for variable" << endl;
                cin >> value;
                calculator.setVariable(variable,value);
                continue;
            }else if(ch == 'q'){
                break;
            }
        }
    }
}
