# Homogeneous coordinates
A vertex has 4 components x,y,z and w. 
If w == 1 then the vertex is a position in space
If w == 0 then the vertex is a direction.

This allows us to use the same mathematical Operations for rotations and directions

# Math basics
If you want to multiply a matrix and a vertex you MUST multiply the matrix with the vertex NOT the other way round

```c++
glm::mat4 myMatrix;
glm::vec4 myVector;
// fill myMatrix and myVector somehow
glm::vec4 transformedVector = myMatrix * myVector; // In this order! this is important
```

```glsl
mat4 myMatrix;
vec4 myVector;
// fill myMatrix and myVector somehow
vec4 transformedVector = myMatrix * myVector; // The same as in c++
```
## Different kind of matrices
The following chapters will explain some kinds of matrices

### Translation Matrix
A translation matrix looks like this
```
1 0 0 X
0 1 0 Y
0 0 1 Z
0 0 0 1
```
This will add X, Y and Z to the vector. So all this does is a vector addition.

In most of the time the matrix will be created in c++/rust and only the matrix and vector multiplication in GLSL.

```c++
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
glm::mat4 matrix = glm::translate(glm::mat4(), glm::vec3(10.0f, 0.0f, 0.0f));
glm::vec4 vector(10.0f, 10.0f, 10.0f, 0.0f);
glm::vec4 transformedVector = matrix * vector; // this will be vector because vector is a direction
```
### Identity-Matrix 
When you multiply a vector (or matrix) with the identity matrix you get the original value. It is the neutral element for vector multiplication.
```
1 0 0 0
0 1 0 0
0 0 1 0
0 0 0 1
```
With glm you can create it with the following code
```c++
glm::mat4 identiyMatrix = glm::mat4(1.0f);
```

### Scaling-Matrix
With this matrix you can scale a vector.
```
A 0 0 0
0 B 0 0
0 0 C 0
0 0 0 1
```
The result of a vector (x, y, z, w) and the scaling matrix is (x*A, y*B, z*C, w)
In c++ you can create it with the following code
```
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
glm::mat4 scalingMatrix = glm::scale(2.0f, 3.0f, 4.0f); // A = 2.0, B = 3.0, C = 4.0
```

### Rotation-Matrix
> This will be filled later because it is complicated and explained later.
```c++
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
glm::vec3 rotationAxis(??, ??, ??);
glm::rotate( angle_in_degrees, rotationAxis);
```

### Model-Matrix
The model matrix puts rotation, scaling and transforming together
> It's really important to do it in the following order! If you do it in a different order that the result is wrong!
```
ModelMatrix = TranslationMatrix * RotationMatrix * ScaleMatrix 
```
The matrix is calculated in c++/rust.
If you multiply a vector (a position) with this matrix the result is in *World Space*. Before this operation it is in *Model Space*. It's called Model

### View-Matrix
This matrix is used to position the camera. It is done in the way that the complete world is moved not the camera. That means if the camera should be translated 3 units to the right (+3 on the X coordinate) you need to move everything in the world 3 units to the LEFT! (-3 on the X coordinate).

```c++
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

glm::mat4 viewMatrix = glm::translate(glm::mat4(), glm::vec3(-3.0f, 0.0f ,0.0f));
```
After this operation the vertices will be in *Camera-Space*.
GLM provides a function to create the Camera-Matrix (is this the same as the View-Matrix? I guess).

```c++
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

glm::mat4 cameraMatrix = glm::lookAt(
    cameraPosition, // The position of your camera, in world space
    cameraTarget,   // where you want to look at, in world space
    upVector        // probably glm::vec3(0, 1, 0), but (0, -1, 0) would make you looking upside-down
)
```
An object with x == 0 and y == 0 should be at the center of the screen

### Projection-Matrix
Create a projection matrix with the following code.
```c++
glm::mat4 projectionMatrix = glm::perspective(
    glm::radians(FoV), // The vertical Field of View, in radians: the amount of "zoom" Think "camera lens". Usually between 90° (extra wide) and 30° (quite zoomed in)
    4.0f / 3.0f, // Aspect Ratio. Depends on the size of your window. Notice that 4/3 = 800/600 = 1280/960, sound familar?
    0.1f,       // Near clipping plane. Keep as big as possible, or you'll get precision issues.
    100.0       // Far clipping plane. Keep as little as possible.
);
```

Now all vertices are in *Homogeneous Space*

### Model-View-Projection-Matrix (MVP-Matrix)
The Model-View-Projection-Matrix combines all three matrices. They are multiplied in reversed order of the name
```c++
glm::mat4 MVPmatrix = projection * view * model;
```
```GLSL
transformed_vertex = MVP * in_vertex;
```

# Glossary
| Term | Definition |
| :-----------: | :----------- |
| Model Space | If a vertex is in model space it means that the vertex is defined relative to the middle of the corresponding model |
| World Space | If a vertex in *Model Space* is multiplied with the Model-Matrix it will be in *World Space*. This means that the vertex is now relative to the world center (the center of the model is also translated, so yes they are still somehow relative to the model center.) |
| Camera Space | A vertix is now relative to the camera (that could be called the new center of everything) |
| Homogeneous Space | The Homogeneous Space is a small qube. Everything that is in the qube will be rendered | 
