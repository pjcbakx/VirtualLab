#include <stdlib.h>
#include <glut\glut.h>
#include "Notes.h"
#include "ProcedureManager.h"
#include "GameManager.h"
#include "Scene.h"
#include "ProcedureObject.h"
#include "Hydra.h"

Notes::Notes() {
	buttonText = "";
}

void Notes::drawNotes(const char *text, int length, int x, int y, int z) {

	glPushMatrix();
	glMatrixMode(GL_PROJECTION);
	glColor3f(1.0f, 0.0f, 0.0f);

	glPushMatrix();
	glLoadIdentity();
	glOrtho(0, 800, 0, 600, -5, 5);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glPushMatrix();
	glLoadIdentity();
	glRasterPos3i(x, y, z);

	for (int i = 0; i < length; i++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, (int)text[i]);
	}

	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

}

void Notes::drawBoardTxt() {

	glPopMatrix();
	int itemNr = 0;
	for (int posY = 100; posY < 400; posY += 20) {
		itemNr++;
		std::string text = "Nr. " + std::to_string(itemNr) + " Column 1 en Column 2";
		drawNotes(text.data(), text.length(), 500, posY, 5);
	}
	glPushMatrix();

}

void Notes::draw()
{

}

void Notes::init()
{

}