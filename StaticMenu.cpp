#include <GL/glew.h>
#include "StaticMenu.h"
#include "Camera.h"
#include <btBulletCollisionCommon.h>
#include <glm/glm.hpp>
#include "Shader.h"
#include "ObjModel.h"

void StaticMenu::init()
{
	shaderID = initShader("procedureObject");
	btScalar mass = 0.0;
	pObjModel = new ObjModel("c:\\VrCave\\Development\\VirtualLab\\Data\\Menu.obj", mass, btVector3(0, 0, 0), btVector3(1, 1, 1));
}

void StaticMenu::draw()
{

	glPushMatrix();
	glMatrixMode(GL_PROJECTION);

	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, 800, 0, 600, 0, 0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glPushMatrix();
	glLoadIdentity();
	glTranslatef(-0.65f,-0.55f,0);

	glUseProgram(shaderID);

	btTransform trans;
	btScalar m[16];
	pObjModel->rigidBody->getMotionState()->getWorldTransform(trans);
	trans.getOpenGLMatrix(m);
	glMultMatrixf(m);

	GLint uniform = 0;

	glScalef(0.5f, 0.4f, 0.4f);

	uniform = glGetUniformLocation(shaderID, "useWater");
	glUniform1i(uniform, false);

	uniform = glGetUniformLocation(shaderID, "useTexture");
	glUniform1i(uniform, true);

	uniform = glGetUniformLocation(shaderID, "materialShininess");
	glUniform1f(uniform, 80.0f);

	uniform = glGetUniformLocation(shaderID, "materialSpecularColor");
	glUniform3f(uniform, 0.5f, 0.3f, 0.1f);

	uniform = glGetUniformLocation(shaderID, "light.position");
	glUniform3f(uniform, 0.82f, 1.85f, 1.45f); //TODO: Take a non-random place

	uniform = glGetUniformLocation(shaderID, "light.intensities");
	glUniform3f(uniform, 2.0f, 2.0f, 2.0f);

	uniform = glGetUniformLocation(shaderID, "light.attenuation");
	glUniform1f(uniform, 0.5f);

	uniform = glGetUniformLocation(shaderID, "light.ambientCoefficient");
	glUniform1f(uniform, 0.3f);

	uniform = glGetUniformLocation(shaderID, "cameraPosition");
	glUniform3f(uniform, fpCameraXCoordinate, fpCameraYCoordinate, fpCameraZCoordinate);

	GLfloat modelViewMatrix[16];
	glGetFloatv(GL_MODELVIEW_MATRIX, modelViewMatrix);
	uniform = glGetUniformLocation(shaderID, "modelView");
	glUniformMatrix4fv(uniform, 1.0f, GL_FALSE, modelViewMatrix);

	GLfloat projectionMatrix[16];
	glGetFloatv(GL_PROJECTION_MATRIX, projectionMatrix);
	uniform = glGetUniformLocation(shaderID, "projection");
	glUniformMatrix4fv(uniform, 1.0f, GL_FALSE, projectionMatrix);

	pObjModel->draw(shaderID);

	glUseProgram(0);

	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

}