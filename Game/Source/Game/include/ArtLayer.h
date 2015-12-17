#pragma once

#include <Entity.h>
#include <ShaderComponentReplace.h>

class ArtLayer : public Entity{
public:
	glm::vec3 colorReplaceWhite;
	glm::vec3 colorReplaceBlack;

	ShaderComponentReplace * replaceComponent;
	
	ArtLayer(ShaderComponentReplace * _replaceComponent);
	~ArtLayer();

	
	virtual void render(sweet::MatrixStack * _matrixStack, RenderOptions * _renderOptions) override;
};