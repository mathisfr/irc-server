# irc-server
## Introduction
Welcome to the ft_irc project! This is a simple but powerful Internet Relay Chat (IRC) server implemented in C++.  
It provides a platform for users to connect, communicate, and exchange messages in real-time.  
This README will guide you through the setup and usage of the ft_irc server.
## Getting Started
To get started with ft_irc, follow the steps below:  
1. Clone the repository:
```bash
git clone https://github.com/your_username/ft_irc.git  
```
2. Change directory to the project folder:
```bash
cd ft_irc 
```
3. Build the project:
```bash
make
```
4. And if you want you can build the bot in bot folder
## Usage
To run the ft_irc server, execute the following command:
```bash
./ft_irc <port> <password>
```
To run the ft_irc bot, execute the following command (in correct bot folder):
```bash
./bot <ip> <port>
```
The server is up and running and has been tested with the IRSSI and Colloquy clients.  
You can download these clients, start the server, and begin communicating.  
However, please be aware that there is an issue with the client-to-client file sharing feature on Colloquy.  
While file sharing is problematic on Colloquy, it works perfectly fine on IRSSI.
