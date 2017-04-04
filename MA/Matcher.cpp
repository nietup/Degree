//
// Created by nietup on 21.03.17.
//

#include "Matcher.h"
using namespace std;


Matcher::Matcher(int t, vector<vector<Relationship*>*> * _parts, vector<LineSegment> * _segments) {
    int s = _segments->size();
    n = 0.5*s*(s-1);
    m = _parts->size();
    threshold = t;
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
    /*pair<int, int> seg = Unpair(x);
    vector<Relationship*> * part = (*parts)[y];
    for (Relationship * rel : *part) {
        char score = rel->Score(&(*segments)[seg.first], &(*segments)[seg.second]);
        cout << "\nScore of pair " << x << " to part " << y << " for constraint " << rel->GetName() << ": "
             << (int)score << endl;
        if (score < threshold) {
            cout << "rejected\n";
            return false;
        }
    }

    return true;*/
    bool res[6][3];
    res[0][0]=1;res[0][1]=1;res[0][2]=1;
    res[1][0]=1;res[1][1]=0;res[1][2]=0;
    res[2][0]=1;res[3][1]=1;res[2][2]=0;
    res[3][0]=0;res[4][1]=0;res[3][2]=0;
    res[4][0]=0;res[5][1]=0;res[4][2]=0;
    res[5][0]=0;res[6][1]=0;res[5][2]=0;

    return res[x][y];
}

void Matcher::InitMatch() {
    /*
     * Initial matching is finished in one of two conditions:
     * - matchedNo == m
     * - last pair was checked for matching
     */

	bool * matched = new bool[m];
	for (int i = 0; i < m; i++) {
        matchRight[i] = -1;
        matched[i] = false;
    }

    for (int i = 0; (i < n) && (matchedNo < m); i++) {
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

    //for debug purpose
    /*cout << "\ninit match:\n"
         << " n: " << n << " m: " << m << endl
         << " matchedNo: " << matchedNo << "\n"
         << " size of matchLeft: " << matchLeft->size() << "\n"
         << " matchRight: ";
    for (int i = 0; i < m; i++)
        cout << matchRight[i] << " ";
    cout << "\n matchLeft: ";
    for (int i = 0; i < n; i++)
        cout << (*matchLeft)[i] << " ";*/
}

/*issue
 * the algorithm doesnt work in the situation when
 * second edge should be chosen from the vector
 */
bool Matcher::CorrectMatches() {
    if (m == matchedNo)
        return false;

    int currentLeft = -1;
    int currentRight;
    for (int i = 0; i < n; i++) //szukamy pierwszego wolnego lewego
        if (-1 == (*matchLeft)[i]) {
            currentLeft = i;
            break;
        }
    if (-1 == currentLeft)
        return false;

    vector<int> alternatingPath;
    alternatingPath.push_back(currentLeft);

    while (true) {
        if (edges[currentLeft].size()) {cout<<"\nenter case 1";
            //loop in ap check
            for (int i = 0; i < alternatingPath.size(); i++)
                if (alternatingPath[i] == edges[currentLeft][0] &&
                    1 == i % 2) {
                    //-1 = matching NOT YET found
                    //-2 = matching impossible
                    (*matchLeft)[alternatingPath[0]] = -2;

                    return true;
                }

            currentRight = edges[currentLeft][0];
            alternatingPath.push_back(currentRight);
        }
        else {cout<<"\nenter case 2";
            (*matchLeft)[alternatingPath[0]] = -2;

            return true;
        }

        if (-1 != matchRight[currentRight]) {cout<<"\nenter case 3";
            currentLeft = matchRight[currentRight];
            alternatingPath.push_back(currentLeft);
        }
        else {cout<<"\nenter case 4";
            int size = alternatingPath.size();
            for (int i = 0; i < size; i += 2) {
                (*matchLeft)[alternatingPath[i]] = alternatingPath[i + 1];
                matchRight[alternatingPath[i + 1]] = alternatingPath[i];
            }
            matchedNo++;

            return true;
        }
    }
}

int Matcher::Pair(int x, int y) {
    return (int) (0.5 * (y * y - 2 * x + y) - 1);
}

pair<int, int> Matcher::Unpair(int z) {
    int y = (int) floor(0.5 * (sqrt(8.0 * z + 1.0) + 1.0));
    int x = (int) (0.5 * (y * y + y - 2 * z) - 1);
    pair<int, int> result(x,y);
    return result;
}