#include "Transform.h"

tfm::point::point() : x(0.), y(0.) {}
tfm::point::point(double x, double y) : x(x), y(y) {}

tfm::transform::transform(point p, double r, double s) : position(p), rotation(r), scale(s) {};
tfm::transform::transform(point p, double r) : position(p), rotation(r), scale(1.) {};
tfm::transform::transform(point p) : position(p), rotation(0.), scale(1.) {};
tfm::transform::transform() : position(tfm::point::point()), rotation(0.), scale(1.) {};