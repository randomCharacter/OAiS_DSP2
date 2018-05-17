/**
 * @file ImageViewer.cpp
 * @date Jan 7, 2014
 *
 * @author Milos Subotic <milos.subotic.sm@gmail.com>
 * @license GPLv3
 *
 * @brief Image viewer.
 *
 * @version 1.0
 * Changelog:
 * 1.0 - Initial version.
 *
 */

///////////////////////////////////////////////////////////////////////////////

#include "ImageViewer.h"

#include <QWheelEvent>
#include <QScrollBar>

#include <QDebug>

///////////////////////////////////////////////////////////////////////////////

#define SCALE_FACTOR 1.0

///////////////////////////////////////////////////////////////////////////////

ImageViewer::ImageViewer(QWidget* parent)
		: QGraphicsView(parent) {

	_firstSetImage = true;

	setDragMode(ScrollHandDrag);
	_scene = new QGraphicsScene(this);

	setScene(_scene);
}

ImageViewer::~ImageViewer() {
}

void ImageViewer::setImage(const QImage& image) {
	_scene->addPixmap(QPixmap::fromImage(image));
	_scene->setSceneRect(0, 0, image.width(), image.height());
	if(_firstSetImage){
		_firstSetImage = false;
	}
}

void ImageViewer::clear() {
	_scene->clear();
}

QImage ImageViewer::getImage() {
	QRectF rect = _scene->sceneRect();
	QImage image(
			rect.size().width(),
			rect.size().height(),
			QImage::Format_RGB32);
	QPainter painter(&image);
	_scene->render(&painter);
	return image;
}

void ImageViewer::zoomIn() {
	scale(SCALE_FACTOR, SCALE_FACTOR);
}

void ImageViewer::zoomOut() {
	scale(1.0 / SCALE_FACTOR, 1.0 / SCALE_FACTOR);
}

void ImageViewer::zoomToFit() {
	if(_scene->sceneRect() != QRect()){
		// TODO Cannot get rect of scroll area white area without scroll bars.

		int sw = _scene->sceneRect().width();
		int sh = _scene->sceneRect().height();
		qreal widthScale = qreal(
				viewport()->rect().width() - verticalScrollBar()->width() - 3)
				/ sw;
		qreal heightScale = qreal(
				viewport()->rect().height() - horizontalScrollBar()->height()
						- 3) / sh;
		qreal minScale = qMin(widthScale, heightScale);

		resetMatrix();
		scale(minScale, minScale);
		centerOn(0, 0);
	}
}

void ImageViewer::wheelEvent(QWheelEvent* event) {
	setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

	// Scale the view / do the zoom
	if(event->delta() > 0){
		zoomIn();
	}else{
		zoomOut();
	}

	// Don't call superclass handler here
	// as wheel is normally used for moving scrollbars
}

///////////////////////////////////////////////////////////////////////////////
