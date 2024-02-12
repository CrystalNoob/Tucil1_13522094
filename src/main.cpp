#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <chrono>
using namespace std;
using namespace std::chrono;

#define buff info[0]
#define width info[1]
#define height info[2]
#define nseq info[3]

typedef long long ll;
typedef struct{
    vector<string> seqLine;
    ll reward;
} Seq;

// Global
vector<ll> info;
vector<vector<string>> mat;
vector<Seq> seq;
vector<string> uniqToken;

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
    system("CLS");
    char input;
    ll normInput;
    string path, line;
    repeat:
        getPrompt(0, &input, &normInput);
    if(normInput == 121){ // Input by file
        getFileName(&path);
        ifstream inputFile(path);
        if(inputFile.is_open()){
            ll i = 0;
            // Read game's metadata
            while(i < 3 && getline(inputFile, line)){
                stringstream ss(line);
                ll temp;
                while(ss >> temp){
                    info.push_back(temp);
                    // info[1] shall be the width and info[2] shall be the height
                    i++;
                }
            }
            printf("Buffer size: %lld\n", buff);
            printf("Width: %lld\n", width);
            printf("Height: %lld\n", height);

            // Read the matrix
            mat.resize(height, vector<string>(width));
            for(i = 0; i < height; i++){
                getline(inputFile, line);
                for(int j = 0; j < width; j++){
                    mat[i][j] = line.substr(3*j, 2);
                    /*  "7A 55 E9 E9"
                         ^  ^  ^  ^
                    pos: 0123456789
                    */
                    cout << mat[i][j] << " ";
                }
                printf("\n");
            }
            // Read the seq's count
            getline(inputFile, line);
            stringstream ss(line);
            ss >> info[3];
            // info[3] shall be the nseq
            printf("Number of sequences: %lld\n", nseq);
            
            // Read the sequences
            seq.resize(nseq);
            for(i = 0; i < nseq; i++){
                getline(inputFile, line);
                for(size_t j = 0; j < (line.length() + 1)/3; j++){
                    string temp = line.substr(3*j, 2);
                    seq[i].seqLine.push_back(temp);
                    cout << seq[i].seqLine[j] << " ";
                }
                printf("\n");
                getline(inputFile, line);
                stringstream ss(line);
                ss >> seq[i].reward;
                cout << "Reward: " << seq[i].reward << "\n";
            }
            printf("\n");
            inputFile.close();
        }
        else{
            system("CLS");
            printf("An attempt to read the file failed!\n");
            goto repeat;
        }
    }
    else{ // Random input, need generator
        ll nToken;
        printf("Jumlah token unik: ");
        scanf("%lld", &nToken);
        getline(cin, line);
        for(int i = 0; i < nToken; i++){
            //uniqToken.push_back()
        }
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