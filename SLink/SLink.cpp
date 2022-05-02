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
#include <algorithm>

using namespace std;

struct index_value {
    int index;
    float value;
};

float round2(float var)
{
    // 37.66666 * 100 =3766.66
    // 3766.66 + .5 =3767.16    for rounding off value
    // then type cast to int so value is 3767
    // then divided by 100 so the value converted into 37.67
    float value = (int)(var * 100 + .5);
    return (float)value / 100;
}

vector<string> split(const string& s, char delim) {
    vector<string> result;
    stringstream ss(s);
    string item;

    while (getline(ss, item, delim)) {
        result.push_back(item);
    }

    return result;
}

void makeSet(vector<int>& set, int size) {
    set.resize(size);
    for (int i = 0; i < size; i++)
        set[i] = i;
}

int find(vector<int>& set, int a) {
    if (set[a] == a)
        return a;
    set[a] = find(set, set[a]);
    return set[a];
}

void join(vector<int>& set, int a, int b) {
    int parentA = find(set, a);
    int parentB = find(set, b);

    // Make sure that the biggest number is the representative
    if (parentB > parentA)
        set[parentA] = parentB;
    else
        set[parentB] = parentA;
}

void fromPointerReprToLinkageMatrix(vector<float> lambdas, vector<float> pies, vector< vector<float> >& linkageMatrix) {
    int n = lambdas.size();

    vector<int> unionFind;
    makeSet(unionFind, n * 2);

    vector<index_value> ivVector;
    for (int i = 0; i < n; i++) {
        index_value iv = {
            i,
            lambdas[i]
        };
        ivVector.push_back(iv);
    }

    std::sort(ivVector.begin(), ivVector.end(), [](const index_value& a, const index_value& b) -> bool {
        return a.value < b.value;
        });

    for (int i = 0; i < n - 1; i++) {
        int index = ivVector[i].index;
        float lambda = lambdas[index];
        int pie = pies[index];

        int reprIndex = find(unionFind, index);
        int dimReprIndex = 1;
        if (reprIndex >= n) {
            dimReprIndex = linkageMatrix[reprIndex - n][3];
        }
        int reprPie = find(unionFind, pie);
        int dimReprPie = 1;
        if (reprPie >= n) {
            dimReprPie = linkageMatrix[reprPie - n][3];
        }

        linkageMatrix.push_back(vector<float> {(float)reprIndex, (float)reprPie, lambda, (float)(dimReprIndex + dimReprPie)});

        join(unionFind, index, n + i);
        join(unionFind, pie, n + i);
    }
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
   
    

    

    //int* pi = (int*)malloc(point.size() * sizeof(int));
    //float* lambda = (float*)malloc(point.size() * sizeof(float));
    
    vector<float> lambda(point.size(), 0);
    vector<float> pi(point.size(), 0);

    int n = -1;
    for (int i = 0; i < point.size(); i++) {
        pair<float, float> appo = point[i];


        float* M = (float*)malloc((n + 1) * sizeof(float));

        pi[n + 1] = n + 1;
        lambda[n + 1] = numeric_limits<float>::infinity();

        for (int j = 0; j < n; j++) {
            /*M[j] = sqrt((point[j].first - point[n + 1].first) * (point[j].first - point[n + 1].first)
            + (point[j].second - point[n + 1].second) * (point[j].second - point[n + 1].second));*/
            
            M[j] = abs(point[j].first - point[n + 1].first) + abs(point[j].second - point[n + 1].second);
        }

        for (int j = 0; j < n; j++) {
            if (lambda[j] >= M[j]) {
                M[(int)pi[j]] = (M[(int)pi[j]] <= lambda[j]) ? M[(int)pi[j]] : lambda[j];
                lambda[j] = M[j];
                pi[j] = n + 1;
            }
            else
                M[(int)pi[j]] = (M[(int)pi[j]] <= M[j]) ? M[(int)pi[j]] : M[j];
        }

        for (int j = 0; j < n; j++) 
            if (lambda[j] >= lambda[pi[j]]) 
                pi[j] = n + 1;
            
        n++;
    }


    for (int i = 0; i < pi.size(); i++)
        pi[i]--;

    for (int i = 0; i < point.size(); i++)
        cout << pi[i] << "  " << lambda[i] << "\n";

   

    vector<vector<float>> linkageMatrix;


    fromPointerReprToLinkageMatrix(lambda, pi, linkageMatrix);

   


        //[[0. 1. 1. 2.]
        // [2. 3. 1. 2.]
        // [4. 5. 4. 4.]]

    ofstream myfile("output.txt");
    if (myfile.is_open()) {

        myfile << "mat = np.array([";

        for (int i = 0; i < linkageMatrix.size(); i++) {
            myfile << "[";
            
            myfile << (int)linkageMatrix.at(i).at(0) << ".,";
            myfile << (int)linkageMatrix.at(i).at(1) << ".,";
            myfile << round2(linkageMatrix.at(i).at(2)) << ",";
            myfile << (int)linkageMatrix.at(i).at(3) << ".";
            myfile << "],";
        }
        myfile << "])";
        
        myfile.close();
    }
    else cout << "Unable to open file";


}

