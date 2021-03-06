#include "Scene.h"
#include "RenderManager.h"
#include "Floor.h"
#include <GL/glew.h>
#include "Shader.h"
#include "ObjModel.h"
#include "Hydra.h"
#include "debug.h"
#include "Main.h"
#include "stb_image.h"
#include "Camera.h"
#include "StaticLabEnvironment.h"
#include "ProcedureManager.h"
#include "ControlEnum.h"
#include "HUD.h"
#include "Menu.h"
#include "StaticBoard.h"
#include "StaticMenu.h"
#include "TextRepresentation.h"

#include <iostream>
#include <VrLib\Application.h>
#include <ctime>
#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>
#include <glm/gtc/type_ptr.hpp>
#include <time.h>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <sys/timeb.h>

CollisionInformation leftHydraCollisionInformation;
CollisionInformation rightHydraCollisionInformation;

bool contactProcessedCallback(btManifoldPoint& cp, void* body0, void* body1)
{
	btRigidBody *rBody0 = (btRigidBody*)body0;
	btRigidBody *rBody1 = (btRigidBody*)body1;

	if (!GameManager::getInstance()->scene->hydra->initRigidbodies)
		return false;

	return false;
}

Scene::Scene()
{
	head = new PositionalDevice();
	WKey = new DigitalDevice();
	AKey = new DigitalDevice();
	SKey = new DigitalDevice();
	DKey = new DigitalDevice();
	QKey = new DigitalDevice();
	EKey = new DigitalDevice();
	UpKey = new DigitalDevice();
	DownKey = new DigitalDevice();
	LeftKey = new DigitalDevice();
	RightKey = new DigitalDevice();
	SpaceKey = new DigitalDevice();

	head->init("MainUserHead");
	WKey->init("WKey");
	AKey->init("AKey");
	SKey->init("SKey");
	DKey->init("DKey");
	UpKey->init("UpKey");
	QKey->init("QKey");
	EKey->init("EKey");
	DownKey->init("DownKey");
	LeftKey->init("LeftKey");
	RightKey->init("RightKey");
	SpaceKey->init("SpaceKey");

	hud = new HUD();

	hydra = new Hydra();

	f = new Floor();

	lab = new StaticLabEnvironment();
	lab->init();

	board = new StaticBoard();
	board->init();

	menu = new Menu();
	menu->init();

	staticMenu = new StaticMenu();
	staticMenu->init();

	broadphase = new btDbvtBroadphase();
	collisionConfiguration = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collisionConfiguration);
	solver = new btSequentialImpulseConstraintSolver();
	world = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);

	world->setGravity(btVector3(0.0f, -9.81f * 20.0f, 0.0f));

#ifdef BULLET_DEBUG_DRAW
	debug* debug_1 = new debug();
	//world->setDebugDrawer(debug_1);
#endif

	srand(static_cast<unsigned int>(time(NULL)));

	gContactProcessedCallback = &contactProcessedCallback;

	procedureManager = new ProcedureManager();
}

Scene::~Scene()
{
	delete head;
	delete WKey;
	delete AKey;
	delete SKey;
	delete QKey;
	delete EKey;
	delete UpKey;
	delete DownKey;
	delete LeftKey;
	delete RightKey;
	delete SpaceKey;

	delete world;
	delete solver;
	delete dispatcher;
	delete collisionConfiguration;
	delete broadphase;

	delete f;
}

void Scene::update()
{
	GLint64 timer;
	glGetInteger64v(GL_TIMESTAMP, &timer);
	timer /= 10000000;

	if (leftHydraCollisionInformation.m_state == CollisionState::COLLISION && leftHydraCollisionInformation.m_collisionTime + 100 <= timer)
	{
		leftHydraCollisionInformation.m_state = CollisionState::NO_COLLISION;
	}
	else if (leftHydraCollisionInformation.m_state == CollisionState::COLLISION)
	{
		if ((!leftHydraCollisionInformation.m_informationUsed))
		{
			//Action because of hydra here.
			leftHydraCollisionInformation.m_informationUsed = true;
		}
	}

	if (rightHydraCollisionInformation.m_state == CollisionState::COLLISION && rightHydraCollisionInformation.m_collisionTime + 100 <= timer)
	{
		rightHydraCollisionInformation.m_state = CollisionState::NO_COLLISION;
	}
	else if (rightHydraCollisionInformation.m_state == CollisionState::COLLISION)
	{
		if ((!rightHydraCollisionInformation.m_informationUsed))
		{
			//Action because of hydra here.
			rightHydraCollisionInformation.m_informationUsed = true;
		}
	}

	//Keyboard
	//if (WKey->isInitialized() && (WKey->getData() == DigitalState::ON || WKey->getData() == DigitalState::TOGGLE_ON))
	//{
	//	MoveForward();
	//}
	//if (AKey->isInitialized() && (AKey->getData() == DigitalState::ON || AKey->getData() == DigitalState::TOGGLE_ON))
	//{
	//	MoveLeft();
	//}
	//if (DKey->isInitialized() && (DKey->getData() == DigitalState::ON || DKey->getData() == DigitalState::TOGGLE_ON))
	//{
	//	MoveRight();
	//}
	//if (SKey->isInitialized() && (SKey->getData() == DigitalState::ON || SKey->getData() == DigitalState::TOGGLE_ON))
	//{
	//	MoveBackward();
	//}
	//if (QKey->isInitialized() && (QKey->getData() == DigitalState::ON || QKey->getData() == DigitalState::TOGGLE_ON))
	//{
	//	MoveUpward();
	//}
	//if (EKey->isInitialized() && (EKey->getData() == DigitalState::ON || EKey->getData() == DigitalState::TOGGLE_ON))
	//{
	//	MoveDownward();
	//}
	if (UpKey->isInitialized() && (UpKey->getData() == DigitalState::ON || UpKey->getData() == DigitalState::TOGGLE_ON))
	{
		if (asMenu == true) {
			if (!buttonPressed)
			{
				if (menuItem < menu->numberOfProcedures-1)
					menuItem += 1;
				buttonPressed = true;

			}
		} else
			RotateUp();
	}
	else if (DownKey->isInitialized() && (DownKey->getData() == DigitalState::ON || DownKey->getData() == DigitalState::TOGGLE_ON))
	{
		if (asMenu == true) {
			if (!buttonPressed)
			{
				if (menuItem > 0)
					menuItem -= 1;
				buttonPressed = true;
			}
		} else
			RotateDown();
	}
	else if (LeftKey->isInitialized() && (LeftKey->getData() == DigitalState::ON || LeftKey->getData() == DigitalState::TOGGLE_ON))
	{
		RotateLeft();
	}
	else if (RightKey->isInitialized() && (RightKey->getData() == DigitalState::ON || RightKey->getData() == DigitalState::TOGGLE_ON))
	{
		RotateRight();
	}
	else if (SpaceKey->isInitialized() && (SpaceKey->getData() == DigitalState::ON || SpaceKey->getData() == DigitalState::TOGGLE_ON))
	{

		if (asMenu == true) {
			hydra->init();
			procedureManager->procedureNumber = menuItem;
			procedureManager->init();
			asMenu = false;
		}

	}
	else if (DownKey->getData() == DigitalState::OFF  || UpKey->getData() == DigitalState::OFF )
	{
		buttonPressed = false;
	}
	//End of keyboard


	if (asMenu == false) {
		hydra->update();
		procedureManager->update(StringToControlEnum(hydra->checkButtons()));
	}
}

void Scene::update(double frameTime, double totalTime)
{
	world->stepSimulation(btScalar(frameTime / 1000.0));
	update();
}

void Scene::draw(DrawMode drawMode)
{
	float InitialModelView[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, &InitialModelView[0]);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	const float LightPosition[4] = { 0.0f, 1.0f, 0.0f, 1.0f };
	glEnable(GL_MULTISAMPLE_ARB);
	glPushMatrix();
	InitCameraRotation();
	InitCameraTranslation();

	glLightfv(GL_LIGHT0, GL_POSITION, LightPosition);

#ifdef BULLET_DEBUG_DRAW
	world->debugDrawWorld();
#endif

	//f->draw();

	if (menu != nullptr && asMenu == true) {
		//staticMenu->draw();
		menu->draw();
		menu->drawList(GameManager::getInstance()->menu->procedureNames);
		menu->drawCursor(menuItem);
	}
	else {
		hydra->draw(InitialModelView);
		procedureManager->draw();
		hud->draw();
	}

	lab->draw();

	board->draw();

	glPopMatrix();

	glDisable(GL_MULTISAMPLE_ARB);
}

void Scene::startProcedure()
{
	//Do init for ProcedureManager here and enable its update
}