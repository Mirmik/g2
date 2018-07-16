#ifndef G2_CORE_H
#define G2_CORE_H

#include <gxx/io/iostorage.h>
#include <g1/tower.h>
#include <gxx/syslock.h>
#include <gxx/refcontrol.h>

namespace g2 {
	extern gxx::syslock lock;

	struct subheader {
		uint16_t sid;
		uint16_t rid;
		uint16_t seqid;
	};

	struct socket {//: public gxx::io::ostream, public gxx::io::istorage {
		dlist_head socklnk;
		uint16_t sockid;

		uint8_t  tgtid;
		uint8_t* addr;
		size_t   alen; 
		gxx::dlist<g1::packet, &g1::packet::lnk> unreaded;

		//char * creadbf;
		//size_t creadsz;

		uint16_t sendid;
		uint16_t readid;

		uint8_t status;

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

	struct server {

	};

	struct socket_ref : public gxx::refcontrol<socket, &socket::refs> {
		using refcontrol = gxx::refcontrol<socket, &socket::refs>;
		socket_ref(socket* sock) : refcontrol(sock) {}
	};

	extern gxx::dlist<g2::socket, &g2::socket::socklnk> sockets;

	void incoming(g1::packet* pack);
	void print(g1::packet* pack);

	socket_ref create_socket(int port);
};

#endif