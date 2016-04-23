#include <iostream>
#include "silcounter.h"

int main(int argc, char* argv[])
{
    SilCounter counter;
    string filePath;
    cout << "Hello! This program will count silhouettes!" << endl << endl;

    if(argc > 1){
        for(int i = 1; i < argc; i++){
            filePath = argv[i];
            cout << "There are: " << counter.countSilhouettes(filePath) << " silhouettes in "<< filePath << endl << endl;
        }
    }else{
        char ch = 0;
        while(1){
            cout << "Press 1 if you want to count silhouettes, or q to quit." << endl;
            cin >> ch;
            if(ch == '1'){
                cout << "Enter file path to image." << endl;
                cin >> filePath;
                cout << "There are: " << counter.countSilhouettes(filePath) << " silhouettes!" << endl << endl;
                continue;
            }else if(ch == 'q'){
                break;
            }
        }
    }
}

