#ifndef A1CALCULATOR
#define A1CALCULATOR
#include <iostream>
#include <string>
#include <cmath>

using namespace std;

class CalcLogic{
private:

    // pointer to char of expression string
    char* pt;
    // function that set expression to lowercase and puts braces
    void validate(string* pExp);
    // function that take's digits
    double digit();
    // function that check braces
    double braces();
    // function that check for variables sin cos ...
    double firstPriority();
    // function that check for * /
    double secondPriority();
    // function that check for + -
    double thirdPriority();
public:
    // function which calculate an expression
   double calculate(string& expr);
};


#endif // A1CALCULATOR

