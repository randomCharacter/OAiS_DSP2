
QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = ImageDSP
TEMPLATE = app
DESTDIR = ../bin

SOURCES += \
    HoverEditTriggerListView.cpp \
    ImageInterpolation.cpp \
    ImageProcessing.cpp \
    ImageViewer.cpp \
    InImgItem.cpp \
    ItemWidget.cpp \
    main.cpp \
    MainWindow.cpp \
    ParamItem.cpp \
    Project.cpp \
    qcustomplot.cpp \
    TableModel.cpp \
    UniversalItemDelegate.cpp

HEADERS += \
    HoverEditTriggerListView.h \
    ImageInterpolation.h \
    ImageProcessing.h \
    ImageViewer.h \
    InImgItem.h \
    ItemWidget.h \
    MainWindow.h \
    ParamItem.h \
    Project.h \
    qcustomplot.h \
    TableModel.h \
    UniversalItemDelegate.h

FORMS += \
    InImgItem.ui \
    MainWindow.ui \
    ParamItem.ui

opencl {
	LIBS += -L../lib -L../bin
	include(../opencl/opencl_dep.pri)
}

SUBDIRS += \
    src.pro

