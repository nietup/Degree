#include <TGUI/TGUI.hpp>
#include "Cli.h"

using namespace std;

int main() {
    auto cli = Cli();
    cli.parseSVGLine("<line x1=\"356.83\" y1=\"371.893\" x2=\"354.136\" y2=\"175.645\" fill=\"none\" stroke =\"green\" stroke-width=\"1\" />");
    //cli.Run();

	return 0;
}


