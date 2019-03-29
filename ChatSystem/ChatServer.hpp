#pragma once

#include <iostream>
#include <pthread.h>
#include <string>
#include "ProtocolUtil.hpp"
#include "UserManager.hpp"
#include "Log.hpp"
#include "DataPool.hpp"
#include "Message.hpp"

class ChatServer;
class Param{
	public:
		ChatServer *sp;
		int sock;
		std::string ip;
		int port;
	public:
		Param(ChatServer *sp_, int &sock_, const std::string &ip_, const int &port_):
			sp(sp_),
			sock(sock_),
			ip(ip_),
			port(port_)
		{
		}
		~Param()
		{
		}
};

class ChatServer{
	private:
		int tcp_listen_sock;
		int udp_work_sock;
		int tcp_port;
		int udp_port;
	
		UserManager um;
		DataPool pool;
	public:
		ChatServer(int tcp_port_ = 8080, int udp_port_ = 8888):
			tcp_port(tcp_port_),
			tcp_listen_sock(-1),
			udp_port(udp_port_),
			udp_work_sock(-1)
		{}

		unsigned int RegisterUser(const std::string &name, const std::string &school, const std::string &passwd){
			return um.Insert(name, school, passwd);
		}

		unsigned int LoginUser(const unsigned int &id, const std::string &passwd, const std::string &ip, int port){
			return um.Check(id, passwd);
		}
		//UDP
		void Product()
		{
			std::string message;
			struct sockaddr_in peer;
			Util::RecvMessage(udp_work_sock, message, peer);
			if(!message.empty()){
				Message m;
				m.ToRecvValue(message);
				if(m.Type() == LOGIN_TYPE){
					um.AddOnlineUser(m.ID(), peer);
					std::string name_,school_;
					um.GetUserInfo(m.ID(), name_, school_);
					Message new_msg(name_, school_, m.Text(), m.ID(), m.Type());
					new_msg.ToSendString(message);
				}
				pool.PutMessage(message);
			}
		}

		void Consume()
		{
			std::string message;
			pool.GetMessage(message);
			auto online = um.OnlineUser();
			for(auto it = online.begin(); it != online.end(); it++){
				Util::SendMessage(udp_work_sock, message, it->second);
			}
		}

		void ServerInit(){
			tcp_listen_sock = SocketApi::Socket(SOCK_STREAM);
			udp_work_sock = SocketApi::Socket(SOCK_DGRAM);
			SocketApi::Bind(tcp_listen_sock, tcp_port);
			SocketApi::Bind(udp_work_sock, udp_port);

			SocketApi::Listen(tcp_listen_sock);
		}

		static void* HandlerRequest(void *arg){
			Param *p = (Param *)arg;
			int sock = p->sock;
			ChatServer *sp = p->sp;
			std::string ip = p->ip;
			int port = p->port;
			delete p;
			pthread_detach(pthread_self());
			
			Request rq;
			Util::RecvRequest(sock, rq);

			Json::Value root;
			LOG(rq.text, NORMAL);
			Util::UnSeralizer(rq.text, root);
			if(rq.method == "REGISTER"){
				std::string name = root["name"].asString();
				std::string school = root["school"].asString();
				std::string passwd = root["passwd"].asString();
				unsigned int id = sp->RegisterUser(name, school, passwd);
				//unsigned int id = 10000;
				send(sock, &id, sizeof(id), 0);
			}else if(rq.method == "LOGIN"){
				unsigned int id = root["id"].asInt();
				std::string passwd = root["passwd"].asString();
				//unsigned int result = 10000;
				unsigned int result = sp->LoginUser(id, passwd, ip, port);
				send(sock, &result, sizeof(result), 0);
			}else{
				//LOGOUT
			}
			close(sock);
		}

		void Start(){
			std::string ip;
			int port;
			for(;;){
				int sock = SocketApi::Accept(tcp_listen_sock, ip, port);
				if(sock > 0){
					std::cout << "get a new client " << ip << ":" << port << std::endl;
					Param *p = new Param(this ,sock, ip, port);
					pthread_t tid;
					pthread_create(&tid, NULL, HandlerRequest, p);
				}
			}
		}
		~ChatServer()
		{}
};
