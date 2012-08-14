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
	bool found = false;
	while ((origStart - start).length() < *far) {
		start += direction*0.05;
		for (int i = 0; i < points.length(); i++) {
			if ((*points[i]-start).length() < 0.1) {
				found = true;
				if (!selected.contains(points[i])) {
					selected.append(points[i]);
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
