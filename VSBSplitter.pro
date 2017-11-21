#-------------------------------------------------
#
# Project created by QtCreator 2017-01-04T15:45:44
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = VSBSplitter
TEMPLATE = app

OTHER_FILES += logo.rc
RC_FILE = logo.rc

SOURCES += main.cpp\
           Dialog.cpp \
           VSBSplit.cpp \
           CVSB.cpp

HEADERS  += Dialog.h \
            VSBSplit.h \
            CVSB.h

FORMS    += Dialog.ui
