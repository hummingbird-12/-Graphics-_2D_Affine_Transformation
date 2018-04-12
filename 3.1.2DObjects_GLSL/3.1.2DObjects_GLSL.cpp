#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "Shaders/LoadShaders.h"
GLuint h_ShaderProgram; // handle to shader program
GLint loc_ModelViewProjectionMatrix, loc_primitive_color; // indices of uniform variables

// include glm/*.hpp only if necessary
//#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp> //translate, rotate, scale, ortho, etc.
glm::mat4 ModelViewProjectionMatrix;
glm::mat4 ViewMatrix, ProjectionMatrix, ViewProjectionMatrix;

#define TO_RADIAN 0.01745329252f  
#define TO_DEGREE 57.295779513f
#define BUFFER_OFFSET(offset) ((GLvoid *) (offset))

#define LOC_VERTEX 0

#define INIT_WIN_WIDTH 1000
#define INIT_WIN_HEIGHT 600

int win_width = INIT_WIN_WIDTH, win_height = INIT_WIN_HEIGHT; 
float centerx = 0.0f, centery = 0.0f, rotate_angle = 0.0f;
float winBorderR, winBorderD, winBorderL, winBorderU;
float groundLevel;

// MODEL : AIRPLANE

#define AIRPLANE_BIG_WING 0
#define AIRPLANE_SMALL_WING 1
#define AIRPLANE_BODY 2
#define AIRPLANE_BACK 3
#define AIRPLANE_SIDEWINDER1 4
#define AIRPLANE_SIDEWINDER2 5
#define AIRPLANE_CENTER 6

float airplane_Xcor = win_width / 2.0f;
int airplane_clock;
int airplane_appearDelay;
float airplane_s_factor = 1.0f;

GLfloat big_wing[6][2] = { { 0.0, 0.0 }, { -20.0, 15.0 }, { -20.0, 20.0 }, { 0.0, 23.0 }, { 20.0, 20.0 }, { 20.0, 15.0 } };
GLfloat small_wing[6][2] = { { 0.0, -18.0 }, { -11.0, -12.0 }, { -12.0, -7.0 }, { 0.0, -10.0 }, { 12.0, -7.0 }, { 11.0, -12.0 } };
GLfloat body[5][2] = { { 0.0, -25.0 }, { -6.0, 0.0 }, { -6.0, 22.0 }, { 6.0, 22.0 }, { 6.0, 0.0 } };
GLfloat back[5][2] = { { 0.0, 25.0 }, { -7.0, 24.0 }, { -7.0, 21.0 }, { 7.0, 21.0 }, { 7.0, 24.0 } };
GLfloat sidewinder1[5][2] = { { -20.0, 10.0 }, { -18.0, 3.0 }, { -16.0, 10.0 }, { -18.0, 20.0 }, { -20.0, 20.0 } };
GLfloat sidewinder2[5][2] = { { 20.0, 10.0 }, { 18.0, 3.0 }, { 16.0, 10.0 }, { 18.0, 20.0 }, { 20.0, 20.0 } };
GLfloat center[1][2] = { { 0.0, 0.0 } };
GLfloat airplane_color[7][3] = {
	{ 150 / 255.0f, 129 / 255.0f, 183 / 255.0f },  // big_wing
	{ 245 / 255.0f, 211 / 255.0f,   0 / 255.0f },  // small_wing
	{ 111 / 255.0f,  85 / 255.0f, 157 / 255.0f },  // body
	{ 150 / 255.0f, 129 / 255.0f, 183 / 255.0f },  // back
	{ 245 / 255.0f, 211 / 255.0f,   0 / 255.0f },  // sidewinder1
	{ 245 / 255.0f, 211 / 255.0f,   0 / 255.0f },  // sidewinder2
	{ 255 / 255.0f,   0 / 255.0f,   0 / 255.0f }   // center
};

GLuint VBO_airplane, VAO_airplane;

void prepare_airplane() {
	GLsizeiptr buffer_size = sizeof(big_wing)+sizeof(small_wing)+sizeof(body)+sizeof(back)
		+sizeof(sidewinder1)+sizeof(sidewinder2)+sizeof(center);

	// Initialize vertex buffer object.
	glGenBuffers(1, &VBO_airplane);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_airplane);
	glBufferData(GL_ARRAY_BUFFER, buffer_size, NULL, GL_STATIC_DRAW); // allocate buffer object memory

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(big_wing), big_wing);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(big_wing), sizeof(small_wing), small_wing);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(big_wing)+sizeof(small_wing), sizeof(body), body);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(big_wing)+sizeof(small_wing)+sizeof(body), sizeof(back), back);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(big_wing)+sizeof(small_wing)+sizeof(body)+sizeof(back),
		sizeof(sidewinder1), sidewinder1);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(big_wing)+sizeof(small_wing)+sizeof(body)+sizeof(back)
		+sizeof(sidewinder1), sizeof(sidewinder2), sidewinder2);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(big_wing)+sizeof(small_wing)+sizeof(body)+sizeof(back)
		+sizeof(sidewinder1)+sizeof(sidewinder2), sizeof(center), center);

	// Initialize vertex array object.
	glGenVertexArrays(1, &VAO_airplane);
	glBindVertexArray(VAO_airplane);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_airplane);
	glVertexAttribPointer(LOC_VERTEX, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void draw_airplane() { // Draw airplane in its MC.
	glBindVertexArray(VAO_airplane);

	glUniform3fv(loc_primitive_color, 1, airplane_color[AIRPLANE_BIG_WING]);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 6);

	glUniform3fv(loc_primitive_color, 1, airplane_color[AIRPLANE_SMALL_WING]);
	glDrawArrays(GL_TRIANGLE_FAN, 6, 6);

	glUniform3fv(loc_primitive_color, 1, airplane_color[AIRPLANE_BODY]);
	glDrawArrays(GL_TRIANGLE_FAN, 12, 5);

	glUniform3fv(loc_primitive_color, 1, airplane_color[AIRPLANE_BACK]);
	glDrawArrays(GL_TRIANGLE_FAN, 17, 5);

	glUniform3fv(loc_primitive_color, 1, airplane_color[AIRPLANE_SIDEWINDER1]);
	glDrawArrays(GL_TRIANGLE_FAN, 22, 5);

	glUniform3fv(loc_primitive_color, 1, airplane_color[AIRPLANE_SIDEWINDER2]);
	glDrawArrays(GL_TRIANGLE_FAN, 27, 5);

	glUniform3fv(loc_primitive_color, 1, airplane_color[AIRPLANE_CENTER]);
	glPointSize(5.0);
	glDrawArrays(GL_POINTS, 32, 1);
	glPointSize(1.0);
	glBindVertexArray(0);
}

// MODEL : HOUSE

#define HOUSE_ROOF 0
#define HOUSE_BODY 1
#define HOUSE_CHIMNEY 2
#define HOUSE_DOOR 3
#define HOUSE_WINDOW 4

#define MAX_HOUSE_CNT 10

int house_appearDelay;
int house_clock;
int house_cnt;
float house_offset[MAX_HOUSE_CNT] = { 2.0f };

GLfloat roof[3][2] = { { -12.0, 0.0 }, { 0.0, 12.0 }, { 12.0, 0.0 } };
GLfloat house_body[4][2] = { { -12.0, -14.0 }, { -12.0, 0.0 }, { 12.0, 0.0 }, { 12.0, -14.0 } };
GLfloat chimney[4][2] = { { 6.0, 6.0 }, { 6.0, 14.0 }, { 10.0, 14.0 }, { 10.0, 2.0 } };
GLfloat door[4][2] = { { -8.0, -14.0 }, { -8.0, -8.0 }, { -4.0, -8.0 }, { -4.0, -14.0 } };
GLfloat window[4][2] = { { 4.0, -6.0 }, { 4.0, -2.0 }, { 8.0, -2.0 }, { 8.0, -6.0 } };

GLfloat house_color[5][3] = {
	{ 200 / 255.0f, 39 / 255.0f, 42 / 255.0f },
	{ 235 / 255.0f, 225 / 255.0f, 196 / 255.0f },
	{ 255 / 255.0f, 0 / 255.0f, 0 / 255.0f },
	{ 233 / 255.0f, 113 / 255.0f, 23 / 255.0f },
	{ 44 / 255.0f, 180 / 255.0f, 49 / 255.0f }
};

GLuint VBO_house, VAO_house;
void prepare_house() {
	GLsizeiptr buffer_size = sizeof(roof)+sizeof(house_body)+sizeof(chimney)+sizeof(door)
		+sizeof(window);

	// Initialize vertex buffer object.
	glGenBuffers(1, &VBO_house);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_house);
	glBufferData(GL_ARRAY_BUFFER, buffer_size, NULL, GL_STATIC_DRAW); // allocate buffer object memory

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(roof), roof);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(roof), sizeof(house_body), house_body);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(roof)+sizeof(house_body), sizeof(chimney), chimney);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(roof)+sizeof(house_body)+sizeof(chimney), sizeof(door), door);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(roof)+sizeof(house_body)+sizeof(chimney)+sizeof(door),
		sizeof(window), window);

	// Initialize vertex array object.
	glGenVertexArrays(1, &VAO_house);
	glBindVertexArray(VAO_house);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_house);
	glVertexAttribPointer(LOC_VERTEX, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void draw_house() {
	glBindVertexArray(VAO_house);

	glUniform3fv(loc_primitive_color, 1, house_color[HOUSE_ROOF]);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 3);

	glUniform3fv(loc_primitive_color, 1, house_color[HOUSE_BODY]);
	glDrawArrays(GL_TRIANGLE_FAN, 3, 4);

	glUniform3fv(loc_primitive_color, 1, house_color[HOUSE_CHIMNEY]);
	glDrawArrays(GL_TRIANGLE_FAN, 7, 4);

	glUniform3fv(loc_primitive_color, 1, house_color[HOUSE_DOOR]);
	glDrawArrays(GL_TRIANGLE_FAN, 11, 4);

	glUniform3fv(loc_primitive_color, 1, house_color[HOUSE_WINDOW]);
	glDrawArrays(GL_TRIANGLE_FAN, 15, 4);

	glBindVertexArray(0);
}

// MODEL : COCKTAIL

#define COCKTAIL_NECK 0
#define COCKTAIL_LIQUID 1
#define COCKTAIL_REMAIN 2
#define COCKTAIL_STRAW 3
#define COCKTAIL_DECO 4

int cocktail_clock;
int cocktail_appearDelay;
int cocktail_Xcor;

GLfloat neck[6][2] = { { -6.0, -12.0 }, { -6.0, -11.0 }, { -1.0, 0.0 }, { 1.0, 0.0 }, { 6.0, -11.0 }, { 6.0, -12.0 } };
GLfloat liquid[6][2] = { { -1.0, 0.0 }, { -9.0, 4.0 }, { -12.0, 7.0 }, { 12.0, 7.0 }, { 9.0, 4.0 }, { 1.0, 0.0 } };
GLfloat remain[4][2] = { { -12.0, 7.0 }, { -12.0, 10.0 }, { 12.0, 10.0 }, { 12.0, 7.0 } };
GLfloat straw[4][2] = { { 7.0, 7.0 }, { 12.0, 12.0 }, { 14.0, 12.0 }, { 9.0, 7.0 } };
GLfloat deco[8][2] = { { 12.0, 12.0 }, { 10.0, 14.0 }, { 10.0, 16.0 }, { 12.0, 18.0 }, { 14.0, 18.0 }, { 16.0, 16.0 }, { 16.0, 14.0 }, { 14.0, 12.0 } };

GLfloat cocktail_color[5][3] = {
	{ 235 / 255.0f, 225 / 255.0f, 196 / 255.0f },
	{ 0 / 255.0f, 63 / 255.0f, 122 / 255.0f },
	{ 235 / 255.0f, 225 / 255.0f, 196 / 255.0f },
	{ 191 / 255.0f, 255 / 255.0f, 0 / 255.0f },
	{ 218 / 255.0f, 165 / 255.0f, 32 / 255.0f }
};

GLuint VBO_cocktail, VAO_cocktail;
void prepare_cocktail() {
	GLsizeiptr buffer_size = sizeof(neck)+sizeof(liquid)+sizeof(remain)+sizeof(straw)
		+sizeof(deco);

	// Initialize vertex buffer object.
	glGenBuffers(1, &VBO_cocktail);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_cocktail);
	glBufferData(GL_ARRAY_BUFFER, buffer_size, NULL, GL_STATIC_DRAW); // allocate buffer object memory

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(neck), neck);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(neck), sizeof(liquid), liquid);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(neck)+sizeof(liquid), sizeof(remain), remain);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(neck)+sizeof(liquid)+sizeof(remain), sizeof(straw), straw);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(neck)+sizeof(liquid)+sizeof(remain)+sizeof(straw),
		sizeof(deco), deco);

	// Initialize vertex array object.
	glGenVertexArrays(1, &VAO_cocktail);
	glBindVertexArray(VAO_cocktail);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_cocktail);
	glVertexAttribPointer(LOC_VERTEX, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void draw_cocktail() {
	glBindVertexArray(VAO_cocktail);

	glUniform3fv(loc_primitive_color, 1, cocktail_color[COCKTAIL_NECK]);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 6);

	glUniform3fv(loc_primitive_color, 1, cocktail_color[COCKTAIL_LIQUID]);
	glDrawArrays(GL_TRIANGLE_FAN, 6, 6);

	glUniform3fv(loc_primitive_color, 1, cocktail_color[COCKTAIL_REMAIN]);
	glDrawArrays(GL_TRIANGLE_FAN, 12, 4);

	glUniform3fv(loc_primitive_color, 1, cocktail_color[COCKTAIL_STRAW]);
	glDrawArrays(GL_TRIANGLE_FAN, 16, 4);

	glUniform3fv(loc_primitive_color, 1, cocktail_color[COCKTAIL_DECO]);
	glDrawArrays(GL_TRIANGLE_FAN, 20, 8);

	glBindVertexArray(0);
}

// MODEL : CAR2

#define CAR2_BODY 0
#define CAR2_FRONT_WINDOW 1
#define CAR2_BACK_WINDOW 2
#define CAR2_FRONT_WHEEL 3
#define CAR2_BACK_WHEEL 4
#define CAR2_LIGHT1 5
#define CAR2_LIGHT2 6

int car2_clock;
float car2_spawnXcor;
float car2_Ycor = 0.0f;
float car2_Xcor = 0.0f;
bool car2_fallingFlag = false;
bool car2_activeFlag = false;

GLfloat car2_body[8][2] = { { -18.0, -7.0 }, { -18.0, 0.0 }, { -13.0, 0.0 }, { -10.0, 8.0 }, { 10.0, 8.0 }, { 13.0, 0.0 }, { 18.0, 0.0 }, { 18.0, -7.0 } };
GLfloat car2_front_window[4][2] = { { -10.0, 0.0 }, { -8.0, 6.0 }, { -2.0, 6.0 }, { -2.0, 0.0 } };
GLfloat car2_back_window[4][2] = { { 0.0, 0.0 }, { 0.0, 6.0 }, { 8.0, 6.0 }, { 10.0, 0.0 } };
GLfloat car2_front_wheel[8][2] = { { -11.0, -11.0 }, { -13.0, -8.0 }, { -13.0, -7.0 }, { -11.0, -4.0 }, { -7.0, -4.0 }, { -5.0, -7.0 }, { -5.0, -8.0 }, { -7.0, -11.0 } };
GLfloat car2_back_wheel[8][2] = { { 7.0, -11.0 }, { 5.0, -8.0 }, { 5.0, -7.0 }, { 7.0, -4.0 }, { 11.0, -4.0 }, { 13.0, -7.0 }, { 13.0, -8.0 }, { 11.0, -11.0 } };
GLfloat car2_light1[3][2] = { { -18.0, -1.0 }, { -17.0, -2.0 }, { -18.0, -3.0 } };
GLfloat car2_light2[3][2] = { { -18.0, -4.0 }, { -17.0, -5.0 }, { -18.0, -6.0 } };

GLfloat car2_color[7][3] = {
	{ 100 / 255.0f, 141 / 255.0f, 159 / 255.0f },
	{ 235 / 255.0f, 219 / 255.0f, 208 / 255.0f },
	{ 235 / 255.0f, 219 / 255.0f, 208 / 255.0f },
	{ 0 / 255.0f, 0 / 255.0f, 0 / 255.0f },
	{ 0 / 255.0f, 0 / 255.0f, 0 / 255.0f },
	{ 249 / 255.0f, 244 / 255.0f, 0 / 255.0f },
	{ 249 / 255.0f, 244 / 255.0f, 0 / 255.0f }
};

GLuint VBO_car2, VAO_car2;
void prepare_car2() {
	GLsizeiptr buffer_size = sizeof(car2_body)+sizeof(car2_front_window)+sizeof(car2_back_window)+sizeof(car2_front_wheel)
		+sizeof(car2_back_wheel)+sizeof(car2_light1)+sizeof(car2_light2);

	// Initialize vertex buffer object.
	glGenBuffers(1, &VBO_car2);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_car2);
	glBufferData(GL_ARRAY_BUFFER, buffer_size, NULL, GL_STATIC_DRAW); // allocate buffer object memory

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(car2_body), car2_body);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(car2_body), sizeof(car2_front_window), car2_front_window);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(car2_body)+sizeof(car2_front_window), sizeof(car2_back_window), car2_back_window);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(car2_body)+sizeof(car2_front_window)+sizeof(car2_back_window), sizeof(car2_front_wheel), car2_front_wheel);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(car2_body)+sizeof(car2_front_window)+sizeof(car2_back_window)+sizeof(car2_front_wheel),
		sizeof(car2_back_wheel), car2_back_wheel);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(car2_body)+sizeof(car2_front_window)+sizeof(car2_back_window)+sizeof(car2_front_wheel)
		+sizeof(car2_back_wheel), sizeof(car2_light1), car2_light1);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(car2_body)+sizeof(car2_front_window)+sizeof(car2_back_window)+sizeof(car2_front_wheel)
		+sizeof(car2_back_wheel)+sizeof(car2_light1), sizeof(car2_light2), car2_light2);

	// Initialize vertex array object.
	glGenVertexArrays(1, &VAO_car2);
	glBindVertexArray(VAO_car2);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_car2);
	glVertexAttribPointer(LOC_VERTEX, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void draw_car2() {
	glBindVertexArray(VAO_car2);

	glUniform3fv(loc_primitive_color, 1, car2_color[CAR2_BODY]);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 8);

	glUniform3fv(loc_primitive_color, 1, car2_color[CAR2_FRONT_WINDOW]);
	glDrawArrays(GL_TRIANGLE_FAN, 8, 4);

	glUniform3fv(loc_primitive_color, 1, car2_color[CAR2_BACK_WINDOW]);
	glDrawArrays(GL_TRIANGLE_FAN, 12, 4);

	glUniform3fv(loc_primitive_color, 1, car2_color[CAR2_FRONT_WHEEL]);
	glDrawArrays(GL_TRIANGLE_FAN, 16, 8);

	glUniform3fv(loc_primitive_color, 1, car2_color[CAR2_BACK_WHEEL]);
	glDrawArrays(GL_TRIANGLE_FAN, 24, 8);

	glUniform3fv(loc_primitive_color, 1, car2_color[CAR2_LIGHT1]);
	glDrawArrays(GL_TRIANGLE_FAN, 32, 3);

	glUniform3fv(loc_primitive_color, 1, car2_color[CAR2_LIGHT2]);
	glDrawArrays(GL_TRIANGLE_FAN, 35, 3);

	glBindVertexArray(0);
}

// MODEL : BIRD

#define BIRD_JUMP_DELAY 50
#define BIRD_ROTATE_ANGLE 70.0f

#define BIRD_LEG1 0
#define BIRD_LEG2 1
#define BIRD_BODY 2
#define BIRD_BEAK 3
#define BIRD_EYE 4
#define BIRD_PUPIL 5

bool bird_jumpFlag = false;
int bird_jumpClock = 0;

GLfloat bird_leg1[2][2] = { { -10.0, -20.0 }, { 0.0, 0.0 } };
GLfloat bird_leg2[2][2] = { { 10.0, -20.0 }, { 0.0, 0.0 } };
GLfloat bird_body[4][2] = { { -15.0, -15.0 }, { -15.0, 15.0 }, { 15.0, 15.0 }, {15.0, -15.0} };
GLfloat bird_beak[3][2] = { { 15.0, 7.0 }, { 15.0, -7.0 }, { 30.0, 0.0 } };
GLfloat bird_eye[1][2] = { 7.0, 7.0 };
GLfloat bird_pupil[1][2] = { 8.0, 7.0 };

GLfloat bird_color[6][3] = {
	{ 0.0f, 0.0f, 0.0f },
	{ 0.0f, 0.0f, 0.0f },
	{ 255.0f / 255.0f, 0.0f / 255.0f, 0.0f / 255.0f },
	{ 255.0f / 255.0f, 200.0f / 255.0f, 0.0f / 255.0f },
	{ 255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f },
	{ 0.0f / 255.0f, 0.0f / 255.0f, 0.0f / 255.0f }
};

GLuint VBO_bird, VAO_bird;
void prepare_bird() {
	GLsizeiptr buffer_size = sizeof(bird_leg1) + sizeof(bird_leg2) + sizeof(bird_body) + sizeof(bird_beak) + sizeof(bird_eye) + sizeof(bird_pupil);

	// Initialize vertex buffer object.
	glGenBuffers(1, &VBO_bird);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_bird);
	glBufferData(GL_ARRAY_BUFFER, buffer_size, NULL, GL_STATIC_DRAW); // allocate buffer object memory

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(bird_leg1), bird_leg1);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(bird_leg1), sizeof(bird_leg2), bird_leg2);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(bird_leg1) + sizeof(bird_leg2), sizeof(bird_body), bird_body);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(bird_leg1) + sizeof(bird_leg2) + sizeof(bird_body), sizeof(bird_beak), bird_beak);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(bird_leg1) + sizeof(bird_leg2) + sizeof(bird_body) + sizeof(bird_beak), sizeof(bird_eye), bird_eye);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(bird_leg1) + sizeof(bird_leg2) + sizeof(bird_body) + sizeof(bird_beak) + sizeof(bird_eye), sizeof(bird_pupil), bird_pupil);

	// Initialize vertex array object.
	glGenVertexArrays(1, &VAO_bird);
	glBindVertexArray(VAO_bird);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_bird);
	glVertexAttribPointer(LOC_VERTEX, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void draw_bird() {
	glBindVertexArray(VAO_bird);

	glUniform3fv(loc_primitive_color, 1, bird_color[BIRD_LEG1]);
	glLineWidth(2.0);
	glDrawArrays(GL_LINES, 0, 2);

	glUniform3fv(loc_primitive_color, 1, bird_color[BIRD_LEG2]);
	glDrawArrays(GL_LINES, 2, 2);

	glUniform3fv(loc_primitive_color, 1, bird_color[BIRD_BODY]);
	glDrawArrays(GL_TRIANGLE_FAN, 4, 4);

	glUniform3fv(loc_primitive_color, 1, bird_color[BIRD_BEAK]);
	glDrawArrays(GL_TRIANGLE_FAN, 8, 3);

	glUniform3fv(loc_primitive_color, 1, bird_color[BIRD_EYE]);
	glPointSize(10.0);
	glDrawArrays(GL_POINTS, 11, 1);
	glPointSize(1.0);

	glUniform3fv(loc_primitive_color, 1, bird_color[BIRD_PUPIL]);
	glPointSize(4.0);
	glDrawArrays(GL_POINTS, 12, 1);
	glPointSize(1.0);

	glBindVertexArray(0);
}

// MODEL : GROUND

#define GROUND_EARTH 0
#define GROUND_GRASS 1

GLfloat ground_earth[4][2] = { { -50.0f, 0.0 }, { -50.0f, -50.0 }, { 50.0f, -50.0 }, { 50.0f, 0.0 } };
GLfloat ground_grass[4][2] = { { -50.0f, 0.0 }, { 50.0f, 0.0 }, { 50.0f, 15.0 }, { -50.0f, 15.0 } };

GLfloat ground_color[2][3] = {
	{ 75.0f / 255.0f, 50.0f / 255.0f, 0.0f / 255.0f },
	{ 80.0f / 255.0f, 150.0f / 255.0f, 50.0f / 255.0f }
};

GLuint VBO_ground, VAO_ground;
void prepare_ground() {
	GLsizeiptr buffer_size = sizeof(ground_earth) + sizeof(ground_grass);

	// Initialize vertex buffer object.
	glGenBuffers(1, &VBO_ground);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_ground);
	glBufferData(GL_ARRAY_BUFFER, buffer_size, NULL, GL_STATIC_DRAW); // allocate buffer object memory

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(ground_earth), ground_earth);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(ground_earth), sizeof(ground_grass), ground_grass);

	// Initialize vertex array object.
	glGenVertexArrays(1, &VAO_ground);
	glBindVertexArray(VAO_ground);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_ground);
	glVertexAttribPointer(LOC_VERTEX, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));

	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void draw_ground() {
	glBindVertexArray(VAO_ground);

	glUniform3fv(loc_primitive_color, 1, ground_color[GROUND_EARTH]);
	glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

	glUniform3fv(loc_primitive_color, 1, ground_color[GROUND_GRASS]);
	glDrawArrays(GL_TRIANGLE_FAN, 4, 4);

	glBindVertexArray(0);
}

// display function

void display(void) {
	int i;
	float x, r, s, delx, delr, dels;
	static float bird_Ycor = 0;
	glm::mat4 ModelMatrix;

	glClear(GL_COLOR_BUFFER_BIT);

	winBorderL = -win_width / 2.0f;
	winBorderR = win_width / 2.0f;
	winBorderD = -win_height / 2.0f;
	winBorderU = win_height / 2.0f;

	groundLevel = winBorderD + 65.0f;

	// COCKTAIL
	ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(winBorderR - cocktail_clock, cocktail_Xcor, 0.0f));
	ModelMatrix = glm::scale(ModelMatrix, glm::vec3(2.0f, 2.0f, 1.0f));
	ModelMatrix = glm::rotate(ModelMatrix, (cocktail_clock + 360 % 360) * TO_RADIAN, glm::vec3(0.0f, 0.0f, 1.0f));
	ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	if (!cocktail_appearDelay)
		draw_cocktail();

	// AIRPLANE
	if(airplane_clock % 100 < 50) // downwards
		ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(winBorderR - airplane_clock, winBorderU - (60.0f + (airplane_clock % 100)), 0.0f));
	else // upwards
		ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(winBorderR - airplane_clock, winBorderU - (160.0f - (airplane_clock % 100)), 0.0f));
	ModelMatrix = glm::scale(ModelMatrix, glm::vec3(1.5f, 1.5f, 1.0f));
	ModelMatrix = glm::rotate(ModelMatrix, -90.0f * TO_RADIAN, glm::vec3(0.0f, 0.0f, 1.0f));
	ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	if (!airplane_appearDelay) {
		draw_airplane();
		airplane_Xcor = winBorderR - airplane_clock;
		if ((int) airplane_Xcor / 3 == (int) car2_spawnXcor / 3) {
			car2_activeFlag = true;
			car2_fallingFlag = true;
			car2_Xcor = airplane_Xcor;
			car2_Ycor = winBorderU - 110.0f;
		}
	}
	
	// CAR (SIDE VIEW)
	ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(car2_Xcor, car2_Ycor + 14.0f, 0.0f));
	ModelMatrix = glm::scale(ModelMatrix, glm::vec3(1.5f, 1.5f, 1.0f));
	ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	if(car2_activeFlag)
		draw_car2();

	// HOUSE
	for (i = 0; i < house_cnt; i++) {
		int curOff = house_offset[i];
		ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(winBorderR - house_clock + 120 * i, groundLevel + 14 * curOff, 0.0f));
		ModelMatrix = glm::scale(ModelMatrix, glm::vec3(curOff, curOff, 1.0f));
		ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
		glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
		if (!house_appearDelay)
			draw_house();
	}

	// BIRD
	for (int j = 0; j < 3; j++) {
		ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(winBorderL + 150.0f - j * 40.0f, 0.0f, 0.0f));
		if (bird_jumpClock) { // bird is jumping
			if (bird_jumpClock >= BIRD_JUMP_DELAY / 2) { // ascending
				bird_Ycor += bird_Ycor + 17.0f <= winBorderU ? 2.0f : 0.0f; // sky is the limit
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(0.0f, bird_Ycor - j * 5.0f, 0.0f));
				// smooth rotating upwards while ascending
				ModelMatrix = glm::rotate(ModelMatrix, (BIRD_ROTATE_ANGLE - bird_jumpClock) * TO_RADIAN, glm::vec3(0.0f, 0.0f, 1.0f));
			}
			else { // descending
				bird_Ycor -= bird_Ycor - 17.0f >= groundLevel ? 1.5f : 0.0f; // check ground level
				ModelMatrix = glm::translate(ModelMatrix, glm::vec3(0.0f, bird_Ycor + j * 5.0f, 0.0f));
				// smooth rotating downwards while descending
				ModelMatrix = glm::rotate(ModelMatrix, (-BIRD_ROTATE_ANGLE + bird_jumpClock) * TO_RADIAN, glm::vec3(0.0f, 0.0f, 1.0f));
			}
		}
		else { // bird is NOT jumping, so descend
			bird_Ycor -= bird_Ycor - 17.0f >= groundLevel ? 1.5f : 0.0f; // check ground level
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(0.0f, bird_Ycor + j * 5.0f, 0.0f));
			// rotate downwards while descending
			ModelMatrix = glm::rotate(ModelMatrix, -BIRD_ROTATE_ANGLE * TO_RADIAN, glm::vec3(0.0f, 0.0f, 1.0f));
		}
		ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
		glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
		draw_bird();
	}

	// GROUND
	ModelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, winBorderD + 50.0f, 0.0f));
	ModelMatrix = glm::scale(ModelMatrix, glm::vec3(win_width / 50.0f, 1.0f, 1.0f));
	ModelViewProjectionMatrix = ViewProjectionMatrix * ModelMatrix;
	glUniformMatrix4fv(loc_ModelViewProjectionMatrix, 1, GL_FALSE, &ModelViewProjectionMatrix[0][0]);
	draw_ground();

	glFlush();	
}   

void keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 27: // ESC key
		glutLeaveMainLoop(); // Incur destuction callback for cleanups.
		break;
	case 32: // SPACE
		bird_jumpFlag = true; // make bird jump
		break;
	}
}

void reshape(int width, int height) {
	win_width = width, win_height = height;
	
  	glViewport(0, 0, win_width, win_height);
	ProjectionMatrix = glm::ortho(-win_width / 2.0, win_width / 2.0, 
		-win_height / 2.0, win_height / 2.0, -1000.0, 1000.0);
	ViewProjectionMatrix = ProjectionMatrix * ViewMatrix;

	glutPostRedisplay();
}

void timer(int value) {
	// COCKTAIL
	if (!cocktail_appearDelay && cocktail_clock + 3 >= win_width) {
		cocktail_appearDelay = rand() % 500 + 70; // delay reappearance
		cocktail_clock = 0;
		cocktail_Xcor = (rand() % (win_height / 3)) - (win_height / 6);
	}
	else if (cocktail_appearDelay)
		cocktail_appearDelay--;
	else
		cocktail_clock = (cocktail_clock + 3) % win_width;

	// HOUSE
	if (!house_appearDelay && house_clock + 3 >= win_width + 120 * MAX_HOUSE_CNT) {
		house_appearDelay = rand() % 100 + 30; // delay reappearance
		house_clock = 0;
		house_cnt = rand() % (MAX_HOUSE_CNT - 5) + 6; // generate house count for new wave
		for (int i = 0; i < house_cnt; i++)
			house_offset[i] = rand() % 5 + 1; // size of each house in wave
	}
	else if (house_appearDelay)
		house_appearDelay--;
	else
		house_clock = (house_clock + 3) % (win_width + 120 * MAX_HOUSE_CNT);

	// AIRPLANE
	if (!airplane_appearDelay && airplane_clock + 3 >= win_width) {
		airplane_appearDelay = rand() % 100 + 30; // delay reappearance
		airplane_clock = 0;
	}
	else if (airplane_appearDelay)
		airplane_appearDelay--;
	else
		airplane_clock = (airplane_clock + 3) % win_width;
	
	// CAR2
	if (car2_fallingFlag) { // decrease in y coordinate
		car2_Ycor -= 3;
		if (car2_Ycor <= groundLevel) { // if car meets ground
			car2_Ycor = groundLevel;
			car2_fallingFlag = false;
		}
	}
	car2_Xcor -= 3;
	if (car2_activeFlag && car2_Xcor < winBorderL) {
		car2_activeFlag = false;
		car2_spawnXcor = rand() % win_width / 2;
	}

	// BIRD
	if (bird_jumpFlag) {
		if (bird_jumpClock != BIRD_JUMP_DELAY)
			bird_jumpClock = BIRD_JUMP_DELAY;
		else
			bird_jumpClock--;
		bird_jumpFlag = false;
	}
	else if(bird_jumpClock)
		bird_jumpClock--;

	glutPostRedisplay();
	glutTimerFunc(10, timer, 0);
}

void cleanup(void) {
	glDeleteVertexArrays(1, &VAO_airplane);
	glDeleteBuffers(1, &VBO_airplane);
}

void register_callbacks(void) {
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutReshapeFunc(reshape);
	glutTimerFunc(10, timer, 0);
	glutCloseFunc(cleanup);
}

void prepare_shader_program(void) {
	ShaderInfo shader_info[3] = {
		{ GL_VERTEX_SHADER, "Shaders/simple.vert" },
		{ GL_FRAGMENT_SHADER, "Shaders/simple.frag" },
		{ GL_NONE, NULL }
	};

	h_ShaderProgram = LoadShaders(shader_info);
	glUseProgram(h_ShaderProgram);

	loc_ModelViewProjectionMatrix = glGetUniformLocation(h_ShaderProgram, "u_ModelViewProjectionMatrix");
	loc_primitive_color = glGetUniformLocation(h_ShaderProgram, "u_primitive_color");
}

void initialize_OpenGL(void) {
	glEnable(GL_MULTISAMPLE); 
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	
	glClearColor( 60.0f / 255.0f, 230.0f / 255.0f, 230.0f / 255.0f, 1.0f);
	ViewMatrix = glm::mat4(1.0f);
}

void prepare_scene(void) {
	prepare_airplane();
	prepare_house();
	prepare_cocktail();
	prepare_car2();
	prepare_bird();
	prepare_ground();
}

void initialize_renderer(void) {
	register_callbacks();
	prepare_shader_program(); 
	initialize_OpenGL();
	prepare_scene();
}

void initialize_glew(void) {
	GLenum error;

	glewExperimental = GL_TRUE;

    error = glewInit();
	if (error != GLEW_OK) { 
		fprintf(stderr, "Error: %s\n", glewGetErrorString(error));
		exit(-1);
	}
	fprintf(stdout, "*********************************************************\n");
	fprintf(stdout, " - GLEW version supported: %s\n", glewGetString(GLEW_VERSION));
	fprintf(stdout, " - OpenGL renderer: %s\n", glGetString(GL_RENDERER));
	fprintf(stdout, " - OpenGL version supported: %s\n", glGetString(GL_VERSION));
	fprintf(stdout, "*********************************************************\n\n");
}

void greetings(char *program_name, char messages[][256], int n_message_lines) {
	fprintf(stdout, "**************************************************************\n\n");
	fprintf(stdout, "  PROGRAM NAME: %s\n\n", program_name);
	fprintf(stdout, "    This program was coded for CSE4170 students\n");
	fprintf(stdout, "      of Dept. of Comp. Sci. & Eng., Sogang University.\n\n");

	for (int i = 0; i < n_message_lines; i++)
		fprintf(stdout, "%s\n", messages[i]);
	fprintf(stdout, "\n**************************************************************\n\n");

	initialize_glew();
}

#define N_MESSAGE_LINES 1
void main(int argc, char *argv[]) {
	char program_name[64] = "Sogang CSE4170 2DObjects_GLSL_3.1";
	char messages[N_MESSAGE_LINES][256] = {
		"    - Keys used: 'ESC' "
	};

	srand(time(NULL));

	// initialize location clock for each object
	airplane_clock = rand() % INIT_WIN_WIDTH;
	house_clock = rand() % INIT_WIN_WIDTH;
	cocktail_clock = car2_clock = 0;

	car2_Xcor = INIT_WIN_WIDTH;
	car2_spawnXcor = rand() % win_width / 2;

	// initialize house count
	house_cnt = rand() % (MAX_HOUSE_CNT - 5) + 6;
	for (int i = 0; i < house_cnt; i++)
		house_offset[i] = rand() % 5 + 1; // size of each house in wave

	airplane_appearDelay = house_appearDelay = cocktail_appearDelay = 0;

	glutInit (&argc, argv);
 	glutInitDisplayMode(GLUT_RGBA | GLUT_MULTISAMPLE);
	glutInitWindowSize (INIT_WIN_WIDTH, INIT_WIN_HEIGHT);
	glutInitContextVersion(4, 0);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutCreateWindow(program_name);

	greetings(program_name, messages, N_MESSAGE_LINES);
	initialize_renderer();

	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
	glutMainLoop ();
}