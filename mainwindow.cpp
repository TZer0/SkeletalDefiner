#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	RenderWidget = new Render(this);
	ui->gridLayout->addWidget(RenderWidget, 0, 0, 1, 1);
	QTimer *timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), RenderWidget, SLOT(redraw()));
	timer->start(0);
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
	if (event->key() ==  Qt::Key_Shift) {
		RenderWidget->setShift(true);
	}
}

void MainWindow::keyReleaseEvent(QKeyEvent *event) {
	if (event->key() ==  Qt::Key_Shift) {
		RenderWidget->setShift(false);
	}
}
