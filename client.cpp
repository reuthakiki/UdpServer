#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#define ENDMENU 14
#include <iostream>
using namespace std;
#pragma comment(lib, "Ws2_32.lib")
#include <winsock2.h> 
#include <string.h>
#include<windows.h>

#define TIME_PORT	27015
const char* cityArr[] = { " Tokyo (Japan)", " Melbourne (Austrlia)", " San Fransisco (USA)", " Porto (Portugal)"," Other city" };
int TheMenu();

void main()
{

	// Initialize Winsock (Windows Sockets).

	WSAData wsaData;
	if (NO_ERROR != WSAStartup(MAKEWORD(2, 2), &wsaData))
	{
		cout << "Time Client: Error at WSAStartup()\n";
		return;
	}

	// Client side:
	// Create a socket and connect to an internet address.

	SOCKET connSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (INVALID_SOCKET == connSocket)
	{
		cout << "Time Client: Error at socket(): " << WSAGetLastError() << endl;
		WSACleanup();
		return;
	}

	// For a client to communicate on a network, it must connect to a server.

	// Need to assemble the required data for connection in sockaddr structure.

	// Create a sockaddr_in object called server. 
	sockaddr_in server;
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = inet_addr("127.0.0.1");
	server.sin_port = htons(TIME_PORT);
	//char chosenCH[255];
	int chosen = 0;
	int bytesSent = 0;
	int bytesRecv = 0;
	char sendBuff[255];
	char recvBuff[255];

	while (chosen != ENDMENU)
	{
		// Asks the server what's the currnet time.
		// The send function sends data on a connected socket.
		// The buffer to be sent and its size are needed.
		// The fourth argument is an idicator specifying the way in which the call is made (0 for default).
		// The two last arguments hold the details of the server to communicate with. 
		// NOTE: the last argument should always be the actual size of the client's data-structure (i.e. sizeof(sockaddr)).
		chosen = TheMenu();

		if (chosen == 4)
		{
			int FirstTimeOfServer = 0;
			int SecondTimeOfServer = 0;
			int sumOfTime = 0;

			for (int i = 0; i < 100; i++)
			{
				memset(sendBuff, NULL, 255);
				snprintf(sendBuff, sizeof(sendBuff), "%d", 4); //The user input goes into string
				bytesSent = sendto(connSocket, sendBuff, strlen(sendBuff), 0, (const sockaddr*)&server, sizeof(server));
				cout << "Time Client: Sent: " << bytesSent << "/" << strlen(sendBuff) << " bytes of \"" << sendBuff << "\" message.\n";
				if (SOCKET_ERROR == bytesSent)
				{
					cout << "Time Client: Error at sendto(): " << WSAGetLastError() << endl;
					closesocket(connSocket);
					WSACleanup();
					return;
				}
			}
			for (int i = 0; i < 100; i++)
			{
				// Gets the server's answer using simple recieve (no need to hold the server's address).
				bytesRecv = recv(connSocket, recvBuff, 255, 0);
				if (SOCKET_ERROR == bytesRecv)
				{
					cout << "Time Client: Error at recv(): " << WSAGetLastError() << endl;
					closesocket(connSocket);
					WSACleanup();
					return;
				}
				recvBuff[bytesRecv] = '\0'; //add the null-terminating to make it a string
				cout << "Time Client: Recieved: " << bytesRecv << " bytes of \"" << recvBuff << "\" message.\n";
				SecondTimeOfServer = atoi(recvBuff);
				if (FirstTimeOfServer != 0)
				{
					sumOfTime += abs(SecondTimeOfServer - FirstTimeOfServer);
				}
				FirstTimeOfServer = SecondTimeOfServer;
			}
			double avgDelay = sumOfTime / 99.0;
			cout << "Time Client: Recieved: " << avgDelay << " milliseconds\n";
		}
		else if (chosen == 5)
		{
			int SendClientTime = 0;
			int recvclientTime = 0;
			int sumTime = 0;
			for (int i = 0; i < 100; i++)
			{
				memset(sendBuff, NULL, 255);
				snprintf(sendBuff, sizeof(sendBuff), "%d", 5); //The user input goes into string
				SendClientTime = GetTickCount();
				bytesSent = sendto(connSocket, sendBuff, strlen(sendBuff), 0, (const sockaddr*)&server, sizeof(server));
				cout << "Time Client: Sent: " << bytesSent << "/" << strlen(sendBuff) << " bytes of \"" << sendBuff << "\" message.\n";
				if (SOCKET_ERROR == bytesSent)
				{
					cout << "Time Client: Error at sendto(): " << WSAGetLastError() << endl;
					closesocket(connSocket);
					WSACleanup();
					return;
				}
				// Gets the server's answer using simple recieve (no need to hold the server's address).
				bytesRecv = recv(connSocket, recvBuff, 255, 0);
				recvclientTime = GetTickCount();
				if (SOCKET_ERROR == bytesRecv)
				{
					cout << "Time Client: Error at recv(): " << WSAGetLastError() << endl;
					closesocket(connSocket);
					WSACleanup();
					return;
				}
				sumTime += abs(recvclientTime - SendClientTime);
			}
			double avgDelay = sumTime / 100.0;
			cout << "Time Client: Recieved: " << avgDelay << " milliseconds\n";
		}
		else if (chosen == 12)
		{
			int cityChosen;
			char num_char[2];
			cout << "Enter which number of city you want to know the time there?" << endl
			<< "1." << cityArr[0] << endl
			<< "2." << cityArr[1] << endl
			<< "3." << cityArr[2] << endl
			<< "4." << cityArr[3] << endl 
			<< "5." << cityArr[4] << endl;
			cin >> cityChosen;
			while (cityChosen > 5 || cityChosen < 1)
			{
				cout << "Invailed number,please choose city again" << endl;
				cin >> cityChosen;
			}
			memset(sendBuff, NULL, 255);
			snprintf(sendBuff, sizeof(sendBuff), "%d", 12); //The user input goes into string
			//sendBuff[2] = ' ';
			sprintf(num_char, "%d", cityChosen-1);
			//num_char[2] = '/0';
			strcat(sendBuff, num_char);//which city
			bytesSent = sendto(connSocket, sendBuff, strlen(sendBuff), 0, (const sockaddr*)&server, sizeof(server));
			cout << "Time Client: Sent: " << bytesSent << "/" << strlen(sendBuff) << " bytes of \"" << sendBuff << "\" message.\n";
			if (SOCKET_ERROR == bytesSent)
			{
				cout << "Time Client: Error at sendto(): " << WSAGetLastError() << endl;
				closesocket(connSocket);
				WSACleanup();
				return;
			}
			bytesRecv = recv(connSocket, recvBuff, 255, 0);
			if (SOCKET_ERROR == bytesRecv)
			{
				cout << "Time Client: Error at recv(): " << WSAGetLastError() << endl;
				closesocket(connSocket);
				WSACleanup();
				return;
			}
			recvBuff[bytesRecv] = '\0'; //add the null-terminating to make it a string
			cout << "Time Client: Recieved: " << bytesRecv << " bytes of \"" << recvBuff << "\" message.\n";
		}
		
		else if (chosen != 14) //not 4 or 5 or 12
		{
			memset(sendBuff, NULL, 255);
			snprintf(sendBuff, sizeof(sendBuff), "%d", chosen); //The user input goes into string
			//strcmp(sendBuff, chosenCH);
					//snprintf(newBuff, sizeof(newBuff), "%d", chosen); //The user input goes into string
					//cin.getline(sendBuff, 255, '\n'); //The user input goes into string
		// Asks the server what's the currnet time.
		// The send function sends data on a connected socket.
		// The buffer to be sent and its size are needed.
		// The fourth argument is an idicator specifying the way in which the call is made (0 for default).
		// The two last arguments hold the details of the server to communicate with. 
		// NOTE: the last argument should always be the actual size of the client's data-structure (i.e. sizeof(sockaddr)).
			bytesSent = sendto(connSocket, sendBuff, (int)strlen(sendBuff), 0, (const sockaddr*)&server, sizeof(server));
			if (SOCKET_ERROR == bytesSent)
			{
				cout << "Time Client: Error at sendto(): " << WSAGetLastError() << endl;
				closesocket(connSocket);
				WSACleanup();
				return;
			}
			cout << "Time Client: Sent: " << bytesSent << "/" << strlen(sendBuff) << " bytes of \"" << sendBuff << "\" message.\n";

			// Gets the server's answer using simple recieve (no need to hold the server's address).
			bytesRecv = recv(connSocket, recvBuff, 255, 0);
			if (SOCKET_ERROR == bytesRecv)
			{
				cout << "Time Client: Error at recv(): " << WSAGetLastError() << endl;
				closesocket(connSocket);
				WSACleanup();
				return;
			}

			recvBuff[bytesRecv] = '\0'; //add the null-terminating to make it a string
			cout << "Time Client: Recieved: " << bytesRecv << " bytes of \"" << recvBuff << "\" message.\n";

		}
		else /*if (chosen = 14)*/
		{
		// Closing connections and Winsock.
		cout << "Time Client: Closing Connection.\n";
		closesocket(connSocket);
		}
	}
}


int TheMenu()
{
	int chosen = 0;
	while (chosen < 1 || chosen > 14)
	{
		cout << "Please select the number of request you want : " << endl;
		cout << "1. what's the time? (includes year, month, day, hour, minute and second)" << endl;
		cout << "2. what's the time without date? includes hour, minuteand and second" << endl;
		cout << "3. what's the time Since Epoch? (Time will be printed in seconds as of 1.1.1970.)" << endl;
		cout << "4. return the delay Estimation from Client to Server. " << endl;
		cout << "5. Measure the Round Trip Time (RTT) and return it." << endl;
		cout << "6. what's the time without date or seconds? includes hour and minute." << endl;
		cout << "7. return the year." << endl;
		cout << "8. return the  month and day." << endl;
		cout << "9. return the seconds since begining of month." << endl;
		cout << "10. return the week number from the beginning of the year." << endl;
		cout << "11. return 1 if daylight saving time is set or 0 for winter time." << endl;
		cout << "12. return The time in an hour, minuteand second configuration in different cities in the world." << endl;
		cout << "13. return The time elapsed between the first measure time lap request received from the clientand the second measure time lap request." << endl;
		cout << "14. Exit " << endl;
		cin >> chosen; 
		if (chosen < 1 || chosen > 14)
			cout << "Invalid number, Press again" << endl;
	}

	return chosen;
}

