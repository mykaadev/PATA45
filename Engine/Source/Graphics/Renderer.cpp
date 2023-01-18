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
	glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
	glCompileShader(vertexShader);

	GLint success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
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
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		SDL_GL_DeleteContext(context);
		SDL_DestroyWindow(Engine::GetInstance()->GetWindow());
		SDL_Quit();
	}

#pragma endregion

#pragma region ShaderProgram

	m_ShaderProgram = glCreateProgram();
	glAttachShader(m_ShaderProgram, vertexShader);
	glAttachShader(m_ShaderProgram, fragmentShader);
	glLinkProgram(m_ShaderProgram);

	glGetProgramiv(m_ShaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(m_ShaderProgram, 512, nullptr, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINK_FAILED\n" << infoLog << std::endl;
	}

	glUseProgram(m_ShaderProgram);

#pragma endregion


	auto textureUniformLocation = glGetUniformLocation(m_ShaderProgram, "Textures");
	int samplers[32];

	for (int i = 0; i < 32; i++)
	{
		samplers[i] = i;
	}

	glUniform1iv(textureUniformLocation, 32, samplers);

	Init();

	ParseTextures("../Assets/Game/TextureParser.tml");
}



void Renderer::OpenGLLoop()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glUseProgram(m_ShaderProgram);
	Renderer::BeginBatch();
	Renderer::GetInstance()->Draw("Mykaa", 960/2, 640/2, 100, 100 ,1 ,1);
	Renderer::GetInstance()->Draw("Mykaa", 960 / 2 - 100, 640 / 2 - 200, 100, 100, 1, 1);


	World::GetInstance()->Render();    ///UNCOMMENT THIS TO CHECK THE ANIMATIONS

	Renderer::EndBatch();
	Renderer::Flush();

	glClearColor(0.1f, 0.1f, 0.1f, 1.f);
}


static const size_t MaxQuadCount = 5000;
static const size_t MaxVertexCount = MaxQuadCount * 4;
static const size_t MaxIndexCount = MaxQuadCount * 6;
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

static RendererData s_Data;

void Renderer::Init()
{
	s_Data.QuadBuffer = new Vertex[MaxVertexCount];

	glCreateVertexArrays(1, &s_Data.Vao);
	glBindVertexArray(s_Data.Vao);

	/* Create empty dynamic vbo that will be populated every frame */
	glCreateBuffers(1, &s_Data.Vbo);
	glBindBuffer(GL_ARRAY_BUFFER, s_Data.Vbo);
	glBufferData(GL_ARRAY_BUFFER, MaxVertexCount * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW);

	glEnableVertexArrayAttrib(s_Data.Vao, 0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, Position));

	glEnableVertexArrayAttrib(s_Data.Vao, 1);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, Color));

	glEnableVertexArrayAttrib(s_Data.Vao, 2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, TexCoords));

	glEnableVertexArrayAttrib(s_Data.Vao, 3);
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)offsetof(Vertex, TexID));

	// CPU Index Buffer
	uint32_t indices[MaxIndexCount];
	uint32_t offset = 0;
	for (size_t i = 0; i < MaxIndexCount; i += 6)
	{
		indices[i + 0] = 0 + offset;
		indices[i + 1] = 1 + offset;
		indices[i + 2] = 2 + offset;

		indices[i + 3] = 2 + offset;
		indices[i + 4] = 3 + offset;
		indices[i + 5] = 0 + offset;

		offset += 4;
	}

	glCreateBuffers(1, &s_Data.Ib);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_Data.Ib);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// Default texture slot is 0 (meaning none)
	for (size_t i = 0; i < MaxTextures; i++)
		s_Data.TextureSlots[i] = 0;
}

void Renderer::ShutDown()
{
	glDeleteVertexArrays(1, &s_Data.Vao);
	glDeleteBuffers(1, &s_Data.Vbo);
	glDeleteBuffers(1, &s_Data.Ib);

	delete[] s_Data.QuadBuffer;
}

void Renderer::BeginBatch()
{
	s_Data.QuadBufferPtr = s_Data.QuadBuffer;
}

void Renderer::EndBatch()
{
	// Calculate the amount of things that need to be rendered. 
	GLsizeiptr size = (uint8_t*)s_Data.QuadBufferPtr - (uint8_t*)s_Data.QuadBuffer;
	glBindBuffer(GL_ARRAY_BUFFER, s_Data.Vbo);
	// Send that data do the dynamic VBO
	glBufferSubData(GL_ARRAY_BUFFER, 0, size, s_Data.QuadBuffer);
}

void Renderer::Flush()
{
	for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++)
		glBindTextureUnit(i, s_Data.TextureSlots[i]);

	glBindVertexArray(s_Data.Vao);
	glDrawElements(GL_TRIANGLES, s_Data.IndexCount, GL_UNSIGNED_INT, nullptr);

	// Reset Stats
	s_Data.IndexCount = 0;
	s_Data.TextureSlotIndex = 1;
}


void Renderer::AddVertexData(const std::string& id, const std::vector<float>& positions, const std::vector<unsigned int>& indices)
{
	VertexData data;
	data.positions = positions;
	data.indices = indices;
	m_VertexData[id] = data;
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


void Renderer::Draw(std::string inID, int x, int y, int width, int height, float xScale /*= 1.0f*/, float yScale /*= 1.0f*/, float lagRatio /*= 0.0f*/, SDL_RendererFlip flip /*= SDL_FLIP_NONE*/)
{

	if (Engine::GetInstance()->UseLegacyRenderer())
	{
		/// LEGACY SDL RENDERING 
		SDL_Rect srcRect = { 0, 0, width, height};

		Vector2 _cameraPosition = Camera::GetInstance()->GetPosition() * lagRatio;

		SDL_Rect destRect = { x - _cameraPosition.X, y - _cameraPosition.Y, width * xScale, height * yScale };
		SDL_RenderCopyEx(Engine::GetInstance()->GetRenderer(), m_TextureMap[inID], &srcRect, &destRect, 0, nullptr, flip);
	}
	else
	{
		GLuint textureID = m_GLTextureMap[m_TextureMap[inID]];
		
		SDL_Rect srcRect = { 0, 0, width, height };
	
		float fX = srcRect.x / srcRect.w;
		float fY = srcRect.y / srcRect.h;

		float drawWidth = srcRect.w * xScale;
		float drawHeight = srcRect.h * yScale;

		drawWidth = MathHelper::MapClampRanged(drawWidth, 0.0f, 960, 0.0f, 2.0f);
		drawHeight = MathHelper::MapClampRanged(drawHeight, 0.0f, 640, 0.0f, 2.0f);

		float drawPosX = MathHelper::MapClampRanged(x, 0.0f, 960, -1.0f, 1.0f) - drawWidth/2;
		float drawPosY = MathHelper::MapClampRanged(y, 0.0f, 640, -1.0f, 1.0f) - drawHeight/2;


		if (s_Data.IndexCount >= MaxIndexCount || s_Data.TextureSlotIndex > 31)
		{
			EndBatch();
			Flush();
			BeginBatch();
		}

		// Check if this texture is already bond and used by other quad
		float textureIndex = 0.f;

		for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++)
		{
			if (s_Data.TextureSlots[i] == textureID)
			{
				textureIndex = (float)i;
				break;
			}
		}

		// If not, bound a new one
		if (textureIndex == 0.f)
		{
			textureIndex = (float)s_Data.TextureSlotIndex;
			s_Data.TextureSlots[s_Data.TextureSlotIndex] = textureID;
			s_Data.TextureSlotIndex++;
		}

		s_Data.QuadBufferPtr->Position = { drawPosX, drawPosY, 0.f };
		s_Data.QuadBufferPtr->Color = { 1.f, 1.f, 1.f };
		s_Data.QuadBufferPtr->TexCoords = { 0.f, 0.f };
		s_Data.QuadBufferPtr->TexID = textureIndex;
		s_Data.QuadBufferPtr++;

		s_Data.QuadBufferPtr->Position = { drawPosX + drawWidth, drawPosY, 0.f };
		s_Data.QuadBufferPtr->Color = { 1.f, 1.f, 1.f };
		s_Data.QuadBufferPtr->TexCoords = { 1.f, 0.f };
		s_Data.QuadBufferPtr->TexID = textureIndex;
		s_Data.QuadBufferPtr++;

		s_Data.QuadBufferPtr->Position = { drawPosX + drawWidth, drawPosY + drawHeight, 0.f };
		s_Data.QuadBufferPtr->Color = { 1.f, 1.f, 1.f };
		s_Data.QuadBufferPtr->TexCoords = { 1.f, 1.f };
		s_Data.QuadBufferPtr->TexID = textureIndex;
		s_Data.QuadBufferPtr++;

		s_Data.QuadBufferPtr->Position = { drawPosX, drawPosY + drawHeight, 0.f };
		s_Data.QuadBufferPtr->Color = { 1.f, 1.f, 1.f };
		s_Data.QuadBufferPtr->TexCoords = { 0.f, 1.f };
		s_Data.QuadBufferPtr->TexID = textureIndex;
		s_Data.QuadBufferPtr++;

		s_Data.IndexCount += 6;

	}
}


void Renderer::DrawFrame(std::string inID, int x, int y, int width, int height, int row, int currentFrame, int startingFrame, int frameCount, SDL_RendererFlip flip /*= SDL_FLIP_NONE*/)
{

	if (Engine::GetInstance()->UseLegacyRenderer())
	{
		/// LEGACY SDL RENDERING  ///
		SDL_Rect srcRect = { (width* currentFrame), height*(row-1), width, height};

		Vector2 _cameraPosition = Camera::GetInstance()->GetPosition();

		SDL_Rect destRect = { (x-width/2) - _cameraPosition.X, (y - height/2) - _cameraPosition.Y, width, height };

		SDL_RenderCopyEx(Engine::GetInstance()->GetRenderer(), m_TextureMap[inID], &srcRect, &destRect, 0, nullptr, flip);
	}
	else
	{
		GLuint textureID = m_GLTextureMap[m_TextureMap[inID]];

		SDL_Rect srcRect = { (width * currentFrame), height * (row - 1), width, height };

		Vector2 _cameraPosition = Camera::GetInstance()->GetPosition();

		SDL_Rect destRect = { (x - width / 2) - _cameraPosition.X, (y - height / 2) - _cameraPosition.Y, width, height };

		float fX = destRect.x / destRect.w;
		float fY = destRect.y / destRect.h;

		float drawWidth = destRect.w;
		float drawHeight = destRect.h;

		drawWidth = MathHelper::MapClampRanged(drawWidth, 0.0f, 960, 0.0f, 2.0f);
		drawHeight = MathHelper::MapClampRanged(drawHeight, 0.0f, 640, 0.0f, 2.0f);

		float drawPosX = MathHelper::MapClampRanged(x, 0.0f, 960, -1.0f, 1.0f) - drawWidth / 2;
		float drawPosY = MathHelper::MapClampRanged(y, 640, 0, -1.0f, 1.0f) - drawHeight / 2;


		if (s_Data.IndexCount >= MaxIndexCount || s_Data.TextureSlotIndex > 31)
		{
			EndBatch();
			Flush();
			BeginBatch();
		}

		// Check if this texture is already bond and used by other quad
		float textureIndex = 0.f;

		for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++)
		{
			if (s_Data.TextureSlots[i] == textureID)
			{
				textureIndex = (float)i;
				break;
			}
		}



		// If not, bound a new one
		if (textureIndex == 0.f)
		{
			textureIndex = (float)s_Data.TextureSlotIndex;
			s_Data.TextureSlots[s_Data.TextureSlotIndex] = textureID;
			s_Data.TextureSlotIndex++;
		}

		
 		
 		s_Data.QuadBufferPtr->Position = { drawPosX, drawPosY, 0.f };
 		s_Data.QuadBufferPtr->Color = { 1.f, 1.f, 1.f };
 		s_Data.QuadBufferPtr->TexCoords = { 0.f, 0.f };
 		s_Data.QuadBufferPtr->TexID = textureIndex;
 		s_Data.QuadBufferPtr++;
 
 		s_Data.QuadBufferPtr->Position = { drawPosX + drawWidth, drawPosY, 0.f };
 		s_Data.QuadBufferPtr->Color = { 1.f, 1.f, 1.f };
 		s_Data.QuadBufferPtr->TexCoords = { 1.f, 0.f };
 		s_Data.QuadBufferPtr->TexID = textureIndex;
 		s_Data.QuadBufferPtr++;
 
 		s_Data.QuadBufferPtr->Position = { drawPosX + drawWidth, drawPosY + drawHeight, 0.f };
 		s_Data.QuadBufferPtr->Color = { 1.f, 1.f, 1.f };
 		s_Data.QuadBufferPtr->TexCoords = { 1.f, 1.f };
 		s_Data.QuadBufferPtr->TexID = textureIndex;
 		s_Data.QuadBufferPtr++;
 
 		s_Data.QuadBufferPtr->Position = { drawPosX, drawPosY + drawHeight, 0.f };
 		s_Data.QuadBufferPtr->Color = { 1.f, 1.f, 1.f };
 		s_Data.QuadBufferPtr->TexCoords = { 0.f, 1.f };
 		s_Data.QuadBufferPtr->TexID = textureIndex;
 		s_Data.QuadBufferPtr++;
 
 		s_Data.IndexCount += 6;
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
	else
	{
			//GLCall(glDeleteProgram(m_Shader));
			delete(m_VB);
			delete(m_IB);
	}
}


