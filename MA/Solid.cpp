#include "Solid.h"



Solid::Solid() {
	Considered->push_back(new Relationship("Orientacja"));
	Considered->push_back(new Relationship("Objetosc"));
	Considered->push_back(new Relationship("Liczba scian"));
}


Solid::~Solid() {
}
