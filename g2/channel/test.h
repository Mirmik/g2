#ifndef G2_CHANNEL_TEST_H
#define G2_CHANNEL_TEST_H

#include <g2/core.h>
#include <gxx/print/stdprint.h>

namespace g2 {
	struct test_channel : public channel {
		void incoming_packet(g1::packet* pack) override {
			auto data = g2::get_datasect(pack);
			gxx::println("test_incoming:", gxx::dstring(data));
		} 
	};

	test_channel* create_test_channel(uint16_t id) {
		auto ptr = new g2::test_channel;
		g2::link_channel(ptr, id);
		return ptr;
	}
}

#endif