#include "Solid.h"



Solid::Solid() {
	Considered = new std::vector<Relationship*>();
	Considered->push_back(new Relationship("Orientacja"));
	Considered->push_back(new Relationship("Objetosc"));
	Considered->push_back(new Relationship("Liczba scian"));
}


Solid::~Solid() {
}
