#include "Abstract.h"



Abstract::Abstract() {
	Considered->push_back(new Relationship("Podobienstwo"));
	Considered->push_back(new Relationship("Podobny rozmiar"));
	Considered->push_back(new Relationship("Ksztalt"));
}


Abstract::~Abstract() {
}
