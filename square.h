#include "globals.h"

// Square
const std::vector<Vector3d> vertices = {
    // Front face
    {0.5, 0.5, 2}, // 0
    {0.5, -0.5, 2}, // 1
    {-0.5, -0.5, 2}, // 2
    {-0.5, 0.5, 2}, // 3
    // Back face
    {0.5, 0.5, 3}, // 4 
    {0.5, -0.5, 3}, // 5
    {-0.5, -0.5, 3}, // 6
    {-0.5, 0.5, 3}, // 7
};

const std::vector<std::vector<int>> edges = {
      {0, 1}, {1, 2}, {2, 3}, {3, 0}, // Front face edges
      {4, 5}, {5, 6}, {6, 7}, {7, 4}, // Back face edges
      {0, 4}, {1, 5}, {2, 6}, {3, 7}  // Side edges
};
