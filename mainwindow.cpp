#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	mUI(new Ui::MainWindow)
{
	mUI->setupUi(this);
	mRenderWidget = new Render(this);
	mRenderWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	mUI->gridLayout->addWidget(mRenderWidget, 0, 0, 1, 1);
	QTreeView *tmp = new QTreeView(this);
	tmp->setVisible(true);
	tmp->setMaximumWidth(250);
	mUI->gridLayout->addWidget(tmp, 0,1,1,1);
	mRenderWidget->setTreeView(tmp);
	QTimer *timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), mRenderWidget, SLOT(redraw()));
	timer->start(0);
}

MainWindow::~MainWindow()
{
	delete mUI;
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
	if (event->key() ==  Qt::Key_Shift) {
		mRenderWidget->setShift(true);
	}
}

void MainWindow::keyReleaseEvent(QKeyEvent *event) {
	if (event->key() ==  Qt::Key_Shift) {
		mRenderWidget->setShift(false);
	}
}
