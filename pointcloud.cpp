#include "pointcloud.h"

PointCloud::PointCloud()
{
	Points.append(new QVector3D(0,0,0));
	Points.append(new QVector3D(3,3,0));
	Points.append(new QVector3D(-3,-3,0));
	Points.append(new QVector3D(-3,3,0));
	Points.append(new QVector3D(3,-3,0));
}

void PointCloud::setMaxdist(double Maxdist) {
	this->Maxdist = Maxdist;
}

void PointCloud::selectNearestPoint(QVector3D direction, QVector3D start) {
	QVector3D origStart = start;
	bool found = false;
	qDebug() << Maxdist;
	while ((origStart - start).length() < Maxdist) {
		start += direction*0.05;
		for (int i = 0; i < Points.length(); i++) {
			if ((*Points[i]-start).length() < 0.2) {
				found = true;
				if (!Selected.contains(Points[i])) {
					Selected.append(Points[i]);
				}
				qDebug() << "found.";
				break;
			}
		}
		if (found) {
			break;
		}
	}
}
