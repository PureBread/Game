#pragma once 

#include <shader\ShaderComponent.h>

#include <glm\glm.hpp>

class ShaderComponentReplace : public ShaderComponent{
private:
	glm::vec3 white, black;
	GLint whiteLoc, blackLoc;
public:
	glm::vec3 getWhite();
	glm::vec3 getBlack();
	void setWhite(glm::vec3 _white);
	void setBlack(glm::vec3 _black);
	void setBoth(glm::vec3 _white, glm::vec3 _black);

	// all values are added to the rgb of the fragColor (therefore, changing the colour of a standard white vertex requires these arguments to be negative)
	ShaderComponentReplace(Shader * _shader, glm::vec3 _white = glm::vec3(1.f), glm::vec3 _black = glm::vec3(0.f));
	~ShaderComponentReplace();
	
	std::string getVertexVariablesString() override;
	std::string getFragmentVariablesString() override;
	std::string getVertexBodyString() override;
	std::string getFragmentBodyString() override;
	std::string getOutColorMod() override;
	void load() override;
	void configureUniforms(sweet::MatrixStack* _matrixStack, RenderOptions* _renderOption, NodeRenderable* _nodeRenderable) override;
};