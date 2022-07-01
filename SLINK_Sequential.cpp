//  PROGRAM MADE BY 
//  NICOLAS PIETRO MARTIGNON (870034) E VISENTIN THOMAS (869438) 

#include <iostream>
#include <sstream>
#include <string>
#include <fstream>
#include <vector>
#include <math.h>
#include <limits>
#include <algorithm>
#include <chrono>

using namespace std;
using namespace std::chrono;


float round2(float var){
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


struct index_value {
    int index;
    float lambda;	
};

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

void fromPointerReprToLinkageMatrix(vector<float> lambda, vector<float> pi, vector< vector<float> >& linkageMatrix) {
    int n = lambda.size();   

    vector<int> unionFind;
    makeSet(unionFind, n * 2);

    vector<index_value> ivVector;
    for (int i = 0; i < n; i++) {
        index_value iv = {
            i,
            lambda[i]
        };
        ivVector.push_back(iv);
    }

    std::sort(ivVector.begin(), ivVector.end(), [](const index_value& a, const index_value& b) -> bool {
        return a.lambda < b.lambda;
    });

    for (int i = 0; i < n - 1; i++) {
        int index = ivVector[i].index;
        float lamb = lambda[index];
        int pie = pi[index]-1;

        int reprIndex = find(unionFind, index);
        int dimReprIndex = 1;
        if (reprIndex >= n) 
            dimReprIndex = linkageMatrix[reprIndex - n][3];
        
        int reprPie = find(unionFind, pie);
        int dimReprPie = 1;
        if (reprPie >= n) 
            dimReprPie = linkageMatrix[reprPie - n][3];
			
        

        linkageMatrix.push_back(vector<float> {(float)reprIndex, (float)reprPie, lamb, (float)(dimReprIndex + dimReprPie)});

        join(unionFind, index, n + i);
        join(unionFind, pie, n + i);
    }
}


int main(){
    cout << "Start parsing" << endl;;
	
    string line;
    ifstream infile("dataset2.csv");

    vector<vector<float>> point;
	point.push_back({0.0,0.0});

    while(getline(infile, line)){
        vector<string> vect = split(line, ',');
       
		vector<float> appo;
		
		for(int i =1; i < vect.size(); i++)
			appo.push_back(stof(vect[i]));
		
		
		point.push_back(appo);
    }
	
	cout << "Parsing finished" << endl;
    
    vector<float> lambda(point.size(), 0);
    vector<float> pi(point.size(), 0);
	
    
	int r = point.size();
	
	
	cout << "Start computing SLINK algo, update every 10'000 points calculated..." << endl;
	
	auto start = high_resolution_clock::now();
	for (int i = 1; i < r; i++) {

		vector<float> M(i);

		pi[i] = i;
		lambda[i] = numeric_limits<float>::infinity();

		for (int j = 1; j <= i-1; j++){
			
			float appo=0.0;
			for(int k = 0 ; k< point[j].size(); k++)
				appo += (point[j][k] - point[i][k]) * (point[j][k] - point[i][k]);
			

			M[j] = sqrt(appo);
		}
	
	
		for (int j = 1; j <= i-1; j++) {
			if (lambda[j] >= M[j]) {
				M[(int)pi[j]] = (M[(int)pi[j]] <= lambda[j]) ? M[(int)pi[j]] : lambda[j];
				lambda[j] = M[j];
				pi[j] = i;
			}
			else
				M[(int)pi[j]] = (M[(int)pi[j]] <= M[j]) ? M[(int)pi[j]] : M[j];
		}

		
		for (int j = 1; j <= i-1; j++) 
			if (lambda[j] >= lambda[(int)pi[j]]) 
				pi[j] = i;
	
	
		if(i%10000==0) cout << i << " Calculated points..." << endl;
		
	}
	
	
	auto stop = high_resolution_clock::now();
	auto duration = duration_cast<milliseconds>(stop - start);
	cout << "Time taken by function: "  << duration.count() << " milliseconds" << endl;

	
	lambda.erase(lambda.begin());
	pi.erase(pi.begin());

	
	//from point to linkage matrix
    vector<vector<float>> linkageMatrix;
	fromPointerReprToLinkageMatrix(lambda, pi, linkageMatrix);

	
	
	// writing to file linkage matrix
    ofstream myfile("linkageCPP.py");
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
    else cout << "Unable to open file" << endl;;

}

