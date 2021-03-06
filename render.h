#ifndef RENDER_H
#define RENDER_H

#include <QtGui>
#include <QGLWidget>
#include <QtOpenGL>
#include "pointcloud.h"
#include "Qt-based-common-libs/globj.h"

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
	void setTreeView(QTreeView *treeView);
	void updateTreeView();
	float getFrustHeight();
	float getFrustWidth();

	GLObj mMesh;
	PointCloud mPC;
	bool Rotating, mShift, mDragging;
	QVector3D mStartPoint, mSelectionDir, mCamOldPoint, mCamPoint;
	QQuaternion mRot, mOld;
	float mFrustWidth, mFrustHeight, mFrustNear, mFrustFar, mFrustRatio, mFrustZoom;
	int mCurDragX, mCurDragY;
	QGLBuffer mBufInt, mBufFloat;
	QTreeView *mTreeView;
	
public slots:
	void redraw();
	
};

#endif // RENDER_H
