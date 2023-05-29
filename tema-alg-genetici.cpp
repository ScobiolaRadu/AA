#include <iostream>
#include <fstream>
#include <string>
#include <bitset>

using namespace std;

ifstream in("in.txt");
ofstream g("out.txt");

int main(){
    int pop;
    float lim_inf, lim_sup;
    float a,b,c;
    int precision;
    float probability_crossover, probability_mutation;
    int generations;

    in>>pop>>lim_inf>>lim_sup>>a>>b>>c>>precision>>probability_crossover>>probability_mutation>>generations;

    float chromosomes[pop+1];
    for(int i=1;i<=pop;i++){
        chromosomes[i] = lim_inf + (lim_sup - lim_inf) * (rand() % 1000) / 1000.0;
    }

    g<<"Populatia initiala\n";
    for(int i=1;i<=pop;i++){
        unsigned int* binary = (unsigned int*) &chromosomes[i];
        std::bitset<sizeof(float)*8> bits(*binary);
        g<<"1: "<<bits<<" x= "<<chromosomes[i]<<endl;
    }
}
