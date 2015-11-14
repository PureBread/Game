#pragma once

#include <Scene.h>
#include <UILayer.h>
#include <BulletDebugDrawer.h>
#include <BulletWorld.h>

#include <ContinuousArtScroller.h>
#include <ShaderComponentReplace.h>
#include <shader/ShaderComponentMask.h>
#include <Slider.h>

#include <PlayerManager.h>
#include <Markers.h>
#include <ArtLayer.h>

class PerspectiveCamera;
class MousePerspectiveCamera;

class Box2DWorld;
class Box2DDebugDrawer;
class Box2DMeshEntity;
class MeshEntity;

class ShaderComponentHsv;

class Shader;
class RenderSurface;
class StandardFrameBuffer;
class Material;
class Sprite;

class PointLight;

class BulletMeshEntity;
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

	BulletWorld * bulletWorld;
	BulletDebugDrawer * debugDrawer;
	
	MousePerspectiveCamera * playerCam;
	
	MousePerspectiveCamera * debugCam;

	Box2DWorld * box2dWorld;
	Box2DDebugDrawer * box2dDebug;

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
	Markers markers;

	MY_Scene(Game * _game);
	~MY_Scene();
};