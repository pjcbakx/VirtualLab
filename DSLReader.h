#ifndef DSL_READER_H
#define DSL_READER_H

#include <vector>
#include <string>

#define PROCEDURE_LOCATION_FILE_LOCATION "C:\\VirtualLab\\ProcedureLocationFile.plf"

class ProcedureInformation;

class DSLReader{
public:
	std::vector<std::string> readProcedureLocationFromFile();
	ProcedureInformation *readProcedureFromFile(std::string procedureFileLocation);
};

#endif