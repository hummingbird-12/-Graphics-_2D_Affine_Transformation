#include "Models.h"

GLuint h_ShaderProgram; // handle to shader program
GLint loc_ModelViewProjectionMatrix, loc_primitive_color; // indices of uniform variables

glm::mat4 ModelViewProjectionMatrix;
glm::mat4 ViewMatrix, ProjectionMatrix, ViewProjectionMatrix;

int win_width, win_height;
float centerx, centery, rotate_angle;
float winBorderR, winBorderD, winBorderL, winBorderU;

// Model: HOUSE

int house_appearDelay;
int house_clock;
int house_cnt;
float house_offset[MAX_HOUSE_CNT];

// Model : AIRPLANE

float airplane_Xcor;
int airplane_clock;
int airplane_appearDelay;
float airplane_s_factor;

// Model : COCKTAIL

int cocktail_clock;
int cocktail_appearDelay;
int cocktail_Xcor;

// Model : CAR

int car2_clock;
float car2_spawnXcor;
float car2_Ycor;
float car2_Xcor;
bool car2_fallingFlag;
bool car2_activeFlag;

// Model : BIRD

bool bird_jumpFlag;
int bird_jumpClock;

// Model : GROUND

float groundLevel;

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
	if (!cocktail_appearDelay && cocktail_clock + 8 >= win_width) {
		cocktail_appearDelay = rand() % 300 + 20; // delay reappearance
		cocktail_clock = 0;
		cocktail_Xcor = (rand() % (win_height / 3)) - (win_height / 6);
	}
	else if (cocktail_appearDelay)
		cocktail_appearDelay--;
	else
		cocktail_clock = (cocktail_clock + 8) % win_width;

	// HOUSE
	if (!house_appearDelay && house_clock + 3 >= win_width + 120 * MAX_HOUSE_CNT) {
		house_appearDelay = rand() % 30;// +30; // delay reappearance
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
		car2_Ycor -= 4;
		if (car2_Ycor <= groundLevel) { // if car meets ground
			car2_Ycor = groundLevel;
			car2_fallingFlag = false;
		}
	}
	car2_Xcor -= 3;
	if (car2_activeFlag && car2_Xcor < winBorderL) { // if car leaves window
		car2_activeFlag = false;
		car2_spawnXcor = (rand() % ((int) winBorderR / 2)) - (int) (winBorderR / 2);
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
	fprintf(stdout, "  STUDENT NAME: INHO KIM\n");
	fprintf(stdout, "  STUDENT ID:   20161577\n\n");

	for (int i = 0; i < n_message_lines; i++)
		fprintf(stdout, "%s\n", messages[i]);
	fprintf(stdout, "\n**************************************************************\n\n");

	initialize_glew();
}

#define N_MESSAGE_LINES 2
void main(int argc, char *argv[]) {
	char program_name[64] = "[GRAPHICS] Programming Homework #1";
	char messages[N_MESSAGE_LINES][256] = {
		"    - Keys used:   'ESC'   : End program",
		"                   'SPACE' : Jump"
	};

	srand(time(NULL));

	centerx = centery = rotate_angle = 0.0f;
	win_width = INIT_WIN_WIDTH;
	win_height = INIT_WIN_HEIGHT;

	// initialize location clock for each object
	airplane_clock = rand() % INIT_WIN_WIDTH;
	house_clock = rand() % INIT_WIN_WIDTH;
	cocktail_clock = car2_clock = 0;

	// initialize object coordinates
	car2_Ycor = car2_Xcor = 0.0f;
	car2_spawnXcor = rand() % win_width / 2;

	airplane_Xcor = win_width / 2.0f;
	airplane_s_factor = 1.0f;

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