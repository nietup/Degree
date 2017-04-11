//
// Created by nietup on 21.03.17.
//

#include "Matcher.h"
using namespace std;


Matcher::Matcher(int t, vector<vector<Relationship*>*> * _parts, vector<LineWrap> * _segments) {
    int s = _segments->size();
    n = 0.5*s*(s-1);
    m = _parts->size();
    threshold = t;
    matchedNo = 0;
    parts = _parts;
    segments = _segments;
    //matchLeft = new vector<int>;
    matchLeft = new int[n];
    matchRight = new int[m];

    edges = new vector<int>[n];
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (Discover(i, j)) {
                edges[i].push_back(j);
            }
        }
    }
}

Matcher::~Matcher() {
}

int * Matcher::Match() {
	InitMatch();
	while (CorrectMatches());
    return matchRight;
}

bool Matcher::Discover(int x, int y) {
    pair<int, int> seg = Unpair(x);
    vector<Relationship*> * part = (*parts)[y];
    for (Relationship * rel : *part) {
        int score = rel->Score(&(*segments)[seg.first], &(*segments)[seg.second]);
        cout << "\nScore of pair " << x << " to part " << y << " for constraint " << rel->GetName() << ": "
             << score << endl;
        if (score < threshold) {
            cout << "rejected\n";
            return false;
        }
    }

    return true;
    //some testing cases
    /*bool res[6][3];

    res[0][0]=1;res[0][1]=1;res[0][2]=1;
    res[1][0]=1;res[1][1]=0;res[1][2]=0;
    res[2][0]=1;res[2][1]=1;res[2][2]=0;
    res[3][0]=0;res[3][1]=0;res[3][2]=0;
    res[4][0]=0;res[4][1]=0;res[4][2]=0;
    res[5][0]=0;res[5][1]=0;res[5][2]=0;

    return res[x][y];*/
}

void Matcher::InitMatch() {
    /*
     * Initial matching is finished in one of two conditions:
     * - matchedNo == m
     * - last pair was checked for matching
     */

    matchLeft = new int[n];
    matchRight = new int[n];
    bool * matched = new bool[n];
    for (int i = 0; i < n; i++) {
        matchLeft[i] = matchRight[i] = -1;
        matched[i] = false;
    }
    for (int i = 0; i < n; i++) {
        int clean = -1;
        int j = 0;
        /*
         * foreach e in edges of i-th left vertex do
         *  if right vertex of e is not matched then match end
         * end
         */
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
    delete matched;

    //for debug purpose
    /*cout << "\ninit match:\n"
         << " n: " << n << " m: " << m << endl
         << " matchedNo: " << matchedNo << "\n"
         << " matchRight: ";
    for (int i = 0; i < m; i++)
        cout << matchRight[i] << " ";
    cout << "\n matchLeft: ";
    for (int i = 0; i < n; i++)
        cout << matchLeft[i] << " ";*/
}

bool Matcher::CorrectMatches() {
    if (m == matchedNo)
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
        int esize = edges[currentLeft].size();
        if (esize) {
            //loop in ap check
            //check if there is a vertex on the right size that is:
            // connected with current left vertex (== is in edges[currentLeft])
            // not yed added to alternating path  (== is not in alternatingPath)
            int notAPVertex = -1;
            for (auto &p : edges[currentLeft]) {
                for (auto &q : alternatingPath)
                    if (q == p) break;
                notAPVertex = p;
                break;
            }

            //failure
            if (-1 == notAPVertex) {
                //-1 = matching NOT YET found
                //-2 = matching impossible
                matchLeft[alternatingPath[0]] = -2;
                return true;
            }

            //success
            if (0 <= notAPVertex) {
                currentRight = edges[currentLeft][notAPVertex];
                alternatingPath.push_back(currentRight);
            }
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