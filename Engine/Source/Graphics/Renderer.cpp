#include "Renderer.h"
#include "../Core/Engine.h"
#include "../Components/Camera.h"
#include "tinyxml.h"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "glad/glad.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Shader.h"


void GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
}


bool GLLogCall(const char* function, const char* file, int line)
{
	while (GLenum error = glGetError())
	{
		std::cout << "[OPENGL ERROR] " << "[" << error << "]" << " - " << function << " in "<< file << ": " << line << std::endl;
		return false;
	}

	return true;
}


void Renderer::InitOpenGL()
{
	float positions[] = {
		-0.5f, -0.5f,
		 0.5f, -0.5f,
		 0.5f,  0.5f,
		-0.5f,  0.5f
	};

	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	GLCall(glGenVertexArrays(1, &m_VAO));
	GLCall(glBindVertexArray(m_VAO));

	m_VB = new VertexBuffer(positions, 4 * 2 * sizeof(float));

	VertexBufferLayout layout;
	layout.Push<float>(2);

	m_VA = new VertexArray();
	m_VA->AddBuffer(m_VB, layout);

	m_IB = new IndexBuffer(indices, 6);

	m_Shader = new Shader("../Source/Graphics/Basic.shader");
	m_Shader->Bind();
	m_Shader->SetUniform4f("u_Color", 0.4f, 0.3f, 0.8f, 1.0f);

	m_VA->Unbind();
	m_VB->Unbind();
	m_IB->Unbind();
	m_Shader->Unbind();
}

void Renderer::OpenGLLoop()
{
	GLCall(glClear(GL_COLOR_BUFFER_BIT));

	m_Shader->Bind();	
	//GLCall(glBindVertexArray(m_VAO));
	m_VA->Bind();
	m_IB->Bind();
	
	GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
}


#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Renderer* Renderer::m_Instance = nullptr;


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

	//GLCall(glDeleteProgram(m_Shader));
	delete(m_VB);
	delete(m_IB);

}


