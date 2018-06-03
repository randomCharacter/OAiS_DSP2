/**
 * @file ImageViewer.h
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

#ifndef IMAGEVIEWER_H_
#define IMAGEVIEWER_H_

///////////////////////////////////////////////////////////////////////////////

#include <QGraphicsView>

///////////////////////////////////////////////////////////////////////////////

class ImageViewer : public QGraphicsView {
Q_OBJECT

public:
	ImageViewer(QWidget* parent = 0);
	virtual ~ImageViewer();

public:
	void setImage(const QImage& image);
	void clear();
	QImage getImage();

public slots:
	void zoomIn();
	void zoomOut();
	void zoomToFit();

protected:
	void wheelEvent(QWheelEvent* event);

protected:
	QGraphicsScene* _scene;
	bool _firstSetImage;

};

///////////////////////////////////////////////////////////////////////////////

#endif /* IMAGEVIEWER_H_ */
