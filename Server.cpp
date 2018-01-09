#include "thread.h"
#include "socketserver.h"
#include <stdlib.h>
#include <time.h>
#include <list>
#include <vector>
#include <string>
#include <thread>
#include "socket.h"

using namespace Sync;

bool cont = true;

void myThreadFunction(Socket s){ //this gets called 
    ByteArray myData(""); //create byte array
    int ar = s.Read(myData); //tell socket to read the byte array
    std::string stringForm = myData.ToString(); //put into string form

    if (stringForm == "stopserver"){ //condition for stopping the server
        cont = false; //set continuation to false
        ByteArray closing("closing clients"); //create byte array to inform of termination of clients
        s.Write(closing); //write closing message to socket
        return;
    }

    //convert the string to uppercase
    for(int i = 0; i < stringForm.length(); i++){ //loop through string and apply ".toupper" to each character
        stringForm[i] = std::toupper(stringForm[i]);
    }

    std::cout << stringForm << std::endl << std::endl;
    ByteArray returnThis(stringForm); //create byte array with modified string
    int hold = s.Write(returnThis); //write to socket
    s.Close(); //close socket
}

int main(void)
{
    //initial message
    std::cout << "I am a server." << std::endl;

    //creating socketserver at Port: 2000
    SocketServer sockServer(2000);

    while(cont){
        try{
            Socket mySocket = sockServer.Accept(); //accept the connection and return the socket
            std::thread myThread(myThreadFunction, mySocket); //create a thread with a connection to the socket
            myThread.detach();  //allows the thread to exsist independently
        }
        catch(std::string &s){ //handles exception and prints string of exception
            std::cout<< s << std::endl;
        }
    }
    
    try{
        sockServer.Shutdown(); //shut down socketServer
    }
    catch(std::string &s){ //catches errors and prints string of exception
            std::cout<< s << std::endl;
    }
    std::cout<<"Shutting down server"<<std::endl<<std::endl; //server termination
}
