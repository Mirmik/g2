#include <g2/core.h>

gxx::dlist<g2::socket, &g2::socket::socklnk> g2::sockets;
//gxx::dlist<g2::server, &g2::server::servlnk> g2::servers;

g2::socket_ref g2::create_socket() {
	/*g2::socket* sock = new g2::socket;
	dlist_init(&sock->socklnk);
	sock->sockid = port;
	g2::sockets.move_back(*sock);

	return g2::socket_ref(sock);*/
}

/*g2::server_ref g2::create_server(int port) {
	g2::server* serv = new g2::server;
	dlist_init(&serv->servlnk);
	serv->servport = port;
	g2::servers.move_back(*serv);

	return g2::server_ref(serv);

}*/

g2::socket* g2::get_socket(uint16_t port) {
	return nullptr;
}

void g2::send_nack(g1::packet* pack) {}

void g2::incoming(g1::packet* pack) {
	g2::subheader* sh = (g2::subheader*) pack->dataptr();
	socket* rsock = get_socket(sh->recvport);

	if (rsock == nullptr) {
		g2::send_nack(pack);
		g1::release(pack);
		return;
	}

	switch (sh->type) {
		case g2_frame_type::CONNECT: {
			if (rsock->state != g2_socket_state::ACCEPTER) {
				gxx::println("warn: connect to non accepter socket");
			}

			g2::socket* dsock = generate_dynsock();
			dsock->state = g2_socket_state::CONNECTED;
			g2::send_conect_answer(dsock);
			rsock->accepthandler(dsock);
			break; 
		}
		case g2_frame_type::CONNECT_ANSWER: break;
		case g2_frame_type::DATA: break;
	}
	g1::release(pack);
}