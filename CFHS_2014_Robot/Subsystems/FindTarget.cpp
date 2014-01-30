#include "FindTarget.h"
#include "../Robotmap.h"
#include <math.h>

FindTarget::FindTarget() : Subsystem("FindTarget") {
	m_camera = NULL;
}
    
void FindTarget::InitDefaultCommand() {
	// Set the default command for a subsystem here.
	//SetDefaultCommand(new MySpecialCommand());
}

void FindTarget::Init() {
	m_findStep = iStart;
}

bool FindTarget::IsFinished() {
	static double startTime;
	
	static bool			vFound = false;
	static TargetData	vTarget;
	static TargetData	hTarget[8];
	static int			hCount;
	static double		rectLong, rectShort;
	static ColorImage* 	cameraImage;
	static BinaryImage* filterImage;
	
	if(m_findStep == iStart) {
		m_foundHotTarget = false;
		hCount = 0;
		startTime = GetClock() * 1000;
		whichTarget = wtUnknown;
		m_findStep = iGetImage;
	}
	
	if(m_findStep == iGetImage) {
		cameraImage = m_camera->GetImage();
		printf("GetImage Time=%f\n", GetClock() * 1000 - startTime);
		m_findStep = iFilterImage;
		
	} else if(m_findStep == iFilterImage) {
		filterImage = cameraImage->ThresholdHSL(10, 120, 70, 255, 70, 255);
		printf("Color Threshold Time=%f\n", GetClock() * 1000 - startTime);
		m_findStep = iParticleFilter;
		
	} else if(m_findStep == iParticleFilter) {
		ParticleFilterCriteria2  filterCriteria[] = {
				{IMAQ_MT_AREA, 150, 65535, false, false}
		};
		filterImage = filterImage->ParticleFilter(filterCriteria, 1);
		printf("Particle Filter Time=%f\n", GetClock() * 1000 - startTime);
		m_findStep = iImageAnalysis;
		
	} else if(m_findStep == iImageAnalysis) {
		vector<ParticleAnalysisReport>* parReports = filterImage->GetOrderedParticleAnalysisReports();
		int parCount = parReports->size();
		
		if (parCount > 0) {
			printf("Found %d Targets\n", parCount);
			
			for (int i = 0; i < 8 && i < parCount; i++) {
				ParticleAnalysisReport* par = &(parReports->at(i));
				double rectArea = par->boundingRect.width * par->boundingRect.height;
				
				if (rectArea != 0) {
					if (par->particleArea / rectArea > 0.5) {
						imaqMeasureParticle(filterImage->GetImaqImage(), par->particleIndex, 0, IMAQ_MT_EQUIVALENT_RECT_LONG_SIDE, &rectLong);
						imaqMeasureParticle(filterImage->GetImaqImage(), par->particleIndex, 0, IMAQ_MT_EQUIVALENT_RECT_SHORT_SIDE, &rectShort);
						printf("%d Equivalent: Long=%f  Short=%f\n", i, rectLong, rectShort);
						
						double aspectRatio = rectLong / rectShort;
						printf("%d: Aspect Ratio=%f\n", i, aspectRatio);
						
						if (par->boundingRect.height > par->boundingRect.width) {
							aspectRatio = fabs(aspectRatio / 8); 
							
							if (aspectRatio > 0.6) {
								if (!vFound || fabs(par->center_mass_x_normalized) < fabs(vTarget.centerX)) {
									vTarget.centerX = par->center_mass_x_normalized;
									vTarget.rectHeight = par->boundingRect.height;
									vTarget.rectLeft = par->boundingRect.left;
									vTarget.rectTop = par->boundingRect.top;
									vTarget.rectLong = rectLong;
									vTarget.rectShort = rectShort;
									vFound = true;
									printf("%d: Vertical Target\n", i);
								}
							}
						} else {
							aspectRatio = fabs(aspectRatio / 5.875);
									
							if (aspectRatio > 0.6) {
								hTarget[hCount].centerX = par->center_mass_x_normalized;
								hTarget[hCount].rectHeight = par->boundingRect.height;
								hTarget[hCount].rectLeft = par->boundingRect.left;
								hTarget[hCount].rectTop = par->boundingRect.top;
								hTarget[hCount].rectLong = rectLong;
								hTarget[hCount].rectShort = rectShort;
								hCount++;
								printf("%d: Horizontal Target\n", i);
							}
						}
					}
				}
			}

			delete cameraImage;
			delete filterImage;
			delete parReports;

			if (vFound) {
				double distance = 10000 / min((double)vTarget.rectHeight, vTarget.rectLong);
				
				for (int i = 0; i < hCount; i++) {
					if (max(vTarget.rectShort, hTarget[i].rectShort) / min(vTarget.rectShort, hTarget[i].rectShort) < 1.5) {
						int yDiff = vTarget.rectTop - hTarget[i].rectTop;
						printf("yDiff=%d  rectShort=%f\n", yDiff, hTarget[i].rectShort);
						
						if (yDiff > 0 && yDiff < hTarget[i].rectShort) {
							if (vTarget.rectLeft > hTarget[i].rectLeft) {
								int xDiff = vTarget.rectLeft - hTarget[i].rectLeft - (int)hTarget[i].rectLong;
								if (xDiff > 0 && xDiff < vTarget.rectShort * 4) {
									whichTarget = wtLeft;
									printf("Found Hot Left Target at %f\n", distance);
									m_foundHotTarget = true;
								}
								
							} else {
								int xDiff = hTarget[i].rectLeft - vTarget.rectLeft;
								if (xDiff > 0 && xDiff < vTarget.rectShort * 4) {
									whichTarget = wtRight;
									printf("Found Hot Right Target at %f\n", distance);
									m_foundHotTarget = true;
								}
							}
						}
					}
				}
				
				if (whichTarget == wtUnknown) printf("Found Target at %f\n", distance);
			}
			
		}
		
		printf("Image Analysis Time=%f\n", GetClock() * 1000 - startTime);
		return true;
	}
	
	return false;
}

bool FindTarget::GetHotTarget() {
	return m_foundHotTarget;
}

void FindTarget::StartCamera(const char* cameraIP) {
	if (m_camera == NULL) m_camera = &AxisCamera::GetInstance(cameraIP);
}
