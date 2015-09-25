#pragma once

#include <MY_Scene.h>
#include <MY_ResourceManager.h>

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









#include <VCam.h>

MY_Scene::MY_Scene(Game * _game) :
	Scene(_game),
	screenSurfaceShader(new Shader("assets/engine basics/DefaultRenderSurface", false, true)),
	screenSurface(new RenderSurface(screenSurfaceShader)),
	screenFBO(new StandardFrameBuffer(true)),
	baseShader(new ComponentShaderBase(true)),
	characterShader(new ComponentShaderBase(true)),
	textShader(new ComponentShaderText(true)),
	debugDrawer(nullptr),
	uiLayer(this, 0,0,0,0),
	box2dWorld(new Box2DWorld(b2Vec2(0.f, -10.0f))),
	box2dDebug(new Box2DDebugDrawer(box2dWorld)),
	progress(0),
	speed(0)
{
	baseShader->addComponent(new ShaderComponentMVP(baseShader));
	//baseShader->addComponent(new ShaderComponentDiffuse(baseShader));
	baseShader->addComponent(new ShaderComponentTexture(baseShader));
	baseShader->compileShader();

	textShader->textComponent->setColor(glm::vec3(0.0f, 0.0f, 0.0f));


	// remove initial camera
	childTransform->removeChild(cameras.at(0)->parents.at(0));
	delete cameras.at(0)->parents.at(0);
	cameras.pop_back();

	//Set up debug camera
	debugCam = new MousePerspectiveCamera();
	cameras.push_back(debugCam);
	childTransform->addChild(debugCam);
	debugCam->farClip = 1000.f;
	debugCam->childTransform->rotate(90, 0, 1, 0, kWORLD);
	debugCam->parents.at(0)->translate(5.0f, 1.5f, 22.5f);
	debugCam->yaw = 90.0f;
	debugCam->pitch = -10.0f;
	debugCam->speed = 1;

	activeCamera = debugCam;

	//
	glm::uvec2 sd = sweet::getScreenDimensions();
	uiLayer.resize(0, sd.x, 0, sd.y);

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

	childTransform->addChild(box2dDebug, false);
	box2dDebug->drawing = true;
	box2dWorld->b2world->SetDebugDraw(box2dDebug);
	box2dDebug->AppendFlags(b2Draw::e_shapeBit);
	box2dDebug->AppendFlags(b2Draw::e_centerOfMassBit);
	box2dDebug->AppendFlags(b2Draw::e_jointBit);





	/** GAME STUFF **/
	VCam * cam = new VCam(1920, 1080/*, 0, 0*/);
	//OrthographicCamera * cam = new OrthographicCamera(-1920/2, 1920/2, -1080/2, 1080/2, -1000, 1000);
	cameras.push_back(cam);
	childTransform->addChild(cam);
	//cam->childTransform->rotate(90, 0, 1, 0, kWORLD);
	cam->parents.at(0)->translate(0, -25, -10.f);
	//cam->yaw = 90.0f;
	//cam->pitch = -10.0f;

	PointLight * l = new PointLight(glm::vec3(1, 1, 1), 1.f, 0.1f, 0.1f);
	lights.push_back(l);
	childTransform->addChild(l)->translate(0, 15, 0);

	for(unsigned long int i = 5; i > 0; --i){
		std::stringstream ss;
		ss << "BG" << i;
		MeshEntity * bg = new MeshEntity(MeshFactory::getPlaneMesh(192/2.f, 108/2.f), baseShader);
		bgLayers.push_back(bg);
		bg->mesh->textures.push_back(MY_ResourceManager::scenario->getTexture(ss.str())->texture);
		bg->mesh->scaleModeMag = GL_NEAREST;
		bg->mesh->scaleModeMin = GL_NEAREST;
		bg->childTransform->translate(0,0,i*5);
		childTransform->addChild(bg);
	}/*for(unsigned long int i = 5; i > 0; --i){
		childTransform->addChild(bgLayers.at(i-1));
	}*/

	/*for (unsigned long int i = 0; i < 250; ++i){
		MeshEntity * cube = new MeshEntity(MeshFactory::getCubeMesh(sweet::NumberUtils::randomFloat(5, 25)), baseShader);
		childTransform->addChild(cube)->translate(sweet::NumberUtils::randomFloat(-100, 100), sweet::NumberUtils::randomFloat(-100, 100), sweet::NumberUtils::randomFloat(-100, 100));
		cube->mesh->textures.push_back(MY_ResourceManager::scenario->getTexture("LOGO")->texture);
	}*/
}

MY_Scene::~MY_Scene(){
	deleteChildTransform();
	baseShader->safeDelete();
	characterShader->safeDelete();
	textShader->safeDelete();

	screenSurface->safeDelete();
	//screenSurfaceShader->safeDelete();
	screenFBO->safeDelete();
}


void MY_Scene::update(Step * _step){
	for(unsigned long int i = 0; i < bgLayers.size(); ++i){
		bgLayers.at(i)->firstParent()->translate(progress*((float)i/bgLayers.size() + 0.5f), 0, 0, false);
	}


	box2dWorld->update(_step);

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

	float camSpeed = 1;
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

	if (keyboard->keyJustDown(GLFW_KEY_A)){
		speed += 0.1;
	}if (keyboard->keyJustDown(GLFW_KEY_D)){
		speed -= 0.1;
	}
	progress += speed;

	debugCam->update(_step);

	Scene::update(_step);
}

void MY_Scene::render(sweet::MatrixStack * _matrixStack, RenderOptions * _renderOptions){
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	clearColor[0] = 1;
	clear();
	screenFBO->resize(game->viewPortWidth, game->viewPortHeight);
	//Bind frameBuffer
	screenFBO->bindFrameBuffer();
	//render the scene to the buffer
	
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	Scene::render(_matrixStack, _renderOptions);

	//Render the buffer to the render surface
	screenSurface->render(screenFBO->getTextureId());
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	uiLayer.render(_matrixStack, _renderOptions);
}

void MY_Scene::load(){
	Scene::load();	

	screenSurface->load();
	screenFBO->load();
	uiLayer.load();
}

void MY_Scene::unload(){
	uiLayer.unload();
	screenFBO->unload();
	screenSurface->unload();

	Scene::unload();	
}