#include <gxx/print.h>
#include <g1/tower.h>
#include <g1/indexes.h>

#include <g2/core.h>

void g1_incoming(g1::packet* pack) {
	gxx::println("g1_incoming");
}

int main() {

	g1::incoming_handler = g1_incoming;

	//g2::socket_ref socket = g2::create_socket(4000);

	//auto serv = g2::create_server(10);

	//g1::send("", 0, "Mirmik", 6);

	g1::onestep();
}