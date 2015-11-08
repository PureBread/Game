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
	Scene(_game),
	screenSurfaceShader(new Shader("assets/engine basics/DefaultRenderSurface", false, true)),
	screenSurface(new RenderSurface(screenSurfaceShader)),
	screenFBO(new StandardFrameBuffer(true)),
	baseShader(new ComponentShaderBase(true)),
	textShader(new ComponentShaderText(true)),
	uiLayer(this, 0,0,0,0)
{
	baseShader->addComponent(new ShaderComponentMVP(baseShader));
	baseShader->addComponent(new ShaderComponentTexture(baseShader));
	baseShader->compileShader();

	textShader->textComponent->setColor(glm::vec3(1.f));

	// remove initial camera
	/*childTransform->removeChild(cameras.at(0)->parents.at(0));
	delete cameras.at(0)->parents.at(0);
	cameras.pop_back();*/

	//
	glm::uvec2 sd = sweet::getScreenDimensions();
	uiLayer.resize(0, sd.x, 0, sd.y);

	// texture
	NodeUI * bg = new NodeUI(uiLayer.world, this);
	bg->background->mesh->pushTexture2D(MY_ResourceManager::scenario->getTexture("MENU_MAIN")->texture);
	bg->setRationalWidth(1.f);
	bg->setRationalHeight(1.f);
	uiLayer.addChild(bg);

	TextLabel * title = new TextLabel(uiLayer.world, this, MY_ResourceManager::scenario->getFont("HURLY-BURLY")->font, textShader);
	title->setText(L"LLAMMIGRATION");
	title->setRationalWidth(1.f);
	title->setMarginBottom(0.8f);
	title->horizontalAlignment = kCENTER;
	uiLayer.addChild(title);

	VerticalLinearLayout * vl = new VerticalLinearLayout(uiLayer.world, this);
	MY_Button * t1 = new MY_Button(uiLayer.world, this, MY_ResourceManager::scenario->getFont("HURLY-BURLY")->font, textShader, 200, 50);
	MY_Button * t2 = new MY_Button(uiLayer.world, this, MY_ResourceManager::scenario->getFont("HURLY-BURLY")->font, textShader, 200, 50);
	MY_Button * t3 = new MY_Button(uiLayer.world, this, MY_ResourceManager::scenario->getFont("HURLY-BURLY")->font, textShader, 200, 50);
	MY_Button * t4 = new MY_Button(uiLayer.world, this, MY_ResourceManager::scenario->getFont("HURLY-BURLY")->font, textShader, 200, 50);
	t1->label->setText(L"New Game");
	t2->label->setText(L"t2");
	t3->label->setText(L"t3");
	t4->label->setText(L"Exit Game");
	
	t1->onClickFunction = [this](){
		game->switchScene("MAIN", false);
	};
	t4->onClickFunction = [this](){
		game->exit();
	};
	
	vl->addChild(t1);
	vl->addChild(t2);
	vl->addChild(t3);
	vl->addChild(t4);
	
	vl->setRationalWidth(1.f);
	vl->setRationalHeight(1.f);
	vl->verticalAlignment = kMIDDLE;
	vl->horizontalAlignment = kCENTER;
	uiLayer.addChild(vl);

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

Scene_MenuMain::~Scene_MenuMain(){
	deleteChildTransform();
	baseShader->safeDelete();
	textShader->safeDelete();

	screenSurface->safeDelete();
	//screenSurfaceShader->safeDelete();
	screenFBO->safeDelete();
}


void Scene_MenuMain::update(Step * _step){
	glm::uvec2 sd = sweet::getScreenDimensions();
	uiLayer.resize(0, sd.x, 0, sd.y);

	mouseIndicator->parents.at(0)->translate(mouse->mouseX(), mouse->mouseY(), 0, false);
	
	if(keyboard->keyJustDown(GLFW_KEY_F12)){
		//game->toggleFullScreen();
		game->takeScreenshot();
	}

	if (keyboard->keyJustDown(GLFW_KEY_1)){
		cycleCamera();
	}if (keyboard->keyJustDown(GLFW_KEY_2)){
		Transform::drawTransforms = !Transform::drawTransforms;
	}

	float camSpeed = 0.3;
	MousePerspectiveCamera * cam = dynamic_cast<MousePerspectiveCamera *>(activeCamera);
	if(cam != nullptr){
		camSpeed = cam->speed;
	}
	// camera controls
	if (keyboard->keyDown(GLFW_KEY_UP)){
		activeCamera->parents.at(0)->translate((activeCamera->upVectorRotated) * camSpeed);
	}
	if (keyboard->keyDown(GLFW_KEY_DOWN)){
		activeCamera->parents.at(0)->translate((activeCamera->upVectorRotated) * -camSpeed);
	}
	if (keyboard->keyDown(GLFW_KEY_LEFT)){
		activeCamera->parents.at(0)->translate((activeCamera->rightVectorRotated) * -camSpeed);
	}
	if (keyboard->keyDown(GLFW_KEY_RIGHT)){
		activeCamera->parents.at(0)->translate((activeCamera->rightVectorRotated) * camSpeed);
	}

	Scene::update(_step);
	uiLayer.update(_step);
}

void Scene_MenuMain::render(sweet::MatrixStack * _matrixStack, RenderOptions * _renderOptions){
	screenFBO->resize(game->viewPortWidth, game->viewPortHeight);
	//Bind frameBuffer
	screenFBO->bindFrameBuffer();
	//render the scene to the buffer
	
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	_renderOptions->clearColour[0] = 0.5;
	_renderOptions->depthEnabled = false;
	_renderOptions->clear();
	Scene::render(_matrixStack, _renderOptions);

	//Render the buffer to the render surface
	screenSurface->render(screenFBO->getTextureId());
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	uiLayer.render(_matrixStack, _renderOptions);
}

void Scene_MenuMain::load(){
	Scene::load();	

	screenSurface->load();
	screenFBO->load();
	uiLayer.load();
}

void Scene_MenuMain::unload(){
	uiLayer.unload();
	screenFBO->unload();
	screenSurface->unload();

	Scene::unload();	
}