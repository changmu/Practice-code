from socket import *

serverName = '10.0.0.181'
'192.168.204.130'
severPort = 12000
clientSocket = socket(AF_INET, SOCK_DGRAM)
message = raw_input('Input lowercase sentence:')
clientSocket.sendto(message, (serverName, severPort))
modifiedMessage, severAddress = clientSocket.recvfrom(2048)
print modifiedMessage
clientSocket.close()
