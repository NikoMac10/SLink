// SLink.cpp : Questo file contiene la funzione 'main', in cui inizia e termina l'esecuzione del programma.
//

#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <map>
#include <vector>
#include <math.h>
#include <limits>

using namespace std;

vector<string> split(const string& s, char delim) {
    vector<string> result;
    stringstream ss(s);
    string item;

    while (getline(ss, item, delim)) {
        result.push_back(item);
    }

    return result;
}


int main(){
    cout << "Start parsing\n";

    string line;
    ifstream infile("point.csv");

    map<int, pair<float, float>> point;

    while(getline(infile, line)){
        vector<string> vect = split(line, ',');
        pair<float, float> appo = { stof(vect[2]), stof(vect[1]) };

        point[stoi(vect[0])] = appo  ;
    }


    map<int, pair<float, float>>::iterator it;

    for (it = point.begin(); it != point.end(); it++)
    {
        std::cout << "ID " << it->first << " = " << " {x: "
            << it->second.first << ", y:" 
            << it->second.second << "}" << std::endl;
    }
   
    
    int* pi = (int*)malloc(point.size() * sizeof(int));
    float* lambda = (float*)malloc(point.size() * sizeof(float));


    int n = -1;
    for (int i = 0; i < point.size(); i++) {
        pair<float, float> appo = point[i];


        float* M = (float*)malloc((n + 1) * sizeof(float));

        pi[n + 1] = n + 1;
        lambda[n + 1] = numeric_limits<float>::max();

        for (int j = 0; j < n; j++) {
            /*M[j] = sqrt((point[j].first - point[n + 1].first) * (point[j].first - point[n + 1].first)
            + (point[j].second - point[n + 1].second) * (point[j].second - point[n + 1].second));*/

            M[j] = abs(point[j].first - point[n + 1].first) + abs(point[j].second - point[n + 1].second);
        }

        for (int j = 0; j < n; j++) {
            if (lambda[j] >= M[j]) {
                M[pi[j]] = (M[pi[j]] < lambda[j]) ? M[pi[j]] : lambda[j];
                lambda[j] = M[j];
                pi[j] = n + 1;
            }
            else
                M[pi[j]] = (M[pi[j]] < lambda[j]) ? M[pi[j]] : M[j];
        }

        for (int j = 0; j < n; j++) 
            if (lambda[j] >= lambda[pi[j]]) 
                pi[j] = n + 1;
            
        n++;
    }

    for (int i = 0; i < point.size(); i++)
        cout << pi[i] << "  " << lambda[i] << "\n";
    
}

