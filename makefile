all:server client

client:Logger.cpp client.cpp
	g++ Logger.cpp client.cpp -o client

server:Logger.cpp main.cpp InetAddress.cpp Socket.cpp Epoll.cpp Channel.cpp Acceptor.cpp Connection.cpp TcpServer.cpp EventLoop.cpp Buffer.cpp EchoServer.cpp
	g++ Logger.cpp main.cpp InetAddress.cpp Socket.cpp Epoll.cpp Channel.cpp Acceptor.cpp Connection.cpp TcpServer.cpp EventLoop.cpp Buffer.cpp EchoServer.cpp -o server

clean:
	rm client server
