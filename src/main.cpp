#include <iostream>
#include <string>
#include <fstream>
#include <chrono>
using namespace std;
using namespace std::chrono;

void getPrompt(bool save, char *input, int *normInput){
    do{
        printf((save) ? ("Apakah ingin menyimpan solusi? (y/n)\n") : ("Input dengan file? (y/n)\n"));
        cin >> *input;
        *normInput = tolower(*input);
    }while(*normInput != 121 && *normInput != 110);
    // Note: 'y' is 121 and 'n' is 110
}

void getFileName(string *path){
    // assumes the file's location is always in the test folder
    *path = "../test/";
    
    string temp;
    printf("Masukkan nama file: ");
    cin >> temp;
    *path = *path + temp + ".txt";
    // assumes the file's extension is always .txt
}

int main(){
    int normInput;
    char input;
    string path;
    getPrompt(0, &input, &normInput);
    if(normInput == 121){
        getFileName(&path);
        string myText;
        ifstream MyFile(path);
        while(getline(MyFile, myText)){
          // Output the text from the file
          cout << myText;
        }
        printf("\n");
        MyFile.close();
    }

    // Timer start
    auto start = high_resolution_clock::now();
    
    // Bruteforce algo dummy
    for(int i = 0; i < 1000000; i++){
        continue;
    }





    // Timer stop
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    printf("%lld ms\n", duration.count());

    getPrompt(1, &input, &normInput);
    if(normInput == 121){
        getFileName(&path);
        ofstream MyFile(path);
        MyFile << "Success\n";
        MyFile.close();
    }
}