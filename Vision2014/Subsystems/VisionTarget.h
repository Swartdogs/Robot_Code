#ifndef VISION_TARGET_H
#define VISION_TARGET_H

#include "Commands/Subsystem.h"
#include "WPILib.h"
#include "Vision/RGBImage.h"
#include "Vision/BinaryImage.h"
#include "Math.h"

/**
 *
 *
 * @author ExampleAuthor
 */
class VisionTarget: public Subsystem {
private:
	struct Scores {
		double rectangularity;
		double aspectRatioVertical;
		double aspectRatioHorizontal;
	};
	
	struct TargetReport {
		int verticalIndex;
		int horizontalIndex;
		bool Hot;
		double totalScore;
		double leftScore;
		double rightScore;
		double tapeWidthScore;
		double verticalScore;
	};

	Scores*	        scores;
	TargetReport	target;
	int				verticalTargets[8];
	int				horizontalTargets[8];
	int				verticalTargetCount, horizontalTargetCount;
	AxisCamera*		m_Camera;

	double 	computeDistance(BinaryImage *image, ParticleAnalysisReport *report);
	double 	scoreAspectRatio(BinaryImage *image, ParticleAnalysisReport *report, bool vertical);
	bool 	scoreCompare(Scores scores, bool vertical);
	double 	scoreRectangularity(ParticleAnalysisReport *report);
	double 	ratioToScore(double ratio);
	bool 	hotOrNot(TargetReport target);


public:
	VisionTarget();
	void InitDefaultCommand();
	void FindTargets();
	void StartCamera(const char *cameraIP);
};

#endif
