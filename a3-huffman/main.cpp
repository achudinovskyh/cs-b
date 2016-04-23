#include "archiver.h"

int main(){
    Archiver myArch;
    string command = "";
    string filePath = "";
    char ch;
    cout << "Hello! This is an archiver based on an Huffman algorithm." << endl << endl;

    if(argc > 1){
        for(int i = 1; i < argc; i++){
            command = argv[i];
            i++;
            filePath = argv[i];
            if(command == "/zip"){
                myArch.zipFile(filePath);
                cout << "File was zipped successfully at: " << filePath+".huff" << endl << endl;
            }else if(command == "/unzip"){
                myArch.unzipFile(filePath);
                cout << "File was unzipped successfully at: " << filePath << endl << endl;
            }
        }
    }else{
        char ch = 0;
        while(1){
            myArch.menu();
            cin >> ch;
            if(ch == '1'){
                cout << "Enter file path." << endl;
                cin >> filePath;
                myArch.zipFile(filePath);
                cout << "File was zipped successfully at: " << filePath+".huff" << endl << endl;
                continue;
            }else if(ch == '2'){
                cout << "Enter file path." << endl;
                cin >> filePath;
                myArch.unzipFile(filePath);
                cout << "File was unzipped successfully at: " << filePath << endl << endl;
                continue;
            }else if(ch == 'q'){
                break;
            }
        }
    }
    return 0;
}
