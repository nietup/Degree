//
// Created by nietup on 21.03.17.
//

#include "Matcher.h"
using namespace std;


Matcher::Matcher(int pairsNo, int partsNo, vector<vector<Relationship*>*> * _parts, vector<LineSegment> * _segments) {
    n = pairsNo;
    m = partsNo;
    matchedNo = 0;
    parts = _parts;
    segments = _segments;
    matchLeft = new vector<int>;
    matchRight = new int[m];

}

Matcher::~Matcher() {
}

int * Matcher::Match() {
	InitMatch();
	while (CorrectMatches());
    return matchRight;
}

bool Matcher::Discover(int x, int y) {
    return true;
}

void Matcher::InitMatch() {
    /*
     * Initial matching is finished in one of two conditions:
     * - matchedNo == m
     * - last pair was checked for matching
     */

	bool * matched = new bool[m];
	for (int i = 0; i < m; i++)
		matched[i] = false;

/*for (int i = 0; i < n; i++) {
		int clean = -1;
		int j = 0;
		for (auto e : edges[i]) {
			if (!matched[e]) {
				matchLeft[i] = e;
				matchRight[e] = i;
				matched[e] = true;
				matchedNo++;
				clean = j;
				break;
			}
			j++;
		}
		if (-1 != clean)
			edges[i].erase(edges[i].begin() + j);
	}
	delete matched;*/


    for (int i = 0; (i < n) && (matchedNo < m); i++){
        bool wasMatched = false;
        for (int j = 0; j < m; j++) {
            if (!matched[j] && Discover(i, j)) {
                matched[j] = true;
                matchRight[j] = i;
                matchLeft->push_back(j);
                matchedNo++;
                wasMatched = true;
                break;
            }
        }
        if (!wasMatched)
            matchLeft->push_back(-1);
    }

    cout << "\ninit match:\n" << " matchedNo: " << matchedNo << "\n"
         << " size of matchLeft: " << matchLeft->size() << "\n"
         << " matchRight: ";
    for (int i = 0; i < m; i++)
        cout << matchRight[i] << " ";
}

bool Matcher::CorrectMatches() {
/*
	if (n == matchedNo)
		return false;

	int currentLeft = -1;
	int currentRight;
	for (int i = 0; i < n; i++)
		if (-1 == matchLeft[i]) {
			currentLeft = i;
			break;
		}
	if (-1 == currentLeft)
		return false;

	vector<int> alternatingPath;
	alternatingPath.push_back(currentLeft);

	while (true) {
		if (edges[currentLeft].size()) {
			//loop in ap check
			for (int i = 0; i < alternatingPath.size(); i++)
				if (alternatingPath[i] == edges[currentLeft][0] &&
					1 == i % 2) {
					//-1 = matching NOT YET found
					//-2 = matching impossible
					matchLeft[alternatingPath[0]] = -2;

					return true;
				}

			currentRight = edges[currentLeft][0];
			alternatingPath.push_back(currentRight);
		}
		else {
			matchLeft[alternatingPath[0]] = -2;

			return true;
		}

		if (-1 != matchRight[currentRight]) {
			currentLeft = matchRight[currentRight];
			alternatingPath.push_back(currentLeft);
		}
		else {
			int size = alternatingPath.size();
			for (int i = 0; i < size; i += 2) {
				matchLeft[alternatingPath[i]] = alternatingPath[i + 1];
				matchRight[alternatingPath[i + 1]] = alternatingPath[i];
			}
			matchedNo++;

			return true;
		}
	}*/


    return false;
}
