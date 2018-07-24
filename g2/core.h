#ifndef G2_CORE_H
#define G2_CORE_H

#include <gxx/io/iostorage.h>
#include <g1/tower.h>
#include <gxx/syslock.h>
#include <gxx/refcontrol.h>

namespace g2 {
	extern gxx::syslock lock;

	enum class g2_frame_type : uint8_t {
		CONREQUEST = 2,
		HANDSHAKE = 3,
		DATA = 4,
		NACK = 5,
	};

	enum class event_type : uint8_t {
		HANDSHAKE,
		NEWDATA,
	};

	enum class g2_socket_state : uint8_t {
		NONE,
		
		ACCEPTER,
		
		WAIT_HANDSHAKE,
		//CONNECTING,
		BRIDGE,

		//WINDSEER,
	};

	struct subheader {
		g2_frame_type type;
		
		uint8_t sendport;
		uint8_t recvport;
			
		union {
			struct {///< CONNECT
			};

			struct {///< CONNECT_ANSWER
				uint8_t dyn_recvport;
			};

			struct {///< DATA
				//uint8_t datasize;
				uint16_t seqid;
				char data[0];
			};
		};
	} G1_PACKED;

	struct socket {
		dlist_head lnk;
		uint16_t port;

		uint8_t* raddr_ptr;
		size_t   raddr_len; 

		union {
			struct { ///< linked
				uint16_t rport;
				uint16_t sendseq;
				uint16_t readseq;
				dlist_head unreaded;
				void* privdata;
				void (*handler) (void*, event_type);
			};

			struct { ///< accepter
				void (*accepthandler) (socket*);
			};
		};
			

		g2_socket_state state = g2_socket_state::NONE;

		//void send(char* data, size_t size);

		uint8_t refs = 0;

		void ref_get() {
		}
		
		void ref_put() {
			dlist_del(&lnk);
			delete this;
		}

		void send_connect(uint8_t* addr, size_t alen, int servid); 
	};

	static inline subheader* get_subheader(g1::packet* pack) {
		return (subheader*) pack->dataptr();
	}/*

	struct sequence_port : public abstract_port {
		dlist_head lnk;
		uint16_t portid;
		
		g1_so

		gxx::dlist<g1::packet, &g1::packet::lnk> msgs;
	
		void incoming(g1::packet* pack) {

		}
		
		void(*newdatahandler)(void*);
		void* newdatahandler_argument;		
	};*/

	/*struct windseer_port {
		void incomming(g1::packet* pack) override {
			auto sh = get_subheader(pack);
			if (sh->type != g2_frame_type::DATA) {
				g1::realise(pack);
			}

			msgs->move_back(*pack);
			newdatahandler(newdatahandler_argument);
		}
	};*/

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

	extern gxx::dlist<g2::socket, &g2::socket::lnk> sockets;
	//extern gxx::dlist<g2::server, &g2::server::servlnk> servers;

	void incoming(g1::packet* pack);
	void print(g1::packet* pack);

	//socket_ref create_socket();
	//socket_ref create_socket(int port);
	g2::socket* create_socket(uint16_t port);

	uint16_t get_dynamic_id();
	socket* get_socket(uint16_t port);

	void send_nack(g1::packet* pack);
	void send_handshake(g2::socket* sock, const char* addr, uint8_t alen, uint16_t port);

	int send(g2::socket* sock, const char* data, size_t size);

	void set_handler(g2::socket* sock, void(*handler)(void*, g2::event_type), void* privdata = nullptr);
};

#endif