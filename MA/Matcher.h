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
	Matcher(int, int, vector<vector<Relationship*>*> *, vector<LineSegment> *);
	~Matcher();

	vector<int> Match();
	void InitMatch();
	bool CorrectMatches();

private:
	int n, //number of pairs of segments
        m, //number of parts of the model
        matchedNo;
    vector<int> * matchLeft;
    vector<int> * matchRight;
    vector<int> * edges;

    //i dont have edges available so i need to discover them
    //so i need a readonly pointer to pairs and parts and scoring function
    vector<vector<Relationship*>*> * parts;
    vector<LineSegment> * segments;
};

#endif //DEGREE_MATCHER_H
