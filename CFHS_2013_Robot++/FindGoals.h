#ifndef FINDGOALS_H_
#define FINDGOALS_H_

#include <math.h>
#include <vector>
#include "Vision/AxisCamera.h"
#include "Vision/BinaryImage.h"

enum EnumFindGoals{fStart, fContinue};

class FindGoals
{
	
public:
	FindGoals();
	~FindGoals();
	
	double GetAngle();
	int	   GetDistance();
	int	   Find();
	void   StartCamera(const char *cameraIP);
	int    Find(char ShootMode);
	
private:
	int    ProcessImage(EnumFindGoals Action);
};

#endif
