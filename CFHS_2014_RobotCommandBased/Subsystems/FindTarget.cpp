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

void FindTarget::Find() {
	
	double StartTime = GetClock() * 1000;
	
	ColorImage*		cameraImage;
	BinaryImage* 	filterImage;
	
	m_horizontalCount = m_verticalCount = 0;
	
	cameraImage = m_camera->GetImage();
	printf("GetImage Time=%f\n", GetClock() * 1000 - StartTime);
	
	filterImage = cameraImage->ThresholdHSL(10, 120, 70, 255, 70, 255);
	printf("Color Threshold Time=%f\n", GetClock() * 1000 - StartTime);
	
	ParticleFilterCriteria2  filterCriteria[] = {
			{IMAQ_MT_AREA, 150, 65535, false, false}
	};

	filterImage = filterImage->ParticleFilter(filterCriteria, 1);
	printf("Particle Filter Time=%f\n", GetClock() * 1000 - StartTime);
	
	vector<ParticleAnalysisReport> *reports = filterImage->GetOrderedParticleAnalysisReports();
	int reportCount = reports->size();
	
	if (reportCount > 0) {
		printf("Found %d Targets\n", reportCount);
		
		for (int pi = 0; pi < 8 && pi < reportCount; pi++) {
			m_report[pi].par = &(reports->at(pi));
			double rectArea = m_report[pi].par->boundingRect.width * m_report[pi].par->boundingRect.height;
			
			if (rectArea != 0) {
				if (m_report[pi].par->particleArea / rectArea > 0.5) {
					imaqMeasureParticle(filterImage->GetImaqImage(), m_report[pi].par->particleIndex, 0, IMAQ_MT_EQUIVALENT_RECT_LONG_SIDE, &m_report[pi].rectLong);
					imaqMeasureParticle(filterImage->GetImaqImage(), m_report[pi].par->particleIndex, 0, IMAQ_MT_EQUIVALENT_RECT_SHORT_SIDE, &m_report[pi].rectShort);
					printf("%d Equivalent: Long=%f  Short=%f\n", pi, m_report[pi].rectLong, m_report[pi].rectShort);
					
					double aspectRatio = m_report[pi].rectLong / m_report[pi].rectShort;
					printf("%d: Aspect Ratio=%f\n", pi, aspectRatio);
					
					if (m_report[pi].par->boundingRect.height > m_report[pi].par->boundingRect.width) {
						aspectRatio = fabs(1.0 - aspectRatio / 8); 
						
						if (aspectRatio < 0.4) {
							m_vertical[m_verticalCount++] = pi; 
							printf("%d: Vertical Target\n", pi);
						}
					} else {
						aspectRatio = fabs(1.0 - aspectRatio / 5.875);
								
						if (aspectRatio < 0.4) {
							m_horizontal[m_horizontalCount++] = pi;
							printf("%d: Horizontal Target\n", pi);
						}
					}
				}
			}
		}

		if (m_verticalCount > 0) {
			for (int vi = 0; vi < m_verticalCount; vi++) {
				m_report[vi].isHotTarget = false;
				m_report[vi].whichTarget = wtUnknown;
				
				for (int hi = 0; hi < m_horizontalCount; hi++) {
					if (max(m_report[vi].rectShort, m_report[hi].rectShort) / min(m_report[vi].rectShort, m_report[hi].rectShort) < 1.5) {
						int yDiff = m_report[vi].par->boundingRect.top - m_report[hi].par->boundingRect.top;
						
						if (yDiff > 0 && yDiff < m_report[hi].rectShort) {
							if (m_report[vi].par->boundingRect.left > m_report[hi].par->boundingRect.left) {
								int xDiff = m_report[vi].par->boundingRect.left - m_report[hi].par->boundingRect.left - (int)m_report[hi].rectLong;
								if (xDiff > 0 && xDiff < m_report[vi].rectShort * 8) {
									m_report[vi].isHotTarget = true;
									m_report[vi].whichTarget = wtLeft;
									printf("%d: Found Hot Left Target\n", vi);
								}
								
							} else  {
								int xDiff = m_report[hi].par->boundingRect.left - m_report[vi].par->boundingRect.left;
								if (xDiff > 0 && xDiff < m_report[vi].rectShort * 8) {
									m_report[vi].isHotTarget = true;
									m_report[vi].whichTarget = wtRight;
									printf("%d: Found Hot Right Target\n", vi);
								}
							}
						}
					}
				}
				
				if (m_report[vi].whichTarget == wtUnknown) {
					printf("%d: Found Vertical Target\n", vi);
				}
			}
		}
		
	}
	
	delete cameraImage;
	delete filterImage;
	delete reports;
	
	printf("Total Time=%f \n", GetClock() * 1000 - StartTime);
}

void FindTarget::FindOne() {

	double StartTime = GetClock() * 1000;
	
	bool							vFound = false;
	TargetData						vTarget;
	TargetData						hTarget[8];
	int								hCount;
	double							rectLong, rectShort;

	hCount = 0;
	whichTarget = wtUnknown;
	
	ColorImage* cameraImage = m_camera->GetImage();
	printf("GetImage Time=%f\n", GetClock() * 1000 - StartTime);
	
	BinaryImage* filterImage = cameraImage->ThresholdHSL(50, 120, 70, 255, 70, 255);
	printf("Color Threshold Time=%f\n", GetClock() * 1000 - StartTime);
	
	ParticleFilterCriteria2  filterCriteria[] = {
			{IMAQ_MT_AREA, 150, 65535, false, false}
	};
	filterImage = filterImage->ParticleFilter(filterCriteria, 1);
	printf("Particle Filter Time=%f\n", GetClock() * 1000 - StartTime);
	
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
						aspectRatio = fabs(1.0 - aspectRatio / 8); 
						
						if (aspectRatio < 0.4) {
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
						aspectRatio = fabs(1.0 - aspectRatio / 5.875);
								
						if (aspectRatio < 0.4) {
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
			double distance = 11400 / min((double)vTarget.rectHeight, vTarget.rectLong);
			
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
							}
							
						} else {
							int xDiff = hTarget[i].rectLeft - vTarget.rectLeft;
							if (xDiff > 0 && xDiff < vTarget.rectShort * 4) {
								whichTarget = wtRight;
								printf("Found Hot Right Target at %f\n", distance);
							}
						}
					}
				}
			}
			
			if (whichTarget == wtUnknown) printf("Found Target at %f\n", distance);
		}
		
	}
	
	printf("Total Time=%f \n", GetClock() * 1000 - StartTime);
}

bool FindTarget::PeriodicFind() {
	static ImageProcessStep ImageStepNow = iIdle;
	double StartTime = GetClock() * 1000;
	
	static bool							vFound = false;
	static TargetData						vTarget;
	static TargetData						hTarget[8];
	static int								hCount;
	static double							rectLong, rectShort;
	static ColorImage* cameraImage;
	static BinaryImage* filterImage;
	
	if(ImageStepNow == iIdle) {
		foundHotTarget = false;
		hCount = 0;
		whichTarget = wtUnknown;
		ImageStepNow = iGetImage;
		
	} else if(ImageStepNow == iGetImage) {
		cameraImage = m_camera->GetImage();
		printf("GetImage Time=%f\n", GetClock() * 1000 - StartTime);
		ImageStepNow = iFilterImage;
		
	} else if(ImageStepNow == iFilterImage) {
		filterImage = cameraImage->ThresholdHSL(10, 120, 70, 255, 70, 255);
		printf("Color Threshold Time=%f\n", GetClock() * 1000 - StartTime);
		ImageStepNow = iParticleFilter;
		
	} else if(ImageStepNow == iParticleFilter) {
		ParticleFilterCriteria2  filterCriteria[] = {
				{IMAQ_MT_AREA, 150, 65535, false, false}
		};
		filterImage = filterImage->ParticleFilter(filterCriteria, 1);
		printf("Particle Filter Time=%f\n", GetClock() * 1000 - StartTime);
		ImageStepNow = iImageAnalysis;
		
	} else if(ImageStepNow == iImageAnalysis) {
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
									foundHotTarget = true;
								}
								
							} else {
								int xDiff = hTarget[i].rectLeft - vTarget.rectLeft;
								if (xDiff > 0 && xDiff < vTarget.rectShort * 4) {
									whichTarget = wtRight;
									printf("Found Hot Right Target at %f\n", distance);
									foundHotTarget = true;
								}
							}
						}
					}
				}
				
				if (whichTarget == wtUnknown) printf("Found Target at %f\n", distance);
			}
			
		}
		
		printf("Image Analysis Time=%f\n", GetClock() * 1000 - StartTime);
		ImageStepNow = iIdle;
		return true;
	}
	
	return false;
}

bool FindTarget::GetHotTarget() {
	return foundHotTarget;
}

void FindTarget::StartCamera(const char* cameraIP) {
	if (m_camera == NULL) m_camera = &AxisCamera::GetInstance(cameraIP);
}

void FindTarget::DriveTest(float move, float rotate) {
	
	float left1, left2, right1, right2;
	static int count = 10;
	
	if (count > 0) {
		count--;

	} else {
		count = 10;
		Arcade1(move, rotate, left1, right1);
		Arcade2(move, rotate, left2, right2);
		
		printf("Move=%f  Rotate=%f   Left=%f  %f   Right=%f  %f \n", move, rotate, left1, left2, right1, right2);
	}
}


void FindTarget::Arcade1(float move, float rotate, float& left, float& right) {
	
	float max;
	
	left = move - rotate;
	right = move + rotate;

	max = (left > right) ? left : right;

	if (max > 1.0) {
		left = left / max;
		right = right / max;
	}
}

void FindTarget::Arcade2(float move, float rotate, float& left, float& right) {

	if (move > 0.0) {
		if (rotate > 0.0) {
			left = move - rotate;
			right = max(move, rotate);
		} else {
			left = max(move, -rotate);
			right = move + rotate;
		}
	} else {
		if (rotate > 0) {
			left = -max(-move, rotate);
			right = move + rotate;
		} else {
			left = move - rotate;
			right = -max(-move, -rotate);
		}
	}
}
