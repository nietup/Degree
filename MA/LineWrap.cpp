//
// Created by nietup on 11.04.17.
//

#include "LineWrap.h"

LineWrap::LineWrap(elsd::LineSegment a) : start(a.startPoint), end(a.endPoint){}
LineWrap::LineWrap(std::pair<double, double> a, std::pair<double, double> b) :
    start(a.first, a.second), end(b.first, b.second) {}

double LineWrap::Distance(const PointWrap & p) const {
    double x1 = start[0],
           y1 = start[1],
           x2 = end[0],
           y2 = end[1],
           lineLen = Length(),
           dx = x2 - x1,
           dy = y2 - y1,
           u = (((p[0] - x1) * dx) + ((p[1] - y1) * dy) / (lineLen * lineLen));

    //closest to edge
    if (u < 0.0 || u > 1.0) {
        double startLen = start.Distance(p);
        double endLen   =   end.Distance(p);
        return startLen < endLen ? startLen : endLen;
    }
        //closest to middle
    else {
        PointWrap intersection(x1 + u * dx, y1 + u * dy);
        return intersection.Distance(p);
    }
}

double LineWrap::Distance(const LineWrap & l) const {
    double res  = l.Distance(start),
           nval = l.Distance(end);
    res  = res < nval ? res : nval;
    nval = Distance(l.GetStart());
    res  = res < nval ? res : nval;
    nval = Distance(l.GetEnd());
    return res < nval ? res : nval;
}

double LineWrap::Length() const {
    double x1 = start[0],
           y1 = start[1],
           x2 = end[0],
           y2 = end[1];
    return sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1));
}

double LineWrap::Dot(const LineWrap & a) const {
    double ax1 = a.GetStart()[0],
           ay1 = a.GetStart()[1],
           ax2 = a.GetEnd()[0],
           ay2 = a.GetEnd()[1],
            x1 =   start[0],
            y1 =   start[1],
            x2 =   end[0],
            y2 =   end[1];
    return (ax2 - ax1) * (x2 - x1) + (ay2 - ay1) * (y2 - y1);
}

double LineWrap::GetCos(const LineWrap & a) const {
    return Dot(a) / (Length() * a.Length());
}
