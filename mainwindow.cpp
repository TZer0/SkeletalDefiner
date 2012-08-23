#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	RenderWidget = new Render(this);
	RenderWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	ui->gridLayout->addWidget(RenderWidget, 0, 0, 1, 1);
	QTreeView *tmp = new QTreeView(this);
	tmp->setVisible(true);
	tmp->setMaximumWidth(250);
	ui->gridLayout->addWidget(tmp, 0,1,1,1);
	RenderWidget->setTreeView(tmp);
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
