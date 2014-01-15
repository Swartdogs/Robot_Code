#ifndef FINDTARGET_H
#define FINDTARGET_H

#include "Commands/Subsystem.h"
#include "WPILib.h"
#include "Vision/RGBImage.h"
#include "Vision/BinaryImage.h"

/**
 *
 *
 * @author Neil
 */
class FindTarget: public Subsystem {
private:
	enum WhichTarget {wtUnknown, wtLeft, wtRight};

	struct TargetData {
		double		centerX;
		double		rectShort;
		double   	rectLong;
		int			rectLeft;
		int			rectTop;
		int			rectHeight;
	};
	
	WhichTarget		whichTarget;
	
	struct TargetReport {
		ParticleAnalysisReport* par;
		double					rectLong;
		double					rectShort;
		bool					isHotTarget;
		WhichTarget				whichTarget;
	};

	AxisCamera*		m_camera;
	TargetReport    m_report[8];
	int				m_horizontal[8];
	int				m_horizontalCount;
	int				m_vertical[8];
	int				m_verticalCount;
	
	bool foundHotTarget;
	
	void Arcade1(float move, float rotate, float& left, float& right);
	void Arcade2(float move, float rotate, float& left, float& right);
	
public:
	
	typedef enum {iIdle, iGetImage, iFilterImage, iParticleFilter, iImageAnalysis} ImageProcessStep;
	
	FindTarget();
	void InitDefaultCommand();
	
	bool PeriodicFind();
	void Find();
	void FindOne();
	void StartCamera(const char* cameraIP);
	void DriveTest(float move, float rotate);
	bool GetHotTarget();
};

#endif
