//
//  main.cpp
//  Naive Bayes Classificator
//
//  Created by Stefka Vladimirova Moskova on 18.12.19.
//  Copyright © 2019 Stefka Vladimirova Moskova. All rights reserved.
//

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <fstream>
#include <array>
#include <map>
#include <ctime>
#include <algorithm>

using namespace std;

#define ATTR_SIZE 16
#define CROSS_VALIDATION_FOLD 10

#include "Individual.h"


// Model based
map<string, double> calculateClassNameProbs(vector<Individual> learn)
{
    map<string, int> classNameCounters;
    classNameCounters["republican"] = 0;
    classNameCounters["democrat"] = 0;

    for (auto &i : learn)
        classNameCounters[i.getClassName()]++;

    map<string, double> classNameProbs;
    
    classNameProbs["republican"] = static_cast<double>(classNameCounters["republican"]) / learn.size();
    classNameProbs["democrat"] = static_cast<double>(classNameCounters["democrat"]) / learn.size();

    return classNameProbs;
}

map<string, map<char, array<double, ATTR_SIZE>>> calculateAttrProbs(vector<Individual> learn)
{
    map<string, map<char, array<int, ATTR_SIZE>>> attrCounters;

    attrCounters["republican"]['y'] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    attrCounters["republican"]['n'] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    attrCounters["republican"]['?'] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    attrCounters["democrat"]['y'] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    attrCounters["democrat"]['n'] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    attrCounters["democrat"]['?'] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    int republicanCounter = 0;
    int democratCounter = 0;

    for (auto &record : learn)
    {
        string className = record.getClassName();
        array<char, ATTR_SIZE> attributes = record.getAttributes();

        if (className == "republican") republicanCounter++;
        else democratCounter++;

        for (int i = 0; i < ATTR_SIZE; ++i)
            attrCounters[className][attributes[i]][i]++;
    }

    map<string, map<char, array<double, ATTR_SIZE>>> attrProbs;

    for (auto &p : attrCounters)
    {
        int counter = (p.first == "republican") ? republicanCounter : democratCounter;

        for (auto &value : p.second)
        {
            int itr = 0;
            for (auto &counters : value.second)
            {
                double prob = static_cast<double>(counters) / counter;
                attrProbs[p.first][value.first][itr++] = prob;
            }
        }
    }

    return attrProbs;
}

int main()
{
    // Open file
    ifstream file("data.csv");

    // Check for errors
    if (!file.is_open())
    {
        cout << "Error while opening the file!" << endl;
        
        exit(0);
    }

    // Vector to store all the individuals
    vector<Individual> individuals;

    // Read file and write in vector
    while(file.good())
    {
        string row;
        getline(file, row, '\n');
        istringstream iss(row);

        string className;
        array<char, ATTR_SIZE> attributes;

        getline(iss, className, ',');
        
        int i = 0;
        for (string attr; getline(iss, attr, ',');)
            attributes[i++] = attr[0];

        Individual individual(className, attributes);
        individuals.push_back(individual);
    }
    
    // Close file
    file.close();

    // Shuffle the individuals and arrange them in 10 groups
    srand(time(NULL));
    std::random_shuffle(individuals.begin(), individuals.end());

    array<vector<Individual>, CROSS_VALIDATION_FOLD> individualsGroups;
    
    int numberOfIndividuals = individuals.size();
    int groupSize = numberOfIndividuals / CROSS_VALIDATION_FOLD;

    int counter = 0;
    int groupId = 0;
    for (auto &individual : individuals)
    {
        if (groupId < CROSS_VALIDATION_FOLD - 1)
        {
            if (counter < groupSize)
                counter++;
            
            else
            {
                counter = 1;
                groupId++;
            }
        }

        individualsGroups[groupId].push_back(individual);
    }

    array<int, CROSS_VALIDATION_FOLD> results;
    
    for(int i = 0; i < CROSS_VALIDATION_FOLD; ++i)
    {
        vector<Individual> test = individualsGroups.at(i);
        vector<Individual> learn;
        
        for (int j = 0; j < CROSS_VALIDATION_FOLD; ++j)
        {
            if (j != i)
                learn.insert(learn.end(), individualsGroups.at(j).begin(), individualsGroups.at(j).end());
        }

        auto attrProbs = calculateAttrProbs(learn);
        auto classNameProbs = calculateClassNameProbs(learn);

        int countCorrect = 0;
        for(auto &testIndividual : test)
        {
            if (testIndividual.classifyIndividual(attrProbs, classNameProbs))
                countCorrect++;
        }

        double accuracy = (static_cast<double>(countCorrect) / test.size()) * 100;
        results[i] = accuracy;
        
        cout << (i + 1) << " ";
        cout << "Accuracy: " << accuracy << "%" << endl;
    }
    
    double sum = 0;
    
    for (auto &res : results)
        sum += res;

    cout << endl << "The Average Accuracy is: " << sum / results.size() << "%" << endl;
}

