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
	void wheelEvent(QWheelEvent *);
	void updateCurDrag(QMouseEvent *);
	void resizeEvent(QResizeEvent *);
	void loadMesh(std::string);
	void calcRatio();
	void updatePointCloudDist();
	float xPixToDouble(int);
	float yPixToDouble(int);
	float xToViewX(int, float);
	float yToViewY(int, float);
	QVector3D getVector(int, int);
	QMatrix4x4 rotToMatrix();
	void rotToFloatArray(float conv[16]);
	void setShift(bool);
	float getFrustHeight();
	float getFrustWidth();

	PointCloud PC;
	bool Rotating, Shift, Dragging;
	QVector3D StartPoint, SelectionDir, CamOldPoint, CamPoint;
	QQuaternion Rot, Old;
	float FrustWidth, FrustHeight, FrustNear, FrustFar, FrustRatio, FrustZoom;
	int CurDragX, CurDragY;

signals:
	
public slots:
	void redraw();
	
};

#endif // RENDER_H
