#ifndef FINDTARGET_H
#define FINDTARGET_H

#include "Commands/Subsystem.h"
#include "WPILib.h"
#include "Vision/RGBImage.h"
#include "Vision/BinaryImage.h"
#include "../RobotLog.h"

/**
 *
 *
 * @author Neil
 */
class FindTarget: public Subsystem {
private:
	typedef enum {iStart, iGetImage, iFilterImage, iParticleFilter, iImageAnalysis} FindStep;
	typedef enum {wtUnknown, wtLeft, wtRight} WhichTarget;

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
	FindStep		m_findStep;
	int				m_horizontal[8];
	int				m_horizontalCount;
	int				m_vertical[8];
	int				m_verticalCount;
	bool 			m_foundHotTarget;
	
	RobotLog*       m_robotLog;
	char            m_log[100];
	
public:
	FindTarget(RobotLog *logDelegate);
	void InitDefaultCommand();
	
	void Init();
	bool IsFinished();
	void StartCamera(const char* cameraIP);
	bool GetHotTarget();
};

#endif
