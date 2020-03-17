/* Dualword-ta (2020) http://github.com/dualword/dualword-ta License:GNU GPL*/

#ifndef DUALWORDTA_H_
#define DUALWORDTA_H_

#include <QApplication>

class MainWindowTA;

class DualwordTA : public QApplication {
    Q_OBJECT

public:
	DualwordTA(int &argc, char **argv);
	virtual ~DualwordTA();

public slots:
	void start();

private:
	MainWindowTA* w;

};

#endif /* DUALWORDTA_H_ */
