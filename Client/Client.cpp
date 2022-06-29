

#pragma comment(lib, "ws2_32.lib")
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <WinSock2.h>
#include <guiddef.h>
#include <combaseapi.h>
#include <string>

int main()
{
    
    WSAData wsaData;
    WORD DLLVersion = MAKEWORD(2, 1);
    if (WSAStartup(DLLVersion, &wsaData) != 0)
    {
        std::cout << "Error: " << WSAGetLastError() << std::endl;
        return 1;
    }
    GUID guidIdentif;
    HRESULT hCreateGuid = CoCreateGuid(&guidIdentif);
    wchar_t wIdentifier[64] = { 0 };
    StringFromGUID2(guidIdentif, wIdentifier, 64);
    std::wstring Identifier(wIdentifier);
    std::wcout <<"Identifier of this client: "<<Identifier << std::endl;
    const int sizeOfBuffer = 36;
    char IdentifierMessage[sizeOfBuffer];
    char CodeMessage[sizeOfBuffer];
    for (int i = 1; i < Identifier.size() - 1; i++)
    {
        IdentifierMessage[i-1] = Identifier[i];
    }

    SOCKADDR_IN addr_1;
    addr_1.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr_1.sin_port = htons(8000);
    addr_1.sin_family = AF_INET;

    SOCKET Connection = socket(AF_INET, SOCK_STREAM, NULL);
    if (connect(Connection, (SOCKADDR*)&addr_1, sizeof(addr_1)) != 0)
    {
        std::cout << "Connection failed" <<std::endl;
        return 1;
    }
    std::cout << "Connected to server" << std::endl;
    send(Connection, IdentifierMessage, sizeOfBuffer, NULL);
    recv(Connection, CodeMessage, sizeOfBuffer, NULL);
    std::cout << "My code is ";
    for (int i = 0; i < sizeOfBuffer; i++)
    {
        std::cout << CodeMessage[i];
    }
    shutdown(Connection, 2);
    closesocket(Connection);
    std::cout << "\nDisconnected from port 8000" << std::endl;

     SOCKADDR_IN addr_2;
    addr_2.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr_2.sin_port = htons(8001);
    addr_2.sin_family = AF_INET;

    SOCKET ConnectionPort8001 = socket(AF_INET, SOCK_STREAM, NULL);
    if (connect(ConnectionPort8001, (SOCKADDR*)&addr_2, sizeof(addr_2)) != 0)
    {
        std::cout << "Connection failed" << std::endl;
        return 1;
    }
    send(ConnectionPort8001, IdentifierMessage, sizeOfBuffer, NULL);
    send(ConnectionPort8001, CodeMessage, sizeOfBuffer, NULL);
    std::cout << "Connected to port 8001" << std::endl;

    char Answer[2];
    recv(ConnectionPort8001, Answer, 2, NULL);
    if (Answer[0] == 'o')
    {

        std::cout << "Enter your message:" << std::endl;
        std::string strMessage;
        std::getline(std::cin, strMessage);
        std::wstring Message = std::wstring(strMessage.begin(), strMessage.end());
        int sizeOfMessage = Message.size();
        char* MessageToSend=new char[sizeOfMessage+1];
        MessageToSend[sizeOfMessage] = '\0';
        for (int j = 0; j < Message.size(); j++)
        {
            MessageToSend[j] = Message[j];
        }
        
        send(ConnectionPort8001, MessageToSend, sizeOfMessage +1, NULL);
    }
    else
        std::cout << "Auth error" << std::endl;




    system("pause");
    return 0;
}

