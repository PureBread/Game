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

Scene_Menu::Scene_Menu(Game * _game) :
	Scene(_game),
	screenSurfaceShader(new Shader("assets/engine basics/DefaultRenderSurface", false, true)),
	screenSurface(new RenderSurface(screenSurfaceShader)),
	screenFBO(new StandardFrameBuffer(true)),
	baseShader(new ComponentShaderBase(true)),
	textShader(new ComponentShaderText(true)),
	uiLayer(0,0,0,0)
{
	baseShader->addComponent(new ShaderComponentMVP(baseShader));
	baseShader->addComponent(new ShaderComponentTexture(baseShader));
	baseShader->compileShader();

	textShader->textComponent->setColor(glm::vec3(1.f));

	glm::uvec2 sd = sweet::getScreenDimensions();
	uiLayer.resize(0, sd.x, 0, sd.y);

	// texture
	/*NodeUI * bg = new NodeUI(uiLayer.world, this);
	bg->background->mesh->pushTexture2D(MY_ResourceManager::scenario->getTexture("MENU_MAIN")->texture);
	bg->setRationalWidth(1.f);
	bg->setRationalHeight(1.f);
	uiLayer.addChild(bg);*/
	
	bg1 = new Sprite(uiLayer.shader);
	bg2 = new Sprite(uiLayer.shader);
	bg3 = new Sprite(uiLayer.shader);
	bg4 = new Sprite(uiLayer.shader);
	bg5 = new Sprite(uiLayer.shader);
	bg1->mesh->pushTexture2D(MY_ResourceManager::scenario->getTexture("MENU_BG1")->texture);
	bg2->mesh->pushTexture2D(MY_ResourceManager::scenario->getTexture("MENU_BG2")->texture);
	bg3->mesh->pushTexture2D(MY_ResourceManager::scenario->getTexture("MENU_BG3")->texture);
	bg4->mesh->pushTexture2D(MY_ResourceManager::scenario->getTexture("MENU_BG4")->texture);
	bg5->mesh->pushTexture2D(MY_ResourceManager::scenario->getTexture("MENU_BG5")->texture);
	
	NodeUI * bgNode = new NodeUI(uiLayer.world);
	uiLayer.addChild(bgNode);
	bgNode->childTransform->addChild(bg5);
	bgNode->childTransform->addChild(bg4);
	bgNode->childTransform->addChild(bg3);
	bgNode->childTransform->addChild(bg2);
	bgNode->childTransform->addChild(bg1);


	// title text
	TextLabel * title = new TextLabel(uiLayer.world, MY_ResourceManager::scenario->getFont("HURLY-BURLY_BIG")->font, textShader);
	title->setText(L"LLAMMIGRATION");
	title->setRationalWidth(1.f);
	title->setMarginBottom(0.9f);
	title->horizontalAlignment = kCENTER;
	uiLayer.addChild(title);
}

Scene_Menu::~Scene_Menu(){
	deleteChildTransform();
	baseShader->safeDelete();
	textShader->safeDelete();

	screenSurface->safeDelete();
	//screenSurfaceShader->safeDelete();
	screenFBO->safeDelete();
}


void Scene_Menu::update(Step * _step){
	glm::uvec2 sd = sweet::getScreenDimensions();
	uiLayer.resize(0, sd.x, 0, sd.y);

	mouseIndicator->firstParent()->translate(mouse->mouseX(), mouse->mouseY(), 0, false);
	
	bg5->firstParent()->scale(sd.x*1.02f, sd.y*1.02f, 1, false);
	bg4->firstParent()->scale(sd.x*1.04f, sd.y*1.04f, 1, false);
	bg3->firstParent()->scale(sd.x*1.06f, sd.y*1.06f, 1, false);
	bg2->firstParent()->scale(sd.x*1.08f, sd.y*1.08f, 1, false);
	bg1->firstParent()->scale(sd.x*1.10f, sd.y*1.10f, 1, false);
	
	float x = mouse->mouseX() - sd.x*0.5f;
	float y = mouse->mouseY() - sd.y*0.5f;

	bg5->firstParent()->translate(x*0.02f + sd.x*0.5f, y*0.02f + sd.y*0.5f, 0, false);
	bg4->firstParent()->translate(x*0.04f + sd.x*0.5f, y*0.04f + sd.y*0.5f, 0, false);
	bg3->firstParent()->translate(x*0.06f + sd.x*0.5f, y*0.06f + sd.y*0.5f, 0, false);
	bg2->firstParent()->translate(x*0.08f + sd.x*0.5f, y*0.08f + sd.y*0.5f, 0, false);
	bg1->firstParent()->translate(x*0.10f + sd.x*0.5f, y*0.10f + sd.y*0.5f, 0, false);

	if(keyboard->keyJustDown(GLFW_KEY_F12)){
		game->takeScreenshot();
	}

	if (keyboard->keyJustDown(GLFW_KEY_2)){
		Transform::drawTransforms = !Transform::drawTransforms;
	}
	
	uiLayer.update(_step);
	Scene::update(_step);
}

void Scene_Menu::render(sweet::MatrixStack * _matrixStack, RenderOptions * _renderOptions){
	screenFBO->resize(game->viewPortWidth, game->viewPortHeight);
	//Bind frameBuffer
	screenFBO->bindFrameBuffer();
	//render the scene to the buffer
	
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	_renderOptions->clearColour[0] = 44.f/255.f;
	_renderOptions->clearColour[1] = 180.f/255.f;
	_renderOptions->clearColour[2] = 180.f/255.f;
	_renderOptions->depthEnabled = false;
	_renderOptions->clear();
	Scene::render(_matrixStack, _renderOptions);

	//Render the buffer to the render surface
	screenSurface->render(screenFBO->getTextureId());
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	uiLayer.render(_matrixStack, _renderOptions);
}

void Scene_Menu::load(){
	Scene::load();	

	screenSurface->load();
	screenFBO->load();
	uiLayer.load();
}

void Scene_Menu::unload(){
	uiLayer.unload();
	screenFBO->unload();
	screenSurface->unload();

	Scene::unload();	
}

void Scene_Menu::addMouse(){
	// mouse cursor
	mouseIndicator = new Sprite();
	uiLayer.childTransform->addChild(mouseIndicator);
	mouseIndicator->mesh->pushTexture2D(MY_ResourceManager::scenario->getTexture("CURSOR")->texture);
	mouseIndicator->parents.at(0)->scale(32, 32, 1);
	mouseIndicator->mesh->scaleModeMag = GL_NEAREST;
	mouseIndicator->mesh->scaleModeMin = GL_NEAREST;

	for(unsigned long int i = 0; i < mouseIndicator->mesh->vertices.size(); ++i){
		mouseIndicator->mesh->vertices[i].x += 0.5f;
		mouseIndicator->mesh->vertices[i].y -= 0.5f;
	}

	mouseIndicator->mesh->dirty = true;
	mouseIndicator->setShader(uiLayer.shader, true);
}