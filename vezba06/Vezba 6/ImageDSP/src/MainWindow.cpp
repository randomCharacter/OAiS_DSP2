#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "Project.h"
#include "ImageViewer.h"
#include "UniversalItemDelegate.h"
#include "HoverEditTriggerListView.h"
#include "ParamItem.h"

#include <QShortcut>
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>

Q_DECLARE_METATYPE(QImage*);

MainWindow::MainWindow(QWidget* parent)
		: QMainWindow(parent), ui(new Ui_mainWindow) {
	ui->setupUi(this);

	tabifyDockWidget(ui->outImgsListDock, ui->inImgsListDock);
	tabifyDockWidget(ui->outImgsListDock, ui->progListDock);

	project = new Project("prjs/dct.imgdsp", this);


	connect(
		project,
		SIGNAL(imageProcessingDone()),
		this,
		SLOT(updateImageViews()));

	QShortcut *shortcut = new QShortcut(QKeySequence("Ctrl+S"), this);
	connect(shortcut, SIGNAL(activated()), project, SLOT(save()));

	initializeProgramListTab();
	initializeInputImageTab();
	initializeOutputImageTab();
	initializeImageViews();
	initializeParamView();
}

MainWindow::~MainWindow() {
	delete ui;
}

void MainWindow::updateImageViews() {
	//TODO Add map of out and in QImage* to ImageViewer* fo updating.

	inputImageViewer->clear();
	outputImageViewer->clear();


	inputImageViewer->setImage(*project->getCurrentInImg());

	outputImageViewer->setImage(*project->getCurrentOutImg());
}

void MainWindow::initializeProgramListTab()
{
	ui->progList->setModel(project->getProgsModel());
	//TODO Use activated() signal?
	connect(
			ui->progList,
			SIGNAL(clicked(QModelIndex)),
			project,
			SLOT(currentProgChanged(QModelIndex)));

	/*
	ui->progList->setCurrentIndex(
		project->getProgsModel()->index(0, 0, QModelIndex())
	);
	*/

}

void MainWindow::initializeOutputImageTab()
{
	QVariant v = project->getOutImgsModel()->data(project->getOutImgsModel()->index(0, 0));
	ui->outImageText->setText(v.toString());
	connect(
		ui->browseOutImg,
		SIGNAL(clicked(bool)),
		this,
		SLOT(outputFileBrowse()));
	connect(
		ui->saveOutImg,
		SIGNAL(clicked(bool)),
		this,
		SLOT(saveOutputFile()));
}


void MainWindow::initializeInputImageTab()
{
	//TO DO: allow multiple images
	ui->inImgsList->setModel(project->getInImgsModel());
	//TODO Use activated() signal?
	connect(
			ui->inImgsList,
			SIGNAL(clicked(QModelIndex)),
			project,
			SLOT(currentInImgChanged(QModelIndex)));
}

void MainWindow::initializeImageViews()
{
	//TODO Setup QListView for in and out images and make them without editors.
	// Make possible to open new ImageView on double click.
	inputImageViewer = new ImageViewer(this);
	ui->inputImgViewLayout->addWidget(inputImageViewer);

	outputImageViewer = new ImageViewer(this);
	ui->outputImgViewLayout->addWidget(outputImageViewer);
}

void MainWindow::initializeParamView()
{
	QListView* paramsList = new HoverEditTriggerListView(this);
	ui->paramsListLayout->addWidget(paramsList);
	ItemWidget* paramItem = new ParamItem();
	//TODO Separate view  and editor,
	// view to have labels and editor line edits.
	UniversalItemDelegate* paramsListDelegate = new UniversalItemDelegate(
			paramItem,
			paramItem,
			this);
	paramsList->setItemDelegate(paramsListDelegate);

	paramsList->setModel(project->getParamsModel());
}

void MainWindow::outputFileBrowse()
{
	QString filename = QFileDialog::getSaveFileName(this,
		tr("Save Image As..."), ui->outImageText->text(), tr("Image Files (*.png *.jpg *.bmp)"));
	ui->outImageText->setText(filename);
}

void MainWindow::saveOutputFile()
{
	QAbstractItemModel* m2 = project->getOutImgsModel();
	QVariant v2 = m2->data(m2->index(0, 1));
	QImage* i2 = v2.value<QImage*>();
	i2->save(ui->outImageText->text());
}

void MainWindow::inputFileAdd()
{
	QString filename = QFileDialog::getOpenFileName(this,
		tr("Open Image"), QString(), tr("Image Files (*.png *.jpg *.bmp)"));


	QImage in = QImage(QString(filename));
	QVariant tmp = QVariant::fromValue(new QImage(in.convertToFormat(QImage().Format_RGB888)));
	//TO DO: add image to table model

}

void MainWindow::inputFileDelete()
{
	//TO DO: remove selected image
}

