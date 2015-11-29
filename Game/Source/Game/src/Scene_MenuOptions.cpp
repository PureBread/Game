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

Scene_MenuOptions::Scene_MenuOptions(Game * _game) :
	Scene_Menu(_game)
{
	// buttons
	VerticalLinearLayout * vl = new VerticalLinearLayout(uiLayer.world);
	vl->setBackgroundColour(1.f, 1.f, 1.f, 1.f);
	vl->background->mesh->pushTexture2D(MY_ResourceManager::scenario->getTexture("SCROLL_MENU")->texture);

	TextLabel * optionsText = new TextLabel(uiLayer.world, MY_ResourceManager::scenario->getFont("HURLY-BURLY")->font, textShader);
	optionsText->setRenderMode(kTEXTURE);

	MY_Button * fullscreenToggle = new MY_Button(uiLayer.world, MY_ResourceManager::scenario->getFont("HURLY-BURLY")->font, textShader, 202, 45);
	volumeText = new TextLabel(uiLayer.world, MY_ResourceManager::scenario->getFont("HURLY-BURLY")->font, textShader);
	volume = new Slider(uiLayer.world, NodeOpenAL::getListenerGain(), 0, 2.f);
	MY_Button * back = new MY_Button(uiLayer.world, MY_ResourceManager::scenario->getFont("HURLY-BURLY")->font, textShader, 202, 45);
	
	optionsText->setText(L"OPTIONS");
	if(sweet::fullscreen){
		fullscreenToggle->label->setText(L"WINDOWED");
	}else{
		fullscreenToggle->label->setText(L"FULLSCREEN");
	}
	volumeText->setText(L"VOLUME");
	volume->setWidth(0.5f);
	volume->setHeight(40);
	volume->setStepped(0.1);
	back->label->setText(L"BACK");
	
	
	fullscreenToggle->eventManager.addEventListener("click", [this, fullscreenToggle](sweet::Event *){
		game->toggleFullScreen();
		if(sweet::fullscreen){
			fullscreenToggle->label->setText(L"WINDOWED");
		}else{
			fullscreenToggle->label->setText(L"FULLSCREEN");
		}
	});
	back->eventManager.addEventListener("click", [this](sweet::Event *){
		game->switchScene("MENU_MAIN", false);
	});
	
	vl->addChild(optionsText);
	vl->addChild(fullscreenToggle);
	vl->addChild(volumeText);
	vl->addChild(volume);
	vl->addChild(back);
	
	vl->boxSizing = BoxSizing::kBORDER_BOX;
	vl->setMargin(0.33f);
	vl->setRationalWidth(1.f);
	vl->setRationalHeight(1.f);
	vl->verticalAlignment = kMIDDLE;
	vl->horizontalAlignment = kCENTER;
	uiLayer.addChild(vl);

	uiLayer.addMouseIndicator();
}

Scene_MenuOptions::~Scene_MenuOptions(){
}


void Scene_MenuOptions::update(Step * _step){
	std::wstringstream ss;
	ss << L"VOLUME: " << (volume->getValue()/2.f);
	volumeText->setText(ss.str());
	NodeOpenAL::setListenerGain(volume->getValue());
	Scene_Menu::update(_step);
}