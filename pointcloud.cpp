#include "pointcloud.h"

PointCloud::PointCloud()
{
}

void PointCloud::setMaxdist(double Maxdist) {
	this->Maxdist = Maxdist;
}

void PointCloud::selectNearestPoint(QVector3D direction, QVector3D start) {
	QVector3D origStart = start;
	bool found = false;
	for (int i = 0; i < Points.length(); i++) {
		//qDebug() << *Points[i];
	}
	qDebug() << "scan";
	while ((origStart - start).length() < Maxdist) {
		start += direction.normalized()*0.01;
		qDebug() << start;
		for (int i = 0; i < Points.length(); i++) {
			if ((*Points[i]-start).length() < 0.3) {
				found = true;
				if (!Selected.contains(Points[i])) {
					Selected.append(Points[i]);
				}
				qDebug() << "found.";
				return;
			}
		}
	}
}
void PointCloud::addPoints(QList<QVector3D> input) {
	for (int i = 0; i < input.length(); i++) {
		qDebug() << input[i];
		Points.append(&input[i]);
	}
}
