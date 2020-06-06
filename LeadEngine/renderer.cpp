#include <vector>
#include <fstream>
#include <SOIL2.h>

#include "glm/ext.hpp"

#include "renderer.h"
#include "engine.h"

// #define GLM_ENABLE_EXPERIMENTAL	/* Debug printing purposes. */
// #include <glm/gtx/string_cast.hpp>

/**********************************************************************
 * RENDERER															  *
 **********************************************************************/

Renderer::Renderer() : light(0.5f)
{
	shader = NULL;
	display = NULL;
}

Renderer::~Renderer()
{
	delete shader;
}

void Renderer::init(Display* display)
{
	this->display = display;

	shader = new Shader();
	shader->createVertex("C:/Users/David/source/repos/LeadEngine/LeadEngine/basic.vs");
	shader->createFragment("C:/Users/David/source/repos/LeadEngine/LeadEngine/basic.fs");
	shader->link();

	/* Matrix uniforms */
	shader->createUniform("projMatrix");
	shader->createUniform("viewMatrix");
	shader->createUniform("modelMatrix");

	/* Texture uniforms*/
	shader->createUniform("texSampler");
	shader->createUniform("colour");
	shader->createUniform("shineDamper");
	shader->createUniform("reflectivity");

	/* Main lighting uniforms */
	shader->createUniform("ambientLight");
	shader->createUniform("lightPos");
	shader->createUniform("lightColour");

	/* Enable depth testing. */
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
}

void Renderer::setCulling(bool val)
{
	if (val)
	{
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
	}
	else
	{
		glDisable(GL_CULL_FACE);
	}
}

void Renderer::clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::render(Camera cam, std::vector<Entity*>& entities, bool clear)
{
	if (clear) /* Clear the screen. */
	{
		this->clear();
	}

	updateLight();

	shader->bind();

	/* Create projetion matrix. */
	// TODO: Update code so projMatrix doesn't need to be made every render call. 
	glm::mat4 projMatrix = Transformation::projMatrix(display->getWidth(), display->getHeight(), cam.getFOV(), cam.getNear(), cam.getFar());
	shader->setUniform("projMatrix", projMatrix);

	/* Create view matrix. */
	glm::mat4 viewMatrix = Transformation::viewMatrix(cam);
	shader->setUniform("viewMatrix", viewMatrix);

	shader->setUniform("texSampler", 0);

	for (int i = 0; i < entities.size(); i++) /* Render each entity. */
	{
		setCulling(true);

		Entity* entity = entities.at(i);

		/* Create model matrix. */
		glm::mat4 modelMatrix = Transformation::modelMatrix(entity->position, entity->rotation, entity->scale);
		shader->setUniform("modelMatrix", modelMatrix);

		Texture* texture = entity->getTexture();
		if (texture != NULL) /* Apply texture variables if entity has one. */
		{
			shader->setUniform("colour", texture->getColour());
			shader->setUniform("shineDamper", texture->getShineDamper());
			shader->setUniform("reflectivity", texture->getReflectivity());
			setCulling(texture->hasTransparency());
		}

		entity->render();
	}

	shader->unbind();
}

/* Update the main light's position, intensity and colour. */
void Renderer::updateLight() 
{
	shader->bind();

	shader->setUniform("ambientLight", light.getIntensity());
	shader->setUniform("lightPos", light.position);
	shader->setUniform("lightColour", light.getColour());

	shader->unbind();
}

void Renderer::cleanup()
{
	if (shader != NULL)
	{
		shader->cleanup();
	}
}

/**********************************************************************
 * CAMERA															  *
 **********************************************************************/

Camera::Camera(float x, float y, float z) : Transform(x, y, z)
{
	fov = 60.0f;
	near = 0.01f;
	far = 100.0f;
}

Camera::Camera() : Camera(0, 0, 0)
{
}

float Camera::getFOV()
{
	return fov;
}

void Camera::setFOV(float fov)
{
	if (fov > 180.0f)
		fov = 180.0f;
	else if (fov < 0.01f)
		fov = 0.01f;
	else
		this->fov = fov;
}

float Camera::getNear()
{
	return near;
}

void Camera::setNear(float near)
{
	if (near >= far)
		return;
	this->near = near;
}

float Camera::getFar()
{
	return far;
}

void Camera::setFar(float)
{
	if (far <= near)
		return;
	this->near = near;
}

/**********************************************************************
 * LIGHT															  *
 **********************************************************************/

Light::Light(glm::vec3 position, float intensity) : Transform(position.x, position.y, position.z)
{
	this->intensity = intensity;
	colour = glm::vec3(1.0f);
}

Light::Light(float intensity) : Light(glm::vec3(0.0f), intensity)
{
}

float Light::getIntensity()
{
	return intensity;
}

void Light::setIntensity(float val)
{
	intensity = val;
}

glm::vec3 Light::getColour()
{
	return colour;
}

void Light::setColour(glm::vec3 colour)
{
	this->colour = colour;
}

/**********************************************************************
 * ENTITY 															  *
 **********************************************************************/

Entity::Entity(Mesh* mesh, Texture* texture) : Transform()
{
	this->mesh = mesh;
	this->texture = texture;
}

Mesh* Entity::getMesh()
{
	return mesh;
}

Texture* Entity::getTexture()
{
	return texture;
}

void Entity::render()
{
	if (texture != NULL)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture->getId());
	}

	if (mesh != NULL)
	{
		mesh->render();
	}
}

void Entity::cleanup()
{
	if (mesh != NULL)
	{
		mesh->cleanup();
		delete mesh;
	}
	if (texture != NULL)
	{
		texture->cleanup();
		delete texture;
	}
}

/**********************************************************************
 * MESH 															  *
 **********************************************************************/

#include "OBJ_Loader.h"

Mesh::Mesh(const char* filename) : Mesh()
{
	objl::Loader loader;

	bool loadSuccess = loader.LoadFile(filename);

	if (loadSuccess)
	{
		objl::Mesh mesh = loader.LoadedMeshes[0];

		glGenVertexArrays(1, &vaoId);
		glBindVertexArray(vaoId);

		GLuint vboId;
		glGenBuffers(1, &vboId);
		vboIdList.push_back(vboId);
		glBindBuffer(GL_ARRAY_BUFFER, vboId);
		glBufferData(GL_ARRAY_BUFFER, mesh.Vertices.size() * 8 * sizeof(float), (float*)mesh.Vertices.data(), GL_STATIC_DRAW);
		
		vertexCount = mesh.Indices.size();

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));

		glGenBuffers(1, &vboId);
		vboIdList.push_back(vboId);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboId);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertexCount * sizeof(unsigned int), mesh.Indices.data(), GL_STATIC_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
}

Mesh::Mesh(unsigned int vertexCount, unsigned int indexCount, float* positions, float* texCoords, float* normals, unsigned int* indices)
{
	this->vertexCount = vertexCount;

	glGenVertexArrays(1, &vaoId);
	glBindVertexArray(vaoId);

	// Position VBO
	GLuint vboId;
	glGenBuffers(1, &vboId);
	vboIdList.push_back(vboId);
	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	glBufferData(GL_ARRAY_BUFFER, vertexCount * 3 * sizeof(float), positions, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// Texture coordinates VBO
	glGenBuffers(1, &vboId);
	vboIdList.push_back(vboId);
	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	glBufferData(GL_ARRAY_BUFFER, vertexCount * 2 * sizeof(float), texCoords, GL_STATIC_DRAW);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

	// Normals VBO
	glGenBuffers(1, &vboId);
	vboIdList.push_back(vboId);
	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	glBufferData(GL_ARRAY_BUFFER, vertexCount * 3 * sizeof(float), normals, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// Index VBO
	glGenBuffers(1, &vboId);
	vboIdList.push_back(vboId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboId);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(unsigned int), indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

Mesh::Mesh()
{
	vaoId = 0;
	vertexCount = 0;
}

int Mesh::getVaoId()
{
	return vaoId;
}

int Mesh::getVertexCount()
{
	return vertexCount;
}

void Mesh::render()
{
	glBindVertexArray(getVaoId());
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glDrawElements(GL_TRIANGLES, vertexCount, GL_UNSIGNED_INT, 0);

	// Restore state
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Mesh::cleanup()
{
	glDisableVertexAttribArray(0);

	// Delete the VBOs
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(vboIdList.size(), vboIdList.data());

	// Delete the VAO
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &vaoId);
}

/**********************************************************************
 * TEXTURE      													  *
 **********************************************************************/

Texture::Texture(const char* filename, float shineDamper, float reflectivity, bool transparency) : id(0)
{
	this->colour = glm::vec3(1.0f);
	this->shineDamper = shineDamper;
	this->reflectivity = reflectivity;
	this->transparency = transparency;

	id = SOIL_load_OGL_texture
	(
		filename,
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);

	if (id == 0)
	{
		fprintf(stderr, "Error loading texture: '%s'\n", SOIL_last_result());
	}
}

Texture::Texture(const char* filename, bool transparency) : Texture(filename, 1, 0, transparency)
{
}

Texture::Texture(const char* filename) : Texture(filename, 1, 0, false)
{
}

unsigned int Texture::getId()
{
	return id;
}

glm::vec3 Texture::getColour()
{
	return colour;
}

void Texture::setColour(float r, float g, float b)
{
	colour = glm::vec3(r, g, b);
}

float Texture::getShineDamper()
{
	return shineDamper;
}

void Texture::setShineDamper(float val)
{
	shineDamper = val;
}

float Texture::getReflectivity()
{
	return reflectivity;
}

void Texture::setReflectivity(float val)
{
	reflectivity = val;
}

bool Texture::hasTransparency()
{
	return transparency;
}

void Texture::setTransparency(bool val)
{
	transparency = val;
}

void Texture::cleanup()
{
	glDeleteTextures(1, &id);
}

/**********************************************************************
 * TRANSFORM														  *
 **********************************************************************/

Transform::Transform(float x, float y, float z)
{
	position = glm::vec3(x, y, z);
	rotation = glm::vec3(0.0f);
	scale = glm::vec3(1.0f);
}

Transform::Transform() : Transform(0, 0, 0)
{
}

void Transform::translate(float x, float y, float z)
{
	position += glm::vec3(x, y, z);
}

void Transform::rotate(float x, float y, float z)
{
	rotation += glm::vec3(x, y, z);
}

void Transform::multScale(float factor)
{
	scale *= factor;
}

/**********************************************************************
 * TRANSFORMATION													  *
 **********************************************************************/

glm::mat4 Transformation::projMatrix(int width, int height, float fov, float near, float far)
{
	glm::mat4 matrix = glm::mat4(1.0f);

	float aspect = (float)width / (float)height;
	matrix = glm::perspective(glm::radians(fov), aspect, near, far);

	return matrix;
}

glm::mat4 Transformation::modelMatrix(glm::vec3& position, glm::vec3& rotation, glm::vec3& scale)
{
	glm::mat4 matrix = glm::mat4(1.0f);

	matrix = glm::translate(matrix, position);

	matrix = glm::rotate(matrix, glm::radians(rotation.x), glm::vec3(1, 0, 0));
	matrix = glm::rotate(matrix, glm::radians(rotation.y), glm::vec3(0, 1, 0));
	matrix = glm::rotate(matrix, glm::radians(rotation.z), glm::vec3(0, 0, 1));

	matrix = glm::scale(matrix, scale);

	return matrix;
}

glm::mat4 Transformation::viewMatrix(Camera cam)
{
	glm::mat4 matrix = glm::mat4(1.0f);

	matrix = glm::rotate(matrix, glm::radians(-cam.rotation.x), glm::vec3(1, 0, 0));
	matrix = glm::rotate(matrix, glm::radians(-cam.rotation.y), glm::vec3(0, 1, 0));
	matrix = glm::rotate(matrix, glm::radians(-cam.rotation.z), glm::vec3(0, 0, 1));

	matrix = glm::translate(matrix, -cam.position);

	return matrix;
}

/**********************************************************************
 * SHADER															  *
 **********************************************************************/

Shader::Shader() : programId(glCreateProgram())
{
	if (programId == 0) {
		fprintf(stderr, "Error: %s\n", "Could not create shader");
		exit(EXIT_FAILURE);
	}
	vertexShaderId = 0;
	fragmentShaderId = 0;
}

GLuint Shader::createShader(const char* filename, GLuint shaderType)
{
	GLuint shaderId = glCreateShader(shaderType);
	if (!shaderId)
	{
		fprintf(stderr, "Error: %s%i\n", "Error creating shader. Type: ", shaderType);
		exit(EXIT_FAILURE);
	}

	std::ifstream in(filename);
	std::string contents((std::istreambuf_iterator<char>(in)),
		std::istreambuf_iterator<char>());
	in.close();

	const GLchar* shaderCode = contents.c_str();

	glShaderSource(shaderId, 1, &shaderCode, NULL);
	glCompileShader(shaderId);

	GLint compileStatus;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &compileStatus);

	if (!compileStatus)
	{
		fprintf(stderr, "Failed to compile %s:\n", filename);
		showShaderInfo(shaderId);
		glDeleteShader(shaderId);
		exit(EXIT_FAILURE);
	}

	glAttachShader(programId, shaderId);

	return shaderId;
}

void Shader::createVertex(const char* filename)
{
	vertexShaderId = createShader(filename, GL_VERTEX_SHADER);
}

void Shader::createFragment(const char* filename)
{
	fragmentShaderId = createShader(filename, GL_FRAGMENT_SHADER);
}

void Shader::showProgramInfo(GLuint programId)
{
	GLint length;
	GLchar infoLog[1024];
	glGetProgramInfoLog(programId, 1024, &length, infoLog);
	fprintf(stderr, "%s", infoLog);
}

void Shader::showShaderInfo(GLuint shaderId)
{
	GLint length;
	GLchar infoLog[1024];
	glGetShaderInfoLog(shaderId, 1024, &length, infoLog);
	fprintf(stderr, "%s", infoLog);
}

void Shader::createUniform(const GLchar* name)
{
	GLuint location = glGetUniformLocation(programId, name);
	if (location == -1)
	{
		fprintf(stderr, "Could not find the uniform: %s\n", name);
	}
	uniforms[name] = location;
}

void Shader::setUniform(const GLchar* name, GLint value)
{
	glUniform1i(uniforms.at(name), value);
}

void Shader::setUniform(const GLchar* name, GLfloat value)
{
	glUniform1f(uniforms.at(name), value);
}

void Shader::setUniform(const GLchar* name, const glm::vec2& value)
{
	glUniform2f(uniforms.at(name), value.x, value.y);
}

void Shader::setUniform(const GLchar* name, const glm::vec3& value)
{
	glUniform3f(uniforms.at(name), value.x, value.y, value.z);
}

void Shader::setUniform(const GLchar* name, const glm::vec4& value)
{
	glUniform4f(uniforms.at(name), value.x, value.y, value.z, value.w);
}

void Shader::setUniform(const GLchar* name, const glm::mat4x4& value)
{
	glUniformMatrix4fv(uniforms.at(name), 1, GL_FALSE, &value[0][0]);
}

void Shader::link()
{
	GLint linkStatus;
	glLinkProgram(programId);
	glGetProgramiv(programId, GL_LINK_STATUS, &linkStatus);
	if (!linkStatus)
	{
		fprintf(stderr, "Error linking shader code:\n");
		showProgramInfo(programId);
		glDeleteProgram(programId);
		exit(EXIT_FAILURE);
	}

	if (vertexShaderId)
	{
		glDetachShader(programId, vertexShaderId);
		glDeleteShader(vertexShaderId);
	}
	if (fragmentShaderId)
	{
		glDetachShader(programId, fragmentShaderId);
		glDeleteShader(fragmentShaderId);
	}

	GLint validateStatus;
	glValidateProgram(programId);
	glGetProgramiv(programId, GL_VALIDATE_STATUS, &validateStatus);
	if (!validateStatus)
	{
		fprintf(stderr, "Warning validating shader code:\n");
		showProgramInfo(programId);
	}
}

void Shader::bind()
{
	glUseProgram(programId);
}

void Shader::unbind()
{
	glUseProgram(0);
}

void Shader::cleanup()
{
	unbind();
	if (programId)
	{
		glDeleteProgram(programId);
	}
}
