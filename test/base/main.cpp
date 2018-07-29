#include <gxx/print.h>
#include <g1/tower.h>
#include <g1/gates/udpgate.h>
#include <g1/indexes.h>

#include <g2/core.h>
//#include <g2/channel/spam.h>
//#include <g2/channel/test.h>
#include <g2/channel/echo.h>

#include <gxx/util/gaddr.h>

#include <thread>
#include <chrono>

void g1_incoming(g1::packet* pack) {
	switch(pack->header.type) {
		case G1_G0TYPE: 
			g0::incoming(pack);
			break;
		default:
			g1::release(pack);
			break;
	}
}

g2::channel* accept_channel() {
	return g2::create_echo_channel(g2::dynport());
}

int main() {
	g1::incoming_handler = g1_incoming;

	auto acceptor = g2::create_acceptor(33, accept_channel);

	/*auto spamch = g2::create_test_channel(1);
	auto testch = g2::create_echo_channel(2);

	std::string raddr = gxx::gaddr("");
	g2::handshake(spamch, 2, raddr.data(), raddr.size(), g1::QoS(0));
	spamch->send("HelloWorld", 10);

	auto thr = new std::thread(g1::spin);


	std::this_thread::sleep_for(std::chrono::milliseconds(100));

	gxx::println((int)spamch->state);*/
}