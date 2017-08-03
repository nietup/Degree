#include <TGUI/TGUI.hpp>
#include "Cli.h"

using namespace std;

int main() {
    auto cli = Cli();
    cli.Run();

	return 0;
}

/*
for (auto & a : posSamples) {
cout << "\nSample:";
for (auto & line : a) {
cout << "\n(" << line->start.first << "; " << line->start.second << "), (" << line->end.first << "; " << line->end.second << ")";
}
}*/
