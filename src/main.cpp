#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <chrono>
using namespace std;
using namespace std::chrono;
typedef long long ll;

void getPrompt(bool save, char *input, ll *normInput){
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
    ll normInput, info[4] = {0}, temp;
    char input;
    string path, line;

    getPrompt(0, &input, &normInput);
    if(normInput == 121){
        getFileName(&path);
        ifstream inputFile(path);
        if(inputFile.is_open()){
            int i = 0;
            // Read the game's metadata
            while(getline(inputFile, line) && i < 3){
                stringstream ss(line);
                while(ss >> temp){
                    info[i] = temp;
                    // info[1] shall be the width and info[2] shall be the height
                    i++;
                }
            }
            ll mat[info[2]][info[1]];
            i = 0;

            while(getline(inputFile, line) && i < info[2] + 3){
                stringstream ss(line);
                while(ss >> temp){
                    mat[i][1] = temp;
                    i++;
                }
            }
            while(getline(inputFile, line) && i < info[2] + 3 + 1){
                stringstream ss(line);
                while(ss >> temp){
                    info[4] = temp;
                    i++;
                }
            }
            printf("Buffer size: %lld\n", info[0]);
            printf("Width: %lld\n", info[1]);
            printf("Height: %lld\n", info[2]);
        }
        printf("\n");
        inputFile.close();
    }
    else{

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
        MyFile << "Write success\n";
        MyFile.close();
    }
}