/* Dualword-ta (2020) http://github.com/dualword/dualword-ta License:GNU GPL*/

#include "DualwordTA.h"
#include "MainWindowTA.h"

DualwordTA::DualwordTA(int &argc, char **argv) : QApplication(argc, argv) {
	setApplicationName("Dualword-TA");
	QApplication::setQuitOnLastWindowClosed(true);

}

DualwordTA::~DualwordTA() {

}

void DualwordTA::start() {
	w = new MainWindowTA();
	w->show();
}
