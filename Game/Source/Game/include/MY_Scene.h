#pragma once

#include <Scene.h>
#include <UILayer.h>

#include <ContinuousArtScroller.h>
#include <ShaderComponentReplace.h>
#include <shader/ShaderComponentMask.h>
#include <Slider.h>

#include <PlayerManager.h>
#include <ArtLayer.h>

class PerspectiveCamera;
class MousePerspectiveCamera;

class MeshEntity;

class ShaderComponentHsv;

class Shader;
class RenderSurface;
class StandardFrameBuffer;
class Material;
class Sprite;

class PointLight;

class ComponentShaderText;

class MY_Scene : public Scene{
public:
	Shader * screenSurfaceShader;
	RenderSurface * screenSurface;
	StandardFrameBuffer * screenFBO;
	
	ComponentShaderBase * baseShader;
	ComponentShaderBase * replaceShader;
	ShaderComponentReplace * replaceShaderComponent;
	ComponentShaderText * textShader;

	//ComponentShaderBase * maskShader;
	//ShaderComponentMask * maskComponent;
	
	MousePerspectiveCamera * playerCam;
	
	MousePerspectiveCamera * debugCam;

	PlayerManager manager;
	Event * currentEvent;

	float speed;
	float progress;
	
	std::vector<ContinuousArtScroller *> bgLayers;
	
	ArtLayer * layerSky;
	MeshEntity * layerSkyMesh;
	ContinuousArtScroller * layerBgDetail;
	ContinuousArtScroller * layerBg;
	ContinuousArtScroller * layerLlamas;
	ContinuousArtScroller * layerFg;
	ContinuousArtScroller * layerFgDetail;

	virtual void update(Step * _step) override;
	virtual void render(sweet::MatrixStack * _matrixStack, RenderOptions * _renderOptions) override;
	
	virtual void load() override;
	virtual void unload() override;

	UILayer uiLayer;

	MY_Scene(Game * _game);
	~MY_Scene();
};