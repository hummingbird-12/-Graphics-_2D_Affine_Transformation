#include "Models.h"

GLfloat ground_earth[4][2] = { { -50.0f, 0.0 },{ -50.0f, -50.0 },{ 50.0f, -50.0 },{ 50.0f, 0.0 } };
GLfloat ground_grass[4][2] = { { -50.0f, 0.0 },{ 50.0f, 0.0 },{ 50.0f, 15.0 },{ -50.0f, 15.0 } };

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