#define _CRT_SECURE_NO_WARNINGS
#pragma comment(lib, "Ws2_32.lib")
#include <winsock2.h>
#include <windows.h>
#include <iostream>
//#include <iomanip>
using namespace std;
#include <string.h>
#include <time.h>
#include <windows.h>
#define TIME_PORT	27015
#define THREE_MINIT_OF_MILISEC 180000
#define INIT 50


int bytesSent = 0;
int bytesRecv = 0;
char sendBuff[255];
char recvBuff[255];
SOCKET m_socket;
sockaddr client_addr;
int client_addr_len;
int FirstReq;    //save the the first request 



void sendTo() 
{
	bytesSent = sendto(m_socket, sendBuff, (int)strlen(sendBuff), 0, (const sockaddr*)&client_addr, client_addr_len);
	if (SOCKET_ERROR == bytesSent)
	{
		cout << "Time Server: Error at sendto(): " << WSAGetLastError() << endl;
		closesocket(m_socket);
		WSACleanup();
		return;
	}
}

void GetTime()
{

	// Get the current time.
	time_t timer;
	time(&timer);
	// Parse the current time to printable string.
	strcpy(sendBuff, ctime(&timer));
	sendBuff[strlen(sendBuff) - 1] = '\0'; 

// Sends the answer to the client, using the client address gathered
// by recvfrom. 
	sendTo();
}

void GetTimeWithoutDate()
{
	char newBuff[INIT];
	time_t timer = time(NULL);
	tm* tm_local = localtime(&timer);
	snprintf(newBuff, sizeof(newBuff), "The time is: %d:%d:%d", tm_local->tm_hour, tm_local->tm_min, tm_local->tm_sec);
	strcpy(sendBuff, newBuff);
	sendBuff[strlen(sendBuff) ] = '\0'; 

	// Sends the answer to the client, using the client address gathered
	// by recvfrom. 
	sendTo();

}

void GetTimeSinceEpoch()
{
	char newBuff[INIT];
	time_t secsSinceEpoch = time(nullptr);
	snprintf(newBuff, sizeof(newBuff), "number of seconds since 1.1.1970 is:  %d" ,secsSinceEpoch);
	strcpy(sendBuff, newBuff);
	sendBuff[strlen(sendBuff)] = '\0'; 

	// Sends the answer to the client, using the client address gathered
	// by recvfrom. 
	sendTo();
}


void GetClientToServerDelayEstimation() 
{
	char newBuff[INIT];
	snprintf(newBuff, sizeof(newBuff), " %d", GetTickCount());
	strcpy(sendBuff, newBuff);
	sendBuff[strlen(sendBuff)] = '\0'; 
	// Sends the answer to the client, using the client address gathered
	// by recvfrom. 
	sendTo();
}
void MeasureRTT()
{
	char newBuff[INIT];
	snprintf(newBuff, sizeof(newBuff), "Estimated average of the delay is: %d", GetTickCount());
	strcpy(sendBuff, newBuff);
	sendBuff[strlen(sendBuff)] = '\0'; 
	// Sends the answer to the client, using the client address gathered
	// by recvfrom. 
	sendTo();
}

void GetTimeWithoutDateOrSeconds() 
{

	char newBuff[INIT];
	time_t timer = time(NULL);
	tm* tm_local = localtime(&timer);
	snprintf(newBuff, sizeof(newBuff),"The hour and minutes number: %d:%d", tm_local->tm_hour, tm_local->tm_min);
	strcpy(sendBuff, newBuff);
	sendBuff[strlen(sendBuff)] = '\0'; 

	// Sends the answer to the client, using the client address gathered
	// by recvfrom. 
	sendTo();
}

void GetYear() 
{
	char newBuff[INIT];
	time_t timer = time(NULL);
	tm* tm_local = localtime(&timer);
	snprintf(newBuff, sizeof(newBuff),"This year is a number: %d", tm_local->tm_year+1900);
	strcpy(sendBuff, newBuff);
	sendBuff[strlen(sendBuff)] = '\0';

	// Sends the answer to the client, using the client address gathered
	// by recvfrom. 
	sendTo();
}

void GetMonthAndDay()
{
	char newBuff[INIT];
	time_t timer = time(NULL);
	tm* tm_local = localtime(&timer);
	snprintf(newBuff, sizeof(newBuff), "Month number: %d,Day number: %d", tm_local->tm_mon+1, tm_local->tm_wday+1);
	strcpy(sendBuff, newBuff);
	sendBuff[strlen(sendBuff)] = '\0';

	// Sends the answer to the client, using the client address gathered
	// by recvfrom. 
	sendTo();
}

void GetSecondsSinceBeginingOfMonth()
{
	char newBuff[INIT];
	time_t timer = time(NULL);
	time(&timer);
	tm tm_local;
	long int secondsSinceBeginingOfMonth;
	tm_local = *localtime(&timer);
	tm_local.tm_hour = 0;
	tm_local.tm_min = 0;
	tm_local.tm_sec = 0;
	tm_local.tm_mday = 1;
	secondsSinceBeginingOfMonth = difftime(timer, mktime(&tm_local));
	snprintf(newBuff, sizeof(newBuff), " %ld", secondsSinceBeginingOfMonth);
	strcpy(sendBuff, newBuff);
	sendBuff[strlen(sendBuff)] = '\0';

	// Sends the answer to the client, using the client address gathered
	// by recvfrom.   
	sendTo();
}

void GetWeekOfYear()
{
	char newBuff[INIT];
	time_t timer = time(NULL);
	struct tm* tm_local;
	time(&timer);
	tm_local = localtime(&timer);
	strftime(newBuff, sizeof(newBuff), "%W", tm_local);  
	strcpy(sendBuff, newBuff);
	sendBuff[strlen(sendBuff)] = '\0'; 

	// Sends the answer to the client, using the client address gathered
	// by recvfrom.   
	sendTo();
}

void GetDaylightSavings() 
{
	char newBuff[INIT];
	tm tm_local;
	time_t timer;
	time(&timer);
	tm_local = *localtime(&timer);
	snprintf(newBuff, sizeof(newBuff), "%d", tm_local.tm_isdst);
	strcpy(sendBuff, newBuff);
	sendBuff[strlen(sendBuff)] = '\0';

	// Sends the answer to the client, using the client address gathered
	// by recvfrom.   
	sendTo();
}

void GetTimeWithoutDateInCityTokyo()
{
	int TOKYUnum = 6;
	char newBuff[INIT];
	time_t timer = time(NULL);
	tm* tm_local = localtime(&timer);
	int hourCity = tm_local->tm_hour + TOKYUnum;

	if (hourCity >= 24)
	{
		hourCity = (tm_local->tm_hour + TOKYUnum) % 24;
	}
	else if (hourCity < 0)
	{
		hourCity += 24;
	}
	snprintf(newBuff, sizeof(newBuff), "%d:%d:%d", hourCity, tm_local->tm_min, tm_local->tm_sec);
	strcpy(sendBuff, newBuff);
	sendBuff[strlen(sendBuff)] = '\0'; 
	// Sends the answer to the client, using the client address gathered
	// by recvfrom.   
	sendTo();
}


void GetTimeWithoutDateInCityMelbourne()
{
	int MELBOURNEnum = 8;
	char newBuff[INIT];
	time_t timer = time(NULL);
	tm* tm_local = localtime(&timer);
	int hourCity = tm_local->tm_hour + MELBOURNEnum;

	if (hourCity >= 24)
	{
		hourCity = (tm_local->tm_hour + MELBOURNEnum) % 24;
	}
	else if (hourCity < 0)
	{
		hourCity += 24;
	}
	snprintf(newBuff, sizeof(newBuff), "%d:%d:%d", hourCity, tm_local->tm_min, tm_local->tm_sec);
	strcpy(sendBuff, newBuff);
	sendBuff[strlen(sendBuff)] = '\0';
	// Sends the answer to the client, using the client address gathered
	// by recvfrom.   
	sendTo();
}

void GetTimeWithoutDateInCitySanFransisco()
{
	int SANFRANCISCOnum = -10;
	char newBuff[INIT];
	time_t timer = time(NULL);
	tm* tm_local = localtime(&timer);
	int hourCity = tm_local->tm_hour + SANFRANCISCOnum;

	if (hourCity >= 24)
	{
		hourCity = (tm_local->tm_hour + SANFRANCISCOnum) % 24;
	}
	else if (hourCity < 0)
	{
		hourCity += 24;
	}
	snprintf(newBuff, sizeof(newBuff), "%d:%d:%d", hourCity, tm_local->tm_min, tm_local->tm_sec);
	strcpy(sendBuff, newBuff);
	sendBuff[strlen(sendBuff)] = '\0';
	// Sends the answer to the client, using the client address gathered
	// by recvfrom.   
	sendTo();
}

void GetTimeWithoutDateInCityPorto()
{
	int PORTOnum = -2;
	char newBuff[INIT];
	time_t timer = time(NULL);
	tm* tm_local = localtime(&timer);
	int hourCity = tm_local->tm_hour + PORTOnum;

	if (hourCity >= 24)
	{
		hourCity = (tm_local->tm_hour + PORTOnum) % 24;
	}
	else if (hourCity < 0)
	{
		hourCity += 24;
	}
	snprintf(newBuff, sizeof(newBuff), "%d:%d:%d", hourCity, tm_local->tm_min, tm_local->tm_sec);
	strcpy(sendBuff, newBuff);
	sendBuff[strlen(sendBuff)] = '\0';
	// Sends the answer to the client, using the client address gathered
	// by recvfrom.   
	sendTo();
	
}

void GetTimeWithoutDateInCityOtherCity()
{
	int UTCnum = -3;
	char newBuff[INIT];
	time_t timer = time(NULL);
	tm* tm_local = localtime(&timer);
	int hourCity = tm_local->tm_hour + UTCnum;

	if (hourCity >= 24)
	{
		hourCity = (tm_local->tm_hour + UTCnum) % 24;
	}
	else if (hourCity < 0)
	{
		hourCity += 24;
	}
	snprintf(newBuff, sizeof(newBuff), "%d:%d:%d", hourCity, tm_local->tm_min, tm_local->tm_sec);
	strcpy(sendBuff, newBuff);
	sendBuff[strlen(sendBuff)] = '\0';
	// Sends the answer to the client, using the client address gathered
	// by recvfrom.   
	sendTo();
}


void MeasureTimeLap()
{
	char newbuff[INIT];
	int currTime = GetTickCount();
	if (FirstReq == 0 || currTime - FirstReq >= THREE_MINIT_OF_MILISEC)
	{
		FirstReq = currTime;
		snprintf(newbuff, sizeof(newbuff), "First request  accepted and the measurement started");
		strcpy(sendBuff, newbuff);
		sendBuff[strlen(sendBuff)] = '\0';
	}
	else
	{
		snprintf(newbuff, sizeof(newbuff), "The time between the two measurements is %d milliseconds", (currTime - FirstReq));
		strcpy(sendBuff, newbuff);
		sendBuff[strlen(sendBuff)] = '\0';
		FirstReq = 0;
	}
	// Sends the answer to the client, using the client address gathered
// by recvfrom.   
	sendTo();  /////???
}



void main()
{
	// Initialize Winsock (Windows Sockets).

	// Create a WSADATA object called wsaData.
	// The WSADATA structure contains information about the Windows 
	// Sockets implementation.
	WSAData wsaData;

	// Call WSAStartup and return its value as an integer and check for errors.
	// The WSAStartup function initiates the use of WS2_32.DLL by a process.
	// First parameter is the version number 2.2.
	// The WSACleanup function destructs the use of WS2_32.DLL by a process.
	if (NO_ERROR != WSAStartup(MAKEWORD(2, 2), &wsaData))
	{
		cout << "Time Server: Error at WSAStartup()\n";
		return;
	}

	// Server side:
	// Create and bind a socket to an internet address.

	// After initialization, a SOCKET object is ready to be instantiated.

	// Create a SOCKET object called m_socket. 
	// For this application:	use the Internet address family (AF_INET), 
	//							datagram sockets (SOCK_DGRAM), 
	//							and the UDP/IP protocol (IPPROTO_UDP).
	m_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	// Check for errors to ensure that the socket is a valid socket.
	// Error detection is a key part of successful networking code. 
	// If the socket call fails, it returns INVALID_SOCKET. 
	// The "if" statement in the previous code is used to catch any errors that
	// may have occurred while creating the socket. WSAGetLastError returns an 
	// error number associated with the last error that occurred.
	if (INVALID_SOCKET == m_socket)
	{
		cout << "Time Server: Error at socket(): " << WSAGetLastError() << endl;
		WSACleanup();
		return;
	}

	// For a server to communicate on a network, it must first bind the socket to 
	// a network address.

	// Need to assemble the required data for connection in sockaddr structure.

	// Create a sockaddr_in object called serverService. 
	sockaddr_in serverService;
	// Address family (must be AF_INET - Internet address family).
	serverService.sin_family = AF_INET;
	// IP address. The sin_addr is a union (s_addr is a unsigdned long (4 bytes) data type).
	// INADDR_ANY means to listen on all interfaces.
	// inet_addr (Internet address) is used to convert a string (char *) into unsigned int.
	// inet_ntoa (Internet address) is the reverse function (converts unsigned int to char *)
	// The IP address 127.0.0.1 is the host itself, it's actually a loop-back.
	serverService.sin_addr.s_addr = INADDR_ANY;	//inet_addr("127.0.0.1");
	// IP Port. The htons (host to network - short) function converts an
	// unsigned short from host to TCP/IP network byte order (which is big-endian).
	serverService.sin_port = htons(TIME_PORT);

	// Bind the socket for client's requests.

	// The bind function establishes a connection to a specified socket.
	// The function uses the socket handler, the sockaddr structure (which
	// defines properties of the desired connection) and the length of the
	// sockaddr structure (in bytes).
	if (SOCKET_ERROR == bind(m_socket, (SOCKADDR*)&serverService, sizeof(serverService)))
	{
		cout << "Time Server: Error at bind(): " << WSAGetLastError() << endl;
		closesocket(m_socket);
		WSACleanup();
		return;
	}

	// Waits for incoming requests from clients.

	// Send and receive data.
			//sockaddr client_addr;
	client_addr_len = sizeof(client_addr);
	//int bytesSent = 0;
	//int bytesRecv = 0;
	//char sendBuff[255];
	//char recvBuff[255];

	// Get client's requests and answer them.
	// The recvfrom function receives a datagram and stores the source address.
	// The buffer for data to be received and its available size are 
	// returned by recvfrom. The fourth argument is an idicator 
	// specifying the way in which the call is made (0 for default).
	// The two last arguments are optional and will hold the details of the client for further communication. 
	// NOTE: the last argument should always be the actual size of the client's data-structure (i.e. sizeof(sockaddr)).
	cout << "Time Server: Wait for clients' requests.\n";

	while (true)
	{
		bytesRecv = recvfrom(m_socket, recvBuff, 255, 0, &client_addr, &client_addr_len);
		if (SOCKET_ERROR == bytesRecv)
		{
			cout << "Time Server: Error at recvfrom(): " << WSAGetLastError() << endl;
			closesocket(m_socket);
			WSACleanup();
			return;
		}

		recvBuff[bytesRecv] = '\0'; //add the null-terminating to make it a string
		cout << "Time Server: Recieved: " << bytesRecv << " bytes of \"" << recvBuff << "\" message.\n";

		// Answer client's request by the current time.
		switch (atoi(recvBuff))
		{
		case 1:
			GetTime();
			break;
		case 2:
			GetTimeWithoutDate();
			break;
		case 3:
			GetTimeSinceEpoch();
			break;
		case 4:
			GetClientToServerDelayEstimation();
			break;
		case 5:
			MeasureRTT();
			break;
		case 6:
			GetTimeWithoutDateOrSeconds();
			break;
		case 7:
			GetYear();
			break;
		case 8:
			GetMonthAndDay();
			break;
		case 9:
			GetSecondsSinceBeginingOfMonth();
			break;
		case 10:
			GetWeekOfYear();
			break;
		case 11:
			GetDaylightSavings();
			break;
		case 120:
			GetTimeWithoutDateInCityTokyo();
			break;
		case 121:
			GetTimeWithoutDateInCityMelbourne();
			break;
		case 122:
			GetTimeWithoutDateInCitySanFransisco();
		case 123:
			GetTimeWithoutDateInCityPorto();
		case 124:
			GetTimeWithoutDateInCityOtherCity();
			break;
		case 13:
			MeasureTimeLap();
			break;
		}
		
		cout << "Time Server: Sent: " << bytesSent << "\\" << strlen(sendBuff) << " bytes of \"" << sendBuff << "\" message.\n";
	}

	// Closing connections and Winsock.
	cout << "Time Server: Closing Connection.\n";
	closesocket(m_socket);
	WSACleanup();
}