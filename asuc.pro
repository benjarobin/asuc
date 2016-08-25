
QT += core gui widgets network

TARGET = asuc
TEMPLATE = app

SOURCES += src/Application.cpp \
    src/MainWindow.cpp \
    src/MainWidget.cpp \
    src/ConsoleTextEdit.cpp

HEADERS += src/MainWindow.h \
    src/MainWidget.h \
    src/ConsoleTextEdit.h \
    src/Version.h

RESOURCES += \
    ressource/img.qrc

