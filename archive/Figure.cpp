#include "Figure.h"



Figure::Figure() {
	Level = 2;
	Considered = new std::vector<Relationship*>();
	Considered->push_back(new Relationship("Podobny rozmiar"));
	Considered->push_back(new Relationship("Prostopadly"));
	Considered->push_back(new Relationship("Rownolegly"));
	Considered->push_back(new Relationship("Przystaje"));
}


Figure::~Figure() {
}
