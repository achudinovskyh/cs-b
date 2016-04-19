#include "archiver.h"



int main()
{
    Archiver myArch = Archiver();
    string filePath;
    char ch;
    cout << "Hello! This is an archiver based on an Huffman algorithm.\n\n";
    while(1){
        myArch.menu();
        cin >> ch;
        if(ch == '1')
        {
            cout << "Enter file path.\n";
            cin >> filePath;
            myArch.zipFile(&filePath);
            //if(!deBug){system("cls");}
            cout << "File was zipped successfully at: " << filePath+".huff" << endl << endl;
            continue;
        }else if(ch == '2')
        {
            cout << "Enter file path.\n";
            cin >> filePath;
            myArch.unzipFile(&filePath);
           // if(!deBug){system("cls");}
            cout << "File was unzipped successfully at: " << filePath << endl << endl;
            continue;
        }else if(ch == 'q')
        {
            break;
        }
    }




    return 0;
}
