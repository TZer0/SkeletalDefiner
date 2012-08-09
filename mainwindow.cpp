#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "render.h"

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	Render *rend = new Render(this);
	//rend->resize(size());
	rend->sizePolicy().setHorizontalPolicy(QSizePolicy::Expanding);
	rend->sizePolicy().setVerticalPolicy(QSizePolicy::Expanding);
	ui->gridLayout->addWidget(rend, 0, 0, 1, 1);
	QTimer *timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), rend, SLOT(redraw()));
	timer->start(0);
}

MainWindow::~MainWindow()
{
	delete ui;
}
