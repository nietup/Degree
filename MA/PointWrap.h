//
// Created by nietup on 11.04.17.
//

#ifndef DEGREE_POINTWRAP_H
#define DEGREE_POINTWRAP_H
#include <elsdshapes.hpp>
#include <cmath>

/*! Simple wrapper for elsd::Point
 *
 */

class PointWrap {
public:
    PointWrap(elsd::Point);
    PointWrap(double a, double b);
    double Distance(const PointWrap & p) const;
    double Distance(elsd::Point & p) const;
    double operator[](uint) const;

    double first;
    double second;
private:
    elsd::Point point;
};


#endif //DEGREE_POINTWRAP_H
