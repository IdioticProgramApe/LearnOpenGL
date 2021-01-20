#ifndef IMAGE_H
#define IMAGE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb/stb_image.h>

#include <iostream>
#include <filesystem>

class TextureFromFile
{
private:
	unsigned int ID;

	unsigned char* m_data;
	int m_width;
	int m_height;
	int m_nChannel;
	int m_wrapParamS;
	int m_wrapParamT;
	int m_filterMagParam;
	int m_filterMinParam;

public:
	TextureFromFile(
		const std::string & name,
		const std::string & directory,
		bool vflip = false
	) : m_wrapParamS{GL_REPEAT}, 
		m_wrapParamT{GL_REPEAT}, 
		m_filterMagParam{GL_LINEAR},
		m_filterMinParam{GL_NEAREST}
	{
		auto path = std::filesystem::path();
		path /= directory;
		path /= name;
		/*const char *path = (directory + "/" + name).c_str();*/
		glGenTextures(1, &this->ID);

		stbi_set_flip_vertically_on_load(vflip);
		this->m_data = stbi_load(path.string().c_str(), &this->m_width, &this->m_height, &this->m_nChannel, NULL);

		if (this->m_data)
		{
			GLenum format;
			if (this->m_nChannel == 1) format = GL_RED;
			else if (this->m_nChannel == 3) format = GL_RGB;
			else if (this->m_nChannel == 4) format = GL_RGBA;

			glBindTexture(GL_TEXTURE_2D, this->ID);
			glTexImage2D(GL_TEXTURE_2D, 0, format, this->m_width, this->m_height, 0, format, GL_UNSIGNED_BYTE, this->m_data);
			glGenerateMipmap(GL_TEXTURE_2D);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->m_wrapParamS);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->m_wrapParamT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->m_filterMagParam);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->m_filterMinParam);
		}
		else
		{
			std::cout << "ERROR::IMAGE::LOAD_FROM_FILE" << std::endl;
		}
		stbi_image_free(this->m_data);
	}

	~TextureFromFile() {};

	unsigned int getTextureID() { return this->ID; }
	int getTextureWidth() { return this->m_width; }
	int getTextureHeight() { return this->m_height; }
	int getTextureNrChannels() { return this->m_nChannel; }
};

#endif // ! IMAGE_H

