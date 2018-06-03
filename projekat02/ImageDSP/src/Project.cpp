#include "Project.h"

#include "TableModel.h"

#include <QFile>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>

#include <QSortFilterProxyModel>

#include <QDebug>

#include "ImageProcessing.h"

Q_DECLARE_METATYPE(QImage*);

Project::Project(const QString& fileName, QObject* parent)
		: QObject(parent), fileName(fileName) {
	progsModel = new TableModel(1, this);

	paramsProxyModel = new QSortFilterProxyModel(this);

	inImgsModel = new TableModel(2, this);
	outImgsModel = new TableModel(2, this);

	currentProg = 0;
	currentInImg = 0;
	currentOutImg = 0;

	connect(
			progsModel,
			SIGNAL(dataChanged(QModelIndex, QModelIndex)),
			this,
			SLOT(dataChanged(QModelIndex, QModelIndex)));

	connect(
			paramsProxyModel,
			SIGNAL(dataChanged(QModelIndex, QModelIndex)),
			this,
			SLOT(dataChanged(QModelIndex, QModelIndex)));

	// Open project file.
	QFile xmlFile(fileName);
	// TODO Check if file is open.
	xmlFile.open(QFile::ReadOnly | QFile::Text);
	QXmlStreamReader xml(&xmlFile);

	QVector<QVariant> r4(4);
	QVector<QVariant> r2(2);
	int nextProg = 0;
	while(!xml.atEnd()) {
		xml.readNext();
		if(xml.isStartElement()){
			if(xml.name() == "program"){
				nextProg++;
				appendProg(xml.attributes().value("name").toString());
			}else if(xml.name() == "param"){
				r4[0] = QString(xml.attributes().value("name").toString());
				// TODO Check is double OK and make error to user if not.
				r4[1] = QString(xml.attributes().value("min").toString())
						.toDouble();
				r4[2] = QString(xml.attributes().value("current").toString())
						.toDouble();
				r4[3] = QString(xml.attributes().value("max").toString())
						.toDouble();
				progSetups[nextProg-1].paramsModel->appendRow(r4);
			}else if(xml.name() == "in_img"){
				QString file_name(
						xml.attributes().value("file_name").toString());
				r2[0] = file_name;
				//TODO Nicer.
				QString full_file_name = QString("in_img/") + file_name;
				r2[1] = QVariant::fromValue(
						new QImage(
								QImage(full_file_name).convertToFormat(
										QImage::Format_RGB888)));
				inImgsModel->appendRow(r2);
			}else if(xml.name() == "out_img"){
				QString file_name(
						xml.attributes().value("file_name").toString());
				r2[0] = file_name;
				//TODO Nicer.
				QString full_file_name = QString("out_img/") + file_name;
				r2[1] = QVariant::fromValue(new QImage());
				outImgsModel->appendRow(r2);
			}
		}
	}
}

void Project::save() {
	QFile xmlFile(fileName);
	// TODO Check if file is open.
	xmlFile.open(QFile::WriteOnly | QFile::Text);
	QXmlStreamWriter xml(&xmlFile);
	xml.setAutoFormatting(true);
	xml.writeStartDocument();
	xml.writeDTD("<!DOCTYPE imgdsp>");
	xml.writeStartElement("imgdsp");

	xml.writeStartElement("programs");
	for(int prog = 0; prog < progsModel->rowCount(); prog++){
		xml.writeStartElement("program");
		xml.writeAttribute(
			"name",
			progsModel->data(progsModel->index(prog, 0)).toString()
		);

		TableModel* paramsModel = progSetups[prog].paramsModel;
		xml.writeStartElement("params");
		for(int param = 0; param < paramsModel->rowCount(); param++){
			xml.writeStartElement("param");
			xml.writeAttribute(
				"name",
				paramsModel->data(paramsModel->index(param, 0)).toString()
			);
			xml.writeAttribute(
				"min",
				paramsModel->data(paramsModel->index(param, 1)).toString()
			);
			xml.writeAttribute(
				"current",
				paramsModel->data(paramsModel->index(param, 2)).toString()
			);
			xml.writeAttribute(
				"max",
				paramsModel->data(paramsModel->index(param, 3)).toString()
			);
			xml.writeEndElement(); // param
		}
		xml.writeEndElement(); // params

		xml.writeEndElement(); // program
	}
	xml.writeEndElement(); // programs

	xml.writeStartElement("in_imgs");
	for(int i = 0; i < inImgsModel->rowCount(); i++){
		xml.writeStartElement("in_img");
		xml.writeAttribute(
			"file_name",
			inImgsModel->data(inImgsModel->index(i, 0)).toString()
		);
		xml.writeEndElement(); // in_ img
	}
	xml.writeEndElement(); // in_imgs

	xml.writeStartElement("out_imgs");
	for(int i = 0; i < outImgsModel->rowCount(); i++){
		xml.writeStartElement("out_img");
		xml.writeAttribute(
			"file_name",
			outImgsModel->data(outImgsModel->index(i, 0)).toString()
		);
		xml.writeEndElement(); // out_img
	}
	xml.writeEndElement(); // out_imgs

	xml.writeEndElement(); // imgdsp
	xml.writeEndDocument();
	xmlFile.close();
}

QAbstractItemModel* Project::getProgsModel() const {
	return progsModel;
}
QAbstractItemModel* Project::getOutImgsModel() const {
	return outImgsModel;
}
QAbstractItemModel* Project::getInImgsModel() const {
	return inImgsModel;
}
QAbstractItemModel* Project::getParamsModel() const {
	return paramsProxyModel;
}

QImage* Project::getCurrentOutImg() const {
	return outImgsModel->data(outImgsModel->index(currentOutImg, 1))
			.value<QImage*>();
}
QImage* Project::getCurrentInImg() const {
	return inImgsModel->data(inImgsModel->index(currentInImg, 1))
			.value<QImage*>();
}

void Project::currentProgChanged(const QModelIndex& index) {
	currentProg = index.row();
	paramsProxyModel->setSourceModel(progSetups[currentProg].paramsModel);

	imageProcessing();
}
void Project::currentInImgChanged(const QModelIndex& index) {
	currentInImg = index.row();

	imageProcessing();
}

void Project::appendProg(const QString& name) {
	QVector<QVariant> row(1, name);
	progsModel->appendRow(row);

	ProgSetup ps;
	ps.paramsModel = new TableModel(4, this);
	progSetups.push_back(ps);
}


void Project::dataChanged(
		const QModelIndex& topLeft,
		const QModelIndex& bottomRight) {
	Q_UNUSED(topLeft);
	Q_UNUSED(bottomRight);
	imageProcessing();
}

void Project::imageProcessing() {

	QString progName = progsModel->get(currentProg, 0).toString();

	int paramsCount = paramsProxyModel->rowCount();
	QVector<double> params(paramsCount);
	for(int i = 0; i < paramsCount; i++){
		QVariant v = paramsProxyModel->data(paramsProxyModel->index(i, 2));
		params[i] = v.toDouble();
	}

	imageProcessingFun(progName, getCurrentOutImg(), getCurrentInImg(), params);

	emit imageProcessingDone();
}

