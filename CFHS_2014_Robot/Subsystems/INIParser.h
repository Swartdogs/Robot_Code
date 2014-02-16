#ifndef INIPARSER_H
#define INIPARSER_H
#include "Commands/Subsystem.h"
#include "WPILib.h"
#include <string>

/**
 *
 *
 * @author Srinu
 */
class INIParser: public Subsystem {
public:
	INIParser(char* filename);
	void InitDefaultCommand();
	void SetSubsystem(char* subsystem);
	
	template <class T> T FindValue(char* key, T defaultValue); 
private:
	char* m_filename;
	char* m_subsystem;
};

#endif
