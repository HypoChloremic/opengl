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


## OpenGL pipeline
1. Vertex input
2. Tesselation
3. Geometry shader (not used)
4. Primitive assembly
5. Rasterization
6. Fragment shading

The two more important parts for us are vertex input and fragment shading. 


## Vertex Shading

### Vertex data

Vertex data is data that represent the vertices of the object we wish to pass to opengl.

The vertex data is then processed in the vertex shader, where we perform mathematical calculations to transfer the vertices from the world space to opengl space. 

* Mathematically we achieve this by: $ScreenSpace = Projection \x View \x Transformation \x vertex$


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




## Buffers

This is a major subject in opengl

### Introduction

Buffers are data stored in memory. block of memory on the gpu to store vertex data, texture data etc.

#### Different buffers

##### GL_ARRAY_BUFFER

* Most often used to store vertex data

##### GL_ELEMENT_ARRAY_BUFFER

* array of elements, just sized integers. 

##### GL_TEXTURE_BUFFER

* Array of pixels. Usually just an image just like a png or jpg. 

##### Other common buffers 
* GL_DRAW_INDIRECT_BUFFER: buffer to store rendering commands
* GL_SHADER_STORAGE_BUFFER: special buffer to pass data to shaders.
* GL_TRANSFORM_FEEDBACK_BUFFER: complex calculations on the gpu like particle simulations
* GL_UNIFORM_BUFFER: another buffer to pass data to shaders


### Sending vertex data to the gpu

Given that buffers are locations in memory in the gpu, that we can store data in, how can we use it. The following will walkthrough the way we handle vertex data.

#### Create an array buffer

To create an array vuffer, there are three basic commands we will use
```cpp
glCreateBuffers(1, &myBufferId);
glBindBuffer(GL_ARRAY_BUFFER, myBufferId);
glBufferData(GL_ARRAY_BUFFER, sizeOfMyData, myData, usage);
```

Note that opengl is a state machine. OpenGl mutates a global state, and we need to tell opengl what we are mutating, by binding for instance buffers to certain objects etc. 

* We need to provide a variable that will hold the bufferid when we create the buffer.
* The bufferid gives us a way to access the buffer as well. 
* ***usage***: what is usage; we help opengl by telling it what we intend to do with the buffer. 
    * GL_STATIC_DRAW: sending data to the gpu only for drawing and not intending to update the data.
    * GL_DYNAMIC_DRAW: sending data to the gpu that will mutate in the future. 

## Matrices

Instead of xyz triplets, we are now going to handle x,y,z,w vectors.
* if w == 1, then the vector (x,y,z,1) is a position in space
* if w == 0, then the vector (x,y,z,0) is a direction
These two rules are essential

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

#### Translation matrices

Are the most simple


| 1 0 0 X |
| 0 1 0 Y |
| 0 0 1 Z |
| 0 0 0 1 |

XYZ are the values we wish to add to the xyz coords in the xyzw vector
