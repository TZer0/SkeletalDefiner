#ifndef RENDER_H
#define RENDER_H

#include <QtGui>
#include <QGLWidget>
#include <assimp/assimp.hpp>
#include <string>

class Render : public QGLWidget
{
	Q_OBJECT
public:
	explicit Render(QWidget *parent = 0);
	void paintGL();
	void initializeGL();
	void mousePressEvent(QMouseEvent *);
	void mouseMoveEvent(QMouseEvent *);
	void mouseReleaseEvent(QMouseEvent *);
	void resizeEvent(QResizeEvent *);
	void loadMesh(std::string);
	void calcRatio();
	float xPixToDouble(int);
	float yPixToDouble(int);
	float xToViewX(int, float);
	float yToViewY(int, float);
	QVector3D getVector(int x, int y);
	QMatrix4x4 rotToMatrix();
	void rotToFloatArray(float conv[16]);
	bool Rotating;
	QVector3D StartPoint, SelectionDir;
	QQuaternion Rot, Old;
	float left, right, top, bottom, near, far, ratio;

signals:
	
public slots:
	void redraw();
	
};

#endif // RENDER_H
