#include <gxx/print.h>
#include <g1/tower.h>
#include <g1/gates/udpgate.h>
#include <g1/indexes.h>

#include <g2/core.h>

#include <gxx/util/gaddr.h>

void g1_incoming(g1::packet* pack) {
	gxx::println("g1_incoming");
}

int main() {
	g1::incoming_handler = g1_incoming;
	g1::create_udpgate(10009);
	
	auto raddr = gxx::gaddr(".12.109.173.108.206:10009");
	g1::send(raddr.data(), raddr.size(), "Mirmik", 6, g1::QoS(2));

	g1::spin();
}