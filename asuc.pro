
QT += core gui widgets network

TARGET = asuc
TEMPLATE = app

QMAKE_CXXFLAGS += -std=c++11 -Wall -Wextra -Wunused -Wcast-qual -Wshadow \
        -DQT_NO_CAST_TO_ASCII=1 -DQT_NO_CAST_FROM_BYTEARRAY=1 -DQT_NO_CAST_FROM_ASCII=1

SOURCES += src/Application.cpp \
    src/MainWindow.cpp \
    src/MainWidget.cpp \
    src/ConsoleTextEdit.cpp

HEADERS += src/MainWindow.h \
    src/MainWidget.h \
    src/ConsoleTextEdit.h \
    src/Version.h \
    src/Settings.h

RESOURCES += \
    ressource/img.qrc
