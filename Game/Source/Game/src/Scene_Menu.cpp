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

Scene_Menu::Scene_Menu(MY_Game * _game) :
	Scene(_game),
	baseShader(new ComponentShaderBase(true)),
	textShader(new ComponentShaderText(true)),
	uiLayer(new UILayer(0,0,0,0))
{
	baseShader->addComponent(new ShaderComponentMVP(baseShader));
	baseShader->addComponent(new ShaderComponentTexture(baseShader));
	baseShader->compileShader();

	textShader->setColor(0,0,0);

	glm::uvec2 sd = sweet::getWindowDimensions();
	uiLayer->resize(0, sd.x, 0, sd.y);

	// texture
	/*NodeUI * bg = new NodeUI(uiLayer->world, this);
	bg->background->mesh->pushTexture2D(MY_ResourceManager::scenario->getTexture("MENU_MAIN")->texture);
	bg->setRationalWidth(1.f);
	bg->setRationalHeight(1.f);
	uiLayer->addChild(bg);*/
	
	bg1 = new Sprite(uiLayer->shader);
	bg2 = new Sprite(uiLayer->shader);
	bg3 = new Sprite(uiLayer->shader);
	bg4 = new Sprite(uiLayer->shader);
	bg5 = new Sprite(uiLayer->shader);
	bg1->mesh->pushTexture2D(MY_ResourceManager::scenario->getTexture("MENU_BG1")->texture);
	bg2->mesh->pushTexture2D(MY_ResourceManager::scenario->getTexture("MENU_BG2")->texture);
	bg3->mesh->pushTexture2D(MY_ResourceManager::scenario->getTexture("MENU_BG3")->texture);
	bg4->mesh->pushTexture2D(MY_ResourceManager::scenario->getTexture("MENU_BG4")->texture);
	bg5->mesh->pushTexture2D(MY_ResourceManager::scenario->getTexture("MENU_BG5")->texture);
	bg1->mesh->setScaleMode(GL_NEAREST);
	bg2->mesh->setScaleMode(GL_NEAREST);
	bg3->mesh->setScaleMode(GL_NEAREST);
	bg4->mesh->setScaleMode(GL_NEAREST);
	bg5->mesh->setScaleMode(GL_NEAREST);
	
	// need to use a NodeUI as a container so that the bg goes behind the menu stuff
	NodeUI * bgNode = new NodeUI(uiLayer->world);
	uiLayer->addChild(bgNode);
	bgNode->childTransform->addChild(bg5);
	bgNode->childTransform->addChild(bg4);
	bgNode->childTransform->addChild(bg3);
	bgNode->childTransform->addChild(bg2);
	bgNode->childTransform->addChild(bg1);


	// title text
	TextLabel * title = new TextLabel(uiLayer->world, MY_ResourceManager::scenario->getFont("HURLY-BURLY_BIG")->font, textShader);
	title->setText(L"LLAMMIGRATION");
	title->setRationalWidth(1.f);
	title->setMarginBottom(0.9f);
	title->horizontalAlignment = kCENTER;
	uiLayer->addChild(title);

	// reference counting for member variables
	++baseShader->referenceCount;
	++textShader->referenceCount;
}

Scene_Menu::~Scene_Menu(){
	delete uiLayer;
	deleteChildTransform();
	
	// auto-delete member variables
	baseShader->decrementAndDelete();
	textShader->decrementAndDelete();
}


void Scene_Menu::update(Step * _step){
	glm::uvec2 sd = sweet::getWindowDimensions();
	uiLayer->resize(0, sd.x, 0, sd.y);
	
	bg5->firstParent()->scale(sd.x*1.20f, sd.y*1.20f, 1.f, false);
	bg4->firstParent()->scale(sd.x*1.22f, sd.y*1.22f, 1.f, false);
	bg3->firstParent()->scale(sd.x*1.24f, sd.y*1.24f, 1.f, false);
	bg2->firstParent()->scale(sd.x*1.26f, sd.y*1.26f, 1.f, false);
	bg1->firstParent()->scale(sd.x*1.28f, sd.y*1.28f, 1.f, false);
	
	float x = mouse->mouseX() - sd.x*0.5f + glm::sin(_step->time*0.5)*500;
	float y = mouse->mouseY() - sd.y*0.5f + glm::sin(_step->time*0.25)*500;

	bg5->firstParent()->translate(x*0.02f + sd.x*0.5f, y*0.02f + sd.y*0.5f, 0, false);
	bg4->firstParent()->translate(x*0.04f + sd.x*0.5f, y*0.04f + sd.y*0.5f, 0, false);
	bg3->firstParent()->translate(x*0.06f + sd.x*0.5f, y*0.06f + sd.y*0.5f, 0, false);
	bg2->firstParent()->translate(x*0.08f + sd.x*0.5f, y*0.08f + sd.y*0.5f, 0, false);
	bg1->firstParent()->translate(x*0.10f + sd.x*0.5f, y*0.10f + sd.y*0.5f, 0, false);


#ifdef _DEBUG
	if(keyboard->keyJustDown(GLFW_KEY_F11)){
		game->toggleFullScreen();
	}

	if (keyboard->keyJustDown(GLFW_KEY_2)){
		Transform::drawTransforms = !Transform::drawTransforms;
	}
#endif
	
	uiLayer->update(_step);
	Scene::update(_step);
}

void Scene_Menu::render(sweet::MatrixStack * _matrixStack, RenderOptions * _renderOptions){
	_renderOptions->setClearColour(44.f/255.f, 180.f/255.f, 180.f/255.f, 1.f);
	_renderOptions->depthEnabled = false;
	_renderOptions->clear();
	Scene::render(_matrixStack, _renderOptions);
	uiLayer->render(_matrixStack, _renderOptions);
}

void Scene_Menu::load(){
	Scene::load();

	baseShader->load();
	textShader->load();
	uiLayer->load();
}

void Scene_Menu::unload(){
	uiLayer->unload();

	textShader->unload();
	baseShader->unload();
	Scene::unload();	
}