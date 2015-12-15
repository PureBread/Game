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

	NodeUI * image = new NodeUI(uiLayer->world);
	MY_Button * back = new MY_Button(uiLayer->world, MY_ResourceManager::scenario->getFont("HURLY-BURLY")->font, 3);
	
	titleText->setText(L"INSTRUCTIONS");
	titleText->horizontalAlignment = kCENTER;


	image->setMarginTop(25);
	image->background->mesh->pushTexture2D(MY_ResourceManager::scenario->getTexture("INSTRUCTIONS")->texture);
	image->setRationalHeight(0.32f);
	image->setRationalWidth(0.7f);
	//image->background->mesh->scaleModeMag = image->background->mesh->scaleModeMin = GL_NEAREST;

	back->setLabel("BACK");
	back->setMarginTop(20);
	
	back->eventManager.addEventListener("click", [this](sweet::Event *){
		game->switchScene("MENU_MAIN", false);
	});
	
	vl->addChild(titleText);
	vl->addChild(image);

	std::vector<TextLabel * > instructions;
	std::vector<std::string> instructionText;
	instructionText.push_back("Move your cursor to the bottom of the screen during travel to bring up the herd's resources.");
	instructionText.push_back("Control food consumption using the right half of the pie menu.");
	instructionText.push_back("Control travel speed using the left half of the pie menu.");
	instructionText.push_back("Events will appear at random and may effect the herd's statistics.");
	instructionText.push_back("Not feeding your llamas will cause their health to drop.");
	instructionText.push_back("Moving fast while not eating will cause your health to drop faster");
	instructionText.push_back("Remember to keep your health up by eating and resting, or you might lose llamas along the way!");

	for (std::string i : instructionText){
		TextLabel * l = new TextLabel(uiLayer->world, MY_ResourceManager::scenario->getFont("HURLY-BURLY")->font, textShader);
		instructions.push_back(l);
		l->horizontalAlignment = kCENTER;
		l->setText(i);
		vl->addChild(l);
		l->setRationalWidth(1.f, vl);
	}

	vl->addChild(back);
	
	vl->boxSizing = BoxSizing::kBORDER_BOX;
	vl->setMargin(0.2f);
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