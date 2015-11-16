#pragma once

#include <ShaderComponentReplace.h>
#include <shader\ShaderVariables.h>
#include <GLUtils.h>
#include <RenderOptions.h>
#include <shader\Shader.h>

ShaderComponentReplace::ShaderComponentReplace(Shader * _shader, glm::vec3 _white, glm::vec3 _black) :
	ShaderComponent(_shader),
	white(_white),
	black(_black),
	whiteLoc(-1),
	blackLoc(-1)
{
}

ShaderComponentReplace::~ShaderComponentReplace(){
}
	
std::string ShaderComponentReplace::getVertexVariablesString(){
	return DEFINE + "SHADER_COMPONENT_REPLACE" + ENDL;
}

std::string ShaderComponentReplace::getFragmentVariablesString(){
	return 
		DEFINE + "SHADER_COMPONENT_REPLACE" + ENDL +
		"uniform vec3 replaceWhite" + SEMI_ENDL + 
		"uniform vec3 replaceBlack" + SEMI_ENDL;
}

std::string ShaderComponentReplace::getVertexBodyString(){
	return "";
}

std::string ShaderComponentReplace::getFragmentBodyString(){
	return "";
}

std::string ShaderComponentReplace::getOutColorMod(){
	return
		"// discard transparent pixels" + ENDL +
		"if("+GL_OUT_OUT_COLOR+".a < 1){ discard; }" + ENDL +
		"// replace based on 0.5 threshold" + ENDL +
		"if("+GL_OUT_OUT_COLOR+".r > 0.5){" + 
			TAB + GL_OUT_OUT_COLOR + " = vec4(replaceWhite,1)" + SEMI_ENDL +
		"}else{" + 
			TAB + GL_OUT_OUT_COLOR + " = vec4(replaceBlack,1)" + SEMI_ENDL +
		"}" + ENDL;
}

void ShaderComponentReplace::load(){
	if(!loaded){
		whiteLoc = glGetUniformLocation(shader->getProgramId(), "replaceWhite");
		blackLoc = glGetUniformLocation(shader->getProgramId(), "replaceBlack");
	}
	ShaderComponent::load();
}

void ShaderComponentReplace::configureUniforms(sweet::MatrixStack * _matrixStack, RenderOptions * _renderOption, NodeRenderable * _nodeRenderable){
	glUniform3f(whiteLoc, white.x, white.y, white.z);
	glUniform3f(blackLoc, black.x, black.y, black.z);
}

void ShaderComponentReplace::setWhite(glm::vec3 _white){
	white = _white;
	makeDirty();
}
void ShaderComponentReplace::setBlack(glm::vec3 _black){
	black = _black;
	makeDirty();
}
void ShaderComponentReplace::setBoth(glm::vec3 _white, glm::vec3 _black){
	white = _white;
	black = _black;
	makeDirty();
}

glm::vec3 ShaderComponentReplace::getWhite(){
	return white;
}
glm::vec3 ShaderComponentReplace::getBlack(){
	return black;
}