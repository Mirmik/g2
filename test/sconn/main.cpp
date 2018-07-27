#include <gxx/print.h>
#include <g1/tower.h>
#include <g1/gates/udpgate.h>
#include <g1/indexes.h>

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
		default:
			g1::release(pack);
			break;
	}
}

void trav(g1::packet* pack) {
	gxx::print("traveling: "); g1::println(pack);
}

int main() {
	g1::traveling_handler = trav;
	g1::create_udpgate(10009);

	g1::incoming_handler = g1_incoming;
	auto testch = g2::create_echo_channel(2);
	g1::spin();
}