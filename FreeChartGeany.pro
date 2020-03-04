QT       += core gui network webkit
CONFIG	 += release
DEFINES  *= QT_NO_DEBUG_OUTPUT
DEFINES  *= QT_USE_QSTRINGBUILDER
TEMPLATE  = app
TARGET    = FreeChartGeany
PKG_NAME  = freechartgeany

INCLUDEPATH += \
	include \
	3rdparty/sqlite3/include \
	3rdparty/ta-lib/include \
	3rdparty/libjson

QMAKE_CFLAGS_RELEASE += \
	-DSQLITE_MAX_COLUMN=256 \
 	-DSQLITE_MAX_SQL_LENGTH=134217728 \
	-DSQLITE_MAX_EXPR_DEPTH=256 \
	-DSQLITE_MAX_COMPOUND_SELECT=128 \  
	-DSQLITE_THREADSAFE=1 \
	-DSQLITE_TEMP_STORE=2 \
	-DSQLITE_DEFAULT_PAGE_SIZE=4096 \
	-DJSON_ISO_STRICT \
	-D_CRT_SECURE_NO_WARNINGS

QMAKE_CXXFLAGS_RELEASE += -DJSON_ISO_STRICT

linux-g++* {
    QMAKE_CXXFLAGS += -std=c++11
}

macx-clang {
    QMAKE_CXXFLAGS += -std=c++11
}

UI_DIR = ui
MOC_DIR = moc
OBJECTS_DIR = obj

win32 {
    gcc:QMAKE_CFLAGS_RELEASE -= -g -Wall -O2 -O1
    gcc:QMAKE_CXXFLAGS_RELEASE -= -g -Wall -O2 -O1
    gcc:QMAKE_CFLAGS_RELEASE += -fomit-frame-pointer -w	-O2
    QMAKE_CXXFLAGS_RELEASE += $$QMAKE_CFLAGS_RELEASE
    RC_FILE = freechartgeany.rc
}

macx {
    clang:QMAKE_CXXFLAGS += -stdlib=libc++ -mmacosx-version-min=10.6
    clang:QMAKE_LFLAGS += -mmacosx-version-min=10.6
    clang:LIBS += -stdlib=libc++ 
    gcc:QMAKE_CXXFLAGS += -mmacosx-version-min=10.6
    gcc:QMAKE_LFLAGS += -mmacosx-version-min=10.6
    QMAKE_INFO_PLIST += default.plist
    QMAKE_POST_LINK += /bin/cp -R licenses ./$${TARGET}.app &&
    QMAKE_POST_LINK += '/usr/bin/sed -i -e "s/@VERSION@/$$VERSION/g" "./$${TARGET}.app/Contents/Info.plist"' &&
    ICON = images/icons/freechartgeany.icns
    BUNDLEID = com.$${TARGET}
    LIBS += -framework CoreServices    
}

unix {
    SNAPCHECK = $$(_)
    LIBS += -ldl
    QMAKE_POST_LINK = strip $(TARGET)
    QMAKE_CFLAGS_RELEASE -= -g -Wall -O2 -O1
    QMAKE_CXXFLAGS_RELEASE -= -g -Wall -O2 -O2
    QMAKE_CFLAGS_RELEASE += -fomit-frame-pointer -w -O2
    QMAKE_CXXFLAGS_RELEASE += -fomit-frame-pointer -Wall -O2 -Wno-reorder -Wno-unused-parameter
    equals(SNAPCHECK, "/usr/bin/snapcraft") {
     target.path = ../install/usr/bin
     INSTALLS += target 
    } else {
     target.path = /usr/bin
     images.path = /usr/share/pixmaps
     desktop.path = /usr/share/applications/
     images.files = images/icons/$${PKG_NAME}.png
     desktop.files = $${PKG_NAME}.desktop
     INSTALLS = target desktop images
   }  
}

SOURCES += 	src/main.cpp \
		src/csv.cpp \
		src/dbman.cpp \
		src/feedyahoo.cpp \
		src/feedgoogle.cpp \
		src/function_dataset.cpp \
		src/priceupdater.cpp \
	   	src/mainwindow.cpp \
		src/loadcsvdialog.cpp \
		src/datamanagerdialog.cpp \
		src/databrowserdialog.cpp \
		src/templatemanagerdialog.cpp \
		src/dynparamsdialog.cpp \
		src/downloaddatadialog.cpp \
		src/optionsdialog.cpp \
		src/progressdialog.cpp \
		src/waitdialog.cpp \
		src/splashdialog.cpp \
		src/infodialog.cpp \
		src/licensedialog.cpp \
		src/manualdialog.cpp \
		src/netservice.cpp \
		src/common.cpp \
		src/qtachart.cpp \
		src/qtachart_methods.cpp \
		src/qtachart_help.cpp \
		src/qtachart_data.cpp \
		src/qtachart_object.cpp \
		src/qtachart_object_draw.cpp \
		src/qtachart_utilities.cpp \
		src/qtachart_properties.cpp \
		src/qtachart_draw.cpp \
		src/qtachart_functions.cpp \
		src/qtachart_objects.cpp \
		src/qtachart_core.cpp \
		src/qtachart_core_barchart.cpp \
		src/qtachart_core_candlechart.cpp \
		src/qtachart_core_drawbar.cpp \
		src/qtachart_core_drawcandle.cpp \
		src/qtachart_core_draw.cpp \
		src/qtachart_core_drawpriceline.cpp \
		src/qtachart_eventfilters.cpp \
		src/textobjectdialog.cpp \
		src/lineobjectdialog.cpp \
		src/appColorDialog.cpp \
		src/qtcgraphicsitem.cpp \
		3rdparty/sqlite3/sqlite3.c \
		3rdparty/ta-lib/ta_abstract/tables/table_s.c \ 
		3rdparty/ta-lib/ta_abstract/tables/table_r.c \ 
		3rdparty/ta-lib/ta_abstract/tables/table_y.c \ 
		3rdparty/ta-lib/ta_abstract/tables/table_j.c \ 
		3rdparty/ta-lib/ta_abstract/tables/table_e.c \ 
		3rdparty/ta-lib/ta_abstract/tables/table_n.c \ 
		3rdparty/ta-lib/ta_abstract/tables/table_q.c \ 
		3rdparty/ta-lib/ta_abstract/tables/table_z.c \ 
		3rdparty/ta-lib/ta_abstract/tables/table_u.c \ 
		3rdparty/ta-lib/ta_abstract/tables/table_x.c \ 
		3rdparty/ta-lib/ta_abstract/tables/table_f.c \ 
		3rdparty/ta-lib/ta_abstract/tables/table_i.c \ 
		3rdparty/ta-lib/ta_abstract/tables/table_g.c \ 
		3rdparty/ta-lib/ta_abstract/tables/table_l.c \ 
		3rdparty/ta-lib/ta_abstract/tables/table_w.c \ 
		3rdparty/ta-lib/ta_abstract/tables/table_h.c \ 
		3rdparty/ta-lib/ta_abstract/tables/table_c.c \ 
		3rdparty/ta-lib/ta_abstract/tables/table_t.c \ 
		3rdparty/ta-lib/ta_abstract/tables/table_k.c \ 
		3rdparty/ta-lib/ta_abstract/tables/table_d.c \ 
		3rdparty/ta-lib/ta_abstract/tables/table_m.c \ 
		3rdparty/ta-lib/ta_abstract/tables/table_v.c \ 
		3rdparty/ta-lib/ta_abstract/tables/table_b.c \ 
		3rdparty/ta-lib/ta_abstract/tables/table_o.c \ 
		3rdparty/ta-lib/ta_abstract/tables/table_p.c \ 
		3rdparty/ta-lib/ta_abstract/tables/table_a.c \ 
		3rdparty/ta-lib/ta_abstract/ta_func_api.c \ 
		3rdparty/ta-lib/ta_abstract/ta_def_ui.c \ 
		3rdparty/ta-lib/ta_abstract/ta_abstract.c \ 
		3rdparty/ta-lib/ta_abstract/frames/ta_frame.c \ 
		3rdparty/ta-lib/ta_abstract/ta_group_idx.c \ 
		3rdparty/ta-lib/ta_common/ta_global.c \ 
		3rdparty/ta-lib/ta_common/ta_version.c \ 
		3rdparty/ta-lib/ta_common/ta_retcode.c \ 
		3rdparty/ta-lib/ta_func/ta_CDLMATCHINGLOW.c \ 
		3rdparty/ta-lib/ta_func/ta_CDLINVERTEDHAMMER.c \ 
		3rdparty/ta-lib/ta_func/ta_FLOOR.c \ 
		3rdparty/ta-lib/ta_func/ta_CDLHAMMER.c \ 
		3rdparty/ta-lib/ta_func/ta_CDLHARAMICROSS.c \ 
		3rdparty/ta-lib/ta_func/ta_CDLSHOOTINGSTAR.c \ 
		3rdparty/ta-lib/ta_func/ta_HT_DCPERIOD.c \ 
		3rdparty/ta-lib/ta_func/ta_CDLHANGINGMAN.c \ 
		3rdparty/ta-lib/ta_func/ta_BETA.c \ 
		3rdparty/ta-lib/ta_func/ta_STOCHF.c \ 
		3rdparty/ta-lib/ta_func/ta_ACOS.c \ 
		3rdparty/ta-lib/ta_func/ta_STDDEV.c \ 
		3rdparty/ta-lib/ta_func/ta_MEDPRICE.c \ 
		3rdparty/ta-lib/ta_func/ta_CDLENGULFING.c \ 
		3rdparty/ta-lib/ta_func/ta_STOCH.c \ 
		3rdparty/ta-lib/ta_func/ta_CDLGRAVESTONEDOJI.c \ 
		3rdparty/ta-lib/ta_func/ta_LOG10.c \ 
		3rdparty/ta-lib/ta_func/ta_LINEARREG_SLOPE.c \ 
		3rdparty/ta-lib/ta_func/ta_RSI.c \ 
		3rdparty/ta-lib/ta_func/ta_CDLRISEFALL3METHODS.c \ 
		3rdparty/ta-lib/ta_func/ta_ADX.c \ 
		3rdparty/ta-lib/ta_func/ta_BOP.c \ 
		3rdparty/ta-lib/ta_func/ta_CEIL.c \ 
		3rdparty/ta-lib/ta_func/ta_CDLINNECK.c \ 
		3rdparty/ta-lib/ta_func/ta_TSF.c \ 
		3rdparty/ta-lib/ta_func/ta_CDLUPSIDEGAP2CROWS.c \ 
		3rdparty/ta-lib/ta_func/ta_CDLMARUBOZU.c \ 
		3rdparty/ta-lib/ta_func/ta_CDLHIGHWAVE.c \ 
		3rdparty/ta-lib/ta_func/ta_NVI.c \ 
		3rdparty/ta-lib/ta_func/ta_ROC.c \ 
		3rdparty/ta-lib/ta_func/ta_LINEARREG.c \ 
		3rdparty/ta-lib/ta_func/ta_CDLCLOSINGMARUBOZU.c \ 
		3rdparty/ta-lib/ta_func/ta_EXP.c \ 
		3rdparty/ta-lib/ta_func/ta_CDLSTALLEDPATTERN.c \ 
		3rdparty/ta-lib/ta_func/ta_HT_DCPHASE.c \ 
		3rdparty/ta-lib/ta_func/ta_CDLTASUKIGAP.c \ 
		3rdparty/ta-lib/ta_func/ta_CDLMORNINGSTAR.c \ 
		3rdparty/ta-lib/ta_func/ta_VAR.c \ 
		3rdparty/ta-lib/ta_func/ta_TRANGE.c \ 
		3rdparty/ta-lib/ta_func/ta_HT_PHASOR.c \ 
		3rdparty/ta-lib/ta_func/ta_CDLDARKCLOUDCOVER.c \ 
		3rdparty/ta-lib/ta_func/ta_CDL3OUTSIDE.c \ 
		3rdparty/ta-lib/ta_func/ta_DX.c \ 
		3rdparty/ta-lib/ta_func/ta_MINUS_DM.c \ 
		3rdparty/ta-lib/ta_func/ta_CDLTHRUSTING.c \ 
		3rdparty/ta-lib/ta_func/ta_MACDEXT.c \ 
		3rdparty/ta-lib/ta_func/ta_COS.c \ 
		3rdparty/ta-lib/ta_func/ta_MINMAXINDEX.c \ 
		3rdparty/ta-lib/ta_func/ta_CDLUNIQUE3RIVER.c \ 
		3rdparty/ta-lib/ta_func/ta_MAXINDEX.c \ 
		3rdparty/ta-lib/ta_func/ta_SAR.c \ 
		3rdparty/ta-lib/ta_func/ta_CDLHOMINGPIGEON.c \ 
		3rdparty/ta-lib/ta_func/ta_CDLONNECK.c \ 
		3rdparty/ta-lib/ta_func/ta_WCLPRICE.c \ 
		3rdparty/ta-lib/ta_func/ta_ATAN.c \ 
		3rdparty/ta-lib/ta_func/ta_CDL3INSIDE.c \ 
		3rdparty/ta-lib/ta_func/ta_ADD.c \ 
		3rdparty/ta-lib/ta_func/ta_AD.c \ 
		3rdparty/ta-lib/ta_func/ta_MACD.c \ 
		3rdparty/ta-lib/ta_func/ta_CDLIDENTICAL3CROWS.c \ 
		3rdparty/ta-lib/ta_func/ta_MACDFIX.c \ 
		3rdparty/ta-lib/ta_func/ta_WILLR.c \ 
		3rdparty/ta-lib/ta_func/ta_CCI.c \ 
		3rdparty/ta-lib/ta_func/ta_MAMA.c \ 
		3rdparty/ta-lib/ta_func/ta_ATR.c \ 
		3rdparty/ta-lib/ta_func/ta_STOCHRSI.c \ 
		3rdparty/ta-lib/ta_func/ta_CDLABANDONEDBABY.c \ 
		3rdparty/ta-lib/ta_func/ta_TANH.c \ 
		3rdparty/ta-lib/ta_func/ta_SAREXT.c \ 
		3rdparty/ta-lib/ta_func/ta_CDLADVANCEBLOCK.c \ 
		3rdparty/ta-lib/ta_func/ta_utility.c \ 
		3rdparty/ta-lib/ta_func/ta_ROCR.c \ 
		3rdparty/ta-lib/ta_func/ta_ROCR100.c \ 
		3rdparty/ta-lib/ta_func/ta_OBV.c \ 
		3rdparty/ta-lib/ta_func/ta_NATR.c \ 
		3rdparty/ta-lib/ta_func/ta_CDLEVENINGSTAR.c \ 
		3rdparty/ta-lib/ta_func/ta_MINUS_DI.c \ 
		3rdparty/ta-lib/ta_func/ta_SUM.c \ 
		3rdparty/ta-lib/ta_func/ta_CDLCONCEALBABYSWALL.c \ 
		3rdparty/ta-lib/ta_func/ta_HT_SINE.c \ 
		3rdparty/ta-lib/ta_func/ta_MININDEX.c \ 
		3rdparty/ta-lib/ta_func/ta_CDL2CROWS.c \ 
		3rdparty/ta-lib/ta_func/ta_CDLSEPARATINGLINES.c \ 
		3rdparty/ta-lib/ta_func/ta_CDLSTICKSANDWICH.c \ 
		3rdparty/ta-lib/ta_func/ta_CDLTRISTAR.c \ 
		3rdparty/ta-lib/ta_func/ta_CDLSHORTLINE.c \ 
		3rdparty/ta-lib/ta_func/ta_SQRT.c \ 
		3rdparty/ta-lib/ta_func/ta_TYPPRICE.c \ 
		3rdparty/ta-lib/ta_func/ta_CDLSPINNINGTOP.c \ 
		3rdparty/ta-lib/ta_func/ta_CDLHARAMI.c \ 
		3rdparty/ta-lib/ta_func/ta_HT_TRENDLINE.c \ 
		3rdparty/ta-lib/ta_func/ta_LINEARREG_ANGLE.c \ 
		3rdparty/ta-lib/ta_func/ta_MOM.c \ 
		3rdparty/ta-lib/ta_func/ta_CDLBREAKAWAY.c \ 
		3rdparty/ta-lib/ta_func/ta_T3.c \ 
		3rdparty/ta-lib/ta_func/ta_CDLRICKSHAWMAN.c \ 
		3rdparty/ta-lib/ta_func/ta_WMA.c \ 
		3rdparty/ta-lib/ta_func/ta_CDL3BLACKCROWS.c \ 
		3rdparty/ta-lib/ta_func/ta_CDLEVENINGDOJISTAR.c \ 
		3rdparty/ta-lib/ta_func/ta_CDLDRAGONFLYDOJI.c \ 
		3rdparty/ta-lib/ta_func/ta_MAX.c \ 
		3rdparty/ta-lib/ta_func/ta_MIN.c \ 
		3rdparty/ta-lib/ta_func/ta_CDLHIKKAKE.c \ 
		3rdparty/ta-lib/ta_func/ta_CDLBELTHOLD.c \ 
		3rdparty/ta-lib/ta_func/ta_CDLDOJISTAR.c \ 
		3rdparty/ta-lib/ta_func/ta_CDLGAPSIDESIDEWHITE.c \ 
		3rdparty/ta-lib/ta_func/ta_PPO.c \ 
		3rdparty/ta-lib/ta_func/ta_AROONOSC.c \ 
		3rdparty/ta-lib/ta_func/ta_SUB.c \ 
		3rdparty/ta-lib/ta_func/ta_TEMA.c \ 
		3rdparty/ta-lib/ta_func/ta_MINMAX.c \ 
		3rdparty/ta-lib/ta_func/ta_EMA.c \ 
		3rdparty/ta-lib/ta_func/ta_CDLKICKING.c \ 
		3rdparty/ta-lib/ta_func/ta_MFI.c \ 
		3rdparty/ta-lib/ta_func/ta_CDLXSIDEGAP3METHODS.c \ 
		3rdparty/ta-lib/ta_func/ta_LN.c \ 
		3rdparty/ta-lib/ta_func/ta_COSH.c \ 
		3rdparty/ta-lib/ta_func/ta_CDLHIKKAKEMOD.c \ 
		3rdparty/ta-lib/ta_func/ta_ADOSC.c \ 
		3rdparty/ta-lib/ta_func/ta_MULT.c \ 
		3rdparty/ta-lib/ta_func/ta_SINH.c \ 
		3rdparty/ta-lib/ta_func/ta_MA.c \ 
		3rdparty/ta-lib/ta_func/ta_CDLCOUNTERATTACK.c \ 
		3rdparty/ta-lib/ta_func/ta_AROON.c \ 
		3rdparty/ta-lib/ta_func/ta_ASIN.c \ 
		3rdparty/ta-lib/ta_func/ta_CDL3WHITESOLDIERS.c \ 
		3rdparty/ta-lib/ta_func/ta_ROCP.c \ 
		3rdparty/ta-lib/ta_func/ta_LINEARREG_INTERCEPT.c \ 
		3rdparty/ta-lib/ta_func/ta_CDLMATHOLD.c \ 
		3rdparty/ta-lib/ta_func/ta_PLUS_DI.c \ 
		3rdparty/ta-lib/ta_func/ta_CDLTAKURI.c \ 
		3rdparty/ta-lib/ta_func/ta_AVGPRICE.c \ 
		3rdparty/ta-lib/ta_func/ta_CDLMORNINGDOJISTAR.c \ 
		3rdparty/ta-lib/ta_func/ta_CDLPIERCING.c \ 
		3rdparty/ta-lib/ta_func/ta_TRIX.c \ 
		3rdparty/ta-lib/ta_func/ta_DEMA.c \ 
		3rdparty/ta-lib/ta_func/ta_HT_TRENDMODE.c \ 
		3rdparty/ta-lib/ta_func/ta_APO.c \ 
		3rdparty/ta-lib/ta_func/ta_CDLKICKINGBYLENGTH.c \ 
		3rdparty/ta-lib/ta_func/ta_CDL3LINESTRIKE.c \ 
		3rdparty/ta-lib/ta_func/ta_SMA.c \ 
		3rdparty/ta-lib/ta_func/ta_MIDPRICE.c \ 
		3rdparty/ta-lib/ta_func/ta_CDLDOJI.c \ 
		3rdparty/ta-lib/ta_func/ta_CMO.c \ 
		3rdparty/ta-lib/ta_func/ta_CORREL.c \ 
		3rdparty/ta-lib/ta_func/ta_MAVP.c \ 
		3rdparty/ta-lib/ta_func/ta_MIDPOINT.c \ 
		3rdparty/ta-lib/ta_func/ta_PVI.c \ 
		3rdparty/ta-lib/ta_func/ta_DIV.c \ 
		3rdparty/ta-lib/ta_func/ta_TRIMA.c \ 
		3rdparty/ta-lib/ta_func/ta_PLUS_DM.c \ 
		3rdparty/ta-lib/ta_func/ta_BBANDS.c \ 
		3rdparty/ta-lib/ta_func/ta_TAN.c \ 
		3rdparty/ta-lib/ta_func/ta_SIN.c \ 
		3rdparty/ta-lib/ta_func/ta_CDLLONGLEGGEDDOJI.c \ 
		3rdparty/ta-lib/ta_func/ta_ULTOSC.c \ 
		3rdparty/ta-lib/ta_func/ta_KAMA.c \ 
		3rdparty/ta-lib/ta_func/ta_CDLLADDERBOTTOM.c \ 
		3rdparty/ta-lib/ta_func/ta_ADXR.c \ 
		3rdparty/ta-lib/ta_func/ta_CDL3STARSINSOUTH.c \ 
		3rdparty/ta-lib/ta_func/ta_CDLLONGLINE.c \
		3rdparty/libjson/_internal/Source/JSONAllocator.cpp \
		3rdparty/libjson/_internal/Source/JSONChildren.cpp \
		3rdparty/libjson/_internal/Source/JSONDebug.cpp \
		3rdparty/libjson/_internal/Source/JSONIterators.cpp \
		3rdparty/libjson/_internal/Source/JSONMemory.cpp \
		3rdparty/libjson/_internal/Source/JSONNode.cpp \
		3rdparty/libjson/_internal/Source/JSONNode_Mutex.cpp \
		3rdparty/libjson/_internal/Source/JSONPreparse.cpp \
		3rdparty/libjson/_internal/Source/JSONStream.cpp \
		3rdparty/libjson/_internal/Source/JSONValidator.cpp \
		3rdparty/libjson/_internal/Source/JSONWorker.cpp \
		3rdparty/libjson/_internal/Source/JSONWriter.cpp \
		3rdparty/libjson/_internal/Source/internalJSONNode.cpp \
		3rdparty/libjson/_internal/Source/libjson.cpp


HEADERS  += 	include/common.h \
		include/priceupdater.h \
		include/mainwindow.h \ 
		include/loadcsvdialog.h \
		include/progressdialog.h \
		include/waitdialog.h \
		include/datamanagerdialog.h \
		include/databrowserdialog.h \
		include/templatemanagerdialog.h \
		include/downloaddatadialog.h \
		include/dynparamsdialog.h \
		include/optionsdialog.h \
		include/feedyahoo.h \
		include/feedgoogle.h \
		include/function_dataset.h \
		include/appdata.h \
		include/defs.h \
		include/splashdialog.h \
		include/infodialog.h \
		include/licensedialog.h \
		include/manualdialog.h \
		include/netservice.h \
		include/qtachart_core.h \
		include/qtachart_eventfilters.h \
		include/qtachart.h \
		include/qtachart_help.h \
		include/qtachart_data.h \
		include/qtachart_object.h \
		include/qtachart_properties.h \
		include/qtachart_draw.h \
		include/qtachart_functions.h \
		include/qtachart_objects.h \
		include/textobjectdialog.h \
		include/lineobjectdialog.h \
		include/appColorDialog.h \
		include/qtcgraphicsitem.h \
		3rdparty/ta-lib/include/ta_frame_priv.h \
		3rdparty/ta-lib/include/ta_java_defs.h \
		3rdparty/ta-lib/include/ta_frame.h \
		3rdparty/ta-lib/include/ta_def_ui.h \
		3rdparty/ta-lib/include/ta_memory.h \
		3rdparty/ta-lib/include/ta_pragma.h \
		3rdparty/ta-lib/include/ta_global.h \
		3rdparty/ta-lib/include/ta_magic_nb.h \
		3rdparty/ta-lib/include/ta_utility.h \
		3rdparty/ta-lib/include/ta_abstract.h \
		3rdparty/ta-lib/include/ta_common.h \
		3rdparty/ta-lib/include/ta_defs.h \
		3rdparty/ta-lib/include/ta_func.h \
		3rdparty/ta-lib/include/ta_libc.h \
		3rdparty/sqlite3/include/sqlite3ext.h \
		3rdparty/sqlite3/include/sqlite3.h \
		3rdparty/libjson/JSONOptions.h \
		3rdparty/libjson/libjson.h

RESOURCES += 	resources/ChartGeany.qrc

FORMS    += forms/mainwindow.ui \
	    	forms/loadcsvdialog.ui \
	    	forms/datamanagerdialog.ui \
	    	forms/databrowserdialog.ui \
			forms/templatemanagerdialog.ui \
	    	forms/downloaddatadialog.ui \
	    	forms/dynparamsdialog.ui \
	    	forms/progressdialog.ui \
	    	forms/waitdialog.ui \
	    	forms/qtachart.ui \
	    	forms/qtacdraw.ui \
	    	forms/qtacfunctions.ui \
	    	forms/qtacobjects.ui \
	    	forms/qtachelp.ui \
			forms/qtacdata.ui \
	    	forms/qtacproperties.ui \
	    	forms/textobjectdialog.ui \
	    	forms/lineobjectdialog.ui \
	    	forms/optionsdialog.ui \
			forms/splashdialog.ui \
			forms/infodialog.ui \
			forms/licensedialog.ui \
			forms/manualdialog.ui 

