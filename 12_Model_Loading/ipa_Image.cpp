#include "ipa_Image.h"

#include <glfw3.h>
#include <iostream>

#if ((defined(_MSVC_LANG) && _MSVC_LANG >= 201703L) || __cplusplus >= 201703L)
#include <filesystem>
#endif


TextureFromFile::TextureFromFile(const std::string & name, const std::string & directory, bool vflip/*=false*/)
{
#if ((defined(_MSVC_LANG) && _MSVC_LANG >= 201703L) || __cplusplus >= 201703L)
	auto filepath = std::filesystem::path();
	filepath /= directory;
	filepath /= name;
	std::string path = filepath.string();
#else
	std::string path = directory;
	path += "/";
	path += name;
#endif
	glGenTextures(1, &m_id);

	stbi_set_flip_vertically_on_load(vflip);
	/// note: NULL for req_comp which allows only comp == 3 and comp == 1
	m_data = stbi_load(path.c_str(), &m_width, &m_height, &m_nChannel, NULL);

	if (m_data)
	{
		GLenum format;
		switch (m_nChannel)
		{
		case 1:
			format = GL_RED;  // no macro for GRAYSCALE, use monochrome to represent
			break;
		case 3:
			format = GL_RGB;  // red, green, blue
			break;
		case 4:
			format = GL_RGBA; // red, green, blue, alpha
			break;
		default:
			break;
		}

		glBindTexture(GL_TEXTURE_2D, m_id);
		/*
		* \note
		*    unsigned char's size is 0-255
		*
		* \todo
		*    the reason of level being 0
		*/
		glTexImage2D(GL_TEXTURE_2D, 0, format, m_width, m_height, 0, format, GL_UNSIGNED_BYTE, m_data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, m_wrapParamS);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, m_wrapParamT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_filterMagParam);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_filterMinParam);
	}
	else
	{
		std::cout << "ERROR::IMAGE::LOAD_FROM_FILE" << std::endl;
	}
	stbi_image_free(m_data);
}
