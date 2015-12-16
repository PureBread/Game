#pragma once

#include <Scene.h>
#include <UILayer.h>
#include <BulletDebugDrawer.h>
#include <BulletWorld.h>

#include <ContinuousArtScroller.h>
#include <ShaderComponentReplace.h>
#include <MY_ResourceManager.h>
#include <MY_Game.h>

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

class Scene_Menu : public Scene{
public:
	Shader * screenSurfaceShader;
	RenderSurface * screenSurface;
	StandardFrameBuffer * screenFBO;
	
	ComponentShaderBase * baseShader;
	ComponentShaderText * textShader;
	
	Sprite * mouseIndicator;
	Sprite * bg1;
	Sprite * bg2;
	Sprite * bg3;
	Sprite * bg4;
	Sprite * bg5;

	virtual void update(Step * _step) override;
	virtual void render(sweet::MatrixStack * _matrixStack, RenderOptions * _renderOptions) override;
	
	virtual void load() override;
	virtual void unload() override;

	UILayer * uiLayer;

	Scene_Menu(MY_Game * _game);
	~Scene_Menu();
};