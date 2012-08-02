#ifndef RENDER_H
#define RENDER_H

#include <QtGui>
#include <QGLWidget>

class Render : public QGLWidget
{
	Q_OBJECT
public:
	explicit Render(QWidget *parent = 0);
	void paintGL();
	void initializeGL();
	void mousePressEvent(QMouseEvent *);
	void mouseMoveEvent(QMouseEvent *);
	void mouseRelaseEvent(QMouseEvent *);
	bool Rotating;
	QVector3D StartPoint;
	QQuaternion Rot, Old;
	QVector3D getVector(int x, int y);

signals:
	
public slots:
	void redraw();
	
};

#endif // RENDER_H
