#include "render.h"

Render::Render(QWidget *parent) :
	QGLWidget(parent)
{
	mRot = QQuaternion::fromAxisAndAngle(0,0,0,90);
	mTreeView = NULL;
	mDragging = mShift = Rotating = false;
	mFrustWidth = mFrustHeight = 1;
	mFrustZoom = 1;
	mFrustNear = 1;
	mFrustFar = 15.1;
	mSelectionDir = QVector3D(1,1,1);
	mCamPoint = QVector3D(0,0,5);
	mCurDragX = mCurDragY = 0;
	calcRatio();
	updatePointCloudDist();
}

void Render::calcRatio() {
	mFrustRatio = height()/((float)width());
}

void Render::redraw() {
	paintGL();
}


void Render::initializeGL() {
	glClearColor( 0.1, 0.1, 0.1, 0.0 );
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_DOUBLE);
	glEnable(GL_BLEND);
	glPushClientAttrib( GL_CLIENT_VERTEX_ARRAY_BIT );
	mMesh.loadFile("box.obj", QVector3D(1,1,1));
	for (int i = 0; i < mMesh.Vertices.size()/3; i++) {
		mMesh.SetColorOrTexPos(i, QVector3D(1,1,1));
	}
	mMesh.uploadVert();
	mPC.addPoints(mMesh.Vertices.toList());
}
QVector3D Render::getVector(int x, int y) {
	float ym, xm, lComp;
	xm = x/((float)this->width())-0.5;
	ym = 0.5-y/((float)this->height());
	float r = sqrt(xm*xm + ym*ym);
	if (r >= 0.5) {
		xm /= r;
		ym /= r;
		lComp = 0;
	} else	{
		xm *= 2;
		ym *= 2;
		lComp = sqrt(1-4*r*r);
	}
	return QVector3D(xm, ym, lComp).normalized();
}

void Render::updatePointCloudDist() {
	qDebug() << sqrt(pow(mFrustFar, 2) + pow(mFrustWidth, 2) + pow(mFrustHeight, 2));
	mPC.setMaxdist(sqrt(pow(mFrustFar, 2) + pow(mFrustWidth, 2) + pow(mFrustHeight, 2)));
}

void Render::resizeEvent(QResizeEvent *event) {
	glViewport( 0, 0, (GLint)event->size().width(), (GLint)event->size().height());
	calcRatio();
}

void Render::setShift(bool state) {
	mShift = state;
}

void Render::mousePressEvent(QMouseEvent *event) {
	if (!mShift) {
		if (event->button() == Qt::RightButton) {
			mOld = mRot;
			Rotating = true;
			mStartPoint = getVector(event->x(), event->y());
		} else if (event->button() == Qt::LeftButton) {
			QMatrix4x4 mat = rotToMatrix();
			QVector3D frustNearPoint = QVector3D(xToViewX(event->x(), mFrustNear), yToViewY(event->y(), mFrustNear), -mFrustNear);
			QVector3D frustFarPoint = QVector3D(xToViewX(event->x(), mFrustFar/mFrustNear), yToViewY(event->y(), mFrustFar/mFrustNear), -mFrustFar);
			QVector3D direction = (frustFarPoint - frustNearPoint);
			QVector3D startPoint = mCamPoint * mat;
			mSelectionDir = (direction * mat);
			qDebug() << mSelectionDir;
			mCamOldPoint = startPoint;
			qDebug() << startPoint;
			mPC.selectNearestPoint(mSelectionDir.normalized(), startPoint);
			for (int i = 0; i < mMesh.Vertices.size()/3; i++) {
				mMesh.SetColorOrTexPos(i, QVector3D(1,1,1));
			}
			for (int i = 0; i < mPC.mSelected.size(); i++) {
				mMesh.SetColorOrTexPos(*mPC.mSelected[i], QVector3D(1,0,0));
			}
			mMesh.uploadVert();
			paintGL();
		}
	} else {
		if (event->button() == Qt::RightButton) {
			mDragging = true;
			updateCurDrag(event);
		}
	}
}

void Render::mouseMoveEvent(QMouseEvent *event) {
	if (Rotating) {
		QVector3D cur = getVector(event->x(), event->y());
		qreal dot = QVector3D::dotProduct(mStartPoint, cur);
		if (fabs(dot) < 0.000001 || acos(dot) > 9000 || acos(dot) < -9000) {
			return;
		}
		QVector3D cross = QVector3D::crossProduct(mStartPoint, cur);
		if (cross.length() < 0.000001) {
			return;
		}
		mRot = (QQuaternion::fromAxisAndAngle(cross, (float)(acos(dot)/3.14 )*180) * mOld).normalized();
		mStartPoint = cur;
		mOld = mRot;
	} else if (mDragging) {
		mCamPoint +=  QVector3D((event->x()-mCurDragX)*0.04, (event->y()-mCurDragY)*0.04, 0);
		updateCurDrag(event);
	}
}

void Render::updateCurDrag(QMouseEvent *event) {
	mCurDragX = event->x();
	mCurDragY = event->y();
}

void Render::mouseReleaseEvent(QMouseEvent *event) {
	if (event->button() == Qt::RightButton) {
		Rotating = false;
		mDragging = false;
	}
}

void Render::wheelEvent(QWheelEvent *event) {
	if (!mShift) {
		mFrustZoom *= (event->delta() > 0 ? 2 : 0.5);
		if (mFrustZoom < 0.125) {
			mFrustZoom = 0.125;
		} else if (mFrustZoom > 8) {
			mFrustZoom = 8;
		}
	} else {
		mCamPoint += QVector3D(0,0,(event->delta() > 0 ? 0.4 : -0.4));
	}
}

float Render::xPixToDouble(int x) {
	return ((x*2)/((float)width())-1);
}
float Render::yPixToDouble(int y) {
	return (1-(y*2)/((float)height()));
}

float Render::xToViewX(int x, float z) {
	return ((xPixToDouble(x)) * z * mFrustZoom);
}

float Render::yToViewY(int y, float z) {
	return ((yPixToDouble(y)) * z * mFrustZoom *mFrustRatio);
}

QMatrix4x4 Render::rotToMatrix(){
	QMatrix4x4 mat = QMatrix4x4();
	mat.setToIdentity();
	mat.rotate(mRot);
	return mat;
}

void Render::rotToFloatArray(float conv[16]) {
	QMatrix4x4 mat = rotToMatrix();
	qreal *from = mat.data();
	for (int i = 0; i < 16; i++) {
		conv[i] = from[i];
	}
}

void Render::setTreeView(QTreeView *treeView) {
	mTreeView = treeView;
}

void Render::updateTreeView() {
}

float Render::getFrustWidth() {
	return mFrustWidth * mFrustZoom;
}
float Render::getFrustHeight() {
	return mFrustHeight * mFrustRatio * mFrustZoom;
}
void Render::paintGL() {
	glClear(GL_DEPTH_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-getFrustWidth(), getFrustWidth(), -getFrustHeight(), getFrustHeight(), mFrustNear, mFrustFar);
	glTranslatef(-mCamPoint.x(),-mCamPoint.y(),-mCamPoint.z());
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	float conv[16];
	rotToFloatArray(conv);
	glMultMatrixf(conv);

	/*float xt = -3;
	float xf = 3;
	float yf = -3;
	float yt = 3;
	float z = 0;*/

	glColor4f(1, 1, 1, 1);
	glLineWidth(10);
	glBegin(GL_LINES);
	glVertex3f(mCamOldPoint.x(),mCamOldPoint.y(),mCamOldPoint.z());
	glVertex3f(mCamOldPoint.x() + mSelectionDir.x(), mCamOldPoint.y() + mSelectionDir.y(), mCamOldPoint.z() + mSelectionDir.z());
	glEnd();

	glColor4f(1, 0, 0, 1);
	glLineWidth(10);
	glBegin(GL_LINES);
	glVertex3f(mCamOldPoint.x() + mSelectionDir.x(), mCamOldPoint.y() + mSelectionDir.y(), mCamOldPoint.z() + mSelectionDir.z());
	glVertex3f(mCamOldPoint.x() + mSelectionDir.x()*2, mCamOldPoint.y() + mSelectionDir.y()*2, mCamOldPoint.z() + mSelectionDir.z()*2);
	glEnd();
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glColor3f(1,1,1);
	mMesh.draw(0,0,0, false, true);
/*	glBegin(GL_QUADS);
	glVertex3f(xf,yt,z);
	glVertex3f(xf,yf,z);
	glVertex3f(xt,yf,z);
	glVertex3f(xt,yt,z);
	glEnd();*/
	glFlush();
	glFinish();
	swapBuffers();
}
