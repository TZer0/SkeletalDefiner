#include "render.h"

Render::Render(QWidget *parent) :
	QGLWidget(parent)
{
}

void Render::redraw() {
	paintGL();
}

void Render::initializeGL() {
	glClearColor( 0.1, 0.1, 0.1, 0.0 );
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_DOUBLE);
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);
	glPushClientAttrib( GL_CLIENT_VERTEX_ARRAY_BIT );
	Rot = QQuaternion::fromAxisAndAngle(0,0,0,90);
	Rotating = true;
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

void Render::mousePressEvent(QMouseEvent *event) {
	Old = Rot;
	Rotating = true;
	StartPoint = getVector(event->x(), event->y());
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

void Render::mouseRelaseEvent(QMouseEvent *event) {
	Rotating = false;
}

void Render::paintGL() {
	glClear(GL_DEPTH_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-1, 1, -1, 1, 0.3, 10.1);
	glTranslatef(0,0,-5);
	QMatrix4x4 *mat = new QMatrix4x4();
	mat->setToIdentity();
	mat->rotate(Rot);
	float conv[16];
	qreal *from = mat->data();
	for (int i = 0; i < 16; i++) {
		conv[i] = from[i];
	}
	glMultMatrixf(conv);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	double xt = -3;
	double xf = 3;
	double yf = -3;
	double yt = 3;
	double z = 0;
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
