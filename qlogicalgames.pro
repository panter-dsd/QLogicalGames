SOURCES += src/qlogicalgames.cpp \
 src/main.cpp \
 src/QNumbersDestroyer/qmynewgamedialog.cpp \
 src/QNumbersDestroyer/qmyundo.cpp \
 src/QNumbersDestroyer/qmyinformationpanel.cpp \
 src/QNumbersDestroyer/mainwidget.cpp
HEADERS += src/qlogicalgames.h \
 src/QNumbersDestroyer/qmyinformationpanel.h \
 src/QNumbersDestroyer/qmyundo.h \
 src/QNumbersDestroyer/qmynewgamedialog.h \
 src/QNumbersDestroyer/mainwidget.h
TEMPLATE = app
CONFIG += qt release warn_on
TARGET = bin/qlogicalgames
DESTDIR = bin
MOC_DIR = build
OBJECTS_DIR = build
UI_DIR = build
QT = core gui sql
RESOURCES += application.qrc
