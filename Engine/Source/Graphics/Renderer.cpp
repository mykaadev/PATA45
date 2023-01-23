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
#include <SDL.h>
#include <array>
#include "../Core/MathHelper.h"
#include "../Core/World.h"


#pragma region TextureParser


#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Renderer* Renderer::m_Instance = nullptr;


bool Renderer::ParseTextures(std::string source)
{
	TiXmlDocument _xml;
	_xml.LoadFile(source);

	if (_xml.Error())
	{
		std::cout << "Failed to load: " << source << std::endl;
		return false;
	}

	TiXmlElement* _root = _xml.RootElement();
	for (TiXmlElement* e = _root->FirstChildElement(); e != nullptr; e = e->NextSiblingElement())
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
	if (Engine::GetInstance()->UseLegacyRenderer())
	{
		/// LEGACY SDL LOADING
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

		m_TextureMapPath[inID] = inFileName.c_str();
		return true;
	}
	else
	{
		// Load the texture using SOIL
		int width = 0, height = 0, numComponents = 0;
		GLint dataFormat = 0;
		stbi_set_flip_vertically_on_load(1);
		unsigned char* image = stbi_load(inFileName.c_str(), &width, &height, &numComponents, 0);


		RemoveColor(image, width, height, 255, 0, 255);

		// Determine the format of the image data

		if (numComponents == 3)
		{
			dataFormat = GL_RGB;
		}
		else
		{
			dataFormat = GL_RGBA;
		}

		// Generate a texture and bind it
		GLuint _gltexture;
		GLCall(glGenTextures(1, &_gltexture));
		GLCall(glBindTexture(GL_TEXTURE_2D, _gltexture));

		// Set the texture parameters
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));


		// Upload the texture data
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, dataFormat, width, height, 0, dataFormat, GL_UNSIGNED_BYTE, image));

		// Free the image data and unbind the texture
		stbi_image_free(image);
		GLCall(glBindTexture(GL_TEXTURE_2D, 0));

		// Store the texture in the texture map
		SDL_Texture* sdlTexture = reinterpret_cast<SDL_Texture*>(_gltexture);

		m_TextureMap[inID] = sdlTexture;
		m_GLTextureMap[m_TextureMap[inID]] = _gltexture;

		m_TextureMapPath[inID] = inFileName.c_str();

		return true;
	}
}

void Renderer::RemoveColor(unsigned char* pixels, int width, int height, unsigned char red, unsigned char green, unsigned char blue)
{
	for (int i = 0; i < width * height * 3; i += 3)
	{
		if (pixels[i] == red && pixels[i + 1] == green && pixels[i + 2] == blue)
		{
			pixels[i] = pixels[i + 1] = pixels[i + 2] = 0;
		}
	}
}



#pragma endregion


#pragma region OPENGL

static const size_t MaxQuadCount = 5000;
static const size_t MaxVertexCount = MaxQuadCount * 4;
static const size_t m_MaxIndexCount = MaxQuadCount * 6;
static const size_t MaxTextures = 32;

struct Vertex
{
	glm::vec3 Position;
	glm::vec3 Color;
	glm::vec2 TexCoords;
	float TexID;
};

struct RendererData
{
	GLuint Vao = 0;
	GLuint Vbo = 0;
	GLuint Ib = 0;

	uint32_t IndexCount = 0;

	Vertex* QuadBuffer = nullptr;
	Vertex* QuadBufferPtr = nullptr;

	std::array<uint32_t, MaxTextures> TextureSlots;

	// Start at 1 because 0 is reserved for no texture
	uint32_t TextureSlotIndex = 1;
};

static RendererData RenderingData;

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

	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 0);

	SDL_GLContext context = SDL_GL_CreateContext(Engine::GetInstance()->GetWindow());

	if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		SDL_Quit();
	}


#pragma region VertexShader

	const char* vertexShaderSource = R"glsl(
	#version 450 core
	
	layout (location = 0) in vec3 position;
	layout (location = 1) in vec3 color;
	layout (location = 2) in vec2 textureCoords;
	layout (location = 3) in float textureIndex;

	out vec3 Color;
	out vec2 TextureCoords;
	out float TextureIndex;
	
	void main()
	{
		Color = color;
		TextureCoords = textureCoords;
		TextureIndex = textureIndex;
		gl_Position = vec4(position, 1.0);
	}

	)glsl";

	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	GLCall(glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr));
	GLCall(glCompileShader(vertexShader));

	GLint success;
	char infoLog[512];
	GLCall(glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success));

	if (!success)
	{
		GLCall(glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog));
		std::cout << "[OPENGL ERROR] VERTEX Shader Compilation Failed \n" << infoLog << std::endl;
		SDL_GL_DeleteContext(context);
		SDL_DestroyWindow(Engine::GetInstance()->GetWindow());
		SDL_Quit();
	}

#pragma endregion

#pragma region FragmentShader

	const char* fragmentShaderSource = R"glsl(
	#version 450 core

	in vec3 Color;
	in vec2 TextureCoords;
	in float TextureIndex;
	uniform sampler2D Textures[32];

	out vec4 outColor;

	void main()
	{
		int index = int(TextureIndex);
		outColor = texture(Textures[index], TextureCoords);
	}

	)glsl";

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	GLCall(glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr));
	GLCall(glCompileShader(fragmentShader));
	GLCall(glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success));

	if (!success)
	{
		GLCall(glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog));
		std::cout << "[OPENGL ERROR] FRAGMENT Shader Compilation Failed \n" << infoLog << std::endl;
		SDL_GL_DeleteContext(context);
		SDL_DestroyWindow(Engine::GetInstance()->GetWindow());
		SDL_Quit();
	}

#pragma endregion

#pragma region ShaderProgram

	m_ShaderProgram = glCreateProgram();
	GLCall(glAttachShader(m_ShaderProgram, vertexShader));
	GLCall(glAttachShader(m_ShaderProgram, fragmentShader));
	GLCall(glLinkProgram(m_ShaderProgram));

	glGetProgramiv(m_ShaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(m_ShaderProgram, 512, nullptr, infoLog);
		std::cout << "[OPENGL ERROR] SHADER PROGRAM Failed \n" << infoLog << std::endl;
	}

	GLCall(glUseProgram(m_ShaderProgram));

#pragma endregion


	auto textureUniformLocation = glGetUniformLocation(m_ShaderProgram, "Textures");
	int samplers[32];

	for (int i = 0; i < 32; i++)
	{
		samplers[i] = i;
	}

	GLCall(glUniform1iv(textureUniformLocation, 32, samplers));

	RenderingData.QuadBuffer = new Vertex[MaxVertexCount];

	GLCall(glCreateVertexArrays(1, &RenderingData.Vao));
	GLCall(glBindVertexArray(RenderingData.Vao));

	/* Create empty dynamic vbo that will be populated every frame */
	GLCall(glCreateBuffers(1, &RenderingData.Vbo));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, RenderingData.Vbo));
	GLCall(glBufferData(GL_ARRAY_BUFFER, MaxVertexCount * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW));

	GLCall(glEnableVertexArrayAttrib(RenderingData.Vao, 0));
	GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, Position)));

	GLCall(glEnableVertexArrayAttrib(RenderingData.Vao, 1));
	GLCall(glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, Color)));

	GLCall(glEnableVertexArrayAttrib(RenderingData.Vao, 2));
	GLCall(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, TexCoords)));

	GLCall(glEnableVertexArrayAttrib(RenderingData.Vao, 3));
	GLCall(glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, TexID)));

	// CPU Index Buffer
	uint32_t _indices[m_MaxIndexCount];
	uint32_t _offset = 0;
	
	for (size_t i = 0; i < m_MaxIndexCount; i += 6)
	{
		_indices[i + 0] = 0 + _offset;
		_indices[i + 1] = 1 + _offset;
		_indices[i + 2] = 2 + _offset;

		_indices[i + 3] = 2 + _offset;
		_indices[i + 4] = 3 + _offset;
		_indices[i + 5] = 0 + _offset;

		_offset += 4;
	}

	GLCall(glCreateBuffers(1, &RenderingData.Ib));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, RenderingData.Ib));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(_indices), _indices, GL_STATIC_DRAW));

	// Default texture slot is 0, where 0 is none
	for (size_t i = 0; i < MaxTextures; i++) { RenderingData.TextureSlots[i] = 0; }

	ParseTextures("../Assets/Game/TextureParser.tml");
}

void Renderer::OpenGLLoop()
{
	GLCall(glClear(GL_COLOR_BUFFER_BIT));
	GLCall(glUseProgram(m_ShaderProgram));
	Renderer::BeginBatch();
	
//	Renderer::GetInstance()->Draw("ScreenTest", 960 / 2, 640 / 2, 960, 640, 1, 1);
//	Renderer::GetInstance()->Draw("Mykaa", 960 / 2 - 100, 640 / 2 - 200, 100, 100, 1, 1);

	World::GetInstance()->Render();

	Renderer::EndBatch();
	Renderer::Flush();

	GLCall(glClearColor(0.1f, 0.1f, 0.1f, 1.f));
}

void Renderer::BeginBatch()
{
	RenderingData.QuadBufferPtr = RenderingData.QuadBuffer;
}

void Renderer::EndBatch()
{
	// Calculate the amount of things that need to be rendered. 
	GLsizeiptr size = (uint8_t*)RenderingData.QuadBufferPtr - (uint8_t*)RenderingData.QuadBuffer;
	glBindBuffer(GL_ARRAY_BUFFER, RenderingData.Vbo);
	// Send that data do the dynamic VBO
	glBufferSubData(GL_ARRAY_BUFFER, 0, size, RenderingData.QuadBuffer);
}

void Renderer::Flush()
{
	for (uint32_t i = 0; i < RenderingData.TextureSlotIndex; i++)
		glBindTextureUnit(i, RenderingData.TextureSlots[i]);

	glBindVertexArray(RenderingData.Vao);
	glDrawElements(GL_TRIANGLES, RenderingData.IndexCount, GL_UNSIGNED_INT, nullptr);

	// Reset Stats
	RenderingData.IndexCount = 0;
	RenderingData.TextureSlotIndex = 1;
}

void Renderer::ShutDown()
{
	glDeleteVertexArrays(1, &RenderingData.Vao);
	glDeleteBuffers(1, &RenderingData.Vbo);
	glDeleteBuffers(1, &RenderingData.Ib);

	delete[] RenderingData.QuadBuffer;
}

#pragma endregion


#pragma region RENDERING



void Renderer::Draw(std::string inID, int x, int y, int width, int height, float xScale /*= 1.0f*/, float yScale /*= 1.0f*/, float lagRatio /*= 0.0f*/, SDL_RendererFlip flip /*= SDL_FLIP_NONE*/)
{

	if (Engine::GetInstance()->UseLegacyRenderer())
	{
		/// LEGACY SDL RENDERING 
		SDL_Rect srcRect = { 0, 0, width, height };

		Vector2 _cameraPosition = Camera::GetInstance()->GetPosition() * lagRatio;

		SDL_Rect destRect = { x - _cameraPosition.X - width/2, y - _cameraPosition.Y - height/2, width * xScale, height * yScale };
		SDL_RenderCopyEx(Engine::GetInstance()->GetRenderer(), m_TextureMap[inID], &srcRect, &destRect, 0, nullptr, flip);
	}
	else
	{
		/// OPEN GL RENDERING 
		GLuint textureID = m_GLTextureMap[m_TextureMap[inID]];

		SDL_Rect srcRect = { 0, 0, width, height };

		float fX = srcRect.x / srcRect.w;
		float fY = srcRect.y / srcRect.h;

		float drawWidth = srcRect.w * xScale;
		float drawHeight = srcRect.h * yScale;

		drawWidth = MathHelper::MapClampRanged(drawWidth, 0.0f, 960, 0.0f, 2.0f);
		drawHeight = MathHelper::MapClampRanged(drawHeight, 0.0f, 640, 0.0f, 2.0f);

		
		float drawPosX = MathHelper::MapClampRanged(x, 0.0f, 960, -1.0f, 1.0f) - drawWidth / 2;
		float drawPosY = MathHelper::MapClampRanged(y, 640, 0, -1.0f, 1.0f) - drawHeight / 2;


		if (RenderingData.IndexCount >= m_MaxIndexCount || RenderingData.TextureSlotIndex > 31)
		{
			EndBatch();
			Flush();
			BeginBatch();
		}

		// Check if this texture is already bond and used by other quad
		float textureIndex = 0.f;

		for (uint32_t i = 1; i < RenderingData.TextureSlotIndex; i++)
		{
			if (RenderingData.TextureSlots[i] == textureID)
			{
				textureIndex = (float)i;
				break;
			}
		}

		// If not, bound a new one
		if (textureIndex == 0.f)
		{
			textureIndex = (float)RenderingData.TextureSlotIndex;
			if (RenderingData.TextureSlotIndex < MaxTextures) {
				RenderingData.TextureSlots[RenderingData.TextureSlotIndex] = textureID;
				RenderingData.TextureSlotIndex++;
			}
			else {
				std::cout << "[OPENGL ERROR]: Maximum number of textures reached" << std::endl;
			}
		}

		AddQuadToBuffer(drawPosX, drawPosY, drawWidth, drawHeight, textureIndex);

	}
}

// This function handles the rendering of a sprite using Open GL
void Renderer::DrawFrame(std::string inID, int x, int y, int width, int height, int row, int currentFrame, int startingFrame, int frameCount, SDL_RendererFlip flip /*= SDL_FLIP_NONE*/)
{

	if (Engine::GetInstance()->UseLegacyRenderer())
	{
		/// LEGACY SDL RENDERING  ///
		SDL_Rect srcRect = { (width * currentFrame), height * (row - 1), width, height };

		Vector2 _cameraPosition = Camera::GetInstance()->GetPosition();

		SDL_Rect destRect = { (x - width / 2) - _cameraPosition.X, (y - height / 2) - _cameraPosition.Y, width, height };

		SDL_RenderCopyEx(Engine::GetInstance()->GetRenderer(), m_TextureMap[inID], &srcRect, &destRect, 0, nullptr, flip);
	}
	else
	{
		
		// Get the GL texture ID associated with the given sprite ID
		GLuint _TextureID = m_GLTextureMap[m_TextureMap[inID]];

		// Determine the source rectangle for the current frame of the spritesheet
		SDL_Rect _SrcRect = { (width * (currentFrame - startingFrame)), height * (row - 1), width, height };

		// Get the current camera position
		Vector2 _cameraPosition = Camera::GetInstance()->GetPosition();

		// Determine the destination rectangle for the sprite, taking into account the camera position
		SDL_Rect _DestRect = { (x - width / 2) - _cameraPosition.X, (y - height / 2) - _cameraPosition.Y, width, height };

		
		// Determine the width and height of the sprite in normalized space
		float drawWidth = _DestRect.w;
		float drawHeight = _DestRect.h;

		// Calculate the normalized X and Y size of the sprite
		drawWidth = MathHelper::MapClampRanged(drawWidth, 0.0f, 960, 0.0f, 2.0f);
		drawHeight = MathHelper::MapClampRanged(drawHeight, 0.0f, 640, 0.0f, 2.0f);

		// Determine the normalized X and Y position of the sprite
		float drawPosX = MathHelper::MapClampRanged(x, 0.0f, 960, -1.0f, 1.0f) - drawWidth / 2;
		float drawPosY = MathHelper::MapClampRanged(y, 640, 0, -1.0f, 1.0f) - drawHeight / 2;

		// If the current index count exceeds the maximum or the number of textures used is greater than 31, end the current batch and start a new one
		if (RenderingData.IndexCount >= m_MaxIndexCount || RenderingData.TextureSlotIndex > 31)
		{
			EndBatch();
			Flush();
			BeginBatch();
		}

		// Check if the current texture is already boundand being used by another quad
		float textureIndex = 0.f;

		for (uint32_t i = 1; i < RenderingData.TextureSlotIndex; i++)
		{
			if (RenderingData.TextureSlots[i] == _TextureID)
			{
				textureIndex = (float)i;
				break;
			}
		}


		// If the texture is not already bound, bind it
		if (textureIndex == 0.f)
		{
			textureIndex = (float)RenderingData.TextureSlotIndex;
			
			if (RenderingData.TextureSlotIndex < MaxTextures) {
				RenderingData.TextureSlots[RenderingData.TextureSlotIndex] = _TextureID;
				RenderingData.TextureSlotIndex++;
			}
			else 
			{
				std::cout << "[OPENGL ERROR]: Maximum number of textures reached" << std::endl;
			}
		}

		AddQuadToBuffer(drawPosX, drawPosY, drawWidth, drawHeight, textureIndex);
	}

}

/*
Add new quad to the buffer.
It creates 4 vertices for the quad, each with a different position and texture coordinate,
and assigns the same color and texture index to all of them. And it also increments the index count by 6.
It is important to have this in the renderer because it is actually adding the quad to the buffer and specifying its properties.
Without it, the quad won't be added to the buffer and therefore won't be rendered.
*/
void Renderer::AddQuadToBuffer(float drawPosX, float drawPosY, float drawWidth, float drawHeight, float textureIndex) 
{
	// Create an array of vertex positions
	glm::vec3 vertexPositions[] = {
		{ drawPosX, drawPosY, 0.f },
		{ drawPosX + drawWidth, drawPosY, 0.f },
		{ drawPosX + drawWidth, drawPosY + drawHeight, 0.f },
		{ drawPosX, drawPosY + drawHeight, 0.f }
	};

	// Create an array of texture coordinates
	glm::vec2 texCoords[] = {
		{ 0.f, 0.f },
		{ 1.f, 0.f },
		{ 1.f, 1.f },
		{ 0.f, 1.f }
	};

	for (int i = 0; i < 4; i++) {
		RenderingData.QuadBufferPtr->Position = vertexPositions[i];
		RenderingData.QuadBufferPtr->Color = { 1.f, 1.f, 1.f };
		RenderingData.QuadBufferPtr->TexCoords = texCoords[i];
		RenderingData.QuadBufferPtr->TexID = textureIndex;
		RenderingData.QuadBufferPtr++;
	}

	// Update index count
	RenderingData.IndexCount += 6;
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
	if (Engine::GetInstance()->UseLegacyRenderer())
	{
		std::map<std::string, SDL_Texture*>::iterator i;

		for (i = m_TextureMap.begin(); i != m_TextureMap.end(); ++i)
		{
			SDL_DestroyTexture(i->second);
		}

		m_TextureMap.clear();
	}
}


#pragma endregion




