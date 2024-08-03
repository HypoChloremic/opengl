#include <GL/freeglut.h>
#include <stdio.h>
#include <iostream>

static void RenderScreenCB(){
	glClear(GL_COLOR_BUFFER_BIT);
	glutSwapBuffers();
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	std::cout << "hello world" << std::endl;
	return 0;
}
