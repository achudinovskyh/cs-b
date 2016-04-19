#include <iostream>
#include "silcounter.h"

int main()
{
    SilCounter counter;
    string filePath;
    char ch = 0;
    cout << "Hello! This program will count silhouettes! \n\n";

    while(1){
        cout << "Press 1 if you want to count silhouettes, or q to quit.\n";
        cin >> ch;
        if(ch == '1')
        {
            cout << "Enter file path to image.\n";
            cin >> filePath;
            cout << "\nThere are: " << counter.countSilhouettes(&filePath) << " silhouettes!\n\n";
            continue;
        }else if(ch == 'q')
        {
            break;
        }
    }
}

