#include "Abstract.h"



Abstract::Abstract() {
	Considered = new std::vector<Relationship*>();
	Considered->push_back(new Relationship("Podobienstwo"));
	Considered->push_back(new Relationship("Podobny rozmiar"));
	Considered->push_back(new Relationship("Ksztalt"));
	Considered->push_back(new Relationship("Podobienstwo"));
	Considered->push_back(new Relationship("Podobny rozmiar"));
	Considered->push_back(new Relationship("Ksztalt"));
	Considered->push_back(new Relationship("Podobienstwo"));
	Considered->push_back(new Relationship("Podobny rozmiar"));
	Considered->push_back(new Relationship("Ksztalt"));
	Considered->push_back(new Relationship("Podobienstwo"));
	Considered->push_back(new Relationship("Podobny rozmiar"));
	Considered->push_back(new Relationship("Ksztalt"));
}


Abstract::~Abstract() {
}
