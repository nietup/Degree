#include "Relationship.h"


Relationship::Relationship(std::string n) : Name(n) {
}


Relationship::~Relationship() {
}

void Relationship::SetScoringFunction(std::function<char (LineSegment *, LineSegment *)> aScoringFunction) {
    ScoringFunction = aScoringFunction;
}
