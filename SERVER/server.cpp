#pragma comment(lib, "ws2_32.lib")
#include <winsock2.h>
#include <iostream>
#include <sstream>
#include <string>
#include <iomanip>
#include <map>
#pragma warning(disable: 4996)

SOCKET Connections[100];
int Counter = 0;

std::map<std::string, std::string> definitions;

void ClientHandler(int index) {
	int msg_size, send_msg_size;
	while (true) {
		recv(Connections[index], (char*)&msg_size, sizeof(int), NULL);
		char* msg = new char[msg_size + 1];
		msg[msg_size] = '\0';
		recv(Connections[index], msg, msg_size, NULL);

		std::string str;
		if (definitions.find(msg) != definitions.end()) {
			str += (std::string)msg + " — " + definitions.at(msg);
		}
		else {
			str = "Определение не найдено";
		}
		send_msg_size = str.size();
		send(Connections[index], (char*)&send_msg_size, sizeof(int), NULL);
		send(Connections[index], str.c_str(), send_msg_size, NULL);
		delete[] msg;
	}
}

int main(int argc, char* argv[]) {
	setlocale(LC_ALL, "ru");

	std::cout << "Кузнецова Анастасия СП841\n";
	time_t time = std::time(nullptr);
	std::cout << std::put_time(std::localtime(&time), "%d.%m.%Y %H:%M:%S") << "\n\n";

	definitions.insert({ "Компьютер", "устройство или система, способная выполнять заданную, чётко определённую, изменяемую последовательность операций" });
	definitions.insert({ "Язык программирования", "формальный язык, предназначенный для записи компьютерных программ" });
	definitions.insert({ "Программное обеспечение", "программа или множество программ, используемых для управления компьютером" });
	definitions.insert({ "Информатика", "наука о методах и процессах сбора, хранения, обработки, передачи, анализа и оценки информации с применением компьютерных технологий, обеспечивающих возможность её использования для принятия решений" });
	definitions.insert({ "Алгоритм", "конечная совокупность точно заданных правил решения произвольного класса задач или набор инструкций, описывающих порядок действий исполнителя для решения некоторой задачи" });
	definitions.insert({ "Информация", "сведения независимо от формы их представления" });

	//WSAStartup
	WSAData wsaData;
	WORD DLLVersion = MAKEWORD(2, 1);
	if (WSAStartup(DLLVersion, &wsaData) != 0) {
		std::cout << "Error #1" << std::endl;
		exit(1);
	}
	SOCKADDR_IN addr;
	int sizeofaddr = sizeof(addr);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	addr.sin_port = htons(1111);
	addr.sin_family = AF_INET;
	SOCKET sListen = socket(AF_INET, SOCK_STREAM, NULL);
	bind(sListen, (SOCKADDR*)&addr, sizeof(addr));
	listen(sListen, SOMAXCONN);
	SOCKET newConnection;
	for (int i = 0; i < 100; i++) {
		newConnection = accept(sListen, (SOCKADDR*)&addr, &sizeofaddr);
		if (newConnection == 0) {
			std::cout << "Error #2\n";
		}
		else {
			std::cout << "Client connected!\n";
			Connections[i] = newConnection;
			Counter++;
			CreateThread(NULL, NULL,
				(LPTHREAD_START_ROUTINE)ClientHandler, (LPVOID)(i), NULL, NULL);
		}
	}
	system("pause");
	return 0;
}