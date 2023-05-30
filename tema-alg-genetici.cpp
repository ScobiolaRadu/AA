#include <iostream>
#include <fstream>
#include <string>
#include <bitset>
#include <vector>

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
    std::vector<int> participating;
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
            participating.push_back(i);
        }
        g << endl;
    }

    // f
    g << endl;
    g << std::endl;
    for (int i = 0; i < participating.size() - 1; i += 2)
    {
        int c1 = participating[i];
        int c2 = participating[i + 1];

        std::bitset<32> bits1(*reinterpret_cast<unsigned *>(&selected[c1]));
        std::bitset<32> bits2(*reinterpret_cast<unsigned *>(&selected[c2]));

        int punct = rand() % (sizeof(float) * 8);

        g << "Recombinare dintre cromozomul " << c1 << " cu cromozomul " << c2 << ":" << std::endl;
        g << bits1 << " " << bits2 << " punct " << punct << std::endl;

        std::bitset<32> combined;
        for (int j = 0; j < punct; j++)
        {
            combined[j] = bits1[j];
        }
        for (int j = punct; j < 32; j++)
        {
            combined[j] = bits2[j - punct];
        }

        std::bitset<32> combined2;
        for (int j = 0; j < punct; j++)
        {
            combined2[j] = bits2[j];
        }
        for (int j = punct; j < 32; j++)
        {
            combined2[j] = bits1[j - punct];
        }

        g << "Rezultat    " << combined << " " << combined2 << std::endl;

        chromosomes[c1] = *reinterpret_cast<float *>(&combined);
        chromosomes[c2] = *reinterpret_cast<float *>(&combined2);
    }

    // g
    g << "\nDupa recombinare:\n";
    for (int i = 1; i <= pop; i++)
    {
        unsigned int *binary = (unsigned int *)&chromosomes[i];
        std::bitset<sizeof(float) * 8> bits(*binary);
        g << i << ": " << bits << " x= " << chromosomes[i] << " ";
        g << "f=" << func(chromosomes[i], a, b, c) << endl;
    }

    // h
    g << "\nProbabilitatea de mutatie pentru fiecare gena " << probability_mutation / 100 << "\n";
    g << "Au fost modificati cromozomii:\n";
    for (int i = 1; i <= pop; i++)
    {
        if (static_cast<float>(rand()) / RAND_MAX < probability_mutation / 100)
        {
            int geneIndex = rand() % 32;
            unsigned int *binary = (unsigned int *)&chromosomes[i];

            std::bitset<32> bits(*binary);
            bits.flip(geneIndex);
            *binary = bits.to_ulong();

            g << i << endl;
        }
    }

    g << "Dupa mutatie:\n";
    for (int i = 1; i <= pop; i++)
    {
        unsigned int *binary = (unsigned int *)&chromosomes[i];
        std::bitset<32> bits(*binary);
        g << i << ": " << bits << " x= " << chromosomes[i] << " ";
        g << "f=" << func(chromosomes[i], a, b, c) << endl;
    }

    // i
    std::vector<float> maxFitnessHistory;
    std::vector<float> meanFitnessHistory;
    float maxFitness = 0.0;
    float sumFitness = 0.0;
    for (int i = 1; i <= pop; i++)
    {
        float fitness = func(chromosomes[i], a, b, c);
        if (fitness >= maxFitness)
        {
            maxFitness = fitness;
        }
        maxFitnessHistory.push_back(maxFitness);
        sumFitness += fitness;
        float meanFitness = sumFitness / pop;
        meanFitnessHistory.push_back(meanFitness);
    }

    g << "\nEvolutia maximului\n";
    for (int i = 0; i < maxFitnessHistory.size(); i++)
    {
        g << maxFitnessHistory[i] << endl;
    }
    g << "\nMedia fitnessului\n";
    for (int i = 0; i < meanFitnessHistory.size(); i++)
    {
        g << meanFitnessHistory[i] << endl;
    }
}
