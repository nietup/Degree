//
// Created by nietup on 21.03.17.
//

#ifndef DEGREE_MATCHER_H
#define DEGREE_MATCHER_H
#include <vector>
#include <algorithm>
#include <string>
#include <iostream>
#include "MenuState.h"
#include <elsdpgmfilereader.hpp>
#include <elsdetector.hpp>
#include <elsdsvgwriter.hpp>

using namespace std;
using namespace elsd;

class Matcher {
public:
	Matcher(vector<vector<Relationship*>*> * model, vector<LineSegment> * segments);
	~Matcher();

	int * Match();

private:
    void InitMatch();
    bool CorrectMatches();
    bool Discover(int x, int y);    //checks if seg x matches model part y
    int Pair(int x, int y);
    pair<int, int> Unpair(int z);

	int n, //number of pairs of segments
        m, //number of parts of the model
        matchedNo;
    vector<int> * matchLeft;
    int * matchRight;
    vector<int> * edges;

    //i dont have edges available so i need to discover them
    //so i need a readonly pointer to pairs and parts and scoring function
    vector<vector<Relationship*>*> * parts;
    vector<LineSegment> * segments;
};

#endif //DEGREE_MATCHER_H
