
QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = ImageDSP
TEMPLATE = app
DESTDIR = ../bin

SOURCES += \
    ColorSpaces.cpp \
    DCT.cpp \
    HoverEditTriggerListView.cpp \
    ImageFIlter.cpp \
    ImageHistogram.cpp \
    ImageProcessing.cpp \
    ImageViewer.cpp \
    InImgItem.cpp \
    ItemWidget.cpp \
    main.cpp \
    MainWindow.cpp \
    NxNDCT.cpp \
    ParamItem.cpp \
    Project.cpp \
    qcustomplot.cpp \
    TableModel.cpp \
    UniversalItemDelegate.cpp

HEADERS += \
    ColorSpaces.h \
    DCT.h \
    HoverEditTriggerListView.h \
    ImageFilter.h \
    ImageHistogram.h \
    ImageProcessing.h \
    ImageViewer.h \
    InImgItem.h \
    ItemWidget.h \
    MainWindow.h \
    NxNDCT.h \
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

