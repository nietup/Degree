//
// Created by nietup on 21.03.17.
//

#include "Matcher.h"
using namespace std;


Matcher::Matcher(double t, vector<vector<Relationship*>*> * _parts,
    vector<LineWrap> * _segments) {
    int s = _segments->size();
    n = 0.5*s*(s-1);
    m = _parts->size();
    threshold = t;
    matchedNo = 0;
    parts = _parts;
    segments = _segments;
    matchLeft = new int[n];
    matchRight = new int[m];

    /* TODO
     * CSP with backtracking
     */
    cout << "s: " << s << " n: " << n << " m: " << m << "\n";
    edges = new vector<int>[n];
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++)
            if (Discover(i, j))
                edges[i].push_back(j);
}

Matcher::~Matcher() {
}

double linegryba = 1;
void SaveDetection(vector<LineWrap> save) {
    string inFile = "./7.pgm";
    string outFile = inFile + ".detection.svg";
    ofstream ofs(outFile, ofstream::out);
    ofs << "<?xml version=\"1.0\" standalone=\"no\"?>" << endl
        << "<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\""
        << " \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">" << endl
        << "<svg width=\"500\" height=\"500\" version=\"1.1\""
        << " xmlns=\"http://www.w3.org/2000/svg\""
        << " xmlns:xlink=\"http://www.w3.org/1999/xlink\">" << endl;
    for (LineWrap l : save) {
        ofs << "<line x1=\"" << l.GetStart()[0] << "\" y1=\"" << l.GetStart()[1]
            << "\" x2=\"" << l.GetEnd()[0] << "\" y2=\"" << l.GetEnd()[1]
            << "\" fill=\"none\" stroke =\"green\" stroke-width=\""
            << floor(linegryba)
            << "\" />" << endl;
        linegryba+=0.5;
    }
    ofs << "</svg>";
    ofs.close();
}

int * Matcher::Match() {
	InitMatch();
	while (CorrectMatches());
    cout << "\nFinal match right:";
    for (int i = 0; i < m; i++)
        cout << " " << matchRight[i];
    cout << "\n\nLine information:";
    vector<LineWrap> save;
    for (int i = 0; i < m; i++) {
        pair<int, int> p = Unpair(matchRight[i]);
        int p1 = p.first;
        int p2 = p.second;
        LineWrap l1 = (*segments)[p1];
        LineWrap l2 = (*segments)[p2];
        save.push_back(l1);
        save.push_back(l2);
        PointWrap pw1s = l1.GetStart();
        PointWrap pw1e = l1.GetEnd();
        PointWrap pw2s = l2.GetStart();
        PointWrap pw2e = l2.GetEnd();
        cout << "\npair " << matchRight[i] << "->[" <<p1 << ", " << p2 << "]\n"
             << "line " << p1 << ":\nx1: " << pw1s[0] << " y1: " << pw1s[1] << endl
             << "x2: " << pw1e[0] << " y2: " << pw1e[1] << endl
             << "line " << p2 << ":\nx1: " << pw2s[0] << " y1: " << pw2s[1] << endl
             << "x2: " << pw2e[0] << " y2: " << pw2e[1] << endl;
    }
    SaveDetection(save);
    return matchRight;
}

bool Matcher::Discover(int x, int y) {
    pair<int, int> seg = Unpair(x);
    cout << "Unpair(x=" << x << "): [" << seg.first << ", " << seg.second << "]\n";
    vector<Relationship*> * part = (*parts)[y];
    cout << "Number of relationships in part y=" << y << ": " << part->size();
    for (Relationship * rel : *part) {
        double score = rel->Score(&(*segments)[seg.first], &(*segments)[seg.second]);
        cout << "\nConstraint: " << rel->GetName() << "\nScore: "
             << score << endl;
        if (score > threshold) {
            cout << "rejected\n";
            cout << "--------------------------------------\n";
            return false;
        }
    }
    cout << "--------------------------------------\n";
    return true;
}

void Matcher::InitMatch() {
    /*
     * Initial matching is finished in one of two conditions:
     * - matchedNo == m
     * - last pair was checked for matching
     */

    matchLeft = new int[n];
    matchRight = new int[m];
    bool * matched = new bool[n];
    for (int i = 0; i < n; i++) {
        matchLeft[i] = -1;
        matched[i] = false;
    }
    for (int i = 0; i < m; i++)
        matchRight[i] = -1;
    for (int i = 0; i < n; i++) {
        int clean = -1;
        int j = 0;
        /*
         * foreach e in edges of i-th left vertex do
         *  if right vertex of e is not matched then
         *   match e
         *  end
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
        //remove matched edge so we don't check it later when searching for ap
        if (-1 != clean)
            edges[i].erase(edges[i].begin() + j);
    }
    delete matched;

    //for debug purpose
    cout << "\ninit match:\n"
         << " n: " << n << " m: " << m << endl
         << " matchedNo: " << matchedNo << "\n"
         << " matchRight:       ";
    for (int i = 0; i < m; i++)
        cout << matchRight[i] << " ";
    cout << "\n matchLeft:        ";
    for (int i = 0; i < n; i++)
        cout << matchLeft[i] << " ";
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
