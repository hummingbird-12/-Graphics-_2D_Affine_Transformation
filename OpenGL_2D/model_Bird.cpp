#include "Models.h"

GLfloat bird_leg1[2][2] = { { -10.0, -20.0 },{ 0.0, 0.0 } };
GLfloat bird_leg2[2][2] = { { 10.0, -20.0 },{ 0.0, 0.0 } };
GLfloat bird_tail[3][2] = { { -20.0, 3.0 }, { 0.0, 0.0 }, { -20.0, -3.0 } };
GLfloat bird_body[4][2] = { { -15.0, -15.0 },{ -15.0, 15.0 },{ 15.0, 15.0 },{ 15.0, -15.0 } };
GLfloat bird_beak[3][2] = { { 15.0, 7.0 },{ 15.0, -7.0 },{ 30.0, 0.0 } };
GLfloat bird_eye[1][2] = { 7.0, 7.0 };
GLfloat bird_pupil[1][2] = { 8.0, 7.0 };
GLfloat bird_wing[7][2] = {
	{ -5.0f, 0.0f }, { -7.0f, -10.0f }, { -3.0f, -5.0f },
	{ 0.0f, -10.0f }, { 3.0f, -5.0f }, { 7.0f, -10.0f }, { 5.0, 0.0f }
};

GLfloat bird_color[8][3] = {
	{ 0.0f, 0.0f, 0.0f },
	{ 0.0f, 0.0f, 0.0f },
	{ 10.0f / 255.0f, 130.0f / 255.0f, 30.0f / 255.0f },
	{ 255.0f / 255.0f, 0.0f / 255.0f, 0.0f / 255.0f },
	{ 255.0f / 255.0f, 200.0f / 255.0f, 0.0f / 255.0f },
	{ 255.0f / 255.0f, 255.0f / 255.0f, 255.0f / 255.0f },
	{ 0.0f / 255.0f, 0.0f / 255.0f, 0.0f / 255.0f },
	{ 0.0f, 0.0f, 0.0f }
};

GLuint VBO_bird, VAO_bird;
void prepare_bird() {
	GLsizeiptr buffer_size =
		sizeof(bird_leg1) + sizeof(bird_leg2) +
		sizeof(bird_tail) + sizeof(bird_body) +
		sizeof(bird_beak) + sizeof(bird_eye) +
		sizeof(bird_pupil) + sizeof(bird_wing);

	// Initialize vertex buffer object.
	glGenBuffers(1, &VBO_bird);

	glBindBuffer(GL_ARRAY_BUFFER, VBO_bird);
	glBufferData(GL_ARRAY_BUFFER, buffer_size, NULL, GL_STATIC_DRAW); // allocate buffer object memory

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(bird_leg1), bird_leg1);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(bird_leg1), sizeof(bird_leg2), bird_leg2);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(bird_leg1) + sizeof(bird_leg2), sizeof(bird_tail), bird_tail);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(bird_leg1) + sizeof(bird_leg2) + sizeof(bird_tail), sizeof(bird_body), bird_body);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(bird_leg1) + sizeof(bird_leg2) + sizeof(bird_tail) + sizeof(bird_body), sizeof(bird_beak), bird_beak);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(bird_leg1) + sizeof(bird_leg2) + sizeof(bird_tail) + sizeof(bird_body) + sizeof(bird_beak), sizeof(bird_eye), bird_eye);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(bird_leg1) + sizeof(bird_leg2) + sizeof(bird_tail) + sizeof(bird_body) + sizeof(bird_beak) + sizeof(bird_eye), sizeof(bird_pupil), bird_pupil);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(bird_leg1) + sizeof(bird_leg2) + sizeof(bird_tail) + sizeof(bird_body) + sizeof(bird_beak) + sizeof(bird_eye) + sizeof(bird_pupil), sizeof(bird_wing), bird_wing);

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
	glLineWidth(2.0);
	glDrawArrays(GL_LINES, 2, 2);

	glUniform3fv(loc_primitive_color, 1, bird_color[BIRD_TAIL]);
	glLineWidth(2.0);
	glDrawArrays(GL_LINE_STRIP, 4, 3);

	glUniform3fv(loc_primitive_color, 1, bird_color[BIRD_BODY]);
	glDrawArrays(GL_TRIANGLE_FAN, 7, 4);

	glUniform3fv(loc_primitive_color, 1, bird_color[BIRD_BEAK]);
	glDrawArrays(GL_TRIANGLE_FAN, 11, 3);

	glUniform3fv(loc_primitive_color, 1, bird_color[BIRD_EYE]);
	glPointSize(10.0);
	glDrawArrays(GL_POINTS, 14, 1);
	glPointSize(1.0);

	glUniform3fv(loc_primitive_color, 1, bird_color[BIRD_PUPIL]);
	glPointSize(4.0);
	glDrawArrays(GL_POINTS, 15, 1);
	glPointSize(1.0);

	glUniform3fv(loc_primitive_color, 1, bird_color[BIRD_WING]);
	glLineWidth(2.0);
	glDrawArrays(GL_LINE_STRIP, 16, 7);

	glBindVertexArray(0);
}