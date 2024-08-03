# Open Graphics Library

## Installation and setup

Creating a build.sh file

```bash
#!/bin/bash

CC=g++
CFLAGS=`pkg-config --cflags glew`
LDFLAGS=`pkg-config --libs glew`
LDFLAGS="$LDFLAGS -lglut"

$CC main.cpp $CFLAGS $LDFLAGS -o progr
```

* pkg-config is a quality of life improver: it finds all outputs all of the relevant flags for the given package we ask for
* These flags are then used to compile the main.cpp code.
* There are several packages that are required to run opengl effectively. These have not been specified here, however are made apparent by the compilation of the code etc.

## Vertex Shading

```glsl
#version 330 core // specifies that we are using opengl version 3.3

layout(location=0) in vec3 vertexPosition_modelspace;
```

* vec3 similar to glm::vec3. In c++ we use three components and in glsl we use 3 components as well.
* layout(location=0): refers to the bufgfer we use to feed vertexPosition_modelspace attribute.
    * Each vertex can have several attributes
    * Position attr, Texture attr, Color attr, etc.
    * opengl does not know what a color is, it just sees a vec3. 
    * So we need to tell opengl what buffer each attribute correspond to. 
     we do that by setting the layout to the same value as the first parameter of glVertexAttribPointer.
    * Value 0 is not important, could be 12, but no mor thatn glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &v);
* vertexPoisition_modelspace could have any other name. it will containt the position of the vertex for each run of the vertex shader.
* in means that this is some input data. 

### The main function

The function that is called for ***each vertex*** is called main, just as in C. 
```
void main(){}
    gl_Position.xyz = vertexPosition_modelspace;
    gl_Position.w = 1.0;
```

Our main function will merely set the vertex position to whatever was in the buffer. 
* so if we gave (1,1) the triangle would have one of its vertices at the top right corner of the screen.
* gl_Position is of of the few bult-in variables wehave to assign to some value. 


### Fragment shader
For our first fragment shader, we will do something really siumple
* set the color of each fragment to red.
* Remember there are 4 fragments in a pixel, because we use 4x anti-aliasing.

```glsl
#version 330 core

out vec3 color;
void main(){
    color = vec3(1,0,0);
}
```

* vec3(1,0,0) means read. Because on the compt screen, color represented by red green and blue triplets. in this order (1,0,0) therefore means red. 


## Matrices

Instead of xyz triplets, we are now going to handle x,y,z,w vectors. 
* if w == 1, then the vector (x,y,z,1) is a position in space
* if w == 0, then the vector (x,y,z,0) is a direction

Homogenous coordinates allow us to use a single mathematical formula to deal with these two cases, if it is a position or a direction. 

### Transformation matrices

#### Intro to matrices

in 3d graphics we mostly use 4x4 matrices. 
* allow us to transform our xyzw vertices. 
* done by multiplying the vertex with the matrix.

Matrix x Vertex (in this order) = TransformedVertex
| a b c d |     |x|     | ax + by + cz + dw | 
| e f g h |  x  |y| =   | ex + fy + gz + hw |
| i j k l |     |z|     | xi + jy+ kz  + lw |
| m n o p |     |w|     | xm + ny + oz + pw |

