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

void ssock_handler(void* privdata, g2::event_type etype) {
	switch(etype) {
		case g2::event_type::HANDSHAKE:
			gxx::println("HANDSHAKE event");
			break;

		case g2::event_type::NEWDATA:
			gxx::println("NEWDATA event");
			break;
	}
}

void msock_handler(void* privdata, g2::event_type etype) {
	gxx::println("msock_handler");
}

int main() {
	g1::incoming_handler = g1_incoming;

	auto ssock = g2::create_socket(10);
	auto msock = g2::create_socket(11);

	g2::set_handler(ssock, ssock_handler);
	g2::set_handler(msock, msock_handler);

	ssock->state = g2::g2_socket_state::WAIT_HANDSHAKE;

	int rport = 10;
	g2::send_handshake(msock, "", 0, rport);
	g2::send(msock, "HelloWorld", 10);

	g1::spin();
}