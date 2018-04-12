#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "Shaders/LoadShaders.h"
extern GLuint h_ShaderProgram; // handle to shader program
extern GLint loc_ModelViewProjectionMatrix, loc_primitive_color; // indices of uniform variables

// include glm/*.hpp only if necessary
//#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp> //translate, rotate, scale, ortho, etc.
extern glm::mat4 ModelViewProjectionMatrix;
extern glm::mat4 ViewMatrix, ProjectionMatrix, ViewProjectionMatrix;

#define TO_RADIAN 0.01745329252f  
#define TO_DEGREE 57.295779513f
#define BUFFER_OFFSET(offset) ((GLvoid *) (offset))

#define LOC_VERTEX 0

#define INIT_WIN_WIDTH 1000
#define INIT_WIN_HEIGHT 600

extern int win_width, win_height;
extern float centerx, centery, rotate_angle;
extern float winBorderR, winBorderD, winBorderL, winBorderU;

// Model: HOUSE

#define HOUSE_ROOF 0
#define HOUSE_BODY 1
#define HOUSE_CHIMNEY 2
#define HOUSE_DOOR 3
#define HOUSE_WINDOW 4
#define MAX_HOUSE_CNT 10

extern int house_appearDelay;
extern int house_clock;
extern int house_cnt;
extern float house_offset[MAX_HOUSE_CNT];

extern GLuint VBO_house, VAO_house;

void prepare_house();
void draw_house();

// Model: AIRPLANE

#define AIRPLANE_BIG_WING 0
#define AIRPLANE_SMALL_WING 1
#define AIRPLANE_BODY 2
#define AIRPLANE_BACK 3
#define AIRPLANE_SIDEWINDER1 4
#define AIRPLANE_SIDEWINDER2 5
#define AIRPLANE_CENTER 6

extern float airplane_Xcor;
extern int airplane_clock;
extern int airplane_appearDelay;
extern float airplane_s_factor;

extern GLuint VBO_airplane, VAO_airplane;

void prepare_airplane();
void draw_airplane();

// Model : COCKTAIL

#define COCKTAIL_NECK 0
#define COCKTAIL_LIQUID 1
#define COCKTAIL_REMAIN 2
#define COCKTAIL_STRAW 3
#define COCKTAIL_DECO 4

extern int cocktail_clock;
extern int cocktail_appearDelay;
extern int cocktail_Xcor;

extern GLuint VBO_cocktail, VAO_cocktail;

void prepare_cocktail();
void draw_cocktail();

// Model : CAR

#define CAR2_BODY 0
#define CAR2_FRONT_WINDOW 1
#define CAR2_BACK_WINDOW 2
#define CAR2_FRONT_WHEEL 3
#define CAR2_BACK_WHEEL 4
#define CAR2_LIGHT1 5
#define CAR2_LIGHT2 6

extern int car2_clock;
extern float car2_spawnXcor;
extern float car2_Ycor;
extern float car2_Xcor;
extern bool car2_fallingFlag;
extern bool car2_activeFlag;

extern GLuint VBO_car2, VAO_car2;

void prepare_car2();
void draw_car2();

// Model : BIRD

#define BIRD_JUMP_DELAY 50
#define BIRD_ROTATE_ANGLE 70.0f

#define BIRD_LEG1 0
#define BIRD_LEG2 1
#define BIRD_TAIL 2
#define BIRD_BODY 3
#define BIRD_BEAK 4
#define BIRD_EYE 5
#define BIRD_PUPIL 6
#define BIRD_WING 7

extern bool bird_jumpFlag;
extern int bird_jumpClock;

extern GLuint VBO_bird, VAO_bird;

void prepare_bird();
void draw_bird();

// Model : GROUND

#define GROUND_EARTH 0
#define GROUND_GRASS 1

extern float groundLevel;

extern GLuint VBO_ground, VAO_ground;

void prepare_ground();
void draw_ground();