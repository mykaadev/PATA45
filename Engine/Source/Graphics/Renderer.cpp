#include "Renderer.h"
#include "../Core/Engine.h"
#include "../Components/Camera.h"
#include "tinyxml.h"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "glad/glad.h"


//introduce a breakpoint if something goes wrong - ONLY WORKING IN MS VC
#define ASSERT(x) if (!(x)) __debugbreak();

//Clears and Checks for errors
#define GLCall(x) GLClearError(); x; ASSERT(GLLogCall(#x,__FILE__, __LINE__))

static void GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

static bool GLLogCall(const char* function, const char* file, int line)
{
	while (GLenum error = glGetError())
	{
		std::cout << "[OPENGL ERROR] " << "[" << error << "]" << " - " << function << " in "<< file << ": " << line << std::endl;
		return false;
	}

	return true;
}


struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};

static ShaderProgramSource ParseShader(const std::string& filePath)
{
	std::ifstream stream(filePath);
	if (stream.fail())
	{
		std::cout << "[OPENGL ERROR] " << " - " << "Shader file not found" << " in " << ": " << filePath << std::endl;
	}
	enum class ShaderType
	{
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	std::string line;
	std::stringstream ss[2];
	ShaderType type = ShaderType::NONE;

	while (getline(stream, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
			{
				type = ShaderType::VERTEX;
			}
			else if (line.find("fragment") != std::string::npos)
			{
				type = ShaderType::FRAGMENT;
			}
		}
		else
		{
			ss[(int)type] << line << '\n';
		}
	}

	return { ss[0].str(), ss[1].str() };
}






static unsigned int CompileShader(unsigned int type, const std::string& source)
{
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	GLCall(glShaderSource(id, 1, &src, nullptr));
	GLCall(glCompileShader(id));

	int result;
	GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));

	if (result == GL_FALSE)
	{
		int length;
		GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
		char* message = (char*)_malloca(length * sizeof(char));
		GLCall(glGetShaderInfoLog(id, length, &length, message));
		std::cout << "Failed to Compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader" << std::endl;
		std::cout << message << std::endl;
		GLCall(glDeleteShader(id));
		return 0;
	}

	return id;
}


static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	unsigned int program = glCreateProgram();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	GLCall(glAttachShader(program, vs));
	GLCall(glAttachShader(program, fs));
	GLCall(glLinkProgram(program));
	GLCall(glValidateProgram(program));

	GLCall(glDeleteShader(vs));
	GLCall(glDeleteShader(fs));

	return program;
}


static void OPENGL()
{
	float positions[] = {
		
		0.5f, 0.5f, 
		0.5f, -0.5f, 
		-0.5f, -0.5f, 
		-0.5f, 0.5f
	};

	unsigned int indices[] = {
		0,1,2,
		2,3,0
	};

	unsigned int _buffer;
	GLCall(glGenBuffers(1, &_buffer));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, _buffer));
	GLCall(glBufferData(GL_ARRAY_BUFFER, sizeof(positions), positions, GL_STATIC_DRAW));

// 	GLCall(glEnableVertexAttribArray(0));
// 	GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0));

	unsigned int _ibo;
	GLCall(glGenBuffers(1, &_ibo));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW));

	

	ShaderProgramSource source = ParseShader("../Source/Graphics/Basic.shader");

	unsigned int _shader = CreateShader(source.VertexSource, source.FragmentSource);

	GLCall(glUseProgram(_shader));

	GLCall(int location = glGetUniformLocation(_shader, "u_Color"));
	ASSERT(location != -1);
	GLCall(glUniform4f(location, 0.2f, 0.3f, 0.8f, 1.0f));


	GLCall(glUseProgram(0)); 
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

	bool yep = true;

	while (yep)
	{
		GLCall(glUseProgram(_shader));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, _buffer));

// 		GLCall(glEnableVertexAttribArray(0));
// 		GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0));

		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));
		GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
	}

}


#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Renderer* Renderer::m_Instance = nullptr;

void Renderer::InitOpenGL()
{
	OPENGL();
}

bool Renderer::Load(std::string inID, std::string inFileName)
{
	SDL_Surface* surface = SDL_LoadBMP(inFileName.c_str());
	if (surface == nullptr)
	{
		SDL_Log("Failed to load .BMP texture: %s, %s", inFileName.c_str(), SDL_GetError());
		return false;
	}

	SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 255, 0, 255));


	SDL_Texture* texture = SDL_CreateTextureFromSurface(Engine::GetInstance()->GetRenderer(), surface);
	if (texture == nullptr)
	{
		SDL_Log("Failed to create texture from surface: %s", SDL_GetError());
		return false;
	}

	m_TextureMap[inID] = texture;

	return true;
}





bool Renderer::ParseTextures(std::string source)
{
	TiXmlDocument _xml;
	_xml.LoadFile(source);

	if (_xml.Error())
	{
		std::cout<< "Failed to load: " << source << std::endl;
		return false;
	}

	TiXmlElement* _root = _xml.RootElement();
	for (TiXmlElement* e=_root->FirstChildElement(); e != nullptr; e = e->NextSiblingElement())
	{
		if (e->Value() == std::string("texture"))
		{
			std::string _id = e->Attribute("id");
			std::string _src = e->Attribute("source");
			Renderer::Load(_id, _src);
		}
	}

	std::cout << "TextureParser loaded: " << source << std::endl;
	return true;
}



void Renderer::Draw(std::string inID, int x, int y, int width, int height, float xScale /*= 1.0f*/, float yScale /*= 1.0f*/, float lagRatio /*= 0.0f*/, SDL_RendererFlip flip /*= SDL_FLIP_NONE*/)
{

	

	/// LEGACY SDL RENDERING  ///
// 	SDL_Rect srcRect = { 0, 0, width, height};
// 
// 	Vector2 _cameraPosition = Camera::GetInstance()->GetPosition() * lagRatio;
// 
// 	SDL_Rect destRect = { x - _cameraPosition.X, y - _cameraPosition.Y, width * xScale, height * yScale };
//	SDL_RenderCopyEx(Engine::GetInstance()->GetRenderer(), m_TextureMap[inID], &srcRect, &destRect, 0, nullptr, flip);

}


void Renderer::DrawFrame(std::string inID, int x, int y, int width, int height, int row, int currentFrame, int startingFrame, int frameCount, SDL_RendererFlip flip /*= SDL_FLIP_NONE*/)
{


	/// LEGACY SDL RENDERING  ///
// 	SDL_Rect srcRect = { (width* currentFrame), height*(row-1), width, height};
// 
// 	Vector2 _cameraPosition = Camera::GetInstance()->GetPosition();
// 
// 	SDL_Rect destRect = { (x-width/2) - _cameraPosition.X, (y - height/2) - _cameraPosition.Y, width, height };
// 
// 	SDL_RenderCopyEx(Engine::GetInstance()->GetRenderer(), m_TextureMap[inID], &srcRect, &destRect, 0, nullptr, flip);

}



void Renderer::DrawTile(std::string inTilesetID, int tileSize, int x, int y, int row, int frame, SDL_RendererFlip flip /*= SDL_FLIP_NONE*/)
{
	/// LEGACY SDL RENDERING  ///
	SDL_Rect srcRect = { tileSize * frame, tileSize * row, tileSize, tileSize };

	Vector2 _cameraPosition = Camera::GetInstance()->GetPosition();

	SDL_Rect destRect = { x - _cameraPosition.X, y - _cameraPosition.Y, tileSize, tileSize };
	SDL_RenderCopyEx(Engine::GetInstance()->GetRenderer(), m_TextureMap[inTilesetID], &srcRect, &destRect, 0, nullptr, flip);
	
}


void Renderer::Drop(std::string inID)
{
	SDL_DestroyTexture(m_TextureMap[inID]);
	m_TextureMap.erase(inID);
}


void Renderer::Clean()
{
	std::map<std::string, SDL_Texture*>::iterator i;
	
	for (i = m_TextureMap.begin(); i != m_TextureMap.end(); ++i)
	{
		SDL_DestroyTexture(i->second);
	}

	m_TextureMap.clear();
}


