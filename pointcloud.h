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
	void addPoints(QList<QVector3D>);

	QList<QVector3D*> mPoints;
	QList<QVector3D*> mSelected;
	double mMaxdist;
};

#endif // POINTCLOUD_H
