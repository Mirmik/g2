#include <gxx/print.h>
#include <g1/tower.h>
#include <g1/indexes.h>

#include <g2/core.h>

void g1_incoming(g1::packet* pack) {
	gxx::println("g1_incoming");
	if (pack->header.type = G1_G2TYPE) {
		g2::incoming(pack);
	}
	else {
		g1::release(pack);
	}
}

int main() {
	g1::incoming_handler = g1_incoming;

	auto sock = g2::create_socket(11);
	//sock->state = g2::g2_socket_state::WAIT_HANDSHAKE;

	char buf[128];
	int alen = gxx::hexer_s(buf, 128, ".12.127.0.0.1:10000");

	int rport = 10;
	g2::send_handshake(sock, buf, alen, rport);

	g1::spin();
}