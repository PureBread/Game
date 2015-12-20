#pragma once

#include <Scene.h>
#include <UILayer.h>

#include <ContinuousArtScroller.h>
#include <ShaderComponentReplace.h>
#include <shader/ShaderComponentHsv.h>
#include <shader/ShaderComponentMask.h>
#include <Slider.h>

#include <PlayerManager.h>
#include <ArtLayer.h>
#include <UI_Event.h>
#include <UI_Controls.h>
#include <Timeout.h>
#include <MY_Game.h>

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
	ShaderComponentHsv * hsvShaderComponent;
	ComponentShaderText * textShader;
	
	PerspectiveCamera * playerCam;
	MousePerspectiveCamera * debugCam;

	PlayerManager manager;
	Event * currentEvent;
	UI_Event * uiEvent;
	UI_Controls * uiControls;
	Timeout * fadeTimeout;

	float speed;
	float progress;
	bool paused;
	
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

	UILayer * uiLayer;
	NodeUI * bubbleNode;

	MY_Scene(MY_Game * _game);
	~MY_Scene();
};