#ifndef G2_CORE_H
#define G2_CORE_H

#include <gxx/io/iostorage.h>
#include <g1/tower.h>
#include <gxx/syslock.h>
#include <gxx/refcontrol.h>

namespace g2 {
	extern gxx::syslock lock;

	enum class g2_frame_type : uint8_t {
		CONNECT,
		CONNECT_ANSWER,
		DATA,
	};

	enum class g2_socket_state : uint8_t {
		NONE,
		
		ACCEPT,
		CONNECTING,
		CONNECTED,

		WINDSEER,
	};

	struct subheader {
		g2_frame_type type;
		//uint16_t recvserv;
		
		uint8_t sendsock;
		uint8_t recvsock;
			
		union {
			//фрейм connect
			struct {};

			//connect answer
			struct {
				uint8_t dyn_recvsock;
			};

			struct {
				uint8_t datasize;
				char data[0];
			};
		};
		//uint16_t sid;
		//uint16_t rid;
		//uint16_t seqid;
	} G1_PACKED;

	struct socket {//: public gxx::io::ostream, public gxx::io::istorage {
		dlist_head socklnk;
		uint16_t sockid;

		uint8_t* raddr_ptr;
		size_t   raddr_len; 
		
		//gxx::dlist<g1::packet, &g1::packet::lnk> unreaded;

		//char * creadbf;
		//size_t creadsz;

		union {
			//linked
			struct {
				uint16_t sendseq;
				uint16_t readseq;
				dlist_head unreaded;
			};

			//accepter
			struct {

			};
		};

		g2_socket_state status;

		//void send(char* data, size_t size);

		uint8_t refs = 0;

		void ref_get() {
		}
		
		void ref_put() {
			dlist_del(&socklnk);
			delete this;
		}

		void send_connect(uint8_t* addr, size_t alen, int servid); 
	};

	/*struct server {
		dlist_head servlnk;
		uint16_t servport;

		gxx::dlist<socket, &socket::socklnk> sockets;

		uint8_t refs = 0;

		void ref_get() {
		}
		
		void ref_put() {
			dlist_del(&servlnk);
			delete this;
		}
	};*/

	struct socket_ref : public gxx::refcontrol<socket, &socket::refs> {
		using refcontrol = gxx::refcontrol<socket, &socket::refs>;
		socket_ref(g2::socket* sock) : refcontrol(sock) {}
	};

	/*struct server_ref : public gxx::refcontrol<server, &server::refs> {
		using refcontrol = gxx::refcontrol<server, &server::refs>;
		server_ref(g2::server* serv) : refcontrol(serv) {}
	};*/

	extern gxx::dlist<g2::socket, &g2::socket::socklnk> sockets;
	//extern gxx::dlist<g2::server, &g2::server::servlnk> servers;

	void incoming(g1::packet* pack);
	void print(g1::packet* pack);

	socket_ref create_socket();
	socket_ref create_socket(int port);
	//server_ref create_server(int port);

	uint16_t get_dynamic_id();
};

#endif