#include "socket.h"
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <string>

using namespace Sync;

int main(void)
{
	//intro message to confirm running of client
	std::cout << "I am a client" << std::endl;
	std::string textIn; //text input variable

	while(true){
		//waiting for string data to be entered
		std::cout<<"Please enter text to send to the server: ";
		std::cin>>textIn;
		std::cout<<std::endl<<std::endl;

		try{
			//creating the socket object
			Socket mySocket("127.0.0.1", 2000);
			mySocket.Open(); //opening the socket
			ByteArray myStrArr(textIn); //creating byte array to send to server
			mySocket.Write(myStrArr); //Writing text to data

			//if statement for shutting down
			if (textIn == "stopserver"){
				//sleep for one second to allow the server to recieve 
				sleep(1);
				Socket lastSocket("127.0.0.1", 2000); //opening to socket to ensure no other blocking calls
				lastSocket.Open(); //open socket
				ByteArray closeArray("Closing Server..."); //create byte array
				lastSocket.Write(closeArray); //write to socket
				lastSocket.Close(); //close socket
				std::cout << closeArray.ToString() << std::endl <<std::endl;
				break;
			}

			//This block of code is dealing with the modified string
			ByteArray modStr("Modified String");
			mySocket.Read(modStr); //read modified string
			std::string modText = modStr.ToString(); //convert to string
			std::cout << modText << std::endl << std::endl;
		}
		catch(std::string &s){ //Catch errors if server is offline.
			std::cout<<"Server Offline."<<std::endl<<std::endl;
			break;
		}
	}
}
