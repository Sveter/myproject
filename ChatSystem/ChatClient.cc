#include <iostream>
#include "ChatClient.hpp"

static void Menu(int &s)
{
	std::cout << "##################################" << std::endl;
	std::cout << "###1. register 2. login 3. exit###" << std::endl;
	std::cout << "##################################" << std::endl;
	std::cout << "Please select> ";
	std::cin >> s;
}

void Usage(std::string proc){
	std::cout << "Usage: " << proc << "peer_ip" << std::endl;
}

int main(int argc, char* argv[])
{
	if(argc != 2){
		Usage(argv[0]);
		exit(1);
	}

	ChatClient *cp = new ChatClient(argv[1]);
	cp->ClientInit();

	int select = 0;
	while(1){
		Menu(select);
		switch(select){
			case 1:
				cp->Register();
				break;
			case 2:
				if(cp->Login()){
					cp->Chat();
				}
				break;
			case 3:
				exit(0);
				break;
			default:
				exit(1);
				break;
		}
	}
	//if(cp->ConnectServer()){
	//	std::cout << "connect success!" << std::endl;
	//}

	return 0;
}
