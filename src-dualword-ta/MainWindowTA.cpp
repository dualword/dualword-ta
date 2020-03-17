/* Dualword-ta (2020) http://github.com/dualword/dualword-ta License:GNU GPL*/

#include "MainWindowTA.h"

#include "sqlite3.h"
#include "qtachart.h"
#include "common.h"

MainWindowTA::MainWindowTA(QWidget *p, Qt::WindowFlags f) : QMainWindow(p, f) {
	setupUi(this);
	setWindowTitle(qApp->applicationName());
	setAttribute(Qt::WA_DeleteOnClose);
	actionCSV->setIcon(style()->standardIcon(QStyle::SP_FileDialogStart, 0, this));
    connect(actionCSV,SIGNAL(triggered()), SLOT(loadCSV()));
	actionDelete->setIcon(style()->standardIcon(QStyle::SP_TrashIcon, 0, this));
    connect(actionDelete,SIGNAL(triggered()), SLOT(deleteEntry()));
    connect(listSymbol,SIGNAL(currentRowChanged(int)), SLOT(currentRowChanged(int)));
    init();
}

MainWindowTA::~MainWindowTA() {

}

void MainWindowTA::resizeEvent (QResizeEvent * event) {
	QTAChart* chart = qobject_cast<QTAChart*>(tachart);
	if(chart) chart->goBack();
}

void MainWindowTA::showEvent (QShowEvent * event) {
	loadSymbols();
}

void MainWindowTA::loadCSV() {
	LoadCSVDialog csv(this);
	csv.exec();
	loadSymbols();
}

void MainWindowTA::currentRowChanged (int i){
	if(i == -1){
		QTAChart* chart = qobject_cast<QTAChart*>(tachart);
		if(chart){
			QWidget* o = new QWidget (this);
			  hLayout->addWidget(o,12);
			  hLayout->removeWidget(tachart);
			  delete tachart;
			  tachart=o;
		}
		return;
	}
	QTAChart* o = new QTAChart (this);
	QListWidgetItem* item= listSymbol->item(i);

	TableDataClass tdc;
	tdc.tablename = item->data(Qt::UserRole).toString();
	tdc.symbol =listSymbol->item(i)->text();
	tdc.timeframe="DAY";
	tdc.name="";
	tdc.adjusted="NO";
	tdc.base =item->data(Qt::UserRole).toString();
	tdc.market ="NONE";
	tdc.source ="CSV";

	o->setVisible(false);
	hLayout->addWidget(o,12);
	fillTableDataVector(tdc.base,tdc.adjusted);
	addChart(TDVector,o);
	hLayout->removeWidget(tachart);
	delete tachart;
	tachart=o;
	o->setVisible(true);
}

/* Modified FreeChartGeany function */
void MainWindowTA::init() {
	int rc, dbversion;
	Application_Settings = &appsettings;
	ComboItems = &comboitems;
	strcpy (comboitems.formats_query, "select FORMAT from FORMATS");
	strcpy (comboitems.timeframes_query, "select TIMEFRAME from TIMEFRAMES_ORDERED");
	strcpy (comboitems.currencies_query, "select SYMBOL from CURRENCIES");
	strcpy (comboitems.markets_query, "select MARKET from MARKETS");
	strcpy (comboitems.datafeeds_query, "select FEEDNAME from DATAFEEDS");

	appsettings.sqlitefile = QDir::homePath () + "/" + ".config" + "/" + APPDIR + "/" + DBNAME;
	QFileInfo dbfile;
	dbfile.setFile (appsettings.sqlitefile);
	if (dbfile.exists () == false)
	{
	if (!QDir (QDir::homePath () + "/" + ".config" + "/" + APPDIR).exists ())
	  QDir ().mkpath (QDir::homePath () + "/" + ".config" + "/" + APPDIR);

	rc = sqlite3_open(qPrintable (appsettings.sqlitefile), &appsettings.db);
	if (rc != SQLITE_OK)
	{
	  showMessage (QString::fromUtf8 ("Cannot create or open database. Application quits."));
	  sqlite3_close (appsettings.db);
	  qApp->exit (1);
	  exit (1);
	}
	QString SQLCommand = "";
	rc = sqlite3_exec(Application_Settings->db, SQLCommand.toUtf8(), NULL, this, NULL);
	if (rc != SQLITE_OK)
	{
	  showMessage (QString::fromUtf8 ("Cannot create or open database. Application quits."));
	  sqlite3_close (appsettings.db);
	  qApp->exit (1);
	  exit (1);
	}

	if (dbman (1, appsettings) != CG_ERR_OK)
	{
	  showMessage (QString::fromUtf8 ("Cannot create or open database. Application quits."));
	  qApp->exit (1);
	  exit (1);
	}

	}
	else
	{
	rc = sqlite3_open(qPrintable (appsettings.sqlitefile), &appsettings.db);
	if (rc != SQLITE_OK)
	{
	  showMessage (QString::fromUtf8 ("Cannot create or open database. Application quits."));
	  sqlite3_close (appsettings.db);
	  qApp->exit (1);
	  exit (1);
	}
	}
	loadSymbols();
}

/* Modified FreeChartGeany function */
void MainWindowTA::addChart (TableDataVector & datavector,QTAChart* tachart)
{

  QTAChartData Data;
  TableDataClass tdc;
  QString SQLCommand, title, subtitle, tablename,
          symbol, timeframe, name, source;
  int rc;
  CG_ERR_RESULT result = CG_ERR_OK;

  foreach (tdc, datavector)
  {
	tablename = tdc.tablename;
	symbol = tdc.symbol;
	source = tdc.source;
	timeframe = tdc.timeframe;
	name = tdc.name;

    Frames.clear ();
    SQLCommand = "select * from " + tablename + ";";
    rc = sqlite3_exec(Application_Settings->db, SQLCommand.toUtf8(),
                      sqlcb_dataframes, NULL, NULL);
    if (rc != SQLITE_OK)
    {
	  delete tachart;
      result = CG_ERR_ACCESS_DATA;
      setGlobalError(result, __FILE__, __LINE__);
      showMessage ("Symbol " +symbol +": " + errorMessage (result));
    }

    if ((tdc.market.trimmed ().toUpper ().left (3) == "LON" ||
         tdc.market.trimmed ().toUpper ().left (3) == "LSE")&&
        Application_Settings->options.longbp)
    {
	   QTAChartFrame frame;

	   for (qint32 counter = 0; counter < Frames.count (); counter ++)
	   {
		 frame = Frames[counter];
		 frame.High /= 100;
		 frame.Low /= 100;
		 frame.Open /= 100;
		 frame.Close /= 100;
		 frame.AdjClose /= 100;
		 Frames[counter] = frame;
       }
    }

    if (timeframe == "DAY")
      tachart->loadFrames (Frames, "D", timeframe);
    else
    if (timeframe == "WEEK")
      tachart->loadFrames (Frames, "W", timeframe);
    else
    if (timeframe == "MONTH")
      tachart->loadFrames (Frames, "M", timeframe);
    else
    if (timeframe == "YEAR")
      tachart->loadFrames (Frames, "Y", timeframe);
  }

  tachart->setSymbolKey (datavector[0].tablename);

  SQLCommand = "select * from basedata where base = '" + datavector[0].base + "'";
  rc = sqlite3_exec(Application_Settings->db, SQLCommand.toUtf8(),
                      sqlcb_fundamentals, (void *) &Data, NULL);
  if (rc != SQLITE_OK)
  {
	delete tachart;
    result = CG_ERR_ACCESS_DATA;
    setGlobalError(result, __FILE__, __LINE__);
    showMessage ("Symbol " +symbol +": " + errorMessage (result));
  }

  tachart->loadData (Data);
  tachart->setSymbol (symbol);
  tachart->setFeed (source);
  title = symbol;
  subtitle = name;
  tachart->setAlwaysRedraw (true);
  tachart->setTitle (title, subtitle);

}

/* Modified FreeChartGeany function */
void MainWindowTA::loadSymbols ()
{
	listSymbol->clear();
	int rc;
	const char query[] = "select SYMBOL, DESCRIPTION, MARKET, SOURCE, TIMEFRAME, \
						DATEFROM, DATETO, CURRENCY, KEY, ADJUSTED, BASE from SYMBOLS_ORDERED where adjusted='NO'";

	rc = sqlite3_exec(Application_Settings->db, query, sqlcb_symbol_table2, this, NULL);
	if (rc != SQLITE_OK)
	{
	setGlobalError(CG_ERR_DBACCESS, __FILE__, __LINE__);
	showMessage (errorMessage (CG_ERR_DBACCESS));
	return;
	}

	if(listSymbol->count() > 0) listSymbol->setCurrentRow(0,QItemSelectionModel::Select);

}

/* Modified FreeChartGeany function */
int sqlcb_symbol_table2(void *classptr, int argc, char **argv, char **column)
{
	MainWindowTA *w = reinterpret_cast <MainWindowTA *> (classptr);

	QListWidgetItem* item = new QListWidgetItem();
	for (qint32 counter = 0; counter < argc; counter ++)
	{
	QString colname = QString::fromUtf8(column[counter]);
	colname = colname.toUpper ();
	if (colname == "SYMBOL"){
		item->setText(argv[counter]);
	}else if(colname == "BASE"){
		  item->setData(Qt::UserRole,argv[counter]);
		  w->getList()->addItem(item);
	}
	}
}

static int /* Modified FreeChartGeany function */
sqlcb_table_data (void *classptr, int argc, char **argv, char **column)
{
	MainWindowTA *w = reinterpret_cast <MainWindowTA *> (classptr);
	TableDataClass tdc;

	for (qint32 counter = 0; counter < argc; counter ++)
	{
	QString colname = QString::fromUtf8(column[counter]);
	colname = colname.toUpper ();

	if (colname == "KEY")
	  tdc.tablename = QString::fromUtf8 (argv[counter]).toUpper ();
	if (colname == "SYMBOL")
	  tdc.symbol = QString::fromUtf8 (argv[counter]).toUpper ();
	if (colname == "TIMEFRAME")
	  tdc.timeframe = QString::fromUtf8 (argv[counter]).toUpper ();
	if (colname == "DESCRIPTION")
	  tdc.name = QString::fromUtf8 (argv[counter]).toUpper ();
	if (colname == "ADJUSTED")
	  tdc.adjusted = QString::fromUtf8 (argv[counter]).toUpper ();
	if (colname == "BASE")
	  tdc.base = QString::fromUtf8 (argv[counter]).toUpper ();
	if (colname == "MARKET")
	  tdc.market = QString::fromUtf8 (argv[counter]).toUpper ();
	if (colname == "SOURCE")
	  tdc.source = QString::fromUtf8 (argv[counter]).toUpper ();
	}
	w->TDVector += tdc;

	return 0;
}

/* Modified FreeChartGeany function */
void MainWindowTA::fillTableDataVector (QString base, QString adjusted)
{
  QString query = "";
  int rc;

  query += "SELECT key, symbol,  timeframe, description, adjusted, base, market, source ";
  query += "FROM symbols WHERE base = '" + base + "' AND ";
  query += "ADJUSTED = '" + adjusted + "' ORDER BY tfresolution ASC;";

  TDVector.clear ();
  rc = sqlite3_exec(Application_Settings->db, query.toUtf8(),
                    sqlcb_table_data, this, NULL);
  if (rc != SQLITE_OK)
  {
    setGlobalError(CG_ERR_DBACCESS, __FILE__, __LINE__);
    showMessage (errorMessage (CG_ERR_DBACCESS));
  }

}

static int /* Modified FreeChartGeany function */
sqlcb_sqlite_master (void *classptr, int argc, char **argv, char **column)
{
	MainWindowTA *w = reinterpret_cast <MainWindowTA *> (classptr);

	for (qint32 counter = 0; counter < argc; counter ++)
	{
	QString colname = QString::fromUtf8(column[counter]);
	colname = colname.toUpper ();
	if (colname == "TYPE")
	  w->sqlite_master_type << QString::fromUtf8 (argv[counter]);
	if (colname == "NAME")
	  w->sqlite_master_name << QString::fromUtf8 (argv[counter]);
	}

	return 0;
}

/* Modified FreeChartGeany function */
QString MainWindowTA::formSQLDropSentence (QString table, qint32 *nentries)
{
	QString query, SQLCommand = "";
	int rc;

	*nentries = 0;
	query = "select type, name from sqlite_master where name like '" +
				table + "%';";
	rc = sqlite3_exec(Application_Settings->db, query.toUtf8(), sqlcb_sqlite_master, this, NULL);

	if (rc == SQLITE_OK)
	for (qint32 counter = 0, maxcounter = sqlite_master_type.size ();
	   counter < maxcounter; counter ++)
	{
	if (sqlite_master_type[counter] == "table" ||
		sqlite_master_type[counter] == "view")
	{

	  (*nentries) ++;
	  SQLCommand += "DROP " +  sqlite_master_type[counter] +
				   " IF EXISTS " + sqlite_master_name[counter] + ";";
	  SQLCommand.append ('\n');
	  SQLCommand += "DELETE FROM CHART_SETTINGS WHERE KEY = '" +
				  sqlite_master_name[counter] + "'; ";
	  SQLCommand.append ('\n');
	  SQLCommand += "DELETE FROM SYMBOLS WHERE KEY = '" +
				  sqlite_master_name[counter] + "'; ";
	  SQLCommand.append ('\n');
	  SQLCommand += "DROP TABLE IF EXISTS template_" +
					 sqlite_master_name[counter] + ";";
	  SQLCommand.append ('\n');
	}
	}

	return SQLCommand;
}

/* Modified FreeChartGeany function */
void MainWindowTA::deleteEntry (){
	if (showOkCancel ("Delete selected entries ?") == false) return;
	if(listSymbol->count() <= 0) return;

	qint32 entries = 0, totalentries = 0;
	int row, maxrow, rc;
	QString table,SQLCommand = "";
	QStringList selected_tables;

	QListWidgetItem* item= listSymbol->currentItem();
	table = item->data(Qt::UserRole).toString();

	SQLCommand = "begin; ";

	SQLCommand += formSQLDropSentence (table, &entries);
	totalentries += entries;
	SQLCommand += "commit;";

	rc = sqlite3_exec(Application_Settings->db, SQLCommand.toUtf8(),
					NULL, NULL, NULL);

	if (rc != SQLITE_OK)
	{
	setGlobalError(CG_ERR_DELETE_DATA, __FILE__, __LINE__);
	showMessage (errorMessage (CG_ERR_DELETE_DATA));
	}
	delete listSymbol->takeItem(listSymbol->currentRow());

}
