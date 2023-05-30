#include <iostream>
#include <fstream>
#include <string>
#include <bitset>

using namespace std;

ifstream in("in.txt");
ofstream g("out.txt");

float func(float x, float a, float b, float c)
{
    return a * x * x + b * x + c;
}

int main()
{
    int pop;
    float lim_inf, lim_sup;
    float a, b, c;
    int precision;
    float probability_crossover, probability_mutation;
    int generations;

    in >> pop >> lim_inf >> lim_sup >> a >> b >> c >> precision >> probability_crossover >> probability_mutation >> generations;

    float chromosomes[pop + 1];
    for (int i = 1; i <= pop; i++)
    {
        chromosomes[i] = lim_inf + (lim_sup - lim_inf) * (rand() % 1000) / 1000.0;
    }

    // a
    g << "Populatia initiala\n";
    for (int i = 1; i <= pop; i++)
    {
        unsigned int *binary = (unsigned int *)&chromosomes[i];
        std::bitset<sizeof(float) * 8> bits(*binary);
        g << i << ": " << bits << " x= " << chromosomes[i] << " ";
        g << "f=" << func(chromosomes[i], a, b, c) << endl;
    }

    // b
    g << "\nProbabilitati selectie\n";
    float prob[pop + 1];
    for (int i = 1; i <= pop; i++)
    {
        float sum = 0.0;
        for (int j = 1; j <= pop; j++)
        {
            sum += func(chromosomes[j], a, b, c);
        }
        prob[i] = func(chromosomes[i], a, b, c) / sum;
        g << "cromozom    " << i << " probabilitate " << prob[i] << endl;
    }

    // c
    g << "\nIntervale probabilitati selectie\n";
    float q[pop + 2];
    q[0] = 0;
    g << q[0] << endl;
    for (int i = 1; i <= pop; i++)
    {
        q[i] = q[i - 1] + prob[i];
        g << q[i] << endl;
    }

    // d
    float selected[pop + 1];
    for (int i = 1; i <= pop; i++)
    {
        float u = (float)rand() / RAND_MAX;

        int left = 1, right = pop;
        int selectedChromosome = -1;

        while (left <= right)
        {
            int mid = (left + right) / 2;
            if (u >= q[mid] && u < q[mid + 1])
            {
                selectedChromosome = mid + 1;
                break;
            }
            else if (u < q[mid])
            {
                right = mid - 1;
            }
            else
            {
                left = mid + 1;
            }
        }

        if (selectedChromosome == -1)
        {
            selectedChromosome = left;
        }

        selected[i] = chromosomes[selectedChromosome];
        g << "u= " << u << "    selectam cromozomul " << selectedChromosome << endl;
    }
    g << "Dupa selectie:\n";
    for (int i = 1; i <= pop; i++)
    {
        unsigned int *binary = (unsigned int *)&selected[i];
        std::bitset<sizeof(float) * 8> bits(*binary);
        g << i << ": " << bits << " x= " << selected[i] << " ";
        g << "f=" << func(selected[i], a, b, c) << endl;
    }

    // e
    g << "\nProbabilitatea de incrucisare " << probability_crossover / 100 << "\n";
    for (int i = 1; i <= pop; i++)
    {
        float u = (float)rand() / RAND_MAX;
        unsigned int *binary = (unsigned int *)&selected[i];
        std::bitset<sizeof(float) * 8> bits(*binary);
        g << i << ": " << bits << " u = " << u;

        if (u < probability_crossover / 100)
        {
            g << "<" << probability_crossover / 100 << " participa";
        }

        g << endl;
    }

    //f
    
}
