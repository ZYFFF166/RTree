#include <iostream>
#include <vector>
#include <string>
#include <stdlib.h>
#include <algorithm>
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point.hpp>
#include <boost/geometry/index/rtree.hpp>


using namespace std;

namespace bg = boost::geometry;
namespace bgi = boost::geometry::index;

typedef bg::model::point<float, 2, bg::cs::cartesian> point;
typedef std::pair<point, unsigned> value;

//Create class Creature

class Creature{
  public:
    Creature() = default;
    point p;
    list<point> nearbycreatures;//This list is use to store the nearby creatures using RTree algorithom
    list<point> nearbycreatures_check;//This list is use to store the nearby creatures using naive algorithom
  
};

//Declaration of the function
void RTree(bgi::rtree< value, bgi::quadratic<16> > & rtree, std::array<Creature, 100> & creatures);//FastNearbyCreatures
void Naive(std::array<Creature, 100> & creatures);//NaiveNearbyCreatures
bool Validation(array<Creature, 100> & creatures);

int main()
{
    bgi::rtree< value, bgi::quadratic<16> > rtree;

    srand((unsigned)time(0));
    std::array<Creature, 100> creatures;
    for(int i=0;i<100;i++)
    {
        //Randomly generate the Location of each Creature to within a cube of length 100 on each side.
        creatures[i].p.set<0>(1.0 * rand() / RAND_MAX * 100);
        creatures[i].p.set<1>(1.0 * rand() / RAND_MAX * 100);
        //insert each creature into RTree
        rtree.insert(std::make_pair(creatures[i].p, i));
    }

    // Call algorithm functions
    RTree(rtree, creatures);
    Naive(creatures);
    bool validation_result = Validation(creatures);
    if(validation_result==1)
    cout << "Validation passed" << endl;//this will print the validation result of the two algorithm

    // Print nearby creatures coordinates of each creature 
    for(int i=0;i<100;i++)
    {
        cout<<"Creature "<< i << " Nearby creatures coordinates: " <<endl;
        for(auto it=creatures[i].nearbycreatures.begin();it != creatures[i].nearbycreatures.end();it++)
        {
            cout<< "(" << it->get<0>() << "," << it->get<1>() << ")"<<endl;
        }
        cout << "----------------------" << endl;
    }


    return 0;
}

//----------------------RTree Algorithm--------------------------

void RTree(bgi::rtree< value, bgi::quadratic<16> > & rtree, std::array<Creature, 100> & creatures)
{
    // Using RTree search for within 10 creatures
    for(int i=0;i<100;i++)
    {
        std::vector<value> returned_values;
        point & tar = creatures[i].p;
        // Search RTree for all the point that within the distance of 10 and add it to the vector returned_value
        rtree.query(bgi::satisfies([&](value const& v) {return bg::distance(v.first, tar) < 10;}),
        std::back_inserter(returned_values));

        size_t len = returned_values.size();
        // Add each point to the nearbycreatures list
        for (size_t j =0; j < len; j ++) 
        {
            creatures[i].nearbycreatures.emplace_back(returned_values[j].first); 
        }

    }
} 

//----------------------Naive Algorithm--------------------------

void Naive(array<Creature, 100> & creatures)
{
    for (int i = 0 ; i < 100 ; i++)
    {
        point & tar = creatures[i].p;
        //For each point in the creatures to check whether it is within distance
        for(int j = 0 ; j <100 ; j++)
        {
            if(bg::distance(creatures[j].p, tar) < 10)
            {
                creatures[i].nearbycreatures_check.emplace_back(creatures[j].p);
            }
        }
    }
} 

//----------------------Validation--------------------------

bool Validation(array<Creature, 100> & creatures)
{
    bool result = true;// I use two bool to make sure that each element in nearbycreatures of each creature is contained in nearbycreatures_check, vice versa
    for(int i=0;i<100;i++)
    {
        for(auto it=creatures[i].nearbycreatures.begin();it != creatures[i].nearbycreatures.end();it++)
        {
            bool found = false;   
            for(auto it2=creatures[i].nearbycreatures_check.begin();it2 != creatures[i].nearbycreatures_check.end();it2++)
            {
                if(it->get<0>()==it2->get<0>() && it->get<1>()==it2->get<1>()) 
                {
                    found = true;
                }

            }
            result = result&found;
            

        }
        

    }
    return result;
}


