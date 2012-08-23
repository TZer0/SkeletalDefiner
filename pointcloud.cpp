#include "pointcloud.h"

PointCloud::PointCloud()
{
}

void PointCloud::setMaxdist(double Maxdist) {
	this->Maxdist = Maxdist;
}

void PointCloud::selectNearestPoint(QVector3D direction, QVector3D start) {
	QVector3D origStart = start;
	while ((origStart - start).length() < Maxdist) {
		start += direction.normalized()*0.01;
		bool found = false;
		for (int i = 0; i < Points.size(); i++) {
			if ((*Points[i]-start).length() < 0.3) {
				if (!Selected.contains(Points[i])) {
					Selected.append(Points[i]);
				} else {
					Selected.removeAll(Points[i]);
				}
				found = true;
			}
		}
		if (found) {
			return;
		}
	}
}
void PointCloud::addPoints(QList<QVector3D> input) {
	for (int i = 0; i < input.length(); i++) {
		Points.append(new QVector3D(input[i]));
	}
}
