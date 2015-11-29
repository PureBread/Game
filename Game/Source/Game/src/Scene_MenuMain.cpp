#pragma once

#include <Scene_MenuMain.h>

#include <Game.h>
#include <MeshEntity.h>
#include <MeshInterface.h>
#include <MeshFactory.h>
#include <Resource.h>

#include <DirectionalLight.h>
#include <PointLight.h>
#include <SpotLight.h>

#include <shader\ComponentShaderBase.h>
#include <shader\ComponentShaderText.h>
#include <shader\ShaderComponentText.h>
#include <shader\ShaderComponentTexture.h>
#include <shader\ShaderComponentDiffuse.h>
#include <shader\ShaderComponentHsv.h>
#include <shader\ShaderComponentMVP.h>

#include <shader\ShaderComponentIndexedTexture.h>
#include <TextureColourTable.h>

#include <Box2DWorld.h>
#include <Box2DMeshEntity.h>
#include <Box2DDebugDrawer.h>

#include <MousePerspectiveCamera.h>
#include <FollowCamera.h>

#include <System.h>
#include <Mouse.h>
#include <Keyboard.h>
#include <GLFW\glfw3.h>

#include <RenderSurface.h>
#include <StandardFrameBuffer.h>
#include <NumberUtils.h>

#include <NodeBulletBody.h>
#include <BulletMeshEntity.h>
#include <TextArea.h>
#include <Box2DWorld.h>
#include <Box2DDebugDrawer.h>

#include <RenderOptions.h>

#include <MY_Button.h>
Scene_MenuMain::Scene_MenuMain(Game * _game) :
	Scene_Menu(_game)
{
	// buttons
	VerticalLinearLayout * vl = new VerticalLinearLayout(uiLayer.world);
	vl->setRenderMode(kTEXTURE);
	vl->setBackgroundColour(1.f, 1.f, 1.f, 1.f);
	vl->background->mesh->pushTexture2D(MY_ResourceManager::scenario->getTexture("SCROLL_MENU")->texture);

	MY_Button * b1 = new MY_Button(uiLayer.world, MY_ResourceManager::scenario->getFont("HURLY-BURLY")->font, textShader, 202, 45);
	b2 = new MY_Button(uiLayer.world, MY_ResourceManager::scenario->getFont("HURLY-BURLY")->font, textShader, 202, 45);
	MY_Button * b3 = new MY_Button(uiLayer.world, MY_ResourceManager::scenario->getFont("HURLY-BURLY")->font, textShader, 202, 45);
	MY_Button * b4 = new MY_Button(uiLayer.world, MY_ResourceManager::scenario->getFont("HURLY-BURLY")->font, textShader, 202, 45);
	b1->label->setText(L"NEW GAME");
	b2->label->setText(L"CONTINUE GAME");
	b3->label->setText(L"OPTIONS");
	b4->label->setText(L"EXIT GAME");
	
	b1->eventManager.addEventListener("click", [this](sweet::Event *){
		// TODO: reset game before switching to scene
		game->switchScene("MAIN", false);
	});
	b2->eventManager.addEventListener("click", [this](sweet::Event *){
		game->switchScene("MENU_INSTRUCTIONS", false);
	});
	b3->eventManager.addEventListener("click", [this](sweet::Event *){
		game->switchScene("MENU_OPTIONS", false);
	});
	b4->eventManager.addEventListener("click", [this](sweet::Event *){
		game->exit();
	});
	
	vl->addChild(b1);
	vl->addChild(b2);
	vl->addChild(b3);
	vl->addChild(b4);
	
	vl->boxSizing = BoxSizing::kBORDER_BOX;
	vl->setMargin(0.33f);
	vl->setRationalWidth(1.f);
	vl->setRationalHeight(1.f);
	vl->verticalAlignment = kMIDDLE;
	vl->horizontalAlignment = kCENTER;
	uiLayer.addChild(vl);
	
	vl->invalidateLayout();
	uiLayer.addMouseIndicator();
}

Scene_MenuMain::~Scene_MenuMain(){
}

void Scene_MenuMain::update(Step * _step){
	Scene_Menu::update(_step);
}