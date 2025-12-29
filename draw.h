#include "globals.h"

void drawPixel(const Vector2d& coords, const int size = 9);

void drawLine(const Vector2d& start, const Vector2d& end);

Vector2d screen(const Vector2d& coords);

Vector2d project(const Vector3d& coords);

Vector3d rotate_xz(Vector3d coords, float angle);

// Vector3d computeCenter(const std::vector<Vector3d>& verts);

enum Axis { X_AXIS, Y_AXIS, Z_AXIS };

std::vector<Vector3d> rotateMeshInPlace(const std::vector<Vector3d> verts, double angle, Axis axis);

uint32_t LerpColor(uint32_t c1, uint32_t c2, float t);

uint32_t FadeToBlack(uint32_t color);