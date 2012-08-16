#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>
#include "render.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT
	
public:
	explicit MainWindow(QWidget *parent = 0);
	void keyPressEvent(QKeyEvent *);
	void keyReleaseEvent(QKeyEvent *);
	~MainWindow();
	
private:
	Ui::MainWindow *ui;
	Render *RenderWidget;
};

#endif // MAINWINDOW_H
