#pragma once
#include <string>
#include "SDL.h"
#include <map>
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

enum SortingLayers
{
	Background2,
	Background1,
	MiddleGround,
	Foreground,
	UI
};


//Introduce a breakpoint if something goes wrong - ONLY WORKING IN MS VC
#define ASSERT(x) if (!(x)) __debugbreak();

//Clears and Checks for errors
#define GLCall(x) GLClearError(); x; ASSERT(GLLogCall(#x,__FILE__, __LINE__))


void GLClearError();
bool GLLogCall(const char* function, const char* file, int line);



struct VertexData {
	std::vector<float> positions;
	std::vector<unsigned int> indices;
};


class Renderer
{
public:
	
	static Renderer* GetInstance() { return m_Instance = (m_Instance != nullptr) ? m_Instance : new Renderer(); }

#pragma region OPENGL Rendering

	void GLDraw(VertexArray* va, IndexBuffer* ib, Shader* shader);
	void GLClear();
	
	void InitOpenGL();
	void AddVertexData(const std::string& id, const std::vector<float>& positions, const std::vector<unsigned int>& indices);
	void OpenGLLoop();

	//GLuint VAO, VBO, EBO, shaderProgram;

	Shader* m_Shader;
	unsigned int m_VAO;
	VertexArray* m_VA;
	IndexBuffer* m_IB;
	VertexBuffer* m_VB;
	inline VertexBuffer* GetVB() { return m_VB; }

	std::map<SDL_Texture*, GLuint> m_GLTextureMap;
	std::map<std::string, std::string> m_TextureMapPath;
	std::map<std::string, VertexData> m_VertexData;

	
#pragma endregion

#pragma region Legacy SDL Rendering

	bool Load(std::string inID, std::string inFileName);
	bool ParseTextures(std::string source);
	void Drop(std::string inID);
	void Clean();

	void Draw(std::string inID, int x, int y, int width, int height, float xScale = 1.0f, float yScale = 1.0f, float lagRatio = 0.5f, SDL_RendererFlip flip = SDL_FLIP_NONE);
	void DrawTile(std::string inTilesetID, int tileSize, int x, int y, int row, int frame, SDL_RendererFlip flip = SDL_FLIP_NONE);
	void DrawFrame(std::string inID, int x, int y, int width, int height, int row, int currentFrame, int startingFrame, int frameCount, SDL_RendererFlip flip = SDL_FLIP_NONE);
private:
	Renderer() { };
	static Renderer* m_Instance;


	std::map<std::string, SDL_Texture*> m_TextureMap;


#pragma endregion


};


