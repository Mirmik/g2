#include <gxx/print.h>
#include <g1/tower.h>
#include <g1/gates/udpgate.h>
#include <g1/indexes.h>

#include <g0/core.h>

#include <g2/core.h>
#include <g2/channel/spam.h>
#include <g2/channel/test.h>
#include <g2/channel/echo.h>

#include <gxx/util/gaddr.h>

#include <thread>
#include <chrono>

void g1_incoming(g1::packet* pack) {
	switch(pack->header.type) {
		case G1_G2TYPE: 
			g2::incoming(pack);
			break;
		case G1_G0TYPE: 
			g0::incoming(pack);
			break;
		default:
			g1::release(pack);
			break;
	}
}

void accept_handler(g2::accept_header* ah) {
	gxx::println("accept_handler");
}

int main() {
	g1::incoming_handler = g1_incoming;

	g2::create_acceptor(28, accept_handler);

	auto ch = g2::create_test_channel(68);

	g2::handshake(ch, 28, "", 0);

	g1::spin();
}