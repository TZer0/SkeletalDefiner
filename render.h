#ifndef RENDER_H
#define RENDER_H

#include <QtGui>
#include <QGLWidget>
#include <assimp/assimp.hpp>
#include <string>
#include "pointcloud.h"

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
	void updatePointCloudDist();
	float xPixToDouble(int);
	float yPixToDouble(int);
	float xToViewX(int, float);
	float yToViewY(int, float);
	QVector3D getVector(int x, int y);
	QMatrix4x4 rotToMatrix();
	void rotToFloatArray(float conv[16]);

	PointCloud PC;
	bool Rotating;
	QVector3D StartPoint, SelectionDir;
	QQuaternion Rot, Old;
	float FrustWidth, FrustHeight, FrustNear, FrustFar, FrustRatio;

signals:
	
public slots:
	void redraw();
	
};

#endif // RENDER_H
