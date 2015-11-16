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

#include <RenderOptions.h>

#include <PlayerManager.h>

#include <LevelPath.h>
#include <Llama.h>


#include <VCam.h>

MY_Scene::MY_Scene(Game * _game) :
	Scene(_game),
	screenSurfaceShader(new Shader("assets/RenderSurface", false, true)),
	screenSurface(new RenderSurface(screenSurfaceShader)),
	screenFBO(new StandardFrameBuffer(true)),
	baseShader(new ComponentShaderBase(true)),
	replaceShader(new ComponentShaderBase(true)),
	replaceShaderComponent(new ShaderComponentReplace(replaceShader)),
	textShader(new ComponentShaderText(true)),
	uiLayer(0,0,0,0),
	progress(0),
	speed(0),
	currentEvent(nullptr)
{
	baseShader->addComponent(new ShaderComponentMVP(baseShader));
	baseShader->addComponent(new ShaderComponentTexture(baseShader));
	baseShader->compileShader();

	replaceShader->addComponent(new ShaderComponentMVP(replaceShader));
	replaceShader->addComponent(new ShaderComponentTexture(replaceShader));
	replaceShader->addComponent(replaceShaderComponent);
	replaceShader->compileShader();
	replaceShader->load();

	screenSurface->uvEdgeMode = GL_CLAMP_TO_EDGE;

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

	activeCamera = debugCam;

	//
	glm::uvec2 sd = sweet::getScreenDimensions();
	uiLayer.resize(0, sd.x, 0, sd.y);


	/** GAME STUFF **/
	playerCam = new PerspectiveCamera();
	cameras.push_back(playerCam);
	childTransform->addChild(playerCam);
	playerCam->yaw = 90;
	playerCam->pitch = 0.520833313;
	playerCam->parents.at(0)->translate(0, -13.8184452f, 57.8584137f);
	playerCam->fieldOfView = 64.3750000;

	PointLight * l = new PointLight(glm::vec3(1, 1, 1), 1.f, 0.1f, 0.1f);
	lights.push_back(l);
	childTransform->addChild(l)->translate(0, 15, 0);

	
	

	layerSky = new ArtLayer(replaceShaderComponent);
	layerBgDetail = new ContinuousArtScroller("BG5", replaceShader);
	layerBg = new ContinuousArtScroller("BG4", replaceShader);
	layerLlamas = new ContinuousArtScroller("BG3", replaceShader);
	layerFg = new ContinuousArtScroller("BG2", replaceShader);
	layerFgDetail = new ContinuousArtScroller("BG1", replaceShader);
	
	bgLayers.push_back(layerBgDetail);
	bgLayers.push_back(layerBg);
	bgLayers.push_back(layerLlamas);
	bgLayers.push_back(layerFg);
	bgLayers.push_back(layerFgDetail);

	childTransform->addChild(layerSky);
	for(signed long int i = 0; i < bgLayers.size(); ++i){
		childTransform->addChild(bgLayers.at(i))->translate(0, 0, i * 10);
		bgLayers.at(i)->firstParent()->scale(50);
	}


	layerSkyMesh = new MeshEntity(MeshFactory::getPlaneMesh(), replaceShader);
	layerSky->childTransform->addChild(layerSkyMesh, false);
	layerSkyMesh->childTransform->translate(0,-50,0);
	layerSkyMesh->meshTransform->translate(0,0.5,0);
	layerSkyMesh->childTransform->scale(150);

	Texture * texture = new Texture("assets/textures/sky.png", true, false);
	texture->load();
	layerSkyMesh->mesh->pushTexture2D(texture);
	layerSkyMesh->mesh->scaleModeMag = layerSkyMesh->mesh->scaleModeMin = GL_NEAREST;

	manager.loadDefaults();

	
	NodeUI * ui = new NodeUI(uiLayer.world);

	SliderControlled * food = new SliderControlled(uiLayer.world, &manager.statistics["food"], 0, 100, false);
	SliderControlled * wool = new SliderControlled(uiLayer.world, &manager.statistics["wool"], 0, 100, false);
	SliderControlled * health = new SliderControlled(uiLayer.world, &manager.statistics["health"], 0, 100);

	//food->background->setShader(maskShader);
	food->fill->background->mesh->pushTexture2D(MY_ResourceManager::scenario->getTexture("SADDLEBAG-R-MASK")->texture);
	wool->fill->background->mesh->pushTexture2D(MY_ResourceManager::scenario->getTexture("SADDLEBAG-L-MASK")->texture);
	ui->background->setVisible(false);
	//ui->background->mesh->pushTexture2D(MY_ResourceManager::scenario->getTexture("BACK")->texture);

	//MeshInterface * p = MeshFactory::getPlaneMesh();
	//for(unsigned long int i = 0; i < p->getVertCount(); ++i){
	//	p->vertices.at(i).x += 0.5f;
	//	p->vertices.at(i).y += 0.5f;
	//}
	//p->dirty = true;
	//food->fill->background->meshTransform->addChild(new MeshEntity(p, maskShader), false);
	//maskComponent->setMaskTex(MY_ResourceManager::scenario->getTexture("SADDLEBAG2-MASK")->texture);

	TextLabel * herdSize = new TextLabel(uiLayer.world, MY_ResourceManager::scenario->getFont("HURLY-BURLY")->font, textShader);
	herdSize->setText(L"99");
	textShader->setColor(178.f/255.f, 178.f/255.f, 178.f/255.f);
	herdSize->horizontalAlignment = kCENTER;

	SliderController * speed = new SliderController(uiLayer.world, &manager.statistics["speed"], 1, 0, 2, false);
	SliderController * rations = new SliderController(uiLayer.world, &manager.statistics["rations"], 1, 0, 2, false);
	
	speed->setStepped(1);
	rations->setStepped(1);
	
	wool->setBackgroundColour(1,1,1);
	wool->fill->setBackgroundColour(1,1,1);
	food->setBackgroundColour(1,1,1);
	food->fill->setBackgroundColour(215.f/255.f, 198.f/255.f, 151.f/255.f);
	health->setBackgroundColour(1,1,1);
	health->fill->setBackgroundColour(129.f/255.f, 208.f/255.f, 217.f/255.f);

	speed->setBackgroundColour(178.f/255.f, 178.f/255.f, 178.f/255.f);
	rations->setBackgroundColour(178.f/255.f, 178.f/255.f, 178.f/255.f);
	
	speed->fill->background->mesh->pushTexture2D(MY_ResourceManager::scenario->getTexture("DIAL-L")->texture);
	rations->fill->background->mesh->pushTexture2D(MY_ResourceManager::scenario->getTexture("DIAL-R")->texture);
	
	wool->background->mesh->pushTexture2D(MY_ResourceManager::scenario->getTexture("SADDLEBAG-L")->texture);
	food->background->mesh->pushTexture2D(MY_ResourceManager::scenario->getTexture("SADDLEBAG-R")->texture);
	

	food->setRationalWidth(0.33f);
	wool->setRationalWidth(0.33f);
	
	food->setMarginTop(0.1f);
	wool->setMarginTop(0.1f);
	
	speed->setRationalWidth(0.1f);
	rations->setRationalWidth(0.1f);
	
	speed->setMarginBottom(0.1f);
	rations->setMarginBottom(0.1f);
	
	speed->setMarginTop(0.2f);
	rations->setMarginTop(0.2f);
	
	health->setMargin(0.25f, 0);
	health->setRationalHeight(0.1f);
	health->setRationalWidth(1.f);

	herdSize->setMargin(0.4f, 0.4f);
	herdSize->setRationalHeight(1.f);
	herdSize->setRationalWidth(1.f);
	

	HorizontalLinearLayout * hlayout = new HorizontalLinearLayout(uiLayer.world);
	
	hlayout->addChild(wool);
	hlayout->addChild(speed);
	hlayout->addChild(rations);
	hlayout->addChild(food);

	ui->addChild(hlayout);
	ui->addChild(herdSize);
	ui->addChild(health);


	
	hlayout->setRationalWidth(1.f);
	hlayout->setRationalHeight(1.f);
	hlayout->horizontalAlignment = kCENTER;
	
	ui->setMargin(0.33f, 0);
	ui->setRationalWidth(1.f);
	ui->setRationalHeight(0.25f);
	ui->setMarginBottom(0.01f);
	uiLayer.addChild(ui);


	uiLayer.addMouseIndicator();
	
	//LevelPath * lp = new LevelPath("walkLayer.png");
	
	MeshEntity * mesh = new MeshEntity(new MeshInterface(GL_LINE_STRIP, GL_STATIC_DRAW), baseShader);
	for (int i = 0; i < manager.levelPath->vertices.size(); ++i){
		mesh->mesh->pushVert(Vertex(glm::vec3(manager.levelPath->vertices.at(i).x, manager.levelPath->vertices.at(i).y, 0)));
	}

	mesh->meshTransform->scale(200, true);
	mesh->meshTransform->translate(glm::vec3(0, 50, 0));
	childTransform->addChild(mesh);

	manager.levelPath->scaleVertices(200);
	manager.levelPath->childTransform->translate(0, 50.f, 0);
	childTransform->addChild(manager.levelPath);
	Llama * llama = new Llama(baseShader);

	llama->childTransform->scale(2.f);
	llama->childTransform->translate(glm::vec3(manager.levelPath->vertices.at(0).x, manager.levelPath->vertices.at(0).y, 0));
	manager.levelPath->addLlama(llama);
	
}

MY_Scene::~MY_Scene(){
	deleteChildTransform();
	baseShader->safeDelete();
	textShader->safeDelete();

	screenSurface->safeDelete();
	//screenSurfaceShader->safeDelete();
	screenFBO->safeDelete();
}


void MY_Scene::update(Step * _step){
	if(keyboard->keyJustDown(GLFW_KEY_L)){
		screenSurfaceShader->unload();
		screenSurfaceShader->loadFromFile(screenSurfaceShader->vertSource, screenSurfaceShader->fragSource);
		screenSurfaceShader->load();
	}
	
	
	glUseProgram(screenSurfaceShader->getProgramId());
	GLint test = glGetUniformLocation(screenSurfaceShader->getProgramId(), "time");
	checkForGlError(0,__FILE__,__LINE__);
	if(test != -1){
		glUniform1f(test, (float)sweet::lastTimestamp);
		checkForGlError(0,__FILE__,__LINE__);
	}


	if(currentEvent != nullptr){
		// if there is an ongoing event, progress it
		std::stringstream ss;
		ss << "Event: ";
		switch (currentEvent->type){
			case kLOSS: ss << "LOSS"; break;
			case kDESTINATION: ss << "DESTINATION"; break;
			case kRANDOM: ss << "RANDOM"; break;
		}
		ss << currentEvent->scenario->id << std::endl;
		Log::info(ss.str());

		// once the event is finished, delete it and remove the reference in order to continue gameplay in the next update
		delete currentEvent;
		currentEvent = nullptr;
	}else{
		// if there isn't an ongoing event, update the statistics and check for a new event
		manager.update(_step);
		currentEvent = manager.consumeEvent();
	}
	
	float x = manager.statistics["progress"];//activeCamera->parents.at(0)->getTranslationVector().x;
	glm::vec3 v = playerCam->firstParent()->getTranslationVector();
	playerCam->firstParent()->translate((x-1)*50.f, v.y, v.z, false);

	
	{
		// update sky layer
		glm::vec3 v = activeCamera->getWorldPos();
		layerSky->childTransform->translate(v.x, v.y, -5, false);
		for(unsigned long int i = 0; i < 4; ++i){
			layerSkyMesh->mesh->vertices.at(i).v -= 0.0001;
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

	

	if (keyboard->keyJustDown(GLFW_KEY_A)){
		speed += 0.1;
	}if (keyboard->keyJustDown(GLFW_KEY_D)){
		speed -= 0.1;
	}
	if (keyboard->keyJustDown(GLFW_KEY_ESCAPE)){
		game->switchScene("MENU_MAIN", false);
	}

	progress += speed;

	Scene::update(_step);
	glm::uvec2 sd = sweet::getScreenDimensions();
	uiLayer.resize(0, sd.x, 0, sd.y);
	uiLayer.layoutDirty = true;
	uiLayer.update(_step);
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