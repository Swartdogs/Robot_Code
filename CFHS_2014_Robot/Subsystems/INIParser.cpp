#include "INIParser.h"
#include "../Robotmap.h"
#include <string>

INIParser::INIParser(char* filename) : Subsystem("INIParser") {
	m_filename = filename;
	m_subsystem = "";
}
    
void INIParser::InitDefaultCommand() {
	// Set the default command for a subsystem here.
	//SetDefaultCommand(new MySpecialCommand());
}

void INIParser::SetSubsystem(char* subsystem) {
	sprintf(m_subsystem, "[%s]", subsystem);
	printf("INIParser: New Subsystem set: %s", m_subsystem);
}

template <class T> T INIParser::FindValue(char* key, T defaultValue) {
	T num = defaultValue;
	FILE* file = fopen(m_filename, "r");
	bool subsystemFound = false;
	bool keyFound = false;
	char* temp;
	
	if(file == NULL) {
		printf("IniParser: Invalid File, using defaultValue \n");
		return num;
	}
	
	while(!feof(file)) {
		fgets(temp, 200, file);
		string line(temp);
		if(!subsystemFound) {
			if(strcmp(m_subsystem, temp) == 0) {
				subsystemFound = true;
			}
		} else {
			if(line[0] == '[') {
				break;
			} else {
				if(line[0] != ' ' && line[0] != '!' && line[0] != '\n') {
					if(!line.compare(0, strlen(key), key)) {
						keyFound = true;
						if(!(num - (int)num)) {
							num = stoi(line.substr(line.find('=')+1));
						} else {
							num = stof(line.substr(line.find('=')+1));
						}
						break;
					}
				}
			}
		}
	}
	
	if(!keyFound) {
		if(!subsystemFound) {
			printf("INIParser: Subsystem not found, using defaultValue \n");
		} else {
			printf("INIParser: Key not found, using defaultValue\n");
		}
	}
	fclose(file);
	return num;
}
