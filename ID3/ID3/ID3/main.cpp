//
//  main.cpp
//  ID3
//
//  Created by Stefka Vladimirova Moskova on 18.12.19.
//  Copyright © 2019 Stefka Vladimirova Moskova. All rights reserved.
//

#include<iostream>
#include<fstream>
#include <map>
#include<vector>
#include <math.h>
#include <set>

using namespace std;

struct Patient
{
     /*1. Class: no-recurrence-events, recurrence-events
       2. age: 10-19, 20-29, 30-39, 40-49, 50-59, 60-69, 70-79, 80-89, 90-99.
       3. menopause: lt40, ge40, premeno.
       4. tumor-size: 0-4, 5-9, 10-14, 15-19, 20-24, 25-29, 30-34, 35-39, 40-44,
                      45-49, 50-54, 55-59.
       5. inv-nodes: 0-2, 3-5, 6-8, 9-11, 12-14, 15-17, 18-20, 21-23, 24-26,
                     27-29, 30-32, 33-35, 36-39.
       6. node-caps: yes, no.
       7. deg-malig: 1, 2, 3.
       8. breast: left, right.
       9. breast-quad: left-up, left-low, right-up,    right-low, central.
      10. irradiat:    yes, no.
    */

    vector<string> values;
    const static string valueOrder[];

    void print()
    {
        for(int i=0; i<values.size(); i++)
        {
            cout<<values[i]<<" ";
        }
        cout<<endl;
    }
};

const string Patient::valueOrder[] = { "className", "age", "menopause","tumorSize","invNodes",
                                       "nodeCaps", "degMalig", "breast", "breastQuad", "irradiat"
                                     };

struct Node
{
    vector<Patient> data;
    int valueIdx;
    string valueName;
    bool isTerminal;
    map<string, Node*> children;
};

vector<Patient> readData()
{
    ifstream file ("breast-cancer.data", std::ifstream::in);
    string line;

    vector<Patient> patients;
    while(file>>line)
    {
        Patient newEntry;
        bool questionMark = false;

        for(int i=0; i<10; i++)
        {
            int idx = line.find(',');
            string value = line.substr(0, idx);
            if(value == "?")
            {
                questionMark = true;
                break;
            }
            newEntry.values.push_back(value);
            line.erase(0, idx + 1);
        }

        if(questionMark == true)
        {
            continue;
        }

        patients.push_back(newEntry);
    }
    return patients;
}

double entropyOneAttribute(vector<Patient> data, int valueIdx)
{
    map<string, int> counter;
    for(int i=0; i<data.size(); i++)
    {
        string value = data[i].values[valueIdx];
        counter[value]++;
    }

    double entropy = 0;
    for (map<string,int>::iterator it=counter.begin(); it!=counter.end(); ++it)
    {
        double p = (double)it->second / data.size();
        entropy += (-1) * p * log2(p);
    }

    return entropy;
}

double entropyAgainstClass(vector<Patient> data, int valueIdx)
{
    map<string, int> counter;
    for(int i=0; i<data.size(); i++)
    {
        string value = data[i].values[valueIdx];
        counter[value]++;
    }


    double entropy = 0;
    for (map<string,int>::iterator it=counter.begin(); it!=counter.end(); ++it)
    {
        vector<Patient> subData;
        for(int i=0; i<data.size(); i++)
        {
            if(data[i].values[valueIdx] == it->first)
            {
                subData.push_back(data[i]);
            }
        }
        double p = (double)it->second / data.size();
        entropy += p * entropyOneAttribute(subData, 0);
    }

    return entropy;
}

double gain(vector<Patient> data, int valueIdx)
{
    return entropyOneAttribute(data, 0) - entropyAgainstClass(data, valueIdx);
}

Node* ID3(vector<Patient> data, set<int> attributesInBranch, int k)
{
    Node* node = new Node();
    if(entropyOneAttribute(data, 0) == 0)
    {
        node->isTerminal = true;
        node->valueName = data[0].values[0];

        return node;
    }

    if(data.size() < k || attributesInBranch.size() == 9)
    {
        node->isTerminal = true;

        map<string, int> counter;
        for(int i=0; i<data.size(); i++)
        {
            string value = data[i].values[0];
            counter[value]++;
        }

        if(counter["no-recurrence-events"] >= counter["recurrence-events"])
        {
            node->valueName = "no-recurrence-events";
        }
        else
        {
            node->valueName = "recurrence-events";
        }

        return node;
    }


    double maxGain = -1;
    int maxIdx = -1;
    for(int i=1; i<10; i++)
    {
        if(attributesInBranch.count(i) == 1)
        {
            continue;
        }
        double iGain = gain(data, i);
        if(iGain > maxGain)
        {
            maxGain = iGain;
            maxIdx = i;
        }
    }

    node->valueIdx = maxIdx;
    node->valueName = Patient::valueOrder[maxIdx];
    node->isTerminal = false;
    node->data = data;

    set<string> valueDO;
    for(int i=0; i<data.size(); i++)
    {
        string value = data[i].values[maxIdx];
        valueDO.insert(value);
    }

    for(set<string>::iterator it=valueDO.begin(); it!=valueDO.end(); ++it)
    {
        vector<Patient> subData;
        for(int i=0; i<data.size(); i++)
        {
            if(data[i].values[maxIdx] == *it)
            {
                subData.push_back(data[i]);
            }
        }
        set<int> attributesInNextIteration = attributesInBranch;
        attributesInNextIteration.insert(maxIdx);
        node->children[*it] = ID3(subData, attributesInNextIteration, k);
    }

    return node;
}

void printer(Node* node, int spaces)
{
    for(int i=0; i<spaces; i++)
    {
        cout<<" ";
    }
    cout<<node->valueName<<endl;
    if(node->isTerminal) return;

    for (std::map<string, Node*>::iterator it=node->children.begin(); it!=node->children.end(); ++it)
    {
        std::cout << it->first << " => "<<endl;
        printer(it->second, spaces+2);
    }
}

vector<Patient> combineFolds(vector<Patient> folds[], int skipFoldIdx)
{
    vector<Patient> combined;

    for(int i=0; i<10; i++)
    {
        if(i == skipFoldIdx)
        {
            continue;
        }

        for(int j=0; j<folds[i].size(); j++)
        {
            combined.push_back(folds[i][j]);
        }
    }
    return combined;
}

string predict(Node* root, Patient newEntry)
{
    Node* node = root;
    Node* parent;
    while(node!=nullptr && !node->isTerminal)
    {
        string newEntryValue = newEntry.values[node->valueIdx];

        parent = node;
        node = node->children[newEntryValue];
    }

    if(node == nullptr)
    {
        map<string, int> counter;
        for(int i=0; i<parent->data.size(); i++)
        {
            string value = parent->data[i].values[0];
            counter[value]++;
        }

        return (counter["no-recurrence-events"] >= counter["recurrence-events"] ?
                "no-recurrence-events" : "recurrence-events");
    }
    else
    {
        return node->valueName;
    }

}

void crossValidate( vector<Patient> folds[], int k )
{
    double meanAccuracy;

    for(int j=0; j<10; j++)
    {
        vector<Patient> trainingSet = combineFolds(folds, j);
        Node* root = ID3(trainingSet, set<int>(), k);

        int correctCount = 0;
        for(int i=0; i<folds[j].size(); i++)
        {
            string prediction = predict(root, folds[j][i]);
            if(prediction == folds[j][i].values[0])
            {
                correctCount++;
            }
        }
        double accuracy = (double)correctCount/folds[j].size();
        cout<<"Accuracy on fold "<<j<<" is: "<<accuracy<<endl;
        meanAccuracy += accuracy;
    }
    meanAccuracy /= 10;
    cout<<"Mean accuracy is: "<<meanAccuracy<<endl;
}

int main()
{
    srand (time(NULL));

    vector<Patient> data = readData();
    int k;
    cout<<"Enter k: ";
    cin>>k;

    vector<Patient> folds[10];

    for(int i=0; i<data.size(); i++)
    {
        int randomIdx = rand() % 10;
        folds[randomIdx].push_back(data[i]);
    }

    crossValidate(folds, k);
}

