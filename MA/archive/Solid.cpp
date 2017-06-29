#include "Solid.h"



Solid::Solid() {
	Level = 3;
	Considered = new std::vector<Relationship*>();
	Considered->push_back(new Relationship("Przystaje"));
	Considered->push_back(new Relationship("Podobny ksztalt"));
	Considered->push_back(new Relationship("Podobna powierzchnia"));
}


Solid::~Solid() {
}
