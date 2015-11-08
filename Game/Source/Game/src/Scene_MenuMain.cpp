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

#include <PlayerResources.h>

#include <MY_Button.h>

Scene_MenuMain::Scene_MenuMain(Game * _game) :
	Scene_Menu(_game)
{
	// buttons
	VerticalLinearLayout * vl = new VerticalLinearLayout(uiLayer.world, this);
	vl->setBackgroundColour(1.f, 1.f, 1.f, 1.f);
	vl->background->mesh->pushTexture2D(MY_ResourceManager::scenario->getTexture("SCROLL_SMALL")->texture);

	MY_Button * b1 = new MY_Button(uiLayer.world, this, MY_ResourceManager::scenario->getFont("HURLY-BURLY")->font, textShader, 202, 45);
	MY_Button * b2 = new MY_Button(uiLayer.world, this, MY_ResourceManager::scenario->getFont("HURLY-BURLY")->font, textShader, 202, 45);
	MY_Button * b3 = new MY_Button(uiLayer.world, this, MY_ResourceManager::scenario->getFont("HURLY-BURLY")->font, textShader, 202, 45);
	MY_Button * b4 = new MY_Button(uiLayer.world, this, MY_ResourceManager::scenario->getFont("HURLY-BURLY")->font, textShader, 202, 45);
	b1->label->setText(L"new game");
	b2->label->setText(L"continue game");
	b3->label->setText(L"options");
	b4->label->setText(L"exit game");
	
	b1->onClickFunction = [this](){
		// TODO: reset game before switching to scene
		game->switchScene("MAIN", false);
	};
	b2->onClickFunction = [this](){
		game->switchScene("MAIN", false);
	};
	b3->onClickFunction = [this](){
		game->switchScene("MENU_OPTIONS", false);
	};
	b4->onClickFunction = [this](){
		game->exit();
	};
	
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

	addMouse();
}

Scene_MenuMain::~Scene_MenuMain(){
}

void Scene_MenuMain::update(Step * _step){
	Scene_Menu::update(_step);
}