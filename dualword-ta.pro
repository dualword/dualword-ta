TARGET = dualword-ta
TEMPLATE = app
CONFIG += qt thread console debug
QT += core gui network webkit

DEPENDPATH += .
INCLUDEPATH += .
	
QMAKE_CXXFLAGS += -std=c++11

include(freechartgeany.pri)
HEADERS += \
	src-dualword-ta/DualwordTA.h \
	src-dualword-ta/MainWindowTA.h

SOURCES += \
	src-dualword-ta/main.cpp \
	src-dualword-ta/DualwordTA.cpp \
	src-dualword-ta/MainWindowTA.cpp

FORMS += \
	src-dualword-ta/MainWindowTA.ui
	
OBJECTS_DIR = .build/obj
MOC_DIR     = .build/moc
RCC_DIR     = .build/rcc
UI_DIR      = .build/ui
