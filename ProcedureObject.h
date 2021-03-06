#ifndef PROCEDURE_OBJECT_H
#define PROCEDURE_OBJECT_H

#include <string>
#include <vector>

#define TO_RADIANS(X) float(X / 180.0f * 3.14159265358979323846f)

struct Control;
class ObjModel;
class btVector3;
class btVector4;

enum OriginAnchor{
	Table,
	Room,
	Error
};

std::string originAnchorToString(OriginAnchor originAnchor);
OriginAnchor stringToOriginAnchor(std::string str);


class ProcedureObject
{
private:
	unsigned int shaderID;
	void applyOriginAnchorTranslation();
public:
	ObjModel *pObjModel;
	std::vector<Control *> controls;
	std::string fileName;
	std::string name;

	//ItemInfo
	bool grabbed = false;
	std::string LeftRight = "None";
	int controlStep = 0;

	//ObjectInfo
	btVector3 *origin;
	bool initRigidbodies = false;
	btVector3 *rotation;
	btVector3 *scale;
	btVector4 *color;
	bool isChangeObject;
	bool useColorInsteadOfTexture = false;
	OriginAnchor originAnchor;
	btVector3 *waterDirectionMin;
	btVector3 *waterDirectionMax;
	bool useWaterOverlay;

	ProcedureObject::ProcedureObject(std::string fileName, std::string name) : fileName(fileName), name(name) { }
	void init();
	void draw();
	void update();
	void rotate(std::string direction, int degrees);
	void deleteRigidBodyFromWorld();
};

#endif