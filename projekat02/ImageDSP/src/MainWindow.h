#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

// Forward declarations.
class Ui_mainWindow;
class Project;
class ImageViewer;

class MainWindow : public QMainWindow {
Q_OBJECT

public:
	explicit MainWindow(QWidget* parent = 0);
	~MainWindow();

private:
	Ui_mainWindow* ui;
	Project* project;
	ImageViewer* inputImageViewer;
	ImageViewer* outputImageViewer;
private slots:
	void updateImageViews();
	void initializeProgramListTab();
	void initializeOutputImageTab();
	void initializeInputImageTab();
	void initializeImageViews();
	void initializeParamView();

	void outputFileBrowse();
	void saveOutputFile();
	void inputFileAdd();
	void inputFileDelete();

};

#endif // MAINWINDOW_H
