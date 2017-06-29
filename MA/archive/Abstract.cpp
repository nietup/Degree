#include "Abstract.h"



Abstract::Abstract() {
	Level = 4;
	Considered = new std::vector<Relationship*>();
	Considered->push_back(new Relationship("Przystaje"));
	Considered->push_back(new Relationship("Podobny ksztalt"));
	Considered->push_back(new Relationship("Podobna objetosc"));
}


Abstract::~Abstract() {
}
