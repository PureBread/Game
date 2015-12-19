#pragma once

#include <Scene_MenuOptions.h>

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

Scene_MenuOptions::Scene_MenuOptions(MY_Game * _game) :
	Scene_Menu(_game),
	newVolume(-1)
{
	// buttons
	VerticalLinearLayout * vl = new VerticalLinearLayout(uiLayer->world);
	vl->setRenderMode(kTEXTURE);
	vl->setBackgroundColour(1.f, 1.f, 1.f, 1.f);
	vl->background->mesh->pushTexture2D(MY_ResourceManager::scenario->getTexture("SCROLL_MENU")->texture);

	TextLabel * titleText = new TextLabel(uiLayer->world, MY_ResourceManager::scenario->getFont("HURLY-BURLY")->font, textShader);
	MY_Button * casualToggle = new MY_Button(uiLayer->world, MY_ResourceManager::scenario->getFont("HURLY-BURLY")->font, 3);
	volumeText = new TextLabel(uiLayer->world, MY_ResourceManager::scenario->getFont("HURLY-BURLY")->font, textShader);
	volume = new Slider(uiLayer->world, NodeOpenAL::getListenerGain(), 0, 2.f);
	MY_Button * back = new MY_Button(uiLayer->world, MY_ResourceManager::scenario->getFont("HURLY-BURLY")->font, 3);
	
	titleText->setText(L"OPTIONS");
	titleText->horizontalAlignment = kCENTER;
	if(_game->casualMode){
		casualToggle->setLabel("CASUAL");
	}else{
		casualToggle->setLabel("CHALLENGE");
	}
	volumeText->setText(L"VOLUME");
	volumeText->horizontalAlignment = kCENTER;
	volumeText->setRationalWidth(1.f);

	volume->setRationalWidth(0.5f);
	volume->setHeight(MY_ResourceManager::scenario->getFont("HURLY-BURLY")->font->getLineHeight());
	volume->setStepped(0.2);
	volume->eventManager.addEventListener("change", [this](sweet::Event * _event){
		newVolume = volume->getValue();
	});


	back->setLabel("BACK");
	
	casualToggle->eventManager.addEventListener("click", [this, _game, casualToggle](sweet::Event *){
		_game->casualMode = !_game->casualMode;
		if(_game->casualMode){
			casualToggle->setLabel("CASUAL");
		}else{
			casualToggle->setLabel("CHALLENGE");
		}
	});
	back->eventManager.addEventListener("click", [this](sweet::Event *){
		game->switchScene("MENU_MAIN", true);
	});
	
	vl->addChild(titleText);
	vl->addChild(casualToggle);
	vl->addChild(volumeText);
	vl->addChild(volume);
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

Scene_MenuOptions::~Scene_MenuOptions(){
}


void Scene_MenuOptions::update(Step * _step){
	if(newVolume >= 0){
		std::wstringstream ss;
		ss << L"VOLUME: " << newVolume*5.f;
		volumeText->setText(ss.str());
		NodeOpenAL::setListenerGain(newVolume);
		newVolume = -1;
	}
	Scene_Menu::update(_step);
}