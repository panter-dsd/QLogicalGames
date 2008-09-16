TEMPLATE = app
QT = gui core sql
CONFIG += qt warn_on release
DESTDIR = bin
OBJECTS_DIR = build
MOC_DIR = build
UI_DIR = build
HEADERS = src/mainwindow.h src/qmynewgamedialog.h src/qmyinformationpanel.h src/qmyundo.h
SOURCES = src/mainwindow.cpp \
 src/main.cpp \
 src/qmynewgamedialog.cpp \
 src/qmyinformationpanel.cpp \
 src/qmyundo.cpp
RESOURCES = resource.qrc
TRANSLATIONS += share/languages/qnumbersdestroyer_ru.ts
