#ifndef G2_CORE_H
#define G2_CORE_H

#include <g1/tower.h>
#include <gxx/syslock.h>

namespace g2 {
	enum class State {
		INIT,
		CONNECTED,
		DISCONNECTED,
	};

	enum class Frame {
		HANDSHAKE = 0,
		DATA = 1,
		REFUSE = 2,
	};

	struct channel {
		dlist_head lnk;
		uint16_t id;
		uint16_t rid;
		void * raddr_ptr;
		size_t raddr_len; 
		g1::QoS qos;
		uint16_t ackquant;
		uint16_t fid = 0;
		State state = State::INIT;
		virtual void incoming_packet(g1::packet* pack) = 0;
		channel() { dlist_init(&lnk); }
	};

	struct sequence_channel : public channel {

	};

	struct subheader {
		uint16_t sid;
		uint16_t rid;
		uint16_t frame_id;	
		Frame ftype;	
	} G1_PACKED;

	struct subheader_handshake {
		g1::QoS qos;
		uint16_t ackquant;
	} G1_PACKED;

	static inline subheader* get_subheader(g1::packet* pack) {
		return (subheader*) pack->dataptr();
	}
	
	static inline subheader_handshake* get_subheader_handshake(g1::packet* pack) {
		return (subheader_handshake*) (pack->dataptr() + sizeof(subheader));
	}

	static inline gxx::buffer get_datasect(g1::packet* pack) {
		return gxx::buffer(pack->dataptr() + sizeof(subheader), pack->datasize() - sizeof(subheader));
	}	

	g2::channel* get_channel(uint16_t id);

	extern gxx::dlist<g2::channel, &g2::channel::lnk> channels;

	/// Добавить сервис к ядру.
	void link_channel(g2::channel* srvs, uint16_t id);
	void incoming(g1::packet* pack);

	void handshake(g2::channel* ch, uint16_t rid, const void* raddr_ptr, size_t raddr_len, g1::QoS qos = g1::QoS(0), uint16_t ackquant = 200);	
	void send(g2::channel* ch, const char* data, size_t size);
};

#endif