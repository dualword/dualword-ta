/* Dualword-ta (2020) http://github.com/dualword/dualword-ta License:GNU GPL*/

#include "DualwordTA.h"

int main(int argc, char *argv[]) {
	DualwordTA app(argc, argv);
	app.start();
	return app.exec();
}

