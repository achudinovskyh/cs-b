#ifndef A1CALCULATOR
#define A1CALCULATOR
#include <string>
#include <cmath>
#include <map>

using namespace std;

class CalcLogic{
private:
    map<char,string> variables;

    // pointer to char of expression string
    char* pt;

    // function that set expression to lowercase and puts braces
    void validate(string& pExp);

    // function that take's digits
    double number();

    // function that check braces
    double brackets();

    // function that check for variables sin cos ...
    double firstPriority();

    // function that check for * /
    double secondPriority();

    // function that check for + -
    double thirdPriority();

public:
    // function which calculate an expression
   double calculate(string& expr);
   void   setVariable(char variable, string value);
   CalcLogic();
};


#endif // A1CALCULATOR

