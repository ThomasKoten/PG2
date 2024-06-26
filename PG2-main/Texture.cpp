#include "Texture.h"

#define MAX_LINE_SIZE 255

GLuint textureInit(const char* filepath)
{
	cv::Mat image = cv::imread(filepath, cv::IMREAD_UNCHANGED);
	if (image.empty()) {
		std::cerr << "no texture: " << filepath << std::endl;
		exit(1);
	}
	GLuint texture = tex_gen(image);

	return texture;
}

GLuint tex_gen(cv::Mat& image)
{
	if (image.empty()) {
		throw std::exception("Image empty?\n");
	}

	// select proper format for image source and destination
	GLint  img_internalformat;
	GLenum img_format;

	if (!GLEW_ARB_texture_compression) {
		throw std::exception("Compressed textures not supported??\n");
	}

	switch (image.channels()) {
	case 1:
		img_internalformat = GL_COMPRESSED_RED;
		img_format = GL_RED;
		break;
	case 3:
		img_internalformat = GL_COMPRESSED_RGB;
		img_format = GL_BGR;
		break;
	case 4:
		// if channels() = RGBA, we have Alpha channel, aka semitransparent texture
		img_internalformat = GL_COMPRESSED_RGBA;
		img_format = GL_BGRA;
		break;
	default:
		throw std::exception("Unsupported # of channels\n");
	}
	//bool transparent = (image.channels() == 4);

	// generate texture name
	GLuint texture;
	glGenTextures(1, &texture);

	// bind texture as active
	glBindTexture(GL_TEXTURE_2D, texture);

	// get list of supported formats
	GLint num_compressed_format;
	glGetIntegerv(GL_NUM_COMPRESSED_TEXTURE_FORMATS, &num_compressed_format);
	if (num_compressed_format > 0) {
		GLint compressed, internalformat, compressed_size;

		std::cout << "COMPRESSION supported, tot. available formats: " << num_compressed_format << std::endl;

		glHint(GL_TEXTURE_COMPRESSION_HINT, GL_NICEST);

		glTexImage2D(GL_TEXTURE_2D, 0, img_internalformat, image.cols, image.rows, 0, img_format, GL_UNSIGNED_BYTE, image.data);

		glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_COMPRESSED, &compressed);
		if (compressed == GL_TRUE)
		{
			glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_INTERNAL_FORMAT, &internalformat);
			glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_COMPRESSED_IMAGE_SIZE, &compressed_size);
			std::cout << "ORIGINAL: " << image.total() * image.elemSize() << " COMPRESSED: " << compressed_size << " INTERNAL FORMAT: " << internalformat << std::endl;
		}
	}
	else {
		throw std::exception("Compression supported, but no formats available?\n");
	}

	//
	// Set texture properties
	//

	// TILED texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// MIPMAP filtering + automatic MIPMAP generation - nicest, needs more memory. Notice: MIPMAP is only for image minifying.
	glGenerateMipmap(GL_TEXTURE_2D);  //Generate mipmaps now.
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // bilinear magnifying
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); //trilinear minifying

	return texture;
}

GLuint createSolidColorTexture(const glm::vec3& color) {
	GLuint texture_id;
	glGenTextures(1, &texture_id);
	glBindTexture(GL_TEXTURE_2D, texture_id);

	// Create a 1x1 pixel texture with the specified color
	unsigned char data[3] = {
		static_cast<unsigned char>(color.r * MAX_LINE_SIZE),
		static_cast<unsigned char>(color.g * MAX_LINE_SIZE),
		static_cast<unsigned char>(color.b * MAX_LINE_SIZE)
	};

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);

	return texture_id;
}
