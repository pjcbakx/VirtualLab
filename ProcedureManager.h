#ifndef PROCEDURE_MANAGER_H
#define PROCEDURE_MANAGER_H

#include <vector>
#include <string>
#include <btBulletCollisionCommon.h>
#include <btBulletDynamicsCommon.h>

enum ControlEnum;
struct ProcedureInformation;
class ProcedureObject;

class ProcedureManager{
public:
	void init();
	void draw();
	void update(ControlEnum controlEnum, btVector3* LeftHydra, btVector3* RightHydra);
private:
	std::vector<std::string> procedureFileLocations;
	ProcedureInformation *currentProcedureInformation;
public:
	ProcedureObject *righternSelectedProcedureObject;
	ProcedureObject *lefternSelectedProcedureObject;
};

#endif