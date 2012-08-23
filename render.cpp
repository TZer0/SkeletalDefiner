#include "render.h"

Render::Render(QWidget *parent) :
	QGLWidget(parent)
{
	Rot = QQuaternion::fromAxisAndAngle(0,0,0,90);
	TreeView = NULL;
	Dragging = Shift = Rotating = false;
	FrustWidth = FrustHeight = 1;
	FrustZoom = 1;
	FrustNear = 1;
	FrustFar = 15.1;
	SelectionDir = QVector3D(1,1,1);
	CamPoint = QVector3D(0,0,5);
	CurDragX = CurDragY = 0;
	calcRatio();
	updatePointCloudDist();
}

void Render::calcRatio() {
	FrustRatio = height()/((float)width());
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
	Mesh.loadFile("box.obj", QVector3D(1,1,1));
	for (int i = 0; i < Mesh.Vertices.size()/3; i++) {
		Mesh.SetColorOrTexPos(i, QVector3D(1,1,1));
	}
	Mesh.uploadVert();
	PC.addPoints(Mesh.Vertices.toList());
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
	qDebug() << sqrt(pow(FrustFar, 2) + pow(FrustWidth, 2) + pow(FrustHeight, 2));
	PC.setMaxdist(sqrt(pow(FrustFar, 2) + pow(FrustWidth, 2) + pow(FrustHeight, 2)));
}

void Render::resizeEvent(QResizeEvent *event) {
	glViewport( 0, 0, (GLint)event->size().width(), (GLint)event->size().height());
	calcRatio();
}

void Render::setShift(bool state) {
	Shift = state;
}

void Render::mousePressEvent(QMouseEvent *event) {
	if (!Shift) {
		if (event->button() == Qt::RightButton) {
			Old = Rot;
			Rotating = true;
			StartPoint = getVector(event->x(), event->y());
		} else if (event->button() == Qt::LeftButton) {
			QMatrix4x4 mat = rotToMatrix();
			QVector3D frustNearPoint = QVector3D(xToViewX(event->x(), FrustNear), yToViewY(event->y(), FrustNear), -FrustNear);
			QVector3D frustFarPoint = QVector3D(xToViewX(event->x(), FrustFar/FrustNear), yToViewY(event->y(), FrustFar/FrustNear), -FrustFar);
			QVector3D direction = (frustFarPoint - frustNearPoint);
			QVector3D startPoint = CamPoint * mat;
			SelectionDir = (direction * mat);
			qDebug() << SelectionDir;
			CamOldPoint = startPoint;
			qDebug() << startPoint;
			PC.selectNearestPoint(SelectionDir.normalized(), startPoint);
			for (int i = 0; i < Mesh.Vertices.size()/3; i++) {
				Mesh.SetColorOrTexPos(i, QVector3D(1,1,1));
			}
			for (int i = 0; i < PC.Selected.size(); i++) {
				Mesh.SetColorOrTexPos(*PC.Selected[i], QVector3D(1,0,0));
			}
			Mesh.uploadVert();
			paintGL();
		}
	} else {
		if (event->button() == Qt::RightButton) {
			Dragging = true;
			updateCurDrag(event);
		}
	}
}

void Render::mouseMoveEvent(QMouseEvent *event) {
	if (Rotating) {
		QVector3D cur = getVector(event->x(), event->y());
		qreal dot = QVector3D::dotProduct(StartPoint, cur);
		if (fabs(dot) < 0.000001 || acos(dot) > 9000 || acos(dot) < -9000) {
			return;
		}
		QVector3D cross = QVector3D::crossProduct(StartPoint, cur);
		if (cross.length() < 0.000001) {
			return;
		}
		Rot = (QQuaternion::fromAxisAndAngle(cross, (float)(acos(dot)/3.14 )*180) * Old).normalized();
		StartPoint = cur;
		Old = Rot;
	} else if (Dragging) {
		CamPoint +=  QVector3D((event->x()-CurDragX)*0.04, (event->y()-CurDragY)*0.04, 0);
		updateCurDrag(event);
	}
}

void Render::updateCurDrag(QMouseEvent *event) {
	CurDragX = event->x();
	CurDragY = event->y();
}

void Render::mouseReleaseEvent(QMouseEvent *event) {
	if (event->button() == Qt::RightButton) {
		Rotating = false;
		Dragging = false;
	}
}

void Render::wheelEvent(QWheelEvent *event) {
	if (!Shift) {
		FrustZoom *= (event->delta() > 0 ? 2 : 0.5);
		if (FrustZoom < 0.125) {
			FrustZoom = 0.125;
		} else if (FrustZoom > 8) {
			FrustZoom = 8;
		}
	} else {
		CamPoint += QVector3D(0,0,(event->delta() > 0 ? 0.4 : -0.4));
	}
}

float Render::xPixToDouble(int x) {
	return ((x*2)/((float)width())-1);
}
float Render::yPixToDouble(int y) {
	return (1-(y*2)/((float)height()));
}

float Render::xToViewX(int x, float z) {
	return ((xPixToDouble(x)) * z * FrustZoom);
}

float Render::yToViewY(int y, float z) {
	return ((yPixToDouble(y)) * z * FrustZoom *FrustRatio);
}

QMatrix4x4 Render::rotToMatrix(){
	QMatrix4x4 mat = QMatrix4x4();
	mat.setToIdentity();
	mat.rotate(Rot);
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
	TreeView = treeView;
}

float Render::getFrustWidth() {
	return FrustWidth * FrustZoom;
}
float Render::getFrustHeight() {
	return FrustHeight * FrustRatio * FrustZoom;
}
void Render::paintGL() {
	glClear(GL_DEPTH_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-getFrustWidth(), getFrustWidth(), -getFrustHeight(), getFrustHeight(), FrustNear, FrustFar);
	glTranslatef(-CamPoint.x(),-CamPoint.y(),-CamPoint.z());
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
	glVertex3f(CamOldPoint.x(),CamOldPoint.y(),CamOldPoint.z());
	glVertex3f(CamOldPoint.x() + SelectionDir.x(), CamOldPoint.y() + SelectionDir.y(), CamOldPoint.z() + SelectionDir.z());
	glEnd();

	glColor4f(1, 0, 0, 1);
	glLineWidth(10);
	glBegin(GL_LINES);
	glVertex3f(CamOldPoint.x() + SelectionDir.x(), CamOldPoint.y() + SelectionDir.y(), CamOldPoint.z() + SelectionDir.z());
	glVertex3f(CamOldPoint.x() + SelectionDir.x()*2, CamOldPoint.y() + SelectionDir.y()*2, CamOldPoint.z() + SelectionDir.z()*2);
	glEnd();
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glColor3f(1,1,1);
	Mesh.draw(0,0,0, false, true);
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
