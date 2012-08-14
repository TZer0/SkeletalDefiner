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
	void setMaxdist(double);
	QList<QVector3D*> Points;
	QList<QVector3D*> Selected;
	double Maxdist;
};

#endif // POINTCLOUD_H
