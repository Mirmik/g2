#include <g2/core.h>
#include <gxx/datastruct/iovec.h>

gxx::dlist<g2::socket, &g2::socket::lnk> g2::sockets;
//gxx::dlist<g2::server, &g2::server::servlnk> g2::servers;

/*g2::socket_ref g2::create_socket() {
	/*g2::socket* sock = new g2::socket;
	dlist_init(&sock->socklnk);
	sock->sockid = port;
	g2::sockets.move_back(*sock);

	return g2::socket_ref(sock);*/
//}

/*g2::server_ref g2::create_server(int port) {
	g2::server* serv = new g2::server;
	dlist_init(&serv->servlnk);
	serv->servport = port;
	g2::servers.move_back(*serv);

	return g2::server_ref(serv);

}*/

g2::socket* g2::create_socket(uint16_t port) {
	g2::socket* sock = new g2::socket;
	dlist_init(&sock->lnk);

	sock->port = port;
	sock->handler = nullptr;
	sock->sendseq = 0;
	sock->readseq = 0;

	g2::sockets.move_back(*sock);
	return sock;
}

g2::socket* g2::get_socket(uint16_t port) {
	for ( auto& sock: g2::sockets ) {
		if (sock.port == port) {
			return &sock;
		}
	}
	return nullptr;
}

/*
void g2::send_nack(g1::packet* pack) {}
*/

void unknown_port(g1::packet* pack) {
	g2::subheader* rh = g2::get_subheader(pack);
	g2::subheader h;
	
	h.type = g2::g2_frame_type::NACK;
	h.sendport = rh->recvport;
	h.recvport = rh->sendport;

	g1::send(pack->addrptr(), pack->addrsize(), (char*)&h, sizeof(h), g1::QoS(1));
}

void add_to_messages_list(g2::socket* sock, g1::packet* pack, g2::subheader* sh) {
	///@todo !!!!!!!Добавить в список сообщений в соответствии с порядковым номером.
	sock->messages.move_back(*pack);
}

void g2::incoming(g1::packet* pack) {
	g1::print(pack);

	g2::subheader* sh = g2::get_subheader(pack);
	socket* rsock = get_socket(sh->recvport);

	if (rsock == nullptr) {
		gxx::println("warn: packet to unrecognized port");
		unknown_port(pack);
		g1::release(pack);
		return;
	}

	gxx::fprintln("g2: incomming for socket {} from remote socket {}", rsock->port, sh->sendport);

	switch( rsock->state ) {
		case g2::g2_socket_state::WAIT_HANDSHAKE: {
			gxx::println("trace: WAIT_HANDSHAKE");
			if (sh->type == g2::g2_frame_type::HANDSHAKE) {
				gxx::println("handshake event");
	
				rsock->raddr_ptr = (uint8_t*)strndup((const char*)pack->addrptr(), pack->addrsize());
				rsock->raddr_len = pack->addrsize();
				rsock->rport = sh->sendport;

				rsock->state = g2::g2_socket_state::BRIDGE;
				if (rsock->handler) rsock->handler(rsock->privdata, g2::event_type::HANDSHAKE);
			} else {
				gxx::println("warn: received DATA frame in WAIT_HANDSHAKE state");
			}
			g1::release(pack);
			return;
		}

		case g2::g2_socket_state::BRIDGE: {
			gxx::println("trace: BRIDGE");
			if (pack->addrsize() == rsock->raddr_len && strncmp((const char*)pack->addrptr(), (const char*)rsock->raddr_ptr, rsock->raddr_len) == 0) {
				//gxx::println("datapackage");
				//rsock->messages.move_back(pack);

				//Добавить в список сообщений в соответствии с порядковым номером.
				add_to_messages_list(rsock, pack, sh);

				if (rsock->handler) rsock->handler(rsock->privdata, g2::event_type::NEWDATA);
				return;
			}
			else {
				gxx::println("warn: message to BRIDGE socket with wrong address");
			}
			g1::release(pack);
			return;
		}

		default:
			gxx::println("warn: unrecognized socket_state state");
	}
}

void g2::send_handshake(g2::socket* sock, const char* addr, uint8_t alen, uint16_t port) {
	sock->raddr_ptr = (uint8_t*)strndup(addr, alen);
	sock->raddr_len = alen;
	sock->rport = port;

	g2::subheader h;
	h.type = g2::g2_frame_type::HANDSHAKE;
	h.sendport = sock->port;
	h.recvport = port;

	g1::send(sock->raddr_ptr, sock->raddr_len, (char*)&h, sizeof(h), g1::QoS(1));
}

int g2::send(g2::socket* sock, const char* data, size_t size) {
	g2::subheader sh;
	sh.sendport = sock->port;
	sh.recvport = sock->rport;
	sh.type = g2::g2_frame_type::DATA;

	sh.seqid = sock->sendseq;
	sock->sendseq++;

	gxx::iovec iov[2] = {
		{ &sh, sizeof(sh) },
		{ data, size }
	};

	g1::send(sock->raddr_ptr, sock->raddr_len, iov, g1::QoS(1));
}

void g2::set_handler(g2::socket* sock, void(*handler)(void*, g2::event_type), void* privdata) {
	sock->handler = handler;
	sock->privdata = privdata;
}