#include "TcpHost.h"
#include "MyRobot.h"

#include <arpa/inet.h>
#include <sstream>

#define BUFFER_LEN 512
#define	TCP_PORT 1180

static void TcpLoop(TcpHost *host) {
	struct sockaddr_in	addrHost, addrClient;
	size_t				position;
	std::string			command;
	std::string			reply;
	std::string			clientMesg;
	unsigned int		clientLen;
	int					clientSocket;
	int					hostSocket;
	int					index;
	int					replySize;

	if ((hostSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {						// Create Host TCP socket
		MyRobot::robotLog->Write("Dashboard: TCP Socket Error");
		return;
	}

	index = 1;
	setsockopt(hostSocket, SOL_SOCKET, SO_REUSEADDR, &index, sizeof(int));					// Allow socket to be reused

	struct timeval tv;																		// Set Timeout on socket recv function
	tv.tv_sec = 10;
	tv.tv_usec = 0;
	setsockopt(hostSocket, SOL_SOCKET, SO_RCVTIMEO, (char *)&tv, sizeof(struct timeval));

	memset(&addrHost, 0, sizeof(addrHost));													// Create and configure Socket address
	addrHost.sin_family = AF_INET;
	addrHost.sin_addr.s_addr = htonl(INADDR_ANY);
	addrHost.sin_port = htons(TCP_PORT);

	if (bind(hostSocket, (struct sockaddr *) &addrHost, sizeof(addrHost)) < 0) {			// Bind address to Host Socket
		MyRobot::robotLog->Write("Dashboard: TCP Binding Error");
		return;
	}

	if (listen(hostSocket, 5) < 0) {														// Open socket to listen for clients
		MyRobot::robotLog->Write("Dashboard: TCP Listen Failed");
		return;
	}

	char buffer[BUFFER_LEN];

	while(true) {
		clientLen = sizeof(addrClient);
																							// Accept connection from Dashboard client
		if ((clientSocket = accept(hostSocket, (struct sockaddr*) &addrClient, &clientLen)) >= 0) {
			MyRobot::robotLog->Write("Dashboard: Connection Accepted");

			while(recv(clientSocket, buffer, BUFFER_LEN, 0) > 0) {							// Wait to receive message from client
//				printf("Client Message: %s\n", buffer);
				clientMesg = std::string(buffer);											// Retrieve message from buffer

				if ((position = clientMesg.find(":")) != std::string::npos) {				// Look for colon at end of command
					command = clientMesg.substr(0, position);								// Parse command
					clientMesg.erase(0, position + 1);										// Erase command from message

					if (command == "COUNT") {												// COUNT command requesting data counts
						reply = host->CountReply();											// Reply from Host

					} else if (command == "GET") {											// GET command requesting Robot data
						if ((position = clientMesg.find(",")) != std::string::npos) {		// Look for comma after begin index
							reply = host->GetReply(atoi(clientMesg.substr(0, position).c_str()),	// Reply from Host
												   atoi(clientMesg.substr(position + 1).c_str()));
						}

					} else if (command == "PUT") {											// PUT command sending Dashboard data
						reply = "PUT:";

						while ((position = clientMesg.find("|")) != std::string::npos) {	// Look for pipe at end of each data packet
							command = clientMesg.substr(0, position);						// Parse data packet
							clientMesg.erase(0, position +1);								// Erase packet from message

							if ((position = command.find(",")) != std::string::npos) {		// Loop for comma after PUT Group (B or V)
								std::string group = command.substr(0, position);			// Parse group
								command.erase(0, position + 1);								// Erase group from packet

								if ((position = command.find(",")) != std::string::npos) {	// Look for comma after PUT Index
									index = atoi(command.substr(0, position).c_str());		// Parse index

									if (group == "V") {										// Set Dashboard Value at index
										if (host->SetDashValue(index, atoi(command.substr(position + 1).c_str()))) {
											reply += "V," + host->DataString(index);		// Acknowledge reply
										}
									} else if (group == "B") {								// Set Dashboard Button at index
										if (host->SetDashButton(index, atoi(command.substr(position + 1).c_str()))) {
											reply += "B," + host->DataString(index);		// Acknowledge reply
										}
									}
								}
							}
						}

						reply += "\r\n";
					}

					replySize = reply.length();

					if (replySize > 0) {													// Send reply to Dashboard client
						memcpy(buffer, reply.data(), BUFFER_LEN);
						if (send(clientSocket, buffer, replySize, 0) != replySize) MyRobot::robotLog->Write("Dashboard: TCP Send Error");
					}
				}
			}

			MyRobot::robotLog->Write("Dashboard: Connection Lost");
		}
	}
}

TcpHost::TcpHost(int robotValueCount, int dbValueCount, int dbButtonCount) : m_task("TcpHost", (FUNCPTR)TcpLoop) {
	m_robotMode = 0;

	m_robotValueCount		= robotValueCount;
	m_dashboardValueCount	= dbValueCount;
	m_dashboardButtonCount	= dbButtonCount;

	m_robotValue 			= new int32_t[m_robotValueCount];
	m_dashboardValue		= new int32_t[m_dashboardValueCount];
	m_dashboardButton		= new button[m_dashboardButtonCount];

	for (int i = 0; i < m_robotValueCount; i++) m_robotValue[i] = 0;
	for (int i = 0; i < m_dashboardValueCount; i++) m_dashboardValue[i] = 0;
	for (int i = 0; i < m_dashboardButtonCount; i++) {
		m_dashboardButton[i].state = 0;
		m_dashboardButton[i].pressed = 0;
	}

	if (!m_task.Start((int32_t)this)) MyRobot::robotLog->Write("Dashboard: Host Failed to Start");
}

TcpHost::~TcpHost() {
	m_task.Stop();
	delete &m_task;
	delete [] m_robotValue;
	delete [] m_dashboardValue;
	delete [] m_dashboardButton;
}

std::string TcpHost::CountReply() {
	return "COUNT:" + DataString(m_robotMode) + DataString(m_robotValueCount)
					+ DataString(m_dashboardValueCount) + DataString(m_dashboardButtonCount) + "\r\n";
}

std::string TcpHost::DataString(int32_t number) {
	std::stringstream ss;
	ss << number;
	return ss.str() + ",";
}

bool TcpHost::GetDashButton(int group, int button) {
	if (button < 16 && group < m_dashboardButtonCount) {
		return ((m_dashboardButton[group].state & (1 << button)) != 0);
	} else {
		return false;
	}
}

bool TcpHost::GetDashButtonPress(int group, int button) {
	bool vReturn = false;

	if (button < 16 && group < m_dashboardButtonCount) {
		int32_t buttonValue = 1 << button;

		if ((m_dashboardButton[group].state & buttonValue) != 0) {
			vReturn = ((m_dashboardButton[group].pressed & buttonValue) == 0);
			m_dashboardButton[group].pressed |= buttonValue;

		} else if ((m_dashboardButton[group].pressed) != 0) {
			m_dashboardButton[group].pressed ^= buttonValue;
		}
	}

	return vReturn;
}

int32_t TcpHost::GetDashValue(int index) {
	if (index < m_dashboardValueCount) return m_dashboardValue[index];
	return 0;
}

std::string TcpHost::GetReply(int beginIndex, int endIndex) {
	std::string data;

	if (beginIndex < 0 || beginIndex >= m_robotValueCount || endIndex < beginIndex || endIndex >= m_robotValueCount ) {
		data = "GET:" + DataString(m_robotMode);
	} else {
		data = "GET:" + DataString(m_robotMode) + DataString(beginIndex);
		for (int i = beginIndex; i <= endIndex; i++) data += DataString(m_robotValue[i]);
	}

	return data += "\r\n";
}

bool TcpHost::SetDashButton(int index, int32_t value) {
	if (index < 0 || index >= m_dashboardButtonCount) {
		return false;
	} else {
		m_dashboardButton[index].state = value;
		return true;
	}
}

bool TcpHost::SetDashValue(int index, int32_t value) {
	if (index < 0 || index >= m_dashboardValueCount) {
		return false;
	} else {
		m_dashboardValue[index] = value;
		return true;
	}
}

bool TcpHost::SetRobotValue(int index, int32_t value) {
	if (index < 0 || index >= m_robotValueCount) {
		return false;
	} else {
		m_robotValue[index] = value;
		return true;
	}
}

void TcpHost::SetRobotMode(int mode) {
	m_robotMode = mode;
}

