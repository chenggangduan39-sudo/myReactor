all:server client

client:Logger.cpp client.cpp
	g++ Logger.cpp client.cpp -o client

server:Logger.cpp server.cpp InetAddress.cpp Socket.cpp
	g++ Logger.cpp server.cpp InetAddress.cpp Socket.cpp -o server

clean:
	rm client server
