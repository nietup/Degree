//
// Created by nietup on 11.04.17.
//

#include "PointWrap.h"

PointWrap::PointWrap(elsd::Point p) : point(p) {}

PointWrap::PointWrap(double a, double b) {
    point[0] = a;
    point[1] = b;
}

double PointWrap::Distance(const PointWrap & p) const {
    double x1 = point[0],
           y1 = point[1],
           x2 = p[0],
           y2 = p[1];
    return sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1));
}

double PointWrap::Distance(elsd::Point & p) const {
    double x1 = point[0],
            y1 = point[1],
            x2 = p[0],
            y2 = p[1];
    return sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1));
}

double PointWrap::operator[](uint i) const {
    return point[i];
}