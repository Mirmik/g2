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

void g2::incoming(g1::packet* pack) {
	g2::subheader* sh = (g2::subheader*) pack->dataptr();
}