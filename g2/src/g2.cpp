#include <g2/core.h>

gxx::dlist<g2::socket, &g2::socket::socklnk> g2::sockets;

void g2::incoming(g1::packet* pack) {}

g2::socket_ref g2::create_socket(int port) {
	g2::socket* sock = new g2::socket;
	dlist_init(&sock->socklnk);
	sock->sockid = port;
	g2::sockets.move_back(*sock);

	return g2::socket_ref(sock);
}