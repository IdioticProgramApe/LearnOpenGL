#ifndef IMAGE_H
#define IMAGE_H

#include <string>
#include <glad/glad.h>
#include <stb/stb_image.h>  // need to be in header, to prevent linkage problems


class TextureFromFile
{
private:
	unsigned int m_id;

	unsigned char* m_data;
	int m_width;
	int m_height;
	int m_nChannel;
	int m_wrapParamS = GL_REPEAT;
	int m_wrapParamT = GL_REPEAT;
	int m_filterMagParam = GL_LINEAR;
	int m_filterMinParam = GL_NEAREST;

public:
	///
	TextureFromFile() = delete;
	///
	TextureFromFile(const std::string & name, const std::string & directory, bool vflip = false);
	///
	virtual ~TextureFromFile() = default;

	unsigned int getTextureID() { return m_id; }
	int getTextureWidth() { return m_width; }
	int getTextureHeight() { return m_height; }
	int getTextureNrChannels() { return m_nChannel; }
};

#endif // ! IMAGE_H

