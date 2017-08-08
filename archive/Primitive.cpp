#include "Primitive.h"



Primitive::Primitive() {
	Level = 1;
	Considered = new std::vector<Relationship*>();
	Considered->push_back(new Relationship("Podobny rozmiar"));
	Considered->push_back(new Relationship("Prostopadly"));
	Considered->push_back(new Relationship("Rownolegly"));
	Considered->push_back(new Relationship("Przystaje"));
	Considered->push_back(new Relationship("Kat"));
}


Primitive::~Primitive() {
}
