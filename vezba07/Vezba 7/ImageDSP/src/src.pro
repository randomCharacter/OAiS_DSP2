
QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ImageDSP
TEMPLATE = app
DESTDIR = ../bin

SOURCES += \
    main.cpp \
    ColorSpaces.cpp \
    DCT.cpp \
    HoverEditTriggerListView.cpp \
    ImageFIlter.cpp \
    ImageProcessing.cpp \
    ImageViewer.cpp \
    InImgItem.cpp \
    ItemWidget.cpp \
    MainWindow.cpp \
    NxNDCT.cpp \
    ParamItem.cpp \
    Project.cpp \
    TableModel.cpp \
    UniversalItemDelegate.cpp


HEADERS += \
    MainWindow.h \
    ColorSpaces.h \
    DCT.h \
    HoverEditTriggerListView.h \
    ImageFilter.h \
    ImageProcessing.h \
    ImageViewer.h \
    InImgItem.h \
    ItemWidget.h \
    NxNDCT.h \
    ParamItem.h \
    Project.h \
    TableModel.h \
    UniversalItemDelegate.h

FORMS += \
        MainWindow.ui \
        InImgItem.ui \
        ParamItem.ui \
    InImgItem.ui \
    MainWindow.ui \
    ParamItem.ui

opencl {
        LIBS += -L../lib -L../bin
        include(../opencl/opencl_dep.pri)
}

SUBDIRS += \
    src.pro

