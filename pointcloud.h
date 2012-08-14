#ifndef POINTCLOUD_H
#define POINTCLOUD_H
#include <QList>
#include <QVector3D>
#include <QDebug>

class PointCloud
{
public:
	PointCloud();
	void selectNearestPoint(QVector3D direction, QVector3D start);
	void setBound(float *);
	QList<QVector3D*> points;
	QList<QVector3D*> selected;
	float *far;
};

#endif // POINTCLOUD_H
