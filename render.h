#ifndef RENDER_H
#define RENDER_H

#include <QtGui>
#include <QGLWidget>

class Render : public QGLWidget
{
	Q_OBJECT
public:
	explicit Render(QWidget *parent = 0);
	void paintGL();
	void initializeGL();
	
signals:
	
public slots:
	void redraw();
	
};

#endif // RENDER_H
