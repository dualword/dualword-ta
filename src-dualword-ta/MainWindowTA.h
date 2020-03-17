/* Dualword-ta (2020) http://github.com/dualword/dualword-ta License:GNU GPL*/

#ifndef MAINWINDOWTA_H_
#define MAINWINDOWTA_H_

#include <QtGui>
#include "ui_MainWindowTA.h"

#include "defs.h"

class QTAChart;

class MainWindowTA : public QMainWindow, private Ui::MainWindowForm {
	  Q_OBJECT

public:
	TableDataVector TDVector;
	QStringList sqlite_master_type, sqlite_master_name;
	MainWindowTA(QWidget *p = 0, Qt::WindowFlags f = 0);
	virtual ~MainWindowTA();
	void addChart (TableDataVector & datavector,QTAChart*);
	void fillTableDataVector (QString base, QString adjusted);

public slots:
	void loadCSV();
	void deleteEntry();
	void loadSymbols();
	QListWidget* getList() { return  listSymbol; };
	void currentRowChanged (int);

protected:
	virtual void resizeEvent (QResizeEvent * event);
	virtual void showEvent (QShowEvent * event);

private:
	void init();
	QString formSQLDropSentence (QString, qint32 *);

	AppSettings appsettings;
	SQLists comboitems;

};

#endif /* MAINWINDOWTA_H_ */
