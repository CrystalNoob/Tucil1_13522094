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
typedef struct{
    string str;
    bool seen;
} Elmt;

class Coordinate{
    public:
        ll row;
        ll col;
        Coordinate(ll r, ll c){
            row = r;
            col = c;
        }
};

class Sequence{
    public:
        string token;
        vector<Coordinate> optPath;
        ll reward;
        Sequence(string t, vector<Coordinate> oP, ll r){
            token = t;
            optPath = oP;
            reward = r;
        }
};

// Global
vector<ll> info;
vector<vector<Elmt>> mat;
vector<Seq> sequences;
vector<string> uniqToken;
vector<Sequence> optSeq;

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

void resetMat(){
    for(int i = 0; i < height; i++){
        for(int j = 0; j < width; j++){
            mat[i][j].seen = false;
        }
    }
}

void fillMat(vector<Coordinate> coordinates){
    for(size_t i = 0; i < coordinates.size(); i++){
        mat[coordinates[i].row][coordinates[i].col].seen = true;
    }
}

void tallyScore(Sequence *seq){
    
}

void solver(ll *count, bool isVertical, Sequence *ans){
    if(*count == 0){
        optSeq.push_back(*ans);
    }
    else{
        *count -= 1;
        Coordinate adr = ans->optPath.back();;
        if(isVertical){
            for(int i = 0; i < height; i++){
                resetMat();
                fillMat(ans->optPath);
                if(!mat[i][adr.col].seen){
                    vector<Coordinate> temp;
                    copy(ans->optPath.begin(), ans->optPath.end(), back_inserter(temp));
                    auto seqTemp = Sequence(ans->token, temp, ans->reward);
                    mat[i][adr.col].seen = true;
                    seqTemp.token += " " + mat[i][adr.col].str;
                    seqTemp.optPath.push_back(Coordinate(i, adr.col));
                    tallyScore(&seqTemp);
                    solver(count, true, &seqTemp);
                }
            }
        }
        else{
            for(int i = 0; i < width; i++){
                resetMat();
                fillMat(ans->optPath);
            if(!mat[adr.row][i].seen){
                    vector<Coordinate> temp;
                    copy(ans->optPath.begin(), ans->optPath.end(), back_inserter(temp));
                    auto seqTemp = Sequence(ans->token, temp, ans->reward);
                    mat[adr.row][i].seen = true;
                    seqTemp.token += " " + mat[adr.row][i].str;
                    seqTemp.optPath.push_back(Coordinate(adr.row, i));
                    tallyScore(&seqTemp);
                    solver(count, false, &seqTemp);
                }
            }
        }
    }
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
            mat.resize(height, vector<Elmt>(width));
            for(i = 0; i < height; i++){
                getline(inputFile, line);
                for(ll j = 0; j < width; j++){
                    mat[i][j].seen = false;
                    mat[i][j].str = line.substr(3*j, 2);
                    /*  "7A 55 E9 E9"
                         ^  ^  ^  ^
                    pos: 0123456789
                    this way of input has a weakness: cannot read false input -> program crashes immediately
                    */
                    cout << mat[i][j].str << " ";
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
            sequences.resize(nseq);
            for(i = 0; i < nseq; i++){
                getline(inputFile, line);
                for(size_t j = 0; j < (line.length() + 1)/3; j++){
                    string temp = line.substr(3*j, 2);
                    sequences[i].seqLine.push_back(temp);
                    cout << sequences[i].seqLine[j] << " ";
                }
                printf("\n");
                getline(inputFile, line);
                stringstream ss(line);
                ss >> sequences[i].reward;
                printf("Reward: %lld\n", sequences[i].reward);
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
        ll nToken, buffTemp, widthTemp, heightTemp, nSeqTemp, maxSeq;
        printf("Jumlah token unik: ");
        scanf("%lld", &nToken);
        cin.ignore();
        getline(cin, line);
        for(ll i = 0; i < nToken; i++){
            uniqToken.push_back(line.substr(3*i, 2));
        }
        printf("Ukuran buffer: ");
        scanf("%lld", &buffTemp);
        printf("Lebar dan tinggi matrix: ");
        scanf("%lld %lld", &widthTemp, &heightTemp);
        // cin >> widthTemp >> heightTemp;
        printf("Jumlah sekuens: ");
        scanf("%lld", &nSeqTemp);
        printf("Ukuran sekuens maksimal: ");
        scanf("%lld", &maxSeq);
        info.push_back(buffTemp); info.push_back(widthTemp);
        info.push_back(heightTemp); info.push_back(nSeqTemp);

        // Testing
        getFileName(&path);
        ofstream MyFile(path);
        MyFile << nToken << "\n" << buffTemp << "\n";

        for(size_t i = 0; i < uniqToken.size(); i++){
            MyFile << uniqToken[i] << " ";
        }
        
        MyFile << "\n" << widthTemp << " " << heightTemp << "\n" << nSeqTemp << "\n" << maxSeq << "\n";
        MyFile.close();
    }
    // Timer start
    auto start = high_resolution_clock::now();
    
    // Bruteforce
    for(ll i = 0; i < width; i++){
        vector<Coordinate> steps;
        steps.push_back(Coordinate(0, i));
        auto temp = Sequence(mat[0][i].str, steps, 0);
        solver(&buff, true, &temp);
    }

    // Timer stop
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);

    printf("Score terbesar: %lld\n", 4);

    printf("%lld ms\n", duration.count());
    
    // Write to file?
    getPrompt(1, &input, &normInput);
    if(normInput == 121){
        getFileName(&path);
        ofstream MyFile(path);
        MyFile << "Write success\n";
        MyFile.close();
    }
}