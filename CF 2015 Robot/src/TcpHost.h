#ifndef TCPHOST_H_
#define TCPHOST_H_

#include "WPILib.h"
#include "RobotLog.h"

class TcpHost
{
public:
	TcpHost(int robotDataCount, int dbDataCount, int dbButtonCount);
	~TcpHost();

	std::string CountReply();										// Returns current state of Robot, DbData, DbButton values
	std::string DataString(int32_t number);							// Returns string with number followed by comma

	bool		GetDashButton(int group, int button);				// Returns state of a Dashboard Button
	bool		GetDashButtonPress(int group, int button);			// Returns whether or not a Dashboard Button has been Pressed
	int32_t		GetDashValue(int index);							// Returns Dashboard value at specified index
	std::string GetReply(int beginIndex, int endIndex);				// Returns GET reply for all Robot Values in the specified range

	bool		SetDashButton(int index, int32_t value);			// Sets Dashboard Button group at specified index
	bool		SetDashValue(int index, int32_t value);				// Sets Dashboard Value at specified index
	bool		SetRobotValue(int index, int32_t value);			// Sets Robot Value at specified index
	void		SetRobotMode(int mode);								// Sets Robot Mode

private:
	struct button {
		int32_t state;
		int32_t pressed;
	};

	Task		m_task;
	int			m_dashboardButtonCount;
	int			m_dashboardValueCount;
	int			m_robotValueCount;
	int			m_robotMode;

	button*		m_dashboardButton;
	int32_t*	m_dashboardValue;
	int32_t*	m_robotValue;
};

#endif
