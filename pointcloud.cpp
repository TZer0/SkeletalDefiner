#include "pointcloud.h"

PointCloud::PointCloud()
{
}

void PointCloud::setMaxdist(double Maxdist) {
	this->mMaxdist = Maxdist;
}

void PointCloud::selectNearestPoint(QVector3D direction, QVector3D start) {
	QVector3D origStart = start;
	while ((origStart - start).length() < mMaxdist) {
		start += direction.normalized()*0.01;
		bool found = false;
		for (int i = 0; i < mPoints.size(); i++) {
			if ((*mPoints[i]-start).length() < 0.3) {
				if (!mSelected.contains(mPoints[i])) {
					mSelected.append(mPoints[i]);
				} else {
					mSelected.removeAll(mPoints[i]);
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
		mPoints.append(new QVector3D(input[i]));
	}
}
