//
//  TravelingSalesmanProblem.cpp
//  TravelingSalesmanProblem
//
//  Created by Stefka Vladimirova Moskova on 3.11.19.
//  Copyright © 2019 Stefka Vladimirova Moskova. All rights reserved.
//

#include <iostream>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <algorithm>
#include <unordered_set>
#include <string>

using namespace std;

struct City //includes (x,y) coordinates of the city and also the ID of the city
{
private:
    int x;
    int y;
    
    int ID;
    
public:
    City(int newX, int newY, int newID) //Constructor
    {
        x = newX;
        y = newY;
        
        ID = newID;
    }
    
    City(int newID) // Constructor with random (x,y) coordinatates in a 1000 unit grid
    {
        x = rand() % 1000;
        y = rand() % 1000;
        
        ID = newID;
    }

    int getX()
    {
        return x;
    }

    int getY()
    {
        return y;
    }

    int getID()
    {
        return ID;
    }
};

class CityList // includes a vector with all the cities
{
private:
    std::vector<City> cities;
    
public:
    void addCity(City city) // Add a city to the vector
    {
        cities.push_back(city);
    }

    City getCity(int i)
    {
        return cities.at(i);
    }

    std::vector<City> getCityList()
    {
        return cities;
    }
};

class Tour
{
private:
    std::vector<City> tour;
    
    double distance = 0;
    double fitness = 0;
    
    int size;
    
    double getDistanceBetweenTwoCities(City city1, City city2)
    {
        int latDist = abs(city1.getX() - city2.getX());
        int lonDist = abs(city1.getY() - city2.getY());
        
        int distance = sqrt((latDist * latDist) + (lonDist * lonDist));
        
        return distance;
    }

public:
    Tour(std::vector<City> cities) // Constructor
    {
        createIndividual(cities);
    }

    Tour() // Default Constructor
    {
        size = 0;
    }

    void createIndividual(std::vector<City> cities) // A random tour of all the cities
    {
        tour = cities;
        random_shuffle(tour.begin(), tour.end());
    }

    City getCity(int i) // Gets city at i
    {
        return tour.at(i);
    }

    void setCity(City c)  // Pushes city onto tour
    {
        tour.push_back(c);
        size++;
    }

    void setCity(int i, City c) // Sets city at position i
    {
        tour.at(i) = c;
    }

    double getDistance()
    {
        if (distance == 0)
        {
            int size = tour.size();
            
            for (int i = 0; i < size - 1; i++)
            {
                distance += getDistanceBetweenTwoCities(tour.at(i), tour.at(i + 1));
            }
            
            distance += getDistanceBetweenTwoCities(tour.at(0), tour.at(size - 1)); // Return to starting city
        }
        
        return distance;
    }

    double getFitness()
    {
        return 1 / getDistance();
    }

    int getTourSize()
    {
        size = tour.size();
        return size;
    }
};

class Population
{
private:
    std::vector<Tour> tours; // Population of tours
    
    CityList cl;
    
    int size = 0;
    
public:
    Population(int popSize, int numCities) // Constructor that seeds random tours of cities
    {
        for (int i = 0; i < numCities; i++) // Add cities to list
        {
            cl.addCity(City(i));
        }
        
        for (int i = 0; i < popSize; i++) // Create population of tours
        {
            tours.push_back(cl.getCityList());
        }
        
        size = tours.size();
    }

    Population() // Default constructor
    {
        size = 0;
    }

    void addTour(Tour t)
    {
        tours.push_back(t);
        size++;
    }

    void addTour(Tour t, int i)
    {
        tours.at(i) = t;
    }

    Tour getTour(int i)
    {
        return tours.at(i);
    }

    Tour getFittest()
    {
        Tour fittest = tours.at(0);
        
        for (int i = 1; i < size; i++)
        {
            if (fittest.getFitness() < tours.at(i).getFitness())
            {
                fittest = tours.at(i);
            }
        }
        
        return fittest;
    }

    int getPopSize()
    {
        return size;
    }
};

class Genetics
{
private:
    const double mutationRate = .005; // Reccomended to be between .005 and .01
    const int tournamentSize = 5;
    bool elitism = true;
    
    std::vector<Tour> tours;
    
    Tour tournament(Population p)  // Decides which tours to crossover
    {
        Population tourney;
        int selectionRange = p.getPopSize();
        
        for (int i = 0; i < tournamentSize; i++)
        {
            int randomTour = rand() % selectionRange;
            tourney.addTour(p.getTour(randomTour));
        }
        
        Tour fit = tourney.getFittest();
        return fit;
    }

    Tour tournament(Population p, Tour t) // Tournament that excludes one tour from population
    {
        Population tourney;
        int selectionRange = p.getPopSize() - 1;
        int size = t.getTourSize();
        std::string p1Tour = "";
        
        for (int i = 0; i < size; i++)
        {
            p1Tour += t.getCity(i).getID();
        }

        int i = 0;
        while (i < tournamentSize)
        {
            int randomTour = rand() % selectionRange;
            for (int j = 0; j < size; j++)
            {
                if (p.getTour(randomTour).getCity(j).getID() != p1Tour[j])
                {
                    tourney.addTour(p.getTour(randomTour));
                    i++;
                    break;
                }
            }

        }
        
        Tour fit = tourney.getFittest();
        return fit;
    }

    //izbirame dva sluchaini grada i smeniame tehnite mesta
    void mutate(Tour t) // Randomly mutates individuals based on mutation rate
    {
        int tSize = t.getTourSize();
        for (int i = 0; i < tSize; i++)
        {
            if (double(rand() / RAND_MAX) <= mutationRate)
            {
                int swap = rand() % tSize;
                City c1 = t.getCity(i);
                City c2 = t.getCity(swap);
                t.setCity(swap, c1);
                t.setCity(i, c2);
            }
        }
    }

    Tour cross(Tour parent1, Tour parent2)  // Crosses a set of parents, returns child
    {
        int tSize = parent1.getTourSize();
        Tour child = Tour();
        std::unordered_set<int> citiesOnTour;
        int split1 = rand() % (tSize / 2);
        int split2 = rand() % (tSize / 2);
        split2 = tSize - split2;
        
        for (int i = 0; i < split1; i++)
        {
            child.setCity(parent1.getCity(i));
            citiesOnTour.insert(parent1.getCity(i).getID());
        }
        
        for (int i = split2; i < tSize; i++)
        {
            child.setCity(parent1.getCity(i));
            citiesOnTour.insert(parent1.getCity(i).getID());
        }
        
        int index = 0;
        while (child.getTourSize() != tSize)
        {
            if (citiesOnTour.count(parent2.getCity(index).getID()) == 0)
            {
                child.setCity(parent2.getCity(index));
            }
            
            index++;
        }
        
        return child;
    }

public:
    Population evolve(Population p)
    {
        int size = p.getPopSize();
        Population nextGen = Population();
        int eliteOffset = 0;
        
        if (elitism) // Saves fittest individual
        {
            eliteOffset = 1;
            nextGen.addTour(p.getFittest());
        }
        
        for (int i = eliteOffset; i < size; i++) // Create next generation
        {
            Tour parent1 = tournament(p);
            Tour parent2 = tournament(p, parent1);
            Tour child = cross(parent1, parent2);
            
            nextGen.addTour(child);
        }

        for (int i = eliteOffset; i < size; i++) // Mutate next generation
        {
            mutate(nextGen.getTour(i));
        }
        
        return nextGen;
    }
};

int main()
{
    cout << "Please enter the number of cities: " << endl;
    int N;
    cin >> N;
    
    int populationSize = 50;
    const int numberGenerations = 3 * N;
    
    Population p = Population(populationSize, N);
    Tour fittest = p.getFittest();
    Genetics g;

    cout << endl;
    
    for (int i = 0; i < numberGenerations; i++)
    {
        p = g.evolve(p);
        
        fittest = p.getFittest();
        
        if (i == 5)
        {
                   cout << "The 5th generation is: "<< fittest.getDistance() << endl;
        }
        
        if (i == 10)
        {
            cout << "The 10th generation is: "<< fittest.getDistance() << endl;
        }
        
        if (i == 14)
        {
                   cout << "The 14th generation is: "<< fittest.getDistance() << endl;
        }
        
        if (i == 16)
        {
                   cout << "The 16th generation is: "<< fittest.getDistance() << endl;
        }
    }
        
    fittest = p.getFittest();
        
    double improvedDistance = fittest.getDistance();
    
    cout << std::endl;
    
    cout << "The final generation is: " << improvedDistance << endl;
    cout << "The final route is:\n";
    for (int i = 0; i < N; i++)
    {
        cout << "(" << fittest.getCity(i).getX() << "," << fittest.getCity(i).getY() << ") -> ";
    }
        
    cout << "(" << fittest.getCity(0).getX() << "," << fittest.getCity(0).getY() << ")" << endl;
    
    return 0;
}

