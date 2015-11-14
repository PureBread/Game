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

#include <PlayerStatistics.h>

#include <LevelPath.h>



#include <VCam.h>

MY_Scene::MY_Scene(Game * _game) :
	Scene(_game),
	screenSurfaceShader(new Shader("assets/engine basics/DefaultRenderSurface", false, true)),
	screenSurface(new RenderSurface(screenSurfaceShader)),
	screenFBO(new StandardFrameBuffer(true)),
	baseShader(new ComponentShaderBase(true)),
	replaceShaderComponent(new ShaderComponentReplace(baseShader)),
	textShader(new ComponentShaderText(true)),
	debugDrawer(nullptr),
	uiLayer(0,0,0,0),
	box2dWorld(new Box2DWorld(b2Vec2(0.f, -10.0f))),
	box2dDebug(new Box2DDebugDrawer(box2dWorld)),
	progress(0),
	speed(0)
{
	baseShader->addComponent(new ShaderComponentMVP(baseShader));
	//baseShader->addComponent(new ShaderComponentDiffuse(baseShader));
	baseShader->addComponent(new ShaderComponentTexture(baseShader));
	baseShader->addComponent(replaceShaderComponent);
	baseShader->compileShader();
	
	maskShader = new ComponentShaderBase(true);
	maskShader->addComponent(new ShaderComponentMVP(maskShader));
	maskShader->addComponent(new ShaderComponentTexture(maskShader));
	maskComponent = new ShaderComponentMask(maskShader);
	maskShader->addComponent(maskComponent);
	maskShader->compileShader();


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

	childTransform->addChild(box2dDebug, false);
	box2dDebug->drawing = true;
	box2dWorld->b2world->SetDebugDraw(box2dDebug);
	box2dDebug->AppendFlags(b2Draw::e_shapeBit);
	box2dDebug->AppendFlags(b2Draw::e_centerOfMassBit);
	box2dDebug->AppendFlags(b2Draw::e_jointBit);





	/** GAME STUFF **/
	//VCam * cam = new VCam(1920, 1080/*, 0, 0*/);
	PerspectiveCamera * cam = new PerspectiveCamera();
	
	//OrthographicCamera * cam = new OrthographicCamera(-1920/2, 1920/2, -1080/2, 1080/2, -1000, 1000);
	cameras.push_back(cam);
	childTransform->addChild(cam);
	//cam->childTransform->rotate(90, 0, 1, 0, kWORLD);
	//cam->childTransform->rotate(90, 0, 1, 0, kOBJECT);
	cam->yaw = 91;
	cam->pitch = 12.5f;
	cam->parents.at(0)->translate(0, -25.f, 50.f);
	cam->fieldOfView = 25;

	PointLight * l = new PointLight(glm::vec3(1, 1, 1), 1.f, 0.1f, 0.1f);
	lights.push_back(l);
	childTransform->addChild(l)->translate(0, 15, 0);

	LevelPath * lp = new LevelPath("BG1/2.png");

	MeshEntity * mesh = new MeshEntity(new MeshInterface(GL_POINTS, GL_STATIC_DRAW), baseShader);
	for (int i = 0; i < lp->vertices.size(); ++i){
		mesh->mesh->pushVert(Vertex(glm::vec3(lp->vertices.at(i).x, lp->vertices.at(i).y, 0)));
	}
	//mesh->meshTransform->rotate(180, 0, 0, 1, kWORLD);
	mesh->meshTransform->scale(0.1, true);
	mesh->meshTransform->translate(glm::vec3(0, 50, 0));
	childTransform->addChild(mesh);

	layerSky = new MeshEntity(MeshFactory::getPlaneMesh(), baseShader);
	layerBgDetail = new ContinuousArtScroller("BG5", baseShader);
	layerBg = new ContinuousArtScroller("BG4", baseShader);
	layerLlamas = new ContinuousArtScroller("BG3", baseShader);
	layerFg = new ContinuousArtScroller("BG2", baseShader);
	layerFgDetail = new ContinuousArtScroller("BG1", baseShader);
	
	bgLayers.push_back(layerBgDetail);
	bgLayers.push_back(layerBg);
	bgLayers.push_back(layerLlamas);
	bgLayers.push_back(layerFg);
	bgLayers.push_back(layerFgDetail);

	childTransform->addChild(layerSky);
	for(signed long int i = 0; i < bgLayers.size(); ++i){
		childTransform->addChild(bgLayers.at(i))->translate(0, 0, i * 5);
		bgLayers.at(i)->firstParent()->scale(50);
	}
	layerSky->childTransform->translate(0,0,-5);
	layerSky->meshTransform->translate(0,0.5,0);
	layerSky->childTransform->scale(50);

	Texture * texture = new Texture("assets/textures/sky.png", true, false);
	texture->load();
	layerSky->mesh->pushTexture2D(texture);
	layerSky->mesh->scaleModeMag = layerSky->mesh->scaleModeMin = GL_NEAREST;

	stats.loadDefaults();

	
	NodeUI * ui = new NodeUI(uiLayer.world);
	ui->background->setVisible(false);

	SliderControlled * food = new SliderControlled(uiLayer.world, &stats.resources["food"], 0, 100, false);
	SliderControlled * wool = new SliderControlled(uiLayer.world, &stats.resources["wool"], 0, 100, false);
	SliderControlled * health = new SliderControlled(uiLayer.world, &stats.resources["health"], 0, 100);

	//food->background->setShader(maskShader);
	food->fill->background->mesh->pushTexture2D(MY_ResourceManager::scenario->getTexture("SADDLEBAG2-MASK")->texture);
	wool->fill->background->mesh->pushTexture2D(MY_ResourceManager::scenario->getTexture("SADDLEBAG-MASK")->texture);

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
	herdSize->horizontalAlignment = kCENTER;

	SliderController * speed = new SliderController(uiLayer.world, &stats.resources["speed"], 1, 0, 2, false);
	SliderController * rations = new SliderController(uiLayer.world, &stats.resources["rations"], 1, 0, 2, false);
	
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

	
	wool->background->mesh->pushTexture2D(MY_ResourceManager::scenario->getTexture("SADDLEBAG")->texture);
	food->background->mesh->pushTexture2D(MY_ResourceManager::scenario->getTexture("SADDLEBAG2")->texture);
	

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
	// update sky layer
	glm::vec3 v = activeCamera->getWorldPos();
	layerSky->childTransform->translate(v.x, v.y, -5, false);
	for(unsigned long int i = 0; i < 4; ++i){
		layerSky->mesh->vertices.at(i).v -= 0.0001;
	}
	layerSky->mesh->dirty = true;

	// update art layers
	float x = activeCamera->parents.at(0)->getTranslationVector().x;
	for(unsigned long int i = 0; i < bgLayers.size(); ++i){
		bgLayers.at(i)->progress = x/50 + 1;
	}

	box2dWorld->update(_step);

	
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
	glPointSize(20);
	
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