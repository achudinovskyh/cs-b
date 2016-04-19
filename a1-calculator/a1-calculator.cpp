#include "a1-calculator.h"

double CalcLogic::calculate(string& expr){
    // initialize pointer to char
    pt = &expr[0];
    validate(&expr);

    // begin to evaluate the expression
    double result = thirdPriority();
    return result;
}

double CalcLogic::digit(){
    // variable that store current number
    double result = 0.0;

    /* while the number is ineger we multiply result by 10
     * to create a space for new nuber that was read from string
     */
    double enlarger = 10;

    /*  if the numbers is real number we change the logic and set enlarger to 1
     *  and divider for point to 10 we will add next nubers after dot and divide result by 10
     *  to move dot left
     */
    double dividerForPoint = 1.0;

    while(true){
        // cheking if the char is a number and convert it from sting to int by subtracting the code of '0'
        if(*pt >= '0' && *pt <= '9'){
            result = result * enlarger + ((*pt) - '0') / dividerForPoint;
            pt++;

            // multiply it by ten each time that we found a digit after dot
            if(dividerForPoint >= 10){
                dividerForPoint *= 10;
            }

        }else if(*pt == '.'){
            pt++;
            enlarger = 1;
            dividerForPoint = 10.0;
        }
        else{
           return result;
        }
    }
}

double  CalcLogic::braces(){
    // checking for a braces if it is not a brace checking for digit
    double result;
    switch (*pt) {
    case '(':
        pt++;
        result = thirdPriority();
        pt++;
        return result;
    default:

        // cheking for digit's
        return digit();
    }
}

double CalcLogic::firstPriority(){
    // walk in recursion to higher priority
    double result = braces();

    // checking for function or veriables
    string function = "";
    while(true) {
        if(*pt >= 'a' && *pt <= 'z'){
            function += *pt;
            pt++;
        }else if (*pt == '^') {
             function += *pt;
             pt++;
        }else{
            break;
        }
    }
        if (function.compare("sin") == 0) {
            result = sin(braces());
        } else if (function.compare("cos") == 0) {
            result = cos(braces());
        } else if(function.compare("tan") == 0){
            result = tan(braces());
        }  else if (function.compare("asin") == 0) {
            result = asin(braces());
        } else if (function.compare("acos") == 0) {
            result = acos(braces());
        } else if(function.compare("atan") == 0){
            result = atan(braces());
        }  else if (function.compare("sinh") == 0) {
            result = sinh(braces());
        } else if (function.compare("cosh") == 0) {
            result = cosh(braces());
        } else if(function.compare("tanh") == 0){
            result = tanh(braces());
        } else if (function.compare("exp") == 0) {
            result = exp(braces());
        } else if(function.compare("sqrt") == 0){
            result = sqrt(braces());
        } else if(function.compare("^") == 0){
            result = pow(result, braces());
        } else if(function.compare("log") == 0){
            result = log2(braces());
        } else if (function.compare("fabs") == 0) {
            result = fabs(braces());
        } else if (function.compare("floor") == 0) {
            result = floor(braces());
        }else if(function.compare("a") == 0){
            cout << "Enter value for a: " << endl;
            double a = 0;
            cin >> a;
            return a;
        }else if(function.compare("b") == 0){
            cout << "Enter value for b: " << endl;
            double b = 0;
            cin >> b;
            return b;
        }else if(function.compare("c") == 0){
            cout << "Enter value for c: " << endl;
            double c = 0;
            cin >> c;
            return c;
        }

    return result;
}

double CalcLogic::secondPriority(){
    // walk in recursion to higher priority
    double result = firstPriority();

    //checking for * /
    while(true){
        switch (*pt) {
        case '*':
            pt++;
            result *= firstPriority();
            break;
        case '/':
            pt++;
            result /= firstPriority();
            break;
        default:
            return result;
        }
    }

}

double CalcLogic::thirdPriority(){
    // walk in recursion to higher priority
    double result = secondPriority();

    //checking for + -
    while(true){
        switch (*pt) {
        case '+':
            pt++;
            result += secondPriority();
            break;
        case '-':
            pt++;
            result -= secondPriority();
            break;
        default:
            return result;
        }
    }
}

void CalcLogic::validate(string* pExp){
    //remove space's and tolowercase simbols
    for(string::size_type i = 0; i < (*pExp).size(); i++){ 
        if((*pExp)[i] == ' '){
            (*pExp).erase(i,1);
            i--;
        }
        if(isalpha((*pExp)[i])){
            tolower((*pExp)[i]);
        }
    }

    // br_counter is a counter that we increment when inserting "("
    int  br_counter = 0;
    // open_br_counter is a counter for open braces that we pass while have enclosed brace
    int open_br_counter = 0;

    // variables for insertion
    string br = "(";
    string br1 = ")";

    for(string::size_type i = 0; i < (*pExp).size(); i++){
            // check whether there is in expression two signs in a row
            if((*pExp)[i] == '+'|| (*pExp)[i] == '-' || (*pExp)[i] == '*' || (*pExp)[i] == '/'){
                if((*pExp)[i+1] == '+'|| (*pExp)[i+1] == '-' || (*pExp)[i+1] == '*' || (*pExp)[i+1] == '/'){

                    // if there is we insert open brace between them and increment a counter
                    (*pExp).insert(i+1,br);
                    br_counter++;

                    // creating another variable for imaginary substring that is in your braces
                    string::size_type j = i+3;

                    // if the next character is a digit
                    if(isdigit((*pExp)[j])){
                        // we move forward until it is over
                        while(isdigit((*pExp)[j]) || (*pExp)[j] == '.'){
                            j++;
                        }
                        // and inserting closing brace
                        while(br_counter != 0){
                            (*pExp).insert(j,br1);
                            br_counter--;
                        }

                    // if we pass an open brace, count it
                    }if((*pExp)[j] == '('){
                       open_br_counter++;
                       j++;

                       // move forward until we close all braces
                       while(open_br_counter != 0){
                           if((*pExp)[j] == '('){
                                open_br_counter++;
                                j++;
                           }else if((*pExp)[j] == ')'){
                               open_br_counter--;
                               j++;
                           }else{
                               j++;
                           }

                       }

                       // closing all open braces that we inserted
                       while(br_counter != 0){
                           (*pExp).insert(j,br1);
                           br_counter--;
                       }
                    }
                }
            }

    }

}
