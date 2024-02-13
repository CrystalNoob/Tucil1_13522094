#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <chrono>
#include <algorithm>
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

class Sequence{
    public:
        vector<ll> steps;
        ll reward;
        Sequence(string t, vector<ll> s, ll r){
            steps = s;
            reward = r;
        }
};

// Global
vector<ll> info;
vector<vector<string>> mat;
vector<Seq> sequences;
vector<string> uniqToken;
vector<ll> steps; // Empty arr
Sequence optimum("", steps, 0);


void getPrompt(bool save, char *input, ll *normInput){
    do{
        printf((save) ? ("\n\nApakah ingin menyimpan solusi? (y/n) ") : ("Input dengan file? (y/n) "));
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

void writeCoordinate(bool file, ofstream *MyFile, vector<ll> steps){
    if(!file){
        for(size_t i = 0; i < steps.size(); i++){
            printf("%lld,%lld\n", steps[i % width] + 1, steps[i / width] + 1);
        }
    }
    else{
        for(size_t i = 0; i < steps.size(); i++){
            *MyFile << steps[i % width] + 1 << "," << steps[i / width] + 1 << "\n";
        }
    }
}

void writeRes(bool file, ofstream *MyFile, vector<ll> steps, duration<long long int, std::ratio<1, 1000>> duration){
    if(file){    
        *MyFile << optimum.reward << "\n";
            if(optimum.reward > 0){
                for(size_t i = 0; i < optimum.steps.size(); i++){
                    *MyFile << mat[optimum.steps[i] / width][optimum.steps[i] % width] <<   " ";
                }
                *MyFile << "\n";
                writeCoordinate(file, MyFile, optimum.steps);
            }
            *MyFile << "\n" << duration.count() << " ms";
    }
    else{
        cout << optimum.reward << "\n";
        if(optimum.reward > 0){
            for(size_t i = 0; i < optimum.steps.size(); i++){
                cout << mat[optimum.steps[i] / width][optimum.steps[i] %width] <<   " ";
            }
            cout << "\n";
            writeCoordinate(file, MyFile, optimum.steps);
        }
        cout << "\n" << duration.count() << " ms";
    }
}

void tallyScore(vector<ll> path){
    ll score = 0;
    string str = "";
    for(size_t i = 0; i < path.size(); i++){
        str.append(mat[path[i] / width][path[i] % width]);
    }
    for(size_t i = 0; i < sequences.size(); i++){
        string temp = "";
        for(size_t j = 0; j < sequences[i].seqLine.size(); j++){
            temp.append(sequences[i].seqLine[j]);
        }
        if(str.find(temp) != string::npos){
            score += sequences[i].reward;
        }
    }
    if(optimum.reward < score){
        optimum.reward = score;
        optimum.steps = path;
    }
    if(optimum.reward == score && path.size() < optimum.steps.size()){
        optimum.reward = score;
        optimum.steps = path;
    }
}

void solver(vector<ll> path){
    ll mod;
    bool isVertical = true;
    if(!path.size()){
        for(ll i = 0;i < width; i++){
            path = {};
            path.push_back(i);
            solver(path);
        }
    }
    else if(path.size() == (size_t) buff){
        tallyScore(path);
    }
    else if(isVertical){
        mod = path.back() % width;
        for(ll i = 0; i < height; i++){
            vector<ll> newPath;
            if(find(path.begin(), path.end(), (width * i) + mod) == path.end()){
                newPath = path;
                newPath.push_back((width * i) + mod);
                solver(newPath);
            }
        }
        isVertical = false;
        tallyScore(path);
    }
    else if(!isVertical){
        mod = (path.back() / width) * width;
        for(ll i = mod; i < mod + width; i++){
            vector<ll> newPath;
            if(find(path.begin(), path.end(), i) == path.end()){
                newPath = path;
                newPath.push_back(i);
                solver(newPath);
            }
        }
        isVertical = true;
        tallyScore(path);
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
            mat.resize(height, vector<string>(width));
            for(i = 0; i < height; i++){
                getline(inputFile, line);
                for(ll j = 0; j < width; j++){
                    mat[i][j] = line.substr(3*j, 2);
                    /*  "7A 55 E9 E9"
                         ^  ^  ^  ^
                    pos: 0123456789
                    this way of input has a weakness: cannot read false input -> program crashes immediately
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

        srand(time(nullptr));
        string token[nToken];
        sequences.resize(nseq);
        for(ll i = 0; i < height; i++){
            vector<string> temp = {};
            for(ll j= 0;j < width; j++){
                temp.push_back(token[rand() % nToken]); // 0 to nToken-1
            }
            mat.push_back(temp);
        }

        for(ll i = 0; i < nseq; i++){
            string temp = "";
            ll randTokenLength = rand() % maxSeq + 1; // 1 to maxSeq
            for(ll j = 0; j < randTokenLength; j++){
                temp += token[rand() % nToken];       // 0 to nToken-1
            }
            sequences[i].seqLine.push_back(temp);
            sequences[i].reward = rand() % 100 + 1;   // 1 to 100
        }


        // Testing
        // getFileName(&path);
        // ofstream MyFile(path);
        // MyFile << nToken << "\n" << buffTemp << "\n";

        // for(size_t i = 0; i < uniqToken.size(); i++){
        //     MyFile << uniqToken[i] << " ";
        // }
        
        // MyFile << "\n" << widthTemp << " " << heightTemp << "\n" << nSeqTemp << "\n" << maxSeq << "\n";
        // MyFile.close();
    }
    // Timer start
    auto start = high_resolution_clock::now();
    
    // Bruteforce
    for(ll i = 0; i < width; i++){
        solver({});
    }

    // Timer stop
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(stop - start);

    printf("Score maksimum: %lld\n", optimum.reward);
    writeRes(false, NULL, steps, duration);
    
    // Write to file?
    getPrompt(1, &input, &normInput);
    if(normInput == 121){
        getFileName(&path);
        ofstream MyFile(path);
        writeRes(true, &MyFile, steps, duration);
        MyFile.close();
    }
}