#include "texture.h";

Texture::Texture(const char* image, const char* texType, GLuint slot)
{

	this->type = texType;

	// Set temporary setter variables
	int widthImg, heightImg, numColCh;

	// Tell to invert plotting plane to be upright
	stbi_set_flip_vertically_on_load(true);


	// Get a stream of binaries of the given image
	unsigned char* bytes = stbi_load(image, &widthImg, &heightImg, &numColCh, 0);

	// Create a texture asset and store in this object
	glGenTextures(1, &this->ID);

	// Activate the given slot where texture will reside
	glActiveTexture(GL_TEXTURE0 + slot);
	this->unit = slot;

	// Bind given image to this activated slot with newly allocated texture
	glBindTexture(GL_TEXTURE_2D, this->ID);

	// Tell way to interpolate binaries
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Tell way to handle repeats if there is some (only will happen [i think] when we set vertex values to higher val than 1
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);


	std::cout << "PASSED COLOR CHANNELS - " << widthImg << " " << heightImg <<  '\n';

	if (numColCh == 4) {

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthImg, heightImg, 0, GL_RGBA, GL_UNSIGNED_BYTE, bytes);

	}
	else if (numColCh == 3) {
		
		// Somehow spreading colors out as when lowering image size it works
		std::cout << "3 Num COLS" << '\n';
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthImg, heightImg, 0, GL_RGB, GL_UNSIGNED_BYTE, bytes);


	}
	else if (numColCh == 1) {

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, widthImg, heightImg, 0, GL_RED, GL_UNSIGNED_BYTE, bytes);

	}
	else {
		
		throw std::invalid_argument("Texture type recognition not worked");
	}

	// Create all varying sizes
	glGenerateMipmap(GL_TEXTURE_2D);

	// Remove the stored bytes
	stbi_image_free(bytes); // I think redunant [local variable]

	// Ground tex type now
	glBindTexture(GL_TEXTURE_2D, 0);

}


void Texture::texUnit(Shader shader, const char* uniform, GLuint unit)
{
	// Grab variable from shader prog
	GLuint texUni = glGetUniformLocation(shader.ID, uniform);

	shader.Activate();
	glUniform1i(texUni, unit);
	shader.Delete();

}


void Texture::Bind()
{
	// Activate texture for use
	glActiveTexture(GL_TEXTURE0 + this->unit);
	glBindTexture(GL_TEXTURE_2D, ID);

}


void Texture::Unbind()
{
	// Deactivate texture for use
	glBindTexture(GL_TEXTURE_2D, 0);

}


void Texture::Delete()
{

	glDeleteTextures(1, &this->ID);

}