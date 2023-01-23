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
	// Check if the engine is using the legacy renderer
	if (Engine::GetInstance()->UseLegacyRenderer())
	{
		// Use SDL to load the BMP file
		SDL_Surface* surface = SDL_LoadBMP(inFileName.c_str());
		if (surface == nullptr)
		{
			SDL_Log("Failed to load .BMP texture: %s, %s", inFileName.c_str(), SDL_GetError());
			return false;
		}

		// Set the color key for transparent pixels
		SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(surface->format, 255, 0, 255));

		// Create a texture from the surface
		SDL_Texture* texture = SDL_CreateTextureFromSurface(Engine::GetInstance()->GetRenderer(), surface);
		if (texture == nullptr)
		{
			SDL_Log("Failed to create texture from surface: %s", SDL_GetError());
			return false;
		}

		// Store the texture in the texture map
		m_TextureMap[inID] = texture;
		m_TextureMapPath[inID] = inFileName;

		return true;
	}
	else
	{
		// Load the texture
		int width, height, numComponents;
		unsigned char* image = stbi_load(inFileName.c_str(), &width, &height, &numComponents, 0);
		if (!image)
		{
			SDL_Log("Failed to load texture: %s", stbi_failure_reason());
			return false;
		}

		GLenum format;
		if (numComponents == 3)
			format = GL_RGB;
		else if (numComponents == 4)
			format = GL_RGBA;
		else
		{
			SDL_Log("Unsupported texture format");
			stbi_image_free(image);
			return false;
		}

		GLuint _gltexture;
		glGenTextures(1, &_gltexture);
		glBindTexture(GL_TEXTURE_2D, _gltexture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, image);
		glBindTexture(GL_TEXTURE_2D, 0);

		// Store the texture in the texture map
		SDL_Texture* sdlTexture = reinterpret_cast<SDL_Texture*>(_gltexture);
		m_TextureMap[inID] = sdlTexture;
		m_GLTextureMap[m_TextureMap[inID]] = _gltexture;
		m_TextureMapPath[inID] = inFileName;

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		// free the image data
		stbi_image_free(image);

		return true;
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

	// Set the OpenGL version and attributes
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 5);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 0);

	// Create the OpenGL context
	SDL_GLContext context = SDL_GL_CreateContext(Engine::GetInstance()->GetWindow());

	// Initialize GLAD
	if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		SDL_Quit();
	}

	// Create and compile the vertex shader
	const char* vertexShaderSource = 
	R"glsl(

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


	const char* fragmentShaderSource = 
	R"glsl(

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
	
//	Renderer::GetInstance()->Draw("ScreenTest", 960 / 2, 640 / 2, 960, 640, 1, 1);
//	Renderer::GetInstance()->Draw("Mykaa", 960 / 2 - 100, 640 / 2 - 200, 100, 100, 1, 1);

	Renderer::BeginBatch();
	World::GetInstance()->Render();	
	Renderer::EndBatch();
	Renderer::Flush();
	
	GLCall(glClearColor(0.1f, 0.1f, 0.1f, 1.f));
}

void Renderer::BeginBatch()
{
	//Sets the pointer RenderingData.QuadBufferPtr to the beginning of the buffer RenderingData.QuadBuffer that holds the data for the quads that will be rendered
	RenderingData.QuadBufferPtr = RenderingData.QuadBuffer;
}

void Renderer::EndBatch()
{
	//called to indicate that the batch of quads has been prepared and is ready to be rendered. 
	//it calculates the size of the data in the buffer by subtracting the address of the end of the buffer from the start of the buffer. 
	//and sends the data in the buffer to the dynamic VBO.
	
	// Calculate the amount of things that need to be rendered. 
	GLsizeiptr size = (uint8_t*)RenderingData.QuadBufferPtr - (uint8_t*)RenderingData.QuadBuffer;
	glBindBuffer(GL_ARRAY_BUFFER, RenderingData.Vbo);

	// Send that data do the dynamic VBO
	glBufferSubData(GL_ARRAY_BUFFER, 0, size, RenderingData.QuadBuffer);
}

void Renderer::Flush()
{
	for (uint32_t i = 0; i < RenderingData.TextureSlotIndex; ++i)
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

		const float sheetWidth = width * frameCount, sheetHeight = height;

		SDL_Rect srcRect = { (width * currentFrame), height * (row - 1), width, height };

		Vector2 _cameraPosition = Camera::GetInstance()->GetPosition();

		SDL_Rect destRect = {(x - width / 2) - _cameraPosition.X, (y - height / 2) - _cameraPosition.Y, width, height };


		if (srcRect.w == 0) {
			return;
		}
		if (srcRect.h == 0) {
			return;
		}
		float px = srcRect.x / srcRect.w;
		float py = srcRect.y / srcRect.h;

		// Convert scale factor to be sprite size * value
		float drawWidth = srcRect.w;
		float drawHeight = srcRect.h;


		// Calculate the normalized X and Y size of the sprite
		drawWidth = MathHelper::MapClampRanged(width, 0.0f, 960, 0.0f, 2.0f);
		drawHeight = MathHelper::MapClampRanged(height, 0.0f, 640, 0.0f, 2.0f);

		// Determine the normalized X and Y position of the sprite
		float drawPosX = MathHelper::MapClampRanged(x, 0.0f, 960, -1.0f, 1.0f) - drawWidth / 2;
		float drawPosY = MathHelper::MapClampRanged(y, 640, 0, -1.0f, 1.0f) - drawHeight / 2;

		// If the max number of quads are already being drawn, draw them and start another batch
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


		/*
		Add new quad to the buffer.
		It creates 4 vertices for the quad, each with a different position and texture coordinate,
		and assigns the same color and texture index to all of them. And it also increments the index count by 6.
		It is important to have this in the renderer because it is actually adding the quad to the buffer and specifying its properties.
		Without it, the quad won't be added to the buffer and therefore won't be rendered.
		*/


		const glm::vec2 textureCoords[] = {
		{(px * srcRect.w) / sheetWidth, (py * srcRect.h) / sheetHeight},
		{((px + 1) * srcRect.w) / sheetWidth, (py * srcRect.h) / sheetHeight},
		{((px + 1) * srcRect.w) / sheetWidth, ((py + 1) * srcRect.h) / sheetHeight},
		{(px * srcRect.w) / sheetWidth, ((py + 1) * srcRect.h) / sheetHeight}
		};

		const glm::vec3 positions[] = {
			{ drawPosX, drawPosY + drawHeight, 0.f},
			{ drawPosX + drawWidth, drawPosY + drawHeight, 0.f},
			{ drawPosX + drawWidth, drawPosY, 0.f },
			{ drawPosX, drawPosY, 0.f}
		};

		for (int i = 0; i < 4; ++i)
		{
			RenderingData.QuadBufferPtr->Position = positions[i];
			RenderingData.QuadBufferPtr->Color = { 1.f, 1.f, 1.f };
			RenderingData.QuadBufferPtr->TexCoords = textureCoords[i];
			RenderingData.QuadBufferPtr->TexID = textureIndex;
			RenderingData.QuadBufferPtr++;
		}
		RenderingData.IndexCount += 6;
	}
}


void Renderer::DrawTile(std::string inTilesetID, int tileSize, int x, int y, int row, int frame, SDL_RendererFlip flip /*= SDL_FLIP_NONE*/)
{
	/// LEGACY SDL RENDERING  ///
	SDL_Rect srcRect = { tileSize * frame, tileSize * row, tileSize, tileSize };

	Vector2 _cameraPosition = Camera::GetInstance()->GetPosition();

	SDL_Rect destRect = { x - _cameraPosition.X, y - _cameraPosition.Y, tileSize, tileSize };

	SDL_RenderCopyEx(Engine::GetInstance()->GetRenderer(), m_TextureMap[inTilesetID], &srcRect, &destRect, 0, nullptr, flip);
}


void Renderer::Draw(std::string inID, int x, int y, int width, int height, float xScale /*= 1.0f*/, float yScale /*= 1.0f*/, float lagRatio /*= 0.0f*/, SDL_RendererFlip flip /*= SDL_FLIP_NONE*/)
{

	if (Engine::GetInstance()->UseLegacyRenderer())
	{
		/// LEGACY SDL RENDERING 
		SDL_Rect srcRect = { 0, 0, width, height };

		Vector2 _cameraPosition = Camera::GetInstance()->GetPosition() * lagRatio;

		SDL_Rect destRect = { x - _cameraPosition.X - width / 2, y - _cameraPosition.Y - height / 2, width * xScale, height * yScale };
		SDL_RenderCopyEx(Engine::GetInstance()->GetRenderer(), m_TextureMap[inID], &srcRect, &destRect, 0, nullptr, flip);
	}
	else
	{



		// Get the GL texture ID associated with the given sprite ID
		GLuint _TextureID = m_GLTextureMap[m_TextureMap[inID]];


		const float sheetWidth = width, sheetHeight = height;

		SDL_Rect srcRect = { 0, 0, width, height };
		

		SDL_Rect destRect = { x  - width / 2, y - height / 2, width * xScale, height * yScale };


		if (srcRect.w == 0) {
			return;
		}
		if (srcRect.h == 0) {
			return;
		}
		float px = srcRect.x / srcRect.w;
		float py = srcRect.y / srcRect.h;

		// Convert scale factor to be sprite size * value
		float drawWidth = srcRect.w;
		float drawHeight = srcRect.h;


		// Calculate the normalized X and Y size of the sprite
		drawWidth = MathHelper::MapClampRanged(width, 0.0f, 960, 0.0f, 2.0f);
		drawHeight = MathHelper::MapClampRanged(height, 0.0f, 640, 0.0f, 2.0f);

		// Determine the normalized X and Y position of the sprite
		float drawPosX = MathHelper::MapClampRanged(x, 0.0f, 960, -1.0f, 1.0f) - drawWidth / 2;
		float drawPosY = MathHelper::MapClampRanged(y, 640, 0, -1.0f, 1.0f) - drawHeight / 2;

		// If the max number of quads are already being drawn, draw them and start another batch
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


		/*
		Add new quad to the buffer.
		It creates 4 vertices for the quad, each with a different position and texture coordinate,
		and assigns the same color and texture index to all of them. And it also increments the index count by 6.
		It is important to have this in the renderer because it is actually adding the quad to the buffer and specifying its properties.
		Without it, the quad won't be added to the buffer and therefore won't be rendered.
		*/


		const glm::vec2 textureCoords[] = {
		{(px * srcRect.w) / sheetWidth, (py * srcRect.h) / sheetHeight},
		{((px + 1) * srcRect.w) / sheetWidth, (py * srcRect.h) / sheetHeight},
		{((px + 1) * srcRect.w) / sheetWidth, ((py + 1) * srcRect.h) / sheetHeight},
		{(px * srcRect.w) / sheetWidth, ((py + 1) * srcRect.h) / sheetHeight}
		};

		const glm::vec3 positions[] = {
			{ drawPosX, drawPosY + drawHeight, 0.f},
			{ drawPosX + drawWidth, drawPosY + drawHeight, 0.f},
			{ drawPosX + drawWidth, drawPosY, 0.f },
			{ drawPosX, drawPosY, 0.f}
		};

		for (int i = 0; i < 4; ++i)
		{
			RenderingData.QuadBufferPtr->Position = positions[i];
			RenderingData.QuadBufferPtr->Color = { 1.f, 1.f, 1.f };
			RenderingData.QuadBufferPtr->TexCoords = textureCoords[i];
			RenderingData.QuadBufferPtr->TexID = textureIndex;
			RenderingData.QuadBufferPtr++;
		}
		RenderingData.IndexCount += 6;
	}	
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




