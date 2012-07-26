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
}
void Render::paintGL() {
	glClear(GL_DEPTH_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-1, 1, -1, 1, 0.3, 5.1);
	glTranslatef(0,0,-1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	double xt = 3;
	double xf = 1;
	double yf = 3;
	double yt = 1;
	double z = 4;
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
