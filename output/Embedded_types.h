#ifndef EMBEDDED_TYPES_H
#define EMBEDDED_TYPES_H

#include <ctime>
#include <cstdlib>

class Path
{
public:
    int id;
    std::string country;
    float difficulty;
    float length;

    Path(int iid, float diff, float  len) : id(iid), difficulty(diff), length(len)
    {
        country = "FRA";
    }
};

class User
{
public:
    int id;
    float grade;
    std::string country;

    User(int number_of_user) : id(number_of_user)
    {
        srand(time(NULL));
        grade = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/10));
        country = "POL";
    }

    float rankRecomendation(Path p)
    {
        return 9.9;
    }
};

class Algorithm
{
public:
    std::string name;
    int pearsonGroupSize;
    float regParameter;
    float lambda;

    Algorithm(std::string name_of_alg) : name(name_of_alg)
    {
        pearsonGroupSize = 3;
        regParameter = 2.2;
        lambda = 3.95;
    }

    Path getRecomendation(User u)
    {
        if(u.grade < 5)
            return Path(1, 4.5, 3.0);
        else
            return Path(2, 5.8, 5.5);
    }
};


#endif // EMBEDDED_TYPES_H
