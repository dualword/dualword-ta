/*
 * 
 * Copyright 2016 Lucas Tsatiris <chartgeany@gmail.com>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */

#include <QPushButton>
#include <QScrollBar>
#include "common.h"
#include "qtachart_core.h"
#include "qtachart_object.h"
#include "templatemanagerdialog.h"
#include "ui_templatemanagerdialog.h"

// keep indicator data localy
struct Lindicator
{
  QString Title;
  ParamVector Plist;	
};

// keep drawing object's data localy
struct Lobject
{
  QString Title;
  QString fontfamily;
  QString text;
  QString trailerCandleText;
  QString trailerCandleText2;
  double color;
  qreal price, price2, pad, pad2, x1, x2, y1, y2;
  qint32 type;	
  qint32 fontsize;
  qint32 fontweight;
  qint16 txtdirection, txtdirection2;
};

// constructor
TemplateManagerDialog::TemplateManagerDialog (QWidget * parent):
  QDialog (parent), ui (new Ui::TemplateManagerDialog)
{
  QString stylesheet = "background: transparent; background-color: white;";
  QStringList cheadersList;
  QFont font;	
  QPushButton *btn;
    	
  ui->setupUi (this);	
  
  btn = ui->buttonBox->button (QDialogButtonBox::Ok);
  font = btn->font ();
  font.setWeight (QFont::DemiBold);
  btn->setFont (font);
  btn->setFocusPolicy (Qt::NoFocus);
  
  btn = ui->buttonBox->button (QDialogButtonBox::Cancel);
  font = btn->font ();
  font.setWeight (QFont::DemiBold);
  btn->setFont (font);
  btn->setFocusPolicy (Qt::NoFocus);
  
  cheadersList << " Indicator Templates "
               << " Serial Number "
               << " Table Name ";
  this->setStyleSheet (stylesheet);
  ui->tableWidget->setColumnCount (3);
  ui->tableWidget->setHorizontalHeaderLabels (cheadersList);
  ui->tableWidget->horizontalHeader()->setResizeMode(0, QHeaderView::Stretch);
  ui->tableWidget->sortByColumn (1, Qt::AscendingOrder);
  ui->tableWidget->setSortingEnabled(false);
  ui->tableWidget->setStyleSheet (stylesheet);
  ui->tableWidget->verticalScrollBar ()->setStyleSheet ("background: transparent; background-color:lightgray;");
  ui->tableWidget->horizontalScrollBar ()->setStyleSheet ("background: transparent; background-color:lightgray;");
  ui->tableWidget->setColumnHidden (1, true);
  ui->tableWidget->setColumnHidden (2, true);
  
  connect (ui->downToolButton, SIGNAL (clicked ()), this,
           SLOT (downButton_clicked ()));
  connect (ui->upToolButton, SIGNAL (clicked ()), this,
           SLOT (upButton_clicked ()));   
  connect(ui->buttonBox, SIGNAL(accepted ()), this, SLOT(ok_clicked ()));
  connect(ui->buttonBox, SIGNAL(rejected ()), this, SLOT(cancel_clicked ()));
  connect(ui->delButton, SIGNAL(clicked ()), this, SLOT(del_clicked ()));
  connect(ui->tableWidget, SIGNAL(cellClicked (int, int)), this, 
          SLOT(cell_clicked (int, int)));
  connect(ui->tableWidget, SIGNAL(cellDoubleClicked (int, int)), this, 
          SLOT(ok_clicked ()));
  correctWidgetFonts (this);
  if (parent != NULL)
    setParent (parent);
}

// destructor
TemplateManagerDialog::~TemplateManagerDialog ()
{
  delete ui;
}

// load
void
TemplateManagerDialog::loadtemplate (void *chart)
{
  CG_ERRORS result;  
  	
  ui->saveasLbl->setVisible(false);
  ui->saveasEdit->setVisible(false);
  savemode = false;
  result = loaddir ();
  if (result != CG_ERR_OK)
    showMessage (errorMessage (result));
  
  referencechart = chart;    
  show ();
}

// save
void
TemplateManagerDialog::savetemplate (void *chart)
{
  CG_ERRORS result;  
  	
  ui->saveasLbl->setVisible(true);
  ui->saveasEdit->setVisible(true);
  ui->saveasEdit->setText("");
  savemode = true;
  
  result = loaddir ();
  if (result != CG_ERR_OK)
    showMessage (errorMessage (result));
  
  referencechart = chart;  
  show ();
}

// set the reference chart
void
TemplateManagerDialog::setReferenceChart (void *chart)
{
  referencechart = chart;	
}

// sql from object list
QString 
TemplateManagerDialog::qtachart2sql (QString tablename)
{
  QTAChart *chart = reinterpret_cast<QTAChart *> (referencechart);
  QTAChartCore *core = reinterpret_cast<QTAChartCore *> (getData (chart));	
  ObjectVector Object;
  QString SQLCommand = "";
  int order = 0;
  
  if (core->Object.size () < 1)
    return SQLCommand;
  
  foreach (QTACObject *object, core->Object)
    if (!object->deleteit)
      Object += object;
  
  SQLCommand += "DELETE FROM " + tablename + ";";
  foreach (QTACObject *object, Object)
  {
	DynParamsDialog *paramDialog;
	paramDialog = object->getParamDialog ();
	 
	if (paramDialog != NULL) // technical indicator object
	{
	  ParamVector pvector;
	  
	  pvector = paramDialog->getPVector ();
	  foreach (const DynParam *param, pvector)
	  {	
		  
	    SQLCommand += "INSERT INTO " + tablename; 
	    SQLCommand += "(SERIAL, DYNPARAM, TYPE, TITLE, PARAMNAME, PARAMLABEL,";
	    SQLCommand += "PARAMTYPE, PARAMDEF, PARAMVALUE) VALUES ";
	    SQLCommand += "(" + QString::number (order) + ", 1, ";
	    SQLCommand += QString::number (object->type) + ", ";
	    SQLCommand += "'" + object->getTitle () + "', ";
	    SQLCommand += "'" + param->paramName + "', ";
        SQLCommand += "'" + param->label + "', ";
        SQLCommand += QString::number (param->type) + ", ";
        SQLCommand += QString::number (param->defvalue, 'f', 1) + ", ";	    
	    SQLCommand += QString::number (param->value, 'f', 1) + ");";
	    SQLCommand.append ("\n");
	  }  	
    }
    else // drawing object
    {
	  if (object->onlineprice == false)
	  {
		qreal x1, x2, y1, y2;
		object->getCoordinates (&x1, &y1, &x2, &y2);
		  
		SQLCommand += "INSERT INTO " + tablename; 
	    SQLCommand += "(SERIAL, TYPE, TITLE, COLOR, ";
	    SQLCommand += " FONTSIZE, FONTWEIGHT, FONT, TEXT, X1, Y1, X2, Y2, ";
	    SQLCommand += "TRAILERCANDLETEXT, PRICE, PAD, TXTDIRECTION, ";
	    SQLCommand += "TRAILERCANDLETEXT2, PRICE2, PAD2, TXTDIRECTION2) VALUES ";
	    SQLCommand += "(" + QString::number (order) + ", ";
	    SQLCommand += QString::number (object->type) + ", ";
	    SQLCommand += "'" + object->getTitle () + "', ";
	    SQLCommand += QString::number (object->getColor().rgb (), 'f', 1) + ", ";
	    SQLCommand += QString::number (object->getFont().pointSize (), 'f', 1) + ", ";
	    SQLCommand += QString::number (object->getFont().weight (), 'f', 1) + ", ";
	    SQLCommand += "'" + object->getFont ().family () + "', ";
	    SQLCommand += "'" + object->getText () + "', ";
	    SQLCommand += QString::number (x1, 'f', 1) + ", ";
	    SQLCommand += QString::number (y1, 'f', 1) + ", ";
	    SQLCommand += QString::number (x2, 'f', 1) + ", ";
	    SQLCommand += QString::number (y2, 'f', 1) + ", ";
	    
	    SQLCommand += "'" + object->getTrailerCandleText () + "', ";
	    if (QString::number (object->getPrice (), 'f', 4).contains ("inf") == true ||
	        QString::number (object->getPrice (), 'f', 4).size () > 64)
	      SQLCommand += "0, ";
	    else  
	      SQLCommand += QString::number (object->getPrice (), 'f', 4) + ", ";
	    SQLCommand += QString::number (object->getPad (), 'f', 4) + ", ";  
	    SQLCommand += QString::number (object->getTxtDirection (), 'f', 0) + ", "; 
	    
	    SQLCommand += "'" + object->getTrailerCandleText2 () + "', ";
	    if (QString::number (object->getPrice2 (), 'f', 4).contains ("inf") == true ||
	        QString::number (object->getPrice2 (), 'f', 4).size () > 64)
	      SQLCommand += "0, ";
	    else  
	      SQLCommand += QString::number (object->getPrice2 (), 'f', 4) + ", ";
	    SQLCommand += QString::number (object->getPad2 (), 'f', 4) + ", ";  
	    SQLCommand += QString::number (object->getTxtDirection2 (), 'f', 0); 
	    
	    SQLCommand += "); ";
	    SQLCommand.append ("\n"); 
      }	  	
    }
    
    order ++;  
  }
  
  return SQLCommand;     
}

// templates' last entry
static int
sqlcb_last (void *dummy, int argc, char **argv, char **column)
{
  QString colname;
  int *newid = (int *) dummy;
  
  for (qint32 counter = 0; counter < argc; counter ++)
  {
    colname = QString::fromUtf8(column[counter]);
    colname = colname.toUpper ();
    if (colname == "NEWID")
      *newid = QString::fromUtf8 (argv[counter]).toInt ();
  }
  return 0;
}

// new
CG_ERRORS
TemplateManagerDialog::newtemplate (void)
{
  QString SQLCommand = "";
  int rc, newid;
  
  SQLCommand += "INSERT INTO TEMPLATES (TEMPLATE) VALUES ('";
  SQLCommand += ui->saveasEdit->text ().trimmed () + "');";	
    
  rc = sqlite3_exec(Application_Settings->db, SQLCommand.toUtf8(), 
                    NULL, NULL, NULL);
  if (rc != SQLITE_OK)
  {
    setGlobalError(CG_ERR_DBACCESS, __FILE__, __LINE__);
    return CG_ERR_DBACCESS;
  }
  
  SQLCommand = "SELECT MAX (TEMPLATE_ID) AS NEWID FROM TEMPLATES;";
  rc = sqlite3_exec(Application_Settings->db, SQLCommand.toUtf8(), 
                    sqlcb_last, (void *) &newid, NULL);
  if (rc != SQLITE_OK)
  {
    setGlobalError(CG_ERR_DBACCESS, __FILE__, __LINE__);
    return CG_ERR_DBACCESS;
  }
  
  SQLCommand =  "UPDATE TEMPLATES SET TABLENAME = '";
  SQLCommand += "template_" + QString::number(newid) + "' ";
  SQLCommand += "WHERE TEMPLATE_ID = " + QString::number(newid) + ";";
  SQLCommand += "DROP TABLE IF EXISTS template_" + QString::number(newid) + ";";
  SQLCommand += "CREATE TABLE template_" + QString::number(newid) + " AS ";
  SQLCommand += "SELECT * FROM templatemodel;";
  SQLCommand += qtachart2sql (QString ("template_" + QString::number(newid)));
  rc = sqlite3_exec(Application_Settings->db, SQLCommand.toUtf8(), 
                    NULL, NULL, NULL);
  if (rc != SQLITE_OK)
  {
    setGlobalError(CG_ERR_DBACCESS, __FILE__, __LINE__);
    return CG_ERR_DBACCESS;
  }
 
  return CG_ERR_OK;
}

// update
CG_ERRORS
TemplateManagerDialog::updatetemplate (QString tablename)
{
  QString SQLCommand = "";
  int rc;
    
  SQLCommand += "DROP TABLE IF EXISTS " + tablename + ";";
  SQLCommand += "CREATE TABLE " + tablename + " AS ";
  SQLCommand += "SELECT * FROM templatemodel;";
  SQLCommand += qtachart2sql (tablename);
  rc = sqlite3_exec(Application_Settings->db, SQLCommand.toUtf8(), 
                    NULL, NULL, NULL);
  if (rc != SQLITE_OK)
  {
    setGlobalError(CG_ERR_DBACCESS, __FILE__, __LINE__);
    return CG_ERR_DBACCESS;
  }
  
  return CG_ERR_OK;	
  	
}

// delete template
CG_ERRORS
TemplateManagerDialog::deletetemplate (int id, QString tablename)
{
  QString SQLCommand = "";
  int rc;
  
  SQLCommand += "DROP TABLE IF EXISTS " + tablename + ";";
  SQLCommand += "DELETE FROM TEMPLATES WHERE TEMPLATE_ID = " + QString::number(id) + ";";
  
  rc = sqlite3_exec(Application_Settings->db, SQLCommand.toUtf8(), 
                    NULL, NULL, NULL);
  if (rc != SQLITE_OK)
  {
    setGlobalError(CG_ERR_DBACCESS, __FILE__, __LINE__);
    return CG_ERR_DBACCESS;
  }
  
  return CG_ERR_OK;
}

// get serials of indicators or objects callback
static int
sqlcb_getobjectserials  (void *seriallist, int argc, char **argv, char **column)
{
  QList <int> *serial;	
  serial = (QList <int> *) seriallist;
  
  for (qint32 counter = 0; counter < argc; counter ++)
  {
	QString colname = QString::fromUtf8(column[counter]);
    colname = colname.toUpper ();
    if (colname == "SERIAL")
      serial->append (QString::fromUtf8 (argv[counter]).toInt ());
  }    
  return 0;
}

// load template callback
static int
sqlcb_loadtemplateindicators (void *dummy, int argc, char **argv, char **column)
{
  QString colname;	
  Lindicator *ind = reinterpret_cast<Lindicator *> (dummy); 
  DynParam *Param = NULL;
  
  for (qint32 counter = 0; counter < argc; counter ++)
  {
    colname = QString::fromUtf8(column[counter]);
    colname = colname.toUpper ();

    if (colname == "TITLE")
    {
      ind->Title = QString::fromUtf8 (argv[counter]);  
      Param = new DynParam;
      ind->Plist += Param;
    }  
    else    
    if (colname == "PARAMNAME" && Param != NULL)
    {
      Param->paramName = QString::fromUtf8 (argv[counter]);  
    }
    else  
    if (colname == "PARAMLABEL" && Param != NULL)
    {
      Param->label = QString::fromUtf8 (argv[counter]); 
    }
    else  
    if (colname == "PARAMTYPE" && Param != NULL)
    {
      Param->type = QString::fromUtf8 (argv[counter]).toInt ();    
    }
    else  
    if (colname == "PARAMDEF" && Param != NULL)
    {
      Param->defvalue = QString::fromUtf8 (argv[counter]).toDouble ();  
    }
    else  
    if (colname == "PARAMVALUE" && Param != NULL)
      Param->value = QString::fromUtf8 (argv[counter]).toDouble ();    
  }
  
  return 0;
}

// load template objects
static int
sqlcb_loadtemplateobjects (void *dummy, int argc, char **argv, char **column)
{
  QString colname;	
  Lobject *obj = reinterpret_cast<Lobject *> (dummy); 
 
  
  for (qint32 counter = 0; counter < argc; counter ++)
  {
    colname = QString::fromUtf8(column[counter]);
    colname = colname.toUpper ();

    if (colname == "TITLE")
      obj->Title = QString::fromUtf8 (argv[counter]);  
    else  
    if (colname == "TYPE")
      obj->type = QString::fromUtf8 (argv[counter]).toInt ();  
    else  
    if (colname == "COLOR")
      obj->color = QString::fromUtf8 (argv[counter]).toDouble ();  
    else  
    if (colname == "FONTSIZE")
      obj->fontsize = QString::fromUtf8 (argv[counter]).toInt ();  
    else  
    if (colname == "FONT")
      obj->fontfamily = QString::fromUtf8 (argv[counter]);       
    else  
    if (colname == "FONTWEIGHT")
      obj->fontweight = QString::fromUtf8 (argv[counter]).toInt ();  
    else  
    if (colname == "TEXT")
      obj->text = QString::fromUtf8 (argv[counter]);  
    else  
    if (colname == "TRAILERCANDLETEXT")
      obj->trailerCandleText = QString::fromUtf8 (argv[counter]);
    else  
    if (colname == "PRICE")
      obj->price = QString::fromUtf8 (argv[counter]).toFloat ();  
    else  
    if (colname == "PAD")
      obj->pad = QString::fromUtf8 (argv[counter]).toFloat ();  
    else  
    if (colname == "TXTDIRECTION")
      obj->txtdirection = QString::fromUtf8 (argv[counter]).toShort ();   
    else  
    if (colname == "TRAILERCANDLETEXT2")
      obj->trailerCandleText2 = QString::fromUtf8 (argv[counter]);
    else  
    if (colname == "PRICE2")
      obj->price2 = QString::fromUtf8 (argv[counter]).toFloat ();  
    else  
    if (colname == "PAD2")
      obj->pad2 = QString::fromUtf8 (argv[counter]).toFloat ();  
    else  
    if (colname == "TXTDIRECTION2")
      obj->txtdirection2 = QString::fromUtf8 (argv[counter]).toShort ();     
    else  
    if (colname == "X1")
      obj->x1 = QString::fromUtf8 (argv[counter]).toFloat ();  
    else  
    if (colname == "X2")
      obj->x2 = QString::fromUtf8 (argv[counter]).toFloat ();  
    else  
    if (colname == "Y1")
      obj->y1 = QString::fromUtf8 (argv[counter]).toFloat ();  
    else  
    if (colname == "Y2")
      obj->y2 = QString::fromUtf8 (argv[counter]).toFloat ();          
  }
  
  return 0;
}
// attach
CG_ERRORS
TemplateManagerDialog::attachtemplate (QString tablename)
{
  QList <int> indserial;
  QList <int> objectserial;		
  QTAChart *chart = reinterpret_cast<QTAChart *> (referencechart);
  QTAChartCore *core = reinterpret_cast<QTAChartCore *> (getData (chart));	
  QTACObject *object;
  QString SQLCommand = "";
  int rc;
  
  // attach technical indicators 
  SQLCommand = "SELECT DISTINCT SERIAL FROM " +  tablename + " WHERE TYPE > 6;";
  rc = sqlite3_exec(Application_Settings->db, SQLCommand.toUtf8(), 
                    sqlcb_getobjectserials,  reinterpret_cast<void*> (&indserial), NULL);
  if (rc != SQLITE_OK)
  {
    setGlobalError(CG_ERR_DBACCESS, __FILE__, __LINE__);
    return CG_ERR_DBACCESS;
  }
  
  if (indserial.size () == 0)
    goto attachtemplate_drawing_objects;
  
  foreach (object, core->Object)
    if (object->getParamDialog () != NULL)
      object->deleteit = true;

  foreach (const int counter, indserial)
  {
	QString query;
	Lindicator *lind;
	DynParamsDialog *pdialog;
	
	lind = new Lindicator;
	query  = "SELECT TITLE, PARAMNAME, PARAMLABEL, PARAMTYPE, ";
	query += "PARAMDEF,PARAMVALUE FROM " + tablename + " WHERE ";
	query += "SERIAL = " + QString::number (counter); 
	rc = sqlite3_exec(Application_Settings->db, query.toUtf8(), 
         sqlcb_loadtemplateindicators, reinterpret_cast<void*> (lind), NULL);
    if (rc != SQLITE_OK)
    {
      setGlobalError(CG_ERR_DBACCESS, __FILE__, __LINE__);
      return CG_ERR_DBACCESS;
    }    
    
    pdialog = new  DynParamsDialog (lind->Plist, lind->Title);
    core->functionScr->addIndicator (pdialog);
    lind->Plist.clear ();
    delete lind;
    delete pdialog;
  }  
  
  // attach drawing objects 
attachtemplate_drawing_objects:  
  SQLCommand = "SELECT SERIAL FROM " +  tablename + " WHERE TYPE <= 6;";
  rc = sqlite3_exec(Application_Settings->db, SQLCommand.toUtf8(), 
                    sqlcb_getobjectserials,  reinterpret_cast<void*> (&objectserial), NULL);
  if (rc != SQLITE_OK)
  {
    setGlobalError(CG_ERR_DBACCESS, __FILE__, __LINE__);
    return CG_ERR_DBACCESS;
  }
  
  if (objectserial.size () == 0)
    goto attachtemplate_end;
  
  foreach (object, core->Object)
    if (object->getParamDialog () == NULL && object->onlineprice == false)
      object->deleteit = true;
  
  foreach (const int counter, objectserial)
  {
	QString query;
	Lobject *lobj;
	
	lobj = new Lobject;
	query  = "SELECT TITLE, TYPE, COLOR, FONTSIZE, ";
	query += "FONT, FONTWEIGHT, TEXT, TRAILERCANDLETEXT, ";
	query += "PRICE, PAD, TXTDIRECTION, X1, X2, Y1, Y2, ";
	query += "TRAILERCANDLETEXT2, PRICE2, PAD2, TXTDIRECTION2 "; 
	query += "FROM " + tablename + " WHERE ";
	query += "SERIAL = " + QString::number (counter); 
	rc = sqlite3_exec(Application_Settings->db, query.toUtf8(), 
         sqlcb_loadtemplateobjects, reinterpret_cast<void*> (lobj), NULL);
    if (rc != SQLITE_OK)
    {
      setGlobalError(CG_ERR_DBACCESS, __FILE__, __LINE__);
      return CG_ERR_DBACCESS;
    }  
    
    if (lobj->type == QTACHART_OBJ_LABEL)
    {
	  QTACObject *qtaco;	
	  QGraphicsTextItem *item = new QGraphicsTextItem;
	  QFont font;
	  
	  item->setDefaultTextColor (lobj->color);
	  font.setFamily (lobj->fontfamily);
	  font.setWeight (lobj->fontweight);
	  font.setPointSize ((lobj->fontsize==-1?7:lobj->fontsize));
	  item->setFont (font);
	  item->setPlainText (lobj->text);
	  core->scene->addItem (item);
	  qtaco = core->addLabel (item, lobj->x1, lobj->y1);
	  qtaco->setPrice (lobj->price);
    }
    
    if (lobj->type == QTACHART_OBJ_TEXT)
    {
	  QGraphicsTextItem *item = new QGraphicsTextItem;
	  QFont font;
	  
	  item->setDefaultTextColor (lobj->color);
	  font.setFamily (lobj->fontfamily);
	  font.setWeight (lobj->fontweight);
	  font.setPointSize ((lobj->fontsize==-1?7:lobj->fontsize));
	  item->setFont (font);
	  item->setPlainText (lobj->text);
	  core->scene->addItem (item);
	  core->addText (item, lobj->trailerCandleText, lobj->price);
    }
    
    if (lobj->type == QTACHART_OBJ_HLINE)
    {
	  QGraphicsLineItem *item = new QGraphicsLineItem;
	  QColor color;
	  
	  item->setPen (QPen (lobj->color));
	  core->scene->addItem (item);
	  core->addHLine (item, lobj->price);	
    }	
    
    if (lobj->type == QTACHART_OBJ_VLINE)
    {
	  QGraphicsLineItem *item = new QGraphicsLineItem;
	  QColor color;
	  
	  item->setPen (QPen (lobj->color));
	  core->scene->addItem (item);
	  core->addVLine (item, lobj->trailerCandleText);	
    }
    
    if (lobj->type == QTACHART_OBJ_LINE)
    {
	  QGraphicsLineItem *item = new QGraphicsLineItem;
	  LineEdge e1, e2;
	  QColor color;
	  
	  item->setPen (QPen (lobj->color));
	  e1.price = lobj->price;
	  e1.pad = lobj->pad;
	  e1.trailerCandleText = lobj->trailerCandleText;
	  e1.txtdirection = lobj->txtdirection;
	  e1.sequence = 1;
	  e2.price = lobj->price2;
	  e2.pad = lobj->pad2;
	  e2.trailerCandleText = lobj->trailerCandleText2;
	  e2.txtdirection = lobj->txtdirection2;
	  e2.sequence = 2;
	  core->scene->addItem (item);
	  core->addTLine (item, e1, e2);	
    }
    
     if (lobj->type == QTACHART_OBJ_FIBO)
    {
	  QGraphicsLineItem *item = new QGraphicsLineItem;
	  LineEdge e1, e2;
	  QColor color;
	  
	  item->setPen (QPen (lobj->color));
	  e1.price = lobj->price;
	  e1.pad = lobj->pad;
	  e1.trailerCandleText = lobj->trailerCandleText;
	  e1.txtdirection = lobj->txtdirection;
	  e1.sequence = 1;
	  e2.price = lobj->price2;
	  e2.pad = lobj->pad2;
	  e2.trailerCandleText = lobj->trailerCandleText2;
	  e2.txtdirection = lobj->txtdirection2;
	  e2.sequence = 2;
	  core->scene->addItem (item);
	  item->setVisible (false);
	  item->setLine (lobj->x1, yOnPrice (core, e1.price), 
	                 lobj->x1, yOnPrice (core, e2.price));
	  core->addFibo (item, e1, e2);	
    }
    
    delete lobj;    
  }	
  
attachtemplate_end:
  chart->goBack ();
  
  return CG_ERR_OK;
}

// load template's directory callback
static int
sqlcb_loaddir (void *dlist, int argc, char **argv, char **column)
{
  QString colname;
  QStringList *dirlist = (QStringList *) dlist;
  
  for (qint32 counter = 0; counter < argc; counter ++)
  {
	QString colname = QString::fromUtf8(column[counter]);
    colname = colname.toUpper ();
    if (colname == "TEMPLATE" || colname == "TEMPLATE_ID" ||
        colname == "TABLENAME")
      dirlist->append (QString::fromUtf8 (argv[0]));
  }    
  return 0;
}

// load templates' directory
CG_ERRORS
TemplateManagerDialog::loaddir (void)
{
  QString SQLCommand = "";
  int rc;
  
  cleartable ();
  description.clear ();
  SQLCommand = "SELECT TEMPLATE FROM TEMPLATES ORDER BY TEMPLATE_ID;";
  rc = sqlite3_exec(Application_Settings->db, SQLCommand.toUtf8(), 
                    sqlcb_loaddir, (void *) &description, NULL);
   
  if (rc != SQLITE_OK)
  {
    setGlobalError(CG_ERR_DBACCESS, __FILE__, __LINE__);
    return CG_ERR_DBACCESS;
  }
  ui->tableWidget->setRowCount (description.size ());	
  fillcolumn (description, 0);
  
  id.clear ();
  SQLCommand = "SELECT TEMPLATE_ID FROM TEMPLATES ORDER BY TEMPLATE_ID;";
  rc = sqlite3_exec(Application_Settings->db, SQLCommand.toUtf8(), 
                    sqlcb_loaddir, (void *) &id, NULL);
   
  if (rc != SQLITE_OK)
  {
    setGlobalError(CG_ERR_DBACCESS, __FILE__, __LINE__);
    return CG_ERR_DBACCESS;
  }
  fillcolumn (id, 1);
  
  tablename.clear ();
  SQLCommand = "SELECT TABLENAME FROM TEMPLATES ORDER BY TEMPLATE_ID;";
  rc = sqlite3_exec(Application_Settings->db, SQLCommand.toUtf8(), 
                    sqlcb_loaddir, (void *) &tablename, NULL);
   
  if (rc != SQLITE_OK)
  {
    setGlobalError(CG_ERR_DBACCESS, __FILE__, __LINE__);
    return CG_ERR_DBACCESS;
  }
  fillcolumn (tablename, 2);
  
  ui->tableWidget->viewport()->update(); 
  return CG_ERR_OK;	
}

// clear table
void
TemplateManagerDialog::cleartable ()
{
  int row, nrows, col, ncols = 3;
  
  nrows = ui->tableWidget->rowCount ();
  for (row = 0; row < nrows; row ++)
    for (col = 0; col < ncols; col ++)
       delete ui->tableWidget->takeItem(row,col);
}

// fill table column
void
TemplateManagerDialog::fillcolumn (QStringList list, int col)
{
  for (qint32 counter = 0; counter < list.size (); counter++)
  {
	QTableWidgetItem *item;
	  
    item = new QTableWidgetItem;
    item->setText (list[counter]);
    ui->tableWidget->setItem(counter,col,item);
  }
}

/// Events
///
// resize
void
TemplateManagerDialog::resizeEvent (QResizeEvent * event)
{
  if (event->spontaneous ())
    return;
    	
  ui->buttonBox->move (width () - 350, height () - 50);	
  ui->upToolButton->move (width () - 50, 10);	
  ui->downToolButton->move (width () - 50, 60);	
  ui->delButton->move (width () - 55, 110);
  ui->saveasLbl->move (10, height () - 100);
  ui->saveasEdit->move (110, height () - 100);
  ui->saveasEdit->resize (width () - 120, 32);
  ui->tableWidget->resize (width () - 70, height () - 120);
}

/// Signals
///
// ok
void
TemplateManagerDialog::ok_clicked ()
{
  QTAChart *chart = reinterpret_cast<QTAChart *> (referencechart);	
  CG_ERRORS result;  
  	
  if (savemode == true) 
  {
	QString tablename;  
	int maxrow;  
	bool exists = false;  
	
	if (ui->saveasEdit->text ().size () == 0)
	  return;
	
	if (showOkCancel ("Save template?") == false)
      return; 
	
	maxrow = ui->tableWidget->rowCount ();
    for (int row = 0; row < maxrow; row ++)
    {
	  if (ui->tableWidget->item (row, 0)->text ().trimmed () == 
	      ui->saveasEdit->text ().trimmed ())
	  {    
	    exists = true;  
	    tablename = ui->tableWidget->item (row, 2)->text ();
	  }  
    }
    
    if (exists == false)
    {
	  result = newtemplate ();	
      if (result != CG_ERR_OK)
      {
        showMessage (errorMessage (result));
        return;   
      }
    }  
    else
    {
      result = updatetemplate (tablename);  
      if (result != CG_ERR_OK)
      {
        showMessage (errorMessage (result));
        return;   
      }
    }  
    
    loaddir ();  	
    showMessage ("Template saved");
    
    return;
  }
  
  if (selectedtable == "")
    return;
  
  result = attachtemplate (selectedtable);
  if (result != CG_ERR_OK)
  {
     showMessage (errorMessage (result));
     return;   
  }
  
  chart->goBack ();
  hide ();
}

// cancel
void
TemplateManagerDialog::cancel_clicked ()
{
  hide ();
}

// del 
void
TemplateManagerDialog::del_clicked ()
{
  CG_ERRORS result;  	
  int maxrow = ui->tableWidget->rowCount ();
  
  if (showOkCancel ("Delete selected templates?") == false)
    return; 
    
  for (int row = 0; row < maxrow; row ++)
    if (ui->tableWidget->item (row, 0)->isSelected ())
    {
	  result = deletetemplate (ui->tableWidget->item (row, 1)->text ().toInt (),
					  ui->tableWidget->item (row, 2)->text ());	
	  if (result != CG_ERR_OK)
	  {
		showMessage (errorMessage (result));
        return;  
      }				  
    }
  
  selectedtable = "";
  loaddir ();  
}

// template selected 
void
TemplateManagerDialog::cell_clicked (int row, int column)
{
  selectedtable = ui->tableWidget->item (row, 2)->text ();
  ui->saveasEdit->setText (ui->tableWidget->item (row, 0)->text ());
}

// down
void 
TemplateManagerDialog::downButton_clicked (void)
{
  QScrollBar *vScrollBar = ui->tableWidget->verticalScrollBar();
  vScrollBar->triggerAction(QAbstractSlider::SliderSingleStepAdd);
  ui->tableWidget->setFocus (Qt::MouseFocusReason);
}

// up
void 
TemplateManagerDialog::upButton_clicked (void)
{
  QScrollBar *vScrollBar = ui->tableWidget->verticalScrollBar();
  vScrollBar->triggerAction(QAbstractSlider::SliderSingleStepSub);	
  ui->tableWidget->setFocus (Qt::MouseFocusReason);
}
