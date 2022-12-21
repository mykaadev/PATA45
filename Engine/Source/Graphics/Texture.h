#pragma once

#include "Renderer.h"

class Texture
{
public:
	unsigned int m_RendererID;
	std::string m_FilePath;
	unsigned char* m_LocalBuffer;
	int m_width, m_height, m_bpp;

	void Bind(unsigned int slot = 0);
	void Unbind();

	inline int GetWidth() { return m_width; }
	inline int GetHeight() { return m_height; }

	Texture(std::string path);
	~Texture();
};

