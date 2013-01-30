// Target Distance: Image Inch Width = Target Inch Width * Image Pixel Width / Target Pixel Width
//					 				 = 62 * 320 / Target Pixel Width
//                                   = 7680 / Target Pixel Width
//					Distance = (Image Inch Width / 2) / Tan(Camera Image Angle / 2)
//							 = (9920 / Tan(24) / Target Pixel Width
//                           = 22280 / Target Pixel Width
//
// (Due to variations in the target image widths resulting from camera angle, the widest target width is used in the
//  distance calculation)

// Target Angle:	Tan = x / y  where x is the normalized location of the target center (-1 to 1)
//					When:  x = 1 the Target Angle is 24 degrees  (Camera Image Angle / 2)
//				           y = 1 / tan(24) = 2.246036
//					Target Angle = atan2(x, y)
//								 = atan2(x, 2.246036)
//
// Camera Settings:		Resolution:	 	320x240
//						Compression:	30
//						Color Level:	100
//						Brightness:		5
//						Sharpness:		0
//						White Balance:	Fixed Outdoor 1
//						Exposure Control:  Hold Current

#include "FindGoals.h"

AxisCamera					   *m_Camera;
vector<ParticleAnalysisReport>	m_Report;

int  	m_GoalDimension;
double  m_GoalCenterX;
char 	m_ShootMode;

FindGoals::FindGoals(){
	m_Camera = NULL;
	m_GoalDimension = 0;
	m_GoalCenterX = 0;
}

FindGoals::~FindGoals(){
	m_Camera->DeleteInstance();
}

int FindGoals::Find(){
	// Call once each periodic loop after the StartFind function is called
	// until a non-negative value is returned
	
	return ProcessImage(fContinue);
}

int FindGoals::Find(char ShootMode){
	// Initiate Camera image analysis
	
	m_ShootMode = ShootMode;
	
	if(m_Camera == NULL){
		return 0;
	}else{
		return ProcessImage(fStart);
	}
}

double FindGoals::GetAngle(){
	// Returns Angle to Goal for the requested Goal Index
	
	return atan2(m_GoalCenterX, 2.246036) * 180 / 3.14159;
}

int FindGoals::GetDistance(){
	// Returns Distance to Goals
	
	if(m_GoalDimension == 0) return 0;
	if(m_ShootMode == 0) return 22280 / m_GoalDimension;
	if(m_ShootMode == 1) return 5390 / m_GoalDimension;
	return 0;
}

int FindGoals::ProcessImage(EnumFindGoals Action){
	// Gets an image from the camera and uses the NI Vision
	// library to find Goal targets
	
	// Processing of each image is broken into 5 steps to minimize interference with the
	// IterativeRobot Periodic functions.
	
	static ColorImage	*CameraImage;
	static BinaryImage	*FilterImage;
	static Image		*BaseImage;
	static int 			 ParticleCount = 0;
	static char			 FindStep = 0;
	
	ParticleFilterCriteria2	FilterCriteria[1];
	ParticleFilterOptions	FilterOptions;
	
	int lowestY = 0;
	int highestY = 0;
	
	if(Action == fStart){
		FindStep = 1;
	}else if(FindStep > 0 && FindStep < 5){
		FindStep++;
	}else{
		return 0;
	}
	
	switch(FindStep){
		case 1:
			m_GoalDimension = 0;
			
			m_GoalCenterX = 0;
			
			if(!m_Camera->IsFreshImage()){
				printf("Find Error");
				return 0;
			}
			if(CameraImage = m_Camera->GetImage()){
				return -1;
				printf("Looking Started");
			}else{
				printf("Find Error");
				return 0;
			}
			
		case 2:
			FilterImage = CameraImage->ThresholdHSI(52, 131, 75, 255, 114, 255);
			delete CameraImage;
			printf("Threshold Set");
			return -1;
			
		case 3:
			BaseImage = FilterImage->GetImaqImage();
			imaqConvexHull(BaseImage, BaseImage, true);
			printf("Targets Filled");
			return -1;
			
		case 4:
			FilterCriteria[0].parameter = IMAQ_MT_AREA;
			FilterCriteria[0].lower = 0;
			FilterCriteria[0].upper = 450;
			FilterCriteria[0].calibrated = 0;
			FilterCriteria[0].exclude = 0;
			
			FilterOptions.connectivity8 = true;
			FilterOptions.rejectMatches = true;
			FilterOptions.rejectBorder = false;
			
			imaqParticleFilter3(BaseImage, BaseImage, FilterCriteria, 1, &FilterOptions, NULL, &ParticleCount);
			printf("Extraneous Particles Removed");
			return -1;
			
		case 5:
			if(ParticleCount){
				m_Report.resize(ParticleCount);
				
				for(int i = 0; i < ParticleCount; i++){
					m_Report[i] = FilterImage->GetParticleAnalysisReport(i);
				}
				
				// Verify that articles are arranged from top to bottom and left to right.
				// Verification is required because slight variations in the Y position on middle targets
				// can result in invlid ordering.
				
				for(int i = 0; i < ParticleCount; i++){
					printf("Properties For Particle %d \n", i);
					printf("CenterX = %d \nWidth = %d \nHeight = %d", m_Report[i].center_mass_x, m_Report[i].boundingRect.width, m_Report[i].boundingRect.height);
					if(m_ShootMode==0){
						if(m_Report[i].boundingRect.width / m_Report[i].boundingRect.height >= 2){
							if(lowestY == 0){
								lowestY = m_Report[i].center_mass_y;
								highestY = m_Report[i].center_mass_y;
								m_GoalCenterX = m_Report[i].center_mass_x_normalized;
								m_GoalDimension = m_Report[i].boundingRect.width;
							} else if(m_Report[i].center_mass_y < lowestY){
								lowestY = m_Report[i].center_mass_y;
								m_GoalCenterX = m_Report[i].center_mass_x_normalized;
							} else if(m_Report[i].center_mass_y >= highestY){
								highestY = m_Report[i].center_mass_y;
								m_GoalDimension = m_Report[i].boundingRect.width;							
							}
						}
					} else if(m_ShootMode==1) {
						if(m_Report[i].boundingRect.width / m_Report[i].boundingRect.height < .5){
							if(lowestY == 0){
								lowestY = m_Report[i].center_mass_y;
								m_GoalCenterX = m_Report[i].center_mass_x_normalized;
								m_GoalDimension = m_Report[i].boundingRect.height;
							} else if(m_Report[i].center_mass_y < lowestY){
								lowestY = m_Report[i].center_mass_y;
								m_GoalCenterX = m_Report[i].center_mass_x_normalized;
								m_GoalDimension = m_Report[i].boundingRect.height;
							}
						}
					}
				}
				
				m_Report.clear();
				printf("Report Cleared");
				
			}
			
			delete FilterImage;
			FindStep = 0;
			printf("Image Deleted");
			if(lowestY != 0){
				return 1;
			} else{
				return 0;
			}
	}
	
	return 0;
}


void FindGoals::StartCamera(const char *cameraIP){
	// Call once to initiate an instance of the Camera
	
	if(m_Camera == NULL) m_Camera = &AxisCamera::GetInstance(cameraIP);
}
