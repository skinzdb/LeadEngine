#ifndef RENDERER_H
#define RENDERER_H

#include <GL/glew.h>
#include <unordered_map>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

class Camera : public Transform
{
private:
	float fov;
	float near;
	float far;

public:
	Camera(float x, float y, float z);
	Camera();
	float getFOV();
	void setFOV(float fov);
	float getNear();
	void setNear(float near);
	float getFar();
	void setFar(float far);
};

class Shader
{
private:
	const GLuint programId;
	GLuint vertexShaderId;
	GLuint fragmentShaderId;
	std::unordered_map<const GLchar*, GLuint> uniforms;

	void showProgramInfo(GLuint shaderId);
	void showShaderInfo(GLuint shaderId);

public:
	Shader();
	GLuint createShader(const char* filename, GLuint shaderType);
	void createVertex(const char* filename);
	void createFragment(const char* filename);
	void createUniform(const GLchar* name);
	void setUniform(const GLchar* name, GLint value);
	void setUniform(const GLchar* name, GLfloat value);
	void setUniform(const GLchar* name, glm::vec2& value);
	void setUniform(const GLchar* name, glm::vec3& value);
	void setUniform(const GLchar* name, glm::vec4& value);
	void setUniform(const GLchar* name, glm::mat4x4& value);
	void link();
	void bind();
	void unbind();
	void cleanup();
};

class Light : public Transform
{
private:
	float intensity;
	glm::vec3 colour;

public:
	Light(glm::vec3 position, float intensity);
	Light(float intensity);
	float getIntensity();
	void setIntensity(float val);
	glm::vec3 getColour();
	void setColour(glm::vec3 colour);
};

class Mesh
{
private:
	unsigned int vaoId;
	std::vector<unsigned int> vboIdList;
	int vertexCount;
public:
	Mesh(unsigned int size1, float* positions, unsigned int size2, float* texCoords, unsigned int size3, float* normals, unsigned int size4, unsigned int* indices);
	Mesh();
	int getVaoId();
	int getVertexCount();
	void render();
	void cleanup();
};

class Texture
{
private:
	unsigned int id;
	glm::vec3 colour;
	float shineDamper;
	float reflectivity;
	bool transparency;
public:
	Texture(const char* filename, float shineDamper, float reflectivity, bool transparency);
	Texture(const char* filename, bool transparency);
	Texture(const char* filename);
	unsigned int getId();
	unsigned int getWidth();
	unsigned int getHeight();
	glm::vec3 getColour();
	void setColour(float r, float g, float b);
	float getShineDamper();
	void setShineDamper(float val);
	float getReflectivity();
	void setReflectivity(float val);
	bool hasTransparency();
	void setTransparency(bool val);
	void cleanup();
};

class Entity : public Transform
{
private:
	Mesh* mesh;
	Texture* texture;
public:
	Entity(Mesh* mesh, Texture* texture);
	Mesh* getMesh();
	Texture* getTexture();
	void render();
	void cleanup();
};

class Transformation
{
public:
	static glm::mat4 projMatrix(int width, int height, float fov, float near, float far);
	static glm::mat4 modelMatrix(glm::vec3& pos, glm::vec3& rot, glm::vec3& scale);
	static glm::mat4 viewMatrix(Camera cam);
};

class Renderer
{
private:
	Shader* shader;
	Display* display;
	Light light;

public:
	Renderer();
	~Renderer();
	void init(Display* display);
	static void setCulling(bool val);
	void clear();
	void render(Camera cam, std::vector<Entity*>& entities, bool clear);
	void updateLight();
	void cleanup();
};
#endif
