#pragma once

#include <ArtLayer.h>

#include <NumberUtils.h>

ArtLayer::ArtLayer(ShaderComponentReplace * _replaceComponent) :
	replaceComponent(_replaceComponent),
	colorReplaceBlack(0),
	colorReplaceWhite(1)
{
}

void ArtLayer::render(sweet::MatrixStack * _matrixStack, RenderOptions * _renderOptions){
	replaceComponent->setWhite(colorReplaceWhite);
	replaceComponent->setBlack(colorReplaceBlack);

	Entity::render(_matrixStack, _renderOptions);
}