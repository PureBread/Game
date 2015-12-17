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

#include <PlayerManager.h>

#include <LevelPath.h>
#include <Llama.h>


#include <VCam.h>

MY_Scene::MY_Scene(MY_Game * _game) :
	Scene(_game),
	screenSurfaceShader(new Shader("assets/RenderSurface", false, true)),
	screenSurface(new RenderSurface(screenSurfaceShader)),
	screenFBO(new StandardFrameBuffer(true)),
	baseShader(new ComponentShaderBase(true)),
	replaceShader(new ComponentShaderBase(true)),
	replaceShaderComponent(new ShaderComponentReplace(replaceShader)),
	hsvShaderComponent(new ShaderComponentHsv(replaceShader, 0, 1, 0)),
	textShader(new ComponentShaderText(true)),
	uiLayer(new UILayer(0,0,0,0)),
	progress(0),
	speed(0),
	currentEvent(nullptr),
	manager(replaceShaderComponent),
	paused(true)
{
	baseShader->addComponent(new ShaderComponentMVP(baseShader));
	baseShader->addComponent(new ShaderComponentTexture(baseShader));
	baseShader->compileShader();

	replaceShader->addComponent(new ShaderComponentMVP(replaceShader));
	replaceShader->addComponent(new ShaderComponentTexture(replaceShader));
	replaceShader->addComponent(replaceShaderComponent);
	replaceShader->addComponent(hsvShaderComponent);
	replaceShader->compileShader();
	replaceShader->unload();
	replaceShader->load();
	
	textShader->setColor(0,0,0);

	screenSurface->uvEdgeMode = GL_CLAMP_TO_EDGE;
	screenSurface->load();


	//maskShader = new ComponentShaderBase(true);
	//maskShader->addComponent(new ShaderComponentMVP(maskShader));
	//maskShader->addComponent(new ShaderComponentTexture(maskShader));
	//maskComponent = new ShaderComponentMask(maskShader);
	//maskShader->addComponent(maskComponent);
	//maskShader->compileShader();


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

	//
	glm::uvec2 sd = sweet::getWindowDimensions();
	uiLayer->resize(0, sd.x, 0, sd.y);


	/** GAME STUFF **/

	// camera
	playerCam = new PerspectiveCamera();
	cameras.push_back(playerCam);
	childTransform->addChild(playerCam);
	playerCam->yaw = 90;
	playerCam->pitch = 0.520833313f;
	playerCam->firstParent()->translate((manager.statistics["progress"]-1)*50.f, -13.8184452f, 57.8584137f);
	playerCam->fieldOfView = 64.3750000;
	playerCam->interpolation = 1;
	activeCamera = playerCam;

	// art layers
	layerSky = new ArtLayer(replaceShaderComponent);
	layerBgDetail = new ContinuousArtScroller("BG5", replaceShader);
	layerBg = new ContinuousArtScroller("BG4", replaceShader);
	layerLlamas = new ContinuousArtScroller("BG3", replaceShader);
	layerFg = new ContinuousArtScroller("BG2", replaceShader);
	layerFgDetail = new ContinuousArtScroller("BG1", replaceShader);

	// level path
	/*MeshEntity * meshLines = new MeshEntity(manager.levelPath->getMesh(), baseShader);
	meshLines->meshTransform->scale(layerLlamas->imageCount, true);
	meshLines->meshTransform->translate(glm::vec3(-1.5f, -0.5f, 0));
	MeshEntity * meshPoints = new MeshEntity(manager.levelPath->getMesh(), baseShader);
	meshPoints->meshTransform->scale(layerLlamas->imageCount, true);
	meshPoints->meshTransform->translate(glm::vec3(-1.5f, -0.5f, 0));
	meshPoints->mesh->polygonalDrawMode = GL_POINTS;*/
	manager.levelPath->scaleVertices(layerLlamas->imageCount);
	manager.levelPath->childTransform->translate(-1.5f, -0.5f, 0);
	
	manager.addLlama(replaceShader, true);
	
	/*Sprite * moveThing = new Sprite(baseShader);
	moveThing->mesh->pushTexture2D(MY_ResourceManager::scenario->getTexture("LOGO")->texture);
	for (unsigned long int i = 0; i < moveThing->mesh->vertices.size(); ++i){
		moveThing->mesh->vertices.at(i).y += 0.5f;
	}
	manager.levelPath->moveThing = moveThing;
	manager.levelPath->moveThing->childTransform->scale(0.01f, 0.5f, 1.f);
	manager.levelPath->childTransform->addChild(moveThing);*/
	
	
	bgLayers.push_back(layerBgDetail);
	bgLayers.push_back(layerBg);
	bgLayers.push_back(layerLlamas);
	bgLayers.push_back(layerFg);
	bgLayers.push_back(layerFgDetail);

	childTransform->addChild(layerSky);
	for(unsigned long int i = 0; i < bgLayers.size(); ++i){
		childTransform->addChild(bgLayers.at(i))->translate(0, 0, i * 10);
		if(i == 2){
			bgLayers.at(i)->childTransform->addChild(manager.levelPath);
			//bgLayers.at(i)->childTransform->addChild(meshLines);
			//bgLayers.at(i)->childTransform->addChild(meshPoints);
		}
		bgLayers.at(i)->firstParent()->scale(50);
	}


	layerSkyMesh = new MeshEntity(MeshFactory::getPlaneMesh(), replaceShader);
	layerSky->childTransform->addChild(layerSkyMesh, false);
	layerSkyMesh->childTransform->translate(0,-50,0);
	layerSkyMesh->meshTransform->translate(0,0.5,0);
	layerSkyMesh->childTransform->scale(150);

	layerSkyMesh->mesh->pushTexture2D(MY_ResourceManager::scenario->getTexture("SKY")->texture);
	layerSkyMesh->mesh->scaleModeMag = layerSkyMesh->mesh->scaleModeMin = GL_NEAREST;

	manager.loadDefaults();

	
	uiControls = new UI_Controls(&manager, uiLayer->world, textShader);
	uiLayer->addChild(uiControls);


	uiEvent = new UI_Event(uiLayer->world, textShader);

	uiLayer->addChild(uiEvent);

	uiLayer->addMouseIndicator();

	fadeTimeout = new Timeout(5, [this](sweet::Event * _event){
		paused = false;
	});
	fadeTimeout->eventManager->addEventListener("progress", [&](sweet::Event * _event){
		hsvShaderComponent->setValue(_event->getFloatData("progress"));
	});
	fadeTimeout->start();
	childTransform->addChild(fadeTimeout, false);



	

	// win listener
	manager.globalEventManager.addEventListener("gameOver", [this](sweet::Event * _event){
		childTransform->removeChild(fadeTimeout);
		delete fadeTimeout;

		paused = true;
		fadeTimeout = new Timeout(2.5f, [this](sweet::Event * _event){
			game->switchScene("MENU_MAIN", true);
		});
		fadeTimeout->eventManager->addEventListener("progress", [this](sweet::Event * _event){
			hsvShaderComponent->setValue(1.f - _event->getFloatData("progress"));
		});
		fadeTimeout->start();
		childTransform->addChild(fadeTimeout, false);
	});

	// sound effect listener
	manager.globalEventManager.addEventListener("sfx", [this](sweet::Event * _event){
		currentEvent->scenario->getAudio(_event->getStringData("name"))->sound->play();
	});


	// reference counting for member variables
	++baseShader->referenceCount;
	++replaceShader->referenceCount;
	++textShader->referenceCount;

	++screenSurface->referenceCount;
	++screenSurfaceShader->referenceCount;
	++screenFBO->referenceCount;
}

MY_Scene::~MY_Scene(){
	delete uiLayer;
	deleteChildTransform();
	
	// auto-delete member variables
	baseShader->decrementAndDelete();
	replaceShader->decrementAndDelete();
	textShader->decrementAndDelete();

	screenSurface->decrementAndDelete();
	screenSurfaceShader->decrementAndDelete();
	screenFBO->decrementAndDelete();
}


void MY_Scene::update(Step * _step){
	
	// loss state check
	if(manager.statistics["herdSize"] == 0 && !paused){
		manager.globalEventManager.triggerEvent("gameOver");
	}

	// herd size checks
	while (_step->time > 1 && manager.levelPath->llamas.size()-1 < manager.statistics["herdSize"]){
		manager.addLlama(replaceShader);
	}
	while (_step->time > 1 && manager.levelPath->llamas.size()-1 > manager.statistics["herdSize"]){
		manager.levelPath->removeLlama();
	}
	std::wstringstream ss;
	ss << manager.statistics["herdSize"];
	uiControls->herdSize->setText(ss.str());



	if(keyboard->keyJustDown(GLFW_KEY_R)){
		manager.globalEventManager.triggerEvent("gameOver");
	}
	manager.globalEventManager.update(_step);

	// screen shader stuff
	if(keyboard->keyJustDown(GLFW_KEY_L)){
		screenSurfaceShader->unload();
		screenSurfaceShader->loadFromFile(screenSurfaceShader->vertSource, screenSurfaceShader->fragSource);
		screenSurfaceShader->load();
	}
	
	screenSurfaceShader->bindShader();
	GLint test = glGetUniformLocation(screenSurfaceShader->getProgramId(), "time");
	checkForGlError(0,__FILE__,__LINE__);
	if(test != -1){
		glUniform1f(test, (float)sweet::lastTimestamp);
		checkForGlError(0,__FILE__,__LINE__);
	}
	
	// main game update stuff
	if(!paused){
		if(currentEvent != nullptr){
			// if there is an ongoing event, the UI_Event popup will handle it
			// once the event is finished, delete it and remove the reference in order to continue gameplay in the next update
			if(uiEvent->currentConversation == nullptr){
				MY_ResourceManager::scenario->getAudio("EVENT_CLOSE")->sound->play();
				delete currentEvent;
				currentEvent = nullptr;
				uiControls->enable();
			}
		}else{
			// if there isn't an ongoing event, update the statistics and check for a new event
			manager.update(_step);
			currentEvent = manager.consumeEvent();
			if(currentEvent != nullptr){
				uiEvent->startEvent(currentEvent);
				uiControls->disable();
			}
		}
	}
	

	// visual update stuff
	float x = manager.statistics["progress"];//activeCamera->parents.at(0)->getTranslationVector().x;
	glm::vec3 v = playerCam->firstParent()->getTranslationVector();
	playerCam->firstParent()->translate(v.x + ((x-1)*50.f - v.x)*_step->deltaTimeCorrection*0.05f, v.y, v.z, false);

	
	{
		// update sky layer
		glm::vec3 v = activeCamera->getWorldPos();
		layerSky->childTransform->translate(v.x, v.y, -5, false);
		for(unsigned long int i = 0; i < 4; ++i){
			layerSkyMesh->mesh->vertices.at(i).v -= 0.0001f;
		}
		layerSkyMesh->mesh->dirty = true;

		// update art layers
		for(unsigned long int i = 0; i < bgLayers.size(); ++i){
			bgLayers.at(i)->progress = x;
		}
	}

	for(unsigned long int i = 0; i < bgLayers.size(); ++i){
		bgLayers.at(i)->colorReplaceBlack = manager.markers.coloursReplaceBlack[i+1];
		bgLayers.at(i)->colorReplaceWhite = manager.markers.coloursReplaceWhite[i+1];
	}
	layerSky->colorReplaceBlack = manager.markers.coloursReplaceBlack[0];
	layerSky->colorReplaceWhite = manager.markers.coloursReplaceWhite[0];
	
	manager.levelPath->colorReplaceBlack = manager.markers.coloursReplaceBlack[NUM_LAYERS-1];
	manager.levelPath->colorReplaceWhite = manager.markers.coloursReplaceBlack[NUM_LAYERS-2];
	
	if(keyboard->keyJustDown(GLFW_KEY_F12)){
		//game->toggleFullScreen();
		game->takeScreenshot();
	}

	if (keyboard->keyJustDown(GLFW_KEY_1)){
		cycleCamera();
	}if (keyboard->keyJustDown(GLFW_KEY_2)){
		Transform::drawTransforms = !Transform::drawTransforms;

		uiLayer->bulletDebugDrawer->setDebugMode(BulletDebugDrawer::DBG_MAX_DEBUG_DRAW_MODE);
	}

	float camSpeed = 0.3f;
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


	if (keyboard->keyJustDown(GLFW_KEY_ESCAPE)){
		game->switchScene("MENU_MAIN", false);
	}

	progress += speed;

	Scene::update(_step);
	glm::uvec2 sd = sweet::getWindowDimensions();
	uiLayer->resize(0, sd.x, 0, sd.y);
	//uiLayer->invalidateLayout();
	uiLayer->update(_step);
}

void MY_Scene::render(sweet::MatrixStack * _matrixStack, RenderOptions * _renderOptions){
	glPointSize(5);
	
	screenFBO->resize(game->viewPortWidth, game->viewPortHeight);
	//Bind frameBuffer
	screenFBO->bindFrameBuffer();
	//render the scene to the buffer
	_renderOptions->clearColour[0] = 1;
	_renderOptions->depthEnabled = false;
	_renderOptions->clear();
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	Scene::render(_matrixStack, _renderOptions);

	//Render the buffer to the render surface

	//glPolygonMode(GL_FRONT_AND_BACK, GL_POLYGON);
	screenSurface->render(screenFBO->getTextureId());
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	uiLayer->render(_matrixStack, _renderOptions);
}

void MY_Scene::load(){
	if(!loaded){
		replaceShader->load();
		baseShader->load();
	
		screenSurfaceShader->load();
		screenSurface->load();
		screenFBO->load();
		uiLayer->load();
	}

	Scene::load();	
}

void MY_Scene::unload(){
	if(loaded){
		uiLayer->unload();
		screenFBO->unload();
		screenSurfaceShader->unload();
		screenSurface->unload();
	
		baseShader->unload();
		replaceShader->unload();
	}

	Scene::unload();	
}