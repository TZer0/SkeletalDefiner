#ifndef RENDER_H
#define RENDER_H

#include <QtGui>
#include <QGLWidget>
#include <assimp/assimp.hpp>

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
	QVector3D getVector(int x, int y);
	bool Rotating;
	QVector3D StartPoint;
	QQuaternion Rot, Old;

signals:
	
public slots:
	void redraw();
	
};

#endif // RENDER_H
