#ifndef G2_CHANNEL_ECHO_H
#define G2_CHANNEL_ECHO_H

#include <g2/core.h>
#include <gxx/print/stdprint.h>

namespace g2 {
	struct echo_channel : public channel {
		void incoming_packet(g1::packet* pack) override {
			gxx::println("incoming_packet");
			auto data = g2::get_datasect(pack);
			gxx::println(data);

			gxx::println("pretest");
			dprhexln(this);
			g2::send(this, "TEST", 4);
			gxx::println("aftertest");

			g1::release(pack);
		} 
	};

	echo_channel* create_echo_channel(uint16_t id) {
		auto ptr = new g2::echo_channel;
		g2::link_channel(ptr, id);
		return ptr;
	}
}

#endif