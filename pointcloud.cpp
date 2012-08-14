#include "pointcloud.h"

PointCloud::PointCloud()
{
	points.append(new QVector3D(0,0,0));
	far = NULL;
}

void PointCloud::setBound(float *far) {
	this->far = far;
}

void PointCloud::selectNearestPoint(QVector3D direction, QVector3D start) {
	QVector3D origStart = start;
	while ((origStart - start).length() < *far) {
		start += direction;
		qDebug() << start;
	}
}
