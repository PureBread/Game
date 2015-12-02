#pragma once

#include <Scene_MenuInstructions.h>

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
#include <Box2DWorld.h>
#include <Box2DDebugDrawer.h>

#include <RenderOptions.h>

#include <MY_Button.h>
#include <sweet/UI.h>

Scene_MenuInstructions::Scene_MenuInstructions(Game * _game) :
	Scene_Menu(_game)
{
	// buttons
	VerticalLinearLayout * vl = new VerticalLinearLayout(uiLayer->world);
	vl->setRenderMode(kTEXTURE);
	vl->setBackgroundColour(1.f, 1.f, 1.f, 1.f);
	vl->background->mesh->pushTexture2D(MY_ResourceManager::scenario->getTexture("SCROLL_MENU")->texture);
	
	TextLabel * titleText = new TextLabel(uiLayer->world, MY_ResourceManager::scenario->getFont("HURLY-BURLY")->font, textShader);
	TextArea * instructionsText = new TextArea(uiLayer->world, MY_ResourceManager::scenario->getFont("HURLY-BURLY")->font, textShader, 1.f);

	MY_Button * back = new MY_Button(uiLayer->world, MY_ResourceManager::scenario->getFont("HURLY-BURLY")->font, textShader, 3);
	
	titleText->setText(L"INSTRUCTIONS");
	titleText->horizontalAlignment = kCENTER;

	instructionsText->setText(L"Bacon ipsum dolor amet anim occaecat pork loin.");
	instructionsText->horizontalAlignment = kCENTER;
	instructionsText->setHeight(0.5f);
	back->label->setText(L"BACK");
	
	
	back->eventManager.addEventListener("click", [this](sweet::Event *){
		game->switchScene("MENU_MAIN", false);
	});
	
	vl->addChild(titleText);
	vl->addChild(instructionsText);
	vl->addChild(back);
	
	vl->boxSizing = BoxSizing::kBORDER_BOX;
	vl->setMargin(0.33f);
	vl->setRationalWidth(1.f);
	vl->setRationalHeight(1.f);
	vl->verticalAlignment = kMIDDLE;
	vl->horizontalAlignment = kCENTER;
	uiLayer->addChild(vl);

	uiLayer->addMouseIndicator();
}

Scene_MenuInstructions::~Scene_MenuInstructions(){
}


void Scene_MenuInstructions::update(Step * _step){
	Scene_Menu::update(_step);
}