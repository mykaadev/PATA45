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
#include "glm.hpp"
#include "ext.hpp"
#include "gtc/matrix_transform.hpp"


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
	/// OPENGL & WORKING STUFF
	float positions[] = {
		-480.0f, -320.0f, 0.0f, 0.0f,
		480.0f, -320.0f, 1.0f, 0.0f,
		480.0f,  320.0f, 1.0f, 1.0f,
		-480.0f, 320.0f, 0.0f, 1.0f
	};

	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};


	GLCall(glEnable(GL_BLEND));
	GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

	GLCall(glGenVertexArrays(1, &m_VAO));
	GLCall(glBindVertexArray(m_VAO));

	m_VB = new VertexBuffer(positions, 4 * 4 * sizeof(float));

	VertexBufferLayout layout;
	layout.Push<float>(2);
	layout.Push<float>(2);

	m_VA = new VertexArray();
	m_VA->AddBuffer(m_VB, layout);

	m_IB = new IndexBuffer(indices, 6);

// 	glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 640.0f, -1.0f, 1.0f);
// 
// 	glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
// 	glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
// 	glm::mat4 mvp = proj * view * model;
// 
	m_Shader = new Shader("../Source/Graphics/Basic.shader");
	m_Shader->Bind();
	m_Shader->SetUniform4f("u_Color", 1.0f, 1.0f, 1.0f, 1.0f);

//	m_Shader->SetUniformMat4f("u_MVP", mvp);
// 
// 	Texture texture("../Assets/Engine/wm.png");
// 	texture.Bind();
// 	m_Shader->SetUniform1i("u_Texture", 0);
// 
// 	m_VA->Unbind();
// 	m_VB->Unbind();
// 	m_IB->Unbind();
// 	m_Shader->Unbind();
}

void Renderer::AddVertexData(const std::string& id, const std::vector<float>& positions, const std::vector<unsigned int>& indices)
{
	VertexData data;
	data.positions = positions;
	data.indices = indices;
	m_VertexData[id] = data;
}

void Renderer::OpenGLLoop()
{
	GLClear();
	m_Shader->Bind();	
	GLDraw(m_VA, m_IB, m_Shader);
}


void Renderer::GLDraw(VertexArray* va, IndexBuffer* ib, Shader* shader)
{
	m_Shader->Bind();
	m_VA->Bind();
	m_IB->Bind();
	GLCall(glDrawElements(GL_TRIANGLES, m_IB->GetCount(), GL_UNSIGNED_INT, nullptr));
}


void Renderer::GLClear()
{
	GLCall(glClear(GL_COLOR_BUFFER_BIT));

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

	// Load the texture using SOIL
	int width = 64, height = 64, numComponents = 3;
	unsigned char* image = stbi_load(inFileName.c_str(), &width, &height, &numComponents, 0);

	// Generate a texture and bind it
	GLuint _gltexture;
	glGenTextures(1, &_gltexture);
	glBindTexture(GL_TEXTURE_2D, _gltexture);

	// Set the texture parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Determine the format of the image data
	GLenum format;
	if (numComponents == 1)
		format = GL_RED;
	else if (numComponents == 3)
		format = GL_RGB;
	else if (numComponents == 4)
		format = GL_RGBA;

	// Upload the texture data
	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, image);
	// Free the image data and unbind the texture
	stbi_image_free(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	// Store the texture in the texture map
	SDL_Texture* sdlTexture = reinterpret_cast<SDL_Texture*>(_gltexture);

	m_TextureMap[inID] = sdlTexture;
	m_GLTextureMap[m_TextureMap[inID]] = _gltexture;

	m_TextureMapPath[inID] = inFileName.c_str();

	return true;

	/// LEGACY SDL LOADING
// 	SDL_Surface* surface = SDL_LoadBMP(inFileName.c_str());
// 	if (surface == nullptr)
// 	{
// 		SDL_Log("Failed to load .BMP texture: %s, %s", inFileName.c_str(), SDL_GetError());
// 		return false;
// 	}
// 
// 	SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 255, 0, 255));
// 
// 	SDL_Texture* texture = SDL_CreateTextureFromSurface(Engine::GetInstance()->GetRenderer(), surface);
// 	if (texture == nullptr)
// 	{
// 		SDL_Log("Failed to create texture from surface: %s", SDL_GetError());
// 		return false;
// 	}
// 
// 	m_TextureMap[inID] = texture;
// 	m_TextureMapPath[inID] = inFileName.c_str();
// 	return true;
 }



void Renderer::Draw(std::string inID, int x, int y, int width, int height, float xScale /*= 1.0f*/, float yScale /*= 1.0f*/, float lagRatio /*= 0.0f*/, SDL_RendererFlip flip /*= SDL_FLIP_NONE*/)
{
	/// CURRENTLY WORKING OPENGL RENDERING BUT IS NOT BATCH RENDERING
	glm::mat4 proj = glm::ortho(0.0f, 960.0f, 640.0f, 0.0f, -1.0f, 1.0f);
	glm::vec2 cameraPosition = glm::vec2(Camera::GetInstance()->GetPosition().X, Camera::GetInstance()->GetPosition().Y) * lagRatio;
	glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(cameraPosition, 0.0f));
	//glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
	glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, 0));
	model = glm::scale(model, glm::vec3((float)width / 960, (float)height / 640, 1.0f));
	glm::mat4 mvp = proj * view * model;

	//m_Shader = new Shader("../Source/Graphics/Basic.shader");
	m_Shader->Bind();
	m_Shader->SetUniform4f("u_Color", 1.0f, 1.0f, 1.0f, 1.0f);
	m_Shader->SetUniformMat4f("u_MVP", mvp);

	Texture texture(m_TextureMapPath[inID]);
	texture.Bind();

	m_Shader->SetUniform1i("u_Texture", 0);

	m_VA->Unbind();
	m_VB->Unbind();
	m_IB->Unbind();
	m_Shader->Unbind();
	

/// ////////////////////////////////////////////////////// 	
///	/// LEGACY SDL RENDERING  ///
// 	SDL_Rect srcRect = { 0, 0, width, height};
// 
// 	Vector2 _cameraPosition = Camera::GetInstance()->GetPosition() * lagRatio;
// 
// 	SDL_Rect destRect = { x - _cameraPosition.X, y - _cameraPosition.Y, width * xScale, height * yScale };
// 	SDL_RenderCopyEx(Engine::GetInstance()->GetRenderer(), m_TextureMap[inID], &srcRect, &destRect, 0, nullptr, flip);
}


void Renderer::DrawFrame(std::string inID, int x, int y, int width, int height, int row, int currentFrame, int startingFrame, int frameCount, SDL_RendererFlip flip /*= SDL_FLIP_NONE*/)
{
	/// CURRENTLY WORKING OPENGL RENDERING BUT IS NOT BATCH RENDERING
	/// CURRENTLY WORKING OPENGL RENDERING BUT IS NOT ANIMATING
	glm::mat4 proj = glm::ortho(0.0f, 960.0f, 640.0f, 0.0f, -1.0f, 1.0f);
	glm::vec2 cameraPosition = glm::vec2(Camera::GetInstance()->GetPosition().X, Camera::GetInstance()->GetPosition().Y);
	glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(cameraPosition, 0.0f));
	glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, 0));
	model = glm::scale(model, glm::vec3((float)width / 960, (float)height / 640, 1.0f));
	glm::mat4 mvp = proj * view * model;

	//m_Shader = new Shader("../Source/Graphics/Basic.shader");
	m_Shader->Bind();
	m_Shader->SetUniform4f("u_Color", 1.0f, 1.0f, 1.0f, 1.0f);
	m_Shader->SetUniformMat4f("u_MVP", mvp);

	Texture texture(m_TextureMapPath[inID]);
	texture.Bind();
	m_Shader->SetUniform1i("u_Texture", 0);

	m_VA->Unbind();
	m_VB->Unbind();
	m_IB->Unbind();
	m_Shader->Unbind();
	
	
	
	///
// 	// Calculate the dimensions of a single frame in the sprite sheet
// 	int frameWidth = width / frameCount;
// 	int frameHeight = height;
// 
// 	// Calculate the source rectangle of the current frame
// 	int srcX = startingFrame + currentFrame * frameWidth;
// 	int srcY = row * frameHeight;
// 
// 	// Modify the model matrix to include a translation and scale operation
// 	// that maps the source rectangle of the texture to the destination rectangle on the screen
// 
// 	glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 640.0f, -1.0f, 1.0f);
// 	glm::vec2 cameraPosition = glm::vec2(Camera::GetInstance()->GetPosition().X, Camera::GetInstance()->GetPosition().Y);
// 	glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(cameraPosition, 0.0f));
// 	//glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
// 	glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(x+ -srcX, y + -srcY, 0));
// 	//model = glm::scale(model, glm::vec3((float)width / 960, (float)height / 640, 1.0f));
// 	model = glm::scale(model, glm::vec3((float)frameWidth * (width / 960), (float)frameHeight * (height / 640), 1.0f));
// 	glm::mat4 mvp = proj * view * model;
// 	// 
// 	//m_Shader = new Shader("../Source/Graphics/Basic.shader");
// 	m_Shader->Bind();
// 	m_Shader->SetUniform4f("u_Color", 1.0f, 1.0f, 1.0f, 1.0f);
// 	m_Shader->SetUniformMat4f("u_MVP", mvp);
// 
// 	Texture texture(m_TextureMapPath[inID]);
// 	texture.Bind();
// 	m_Shader->SetUniform1i("u_Texture", 0);
// 
// 	m_VA->Unbind();
// 	m_VB->Unbind();
// 	m_IB->Unbind();
// 	m_Shader->Unbind();
///

// 		Bind the texture
// 		glBindTexture(GL_TEXTURE_2D, m_GLTextureMap[m_TextureMap[inID]]);
// 	
// 		// Set up the projection matrix
// 		glm::mat4 projection = glm::ortho(0.0f, 960.0f, 0.0f, 640.0f, -1.0f, 1.0f);
// 	
// 		// Set up the camera transform matrix
// 		Vector2 _cameraPosition = Camera::GetInstance()->GetPosition();
// 		glm::mat4 cameraTransform = glm::translate(glm::mat4(1.0f), glm::vec3(-_cameraPosition.X, -_cameraPosition.Y, 0.0f));
// 	
// 		// Set up the model matrix
// 		glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, 0.0f));
// 		model = glm::scale(model, glm::vec3(width, height, 1.0f));
// 	
// 		// Calculate the MVP matrix
// 		glm::mat4 MVP = projection * cameraTransform * model;
// 	
// 		// Set the MVP matrix uniform in the shader program
// 		GLuint shaderProgram = 0;
// 		GLint MVPUniform = glGetUniformLocation(shaderProgram, "MVP");
// 		glUniformMatrix4fv(MVPUniform, 1, GL_FALSE, glm::value_ptr(MVP));
// 	
// 		// Set up the texture coordinates
// 		GLfloat texCoords[] = {
// 			(currentFrame * width) / width, (row * height) / height,
// 			(currentFrame * width) / width, ((row - 1) * height) / height,
// 			((currentFrame - 1) * width) / width, ((row - 1) * height) / height,
// 			((currentFrame - 1) * width) / width, (row * height) / height
// 		};
// 	
// 		// Set up the vertex positions
// 		GLfloat vertices[] = {
// 			-0.5f, -0.5f,
// 			-0.5f, 0.5f,
// 			0.5f, 0.5f,
// 			0.5f, -0.5f
// 		};
// 	
// 		// Set up the indices
// 		GLuint indices[] = {
// 			0, 1, 3,
// 			1, 2, 3
// 		};
// 	
// 	
// 		// Set up the VAO, VBO, and EBO
// 		GLuint VAO, VBO, EBO;
// 		glGenVertexArrays(1, &VAO);
// 		glGenBuffers(1, &VBO);
// 		glGenBuffers(1, &EBO);
// 	
// 	
// 		glBindVertexArray(VAO);
// 	
// 		glBindBuffer(GL_ARRAY_BUFFER, VBO);
// 		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
// 	
// 		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
// 		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
// 	
// 		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)0);
// 		glEnableVertexAttribArray(0);
// 	
// 		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid*)(sizeof(texCoords)));
// 		glEnableVertexAttribArray(1);
// 	
// 		glBindVertexArray(0);
// 	
// 		// Draw the texture
// 		glBindVertexArray(VAO);
// 		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
// 		glBindVertexArray(0);


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
	
// 	for (i = m_TextureMap.begin(); i != m_TextureMap.end(); ++i)
// 	{
// 		SDL_DestroyTexture(i->second);
// 	}

	m_TextureMap.clear();

	//GLCall(glDeleteProgram(m_Shader));
	delete(m_VB);
	delete(m_IB);

}


