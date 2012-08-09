#include "render.h"

Render::Render(QWidget *parent) :
	QGLWidget(parent)
{
	Rot = QQuaternion::fromAxisAndAngle(0,0,0,90);
	Rotating = true;
	left = bottom = -1;
	right = top = 1;
	near = 0.3;
	far = 10.1;
	calcRatio();
}

void Render::calcRatio() {
	ratio = height()/((float)width());
}

void Render::redraw() {
	paintGL();
}

void Render::loadMesh(std::string path) {
	Assimp::Importer importer;
	importer.ReadFile(path, 0);
}

void Render::initializeGL() {
	glClearColor( 0.1, 0.1, 0.1, 0.0 );
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_DOUBLE);
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	glPushClientAttrib( GL_CLIENT_VERTEX_ARRAY_BIT );
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

void Render::resizeEvent(QResizeEvent *event) {
	glViewport( 0, 0, (GLint)event->size().width(), (GLint)event->size().height());
	calcRatio();
}

void Render::mousePressEvent(QMouseEvent *event) {
	if (event->button() == Qt::RightButton) {
		Old = Rot;
		Rotating = true;
		StartPoint = getVector(event->x(), event->y());
	} else if (event->button() == Qt::LeftButton) {
		QVector3D nearPoint = QVector3D(xToViewX(event->x(), near), yToViewY(event->y(), near), near);
		QVector3D farPoint = QVector3D(xToViewX(event->x(), far), yToViewY(event->y(), far), far);
		QVector3D direction = (farPoint - nearPoint).normalized();
		QMatrix4x4 mat = rotToMatrix();
		qDebug() << mat;
		qDebug() << mat*direction;
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
	}
}

void Render::mouseReleaseEvent(QMouseEvent *event) {
	if (event->button() == Qt::RightButton) {
		Rotating = false;
	}
}

float Render::xPixToDouble(int x) {
	return ((x*2)/((float)width())-1);
}
float Render::yPixToDouble(int y) {
	return (1-(y*2)/((float)height()));
}

float Render::xToViewX(int x, float z) {
	return ((xPixToDouble(x) * z));
}

float Render::yToViewY(int y, float z) {
	return ((yPixToDouble(y)) * z * ratio);
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


void Render::paintGL() {
	glClear(GL_DEPTH_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(left, right, bottom*ratio, top*ratio, near, far);
	glTranslatef(0,0,-5);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	float conv[16];
	rotToFloatArray(conv);
	glMultMatrixf(conv);

	float xt = -3;
	float xf = 3;
	float yf = -3;
	float yt = 3;
	float z = 0;
	glColor4f(1,1,1,1);
	glBegin(GL_QUADS);
	glVertex3f(xf,yt,z);
	glVertex3f(xf,yf,z);
	glVertex3f(xt,yf,z);
	glVertex3f(xt,yt,z);
	glEnd();


	glFlush();
	glFinish();
	swapBuffers();
}
