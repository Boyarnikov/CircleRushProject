#pragma once

// transform and point structure. used to store positions and other physical aspects of gameobjects
namespace tfm {
	struct point {
		double x;
		double y;

		point();
		point(double, double);
	};

	struct transform {
		point position;
		double rotation;
		double scale;

		transform(point, double, double);
		transform(point, double);
		transform(point);
		transform();
	};
}
