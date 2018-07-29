#ifndef G2_CORE_H
#define G2_CORE_H

#include <g0/core.h>
#include <g1/tower.h>
#include <gxx/syslock.h>
#include <gxx/event/delegate.h>

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

	struct channel : public g0::service {
		/*dlist_head lnk;
		uint16_t id;*/
		uint16_t rid;
		void * raddr_ptr;
		size_t raddr_len; 
		g1::QoS qos;
		uint16_t ackquant;
		uint16_t fid = 0;
		State state = State::INIT;
		void incoming_packet(g1::packet* pack) override;
		channel() { dlist_init(&lnk); }
	};

	struct subheader {
		//uint16_t sid;
		//uint16_t rid;
		uint16_t frame_id;	
		Frame ftype;	
	} G1_PACKED;

	struct subheader_handshake {
		g1::QoS qos;
		uint16_t ackquant;
	} G1_PACKED;

	static inline subheader* get_subheader(g1::packet* pack) {
		return (subheader*) (pack->dataptr() + sizeof(g0::subheader));
	}
	
	static inline subheader_handshake* get_subheader_handshake(g1::packet* pack) {
		return (subheader_handshake*) (pack->dataptr() + sizeof(g0::subheader) + sizeof(g2::subheader));
	}

	static inline gxx::buffer get_datasect(g1::packet* pack) {
		return gxx::buffer(pack->dataptr() + sizeof(g0::subheader) + sizeof(g2::subheader), pack->datasize() - sizeof(g2::subheader) - sizeof(g0::subheader));
	}	

	//g2::channel* get_channel(uint16_t id);

	//extern gxx::dlist<g2::channel, &g2::channel::lnk> channels;

	/// Добавить сервис к ядру.
	void link_channel(g2::channel* srvs, uint16_t id);
	void incoming(g1::packet* pack);

	void handshake(g2::channel* ch, uint16_t rid, const void* raddr_ptr, size_t raddr_len, g1::QoS qos = g1::QoS(0), uint16_t ackquant = 200);	
	void send(g2::channel* ch, const char* data, size_t size);

	struct accept_header {
		uint16_t rchid;
		g1::QoS qos;
		uint16_t ackquant;
	};

	struct acceptor : public g0::service {
		gxx::delegate<g2::channel*> init_channel;
		acceptor(gxx::delegate<g2::channel*> init_channel) : init_channel(init_channel) {}

		void incoming_packet(g1::packet* pack) override;
	};

	static inline acceptor* create_acceptor(uint16_t port, gxx::delegate<g2::channel*> dlg) {
		auto asrv = new g2::acceptor(dlg);
		g0::link_service(asrv, port);
		return asrv;
	}

	uint16_t dynport();
};

#endif