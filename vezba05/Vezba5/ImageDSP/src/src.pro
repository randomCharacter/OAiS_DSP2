
QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ImageDSP
TEMPLATE = app
DESTDIR = ../bin

SOURCES += \
	main.cpp \
	MainWindow.cpp \
	ImageViewer.cpp \
	ItemWidget.cpp \
	InImgItem.cpp \
	ParamItem.cpp \
	UniversalItemDelegate.cpp \
	HoverEditTriggerListView.cpp \
	TableModel.cpp \
	Project.cpp \
	ImageProcessing.cpp \
	ColourSpaces.cpp

HEADERS += \
	MainWindow.h \
	ImageViewer.h \
	ItemWidget.h \
	InImgItem.h \
	ParamItem.h \
	UniversalItemDelegate.h \
	HoverEditTriggerListView.h \
	TableModel.h \
	Project.h \
	ImageProcessing.h \
	ColourSpaces.h

FORMS += \
	MainWindow.ui \
	InImgItem.ui \
	ParamItem.ui

opencl {
	LIBS += -L../lib -L../bin
	include(../opencl/opencl_dep.pri)
}

