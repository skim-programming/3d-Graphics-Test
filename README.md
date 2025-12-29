### 3d Graphics Test

This is a 3d graphics renderer I've created with some simple algorithms and a vertice-edge rendering system. It can capably render complex meshes given the correct vertices and edges, making sure to keep safety measures in case of error. Examples:

https://github.com/user-attachments/assets/1ed79efe-e603-4b57-86b5-88b3d4d121a6

https://github.com/user-attachments/assets/d4aa6b2a-bd7a-4042-b233-66ebb9552eb0

Include either the square.h file for the square mesh, or the pengu.h file for the penguin mesh. Don't include both, or you'll get a multiple/redefinition error.
You can make your own meshes by creating a header file, importing globals.h, then following this format.

```cpp
const std::vector<Vector3d> vertices = {
  // Vertices {x, y, z}
};

const std::vector<std::vector<int>> edges = {
  // Edges {index, index} (multiple indexes are allowed, e.g. {0, 1, 2})
}
```


## Update
### Added Controls, Borders, and more
W, A, S, D, and spacebar work as valid inputs, moving the camera(x, y, z axis)
All four arrow keys are used to rotate the model rather than it being rotated automatically
E, V used to toggle edges and vertices respectively
R to reset the mesh & position
Borders added to prevent going through meshes(bugs out the math, messes with controls)

Bonus penguin:

https://github.com/user-attachments/assets/1a53c7ec-5cce-478f-b112-e3cff1909d9a
