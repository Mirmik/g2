#include <g2/core.h>
#include <gxx/datastruct/iovec.h>

/*gxx::dlist<g2::channel, &g2::channel::lnk> g2::channels;
*/
void g2::link_channel(g2::channel* ch, uint16_t id) {
	ch->id = id;
	g0::services.move_back(*ch);
}
/*
g2::channel* g2::get_channel(uint16_t id) {
	for (auto& ch : g2::channels) {;
		if (ch.id == id) return &ch;
	}
	return nullptr;
}*/
/*
void unknown_port(g1::packet* pack) {
	g2::subheader* rsh = g2::get_subheader(pack);
	g2::subheader sh;

	sh.sid = 0;
	sh.rid = rsh->sid;
	sh.frame_id = 0;
	sh.ftype = g2::Frame::REFUSE;

	g1::send(pack->addrptr(), pack->addrsize(), (const char*)&sh, sizeof(g2::subheader), G1_G2TYPE, g1::QoS(2), pack->header.ackquant);
}
*/
/*void g2::incoming(g1::packet* pack) {
	gxx::println("g2::incomming");
	g1::println(pack);

	g2::subheader* sh = g2::get_subheader(pack);
	g2::channel* ch = get_channel(sh->rid);

	gxx::fprintln("g2 subheader: sid={}, rid={}", (uint16_t)sh->sid, (uint16_t)sh->rid);
	dprhex(ch);

	if (ch == nullptr) {
		gxx::println("warn: packet to unrecognized port");
		unknown_port(pack);
		g1::release(pack);
		return;
	}

	switch(sh->ftype) {
		case g2::Frame::HANDSHAKE:
			gxx::println("HANDSHAKE");
			if (ch->state == g2::State::INIT) {
				g2::subheader_handshake* shh = g2::get_subheader_handshake(pack);
				ch->rid = sh->sid;
				ch->qos = shh->qos;
				ch->ackquant = shh->ackquant;
				ch->raddr_ptr = malloc(pack->header.alen);
				memcpy(ch->raddr_ptr, pack->addrptr(), pack->header.alen);
				ch->raddr_len = pack->header.alen;
				ch->state = g2::State::CONNECTED;

			}
			else {
				unknown_port(pack);
			}
			break;
		case g2::Frame::DATA:
			gxx::println("DATA");
			ch->incoming_packet(pack);
			gxx::println("OUT_DATA");
			return;
		case g2::Frame::REFUSE:
			gxx::println("REFUSE");
			ch->state = g2::State::DISCONNECTED;
			break;
		default: break;
	}
	g1::release(pack);
}*/
/*
	gxx::fprintln("g2: incomming for socket {} from remote socket {}", rsock->port, sh->sendport);

	switch( rsock->state ) {
		case g2::g2_socket_state::WAIT_HANDSHAKE: {
			gxx::println("trace: WAIT_HANDSHAKE");
			if (sh->type == g2::g2_frame_type::HANDSHAKE) {
				gxx::println("handshake event");
	
				rsock->raddr_ptr = (uint8_t*)strndup((const char*)pack->addrptr(), pack->addrsize());
				rsock->raddr_len = pack->addrsize();
				rsock->rport = sh->sendport;

				rsock->state = g2::g2_socket_state::BRIDGE;
				if (rsock->handler) rsock->handler(rsock->privdata, g2::event_type::HANDSHAKE);
			} else {
				gxx::println("warn: received DATA frame in WAIT_HANDSHAKE state");
			}
			g1::release(pack);
			return;
		}

		case g2::g2_socket_state::BRIDGE: {
			gxx::println("trace: BRIDGE");
			if (pack->addrsize() == rsock->raddr_len && strncmp((const char*)pack->addrptr(), (const char*)rsock->raddr_ptr, rsock->raddr_len) == 0) {
				//gxx::println("datapackage");
				//rsock->messages.move_back(pack);

				//Добавить в список сообщений в соответствии с порядковым номером.
				add_to_messages_list(rsock, pack, sh);

				if (rsock->handler) rsock->handler(rsock->privdata, g2::event_type::NEWDATA);
				return;
			}
			else {
				gxx::println("warn: message to BRIDGE socket with wrong address");
			}
			g1::release(pack);
			return;
		}

		default:
			gxx::println("warn: unrecognized socket_state state");
	}
*/
//}

void g2::channel::incoming_packet(g1::packet* pack) {
	g0::subheader* sh0 = g0::get_subheader(pack);
	g2::subheader* sh2 = g2::get_subheader(pack);
	
	/*gxx::fprintln("g2 subheader: sid={}, rid={}", (uint16_t)sh->sid, (uint16_t)sh->rid);
	dprhex(ch);

	if (ch == nullptr) {
		gxx::println("warn: packet to unrecognized port");
		unknown_port(pack);
		g1::release(pack);
		return;
	}*/

	switch(sh2->ftype) {
		case g2::Frame::HANDSHAKE:
			gxx::println("HANDSHAKE");
			if (state == g2::State::INIT) {
				g2::subheader_handshake* shh = g2::get_subheader_handshake(pack);
				rid = sh0->sid;
				qos = shh->qos;
				ackquant = shh->ackquant;
				raddr_ptr = malloc(pack->header.alen);
				memcpy(raddr_ptr, pack->addrptr(), pack->header.alen);
				raddr_len = pack->header.alen;
				state = g2::State::CONNECTED;

			}
			else {
				gxx::panic("no INIT state");
				//unknown_port(pack);
			}
			break;
		case g2::Frame::DATA:
			gxx::println("DATA");
			//incoming_packet(pack);
			gxx::println("OUT_DATA");
			return;
		case g2::Frame::REFUSE:
			gxx::println("REFUSE");
			state = g2::State::DISCONNECTED;
			break;
		default: break;
	}
	g1::release(pack);
}

void g2::acceptor::incoming_packet(g1::packet* pack) {
	g0::subheader* sh0 = g0::get_subheader(pack);
	g2::subheader* sh2 = g2::get_subheader(pack);
	g2::subheader_handshake* shh = g2::get_subheader_handshake(pack);

	auto ch = init_channel();

	g2::handshake(ch, sh0->sid, pack->addrptr(), pack->addrsize(), shh->qos, shh->ackquant);
	g1::release(pack);
}

void g2::handshake(g2::channel* ch, uint16_t rid, const void* raddr_ptr, size_t raddr_len, g1::QoS qos, uint16_t ackquant) {
	g0::subheader sh0;
	g2::subheader sh2;
	g2::subheader_handshake shh;

	sh0.sid = ch->id;
	sh0.rid = ch->rid = rid;
	sh2.frame_id = 0;
	sh2.ftype = g2::Frame::HANDSHAKE;

	ch->raddr_ptr = malloc(raddr_len);
	memcpy(ch->raddr_ptr, raddr_ptr, raddr_len);
	dprhexln(ch->raddr_ptr);
	dprln("arrrr");
	ch->raddr_len = raddr_len;

	ch->qos = shh.qos = qos;
	ch->ackquant = shh.ackquant = ackquant;

	gxx::iovec vec[] = {
		{&sh0, sizeof(sh0)},
		{&sh2, sizeof(sh2)},
		{&shh, sizeof(shh)},
	};

	ch->state = g2::State::CONNECTED;
	g1::send(raddr_ptr, raddr_len, vec, sizeof(vec) / sizeof(gxx::iovec), G1_G2TYPE, g1::QoS(2), ackquant);
}

void g2::send(g2::channel* ch, const char* data, size_t size) {
	g0::subheader sh0;
	g2::subheader sh2;
	sh0.sid = ch->id;
	sh0.rid = ch->rid;
	sh2.frame_id = ch->fid++;
	sh2.ftype = g2::Frame::DATA;	

	gxx::iovec vec[] = {
		{&sh0, sizeof(sh0)},
		{&sh2, sizeof(sh2)},
		{data, size},
	};
	g1::send(ch->raddr_ptr, ch->raddr_len, vec, sizeof(vec) / sizeof(gxx::iovec), G1_G2TYPE, ch->qos, ch->ackquant);
}

uint16_t g2::dynport() {
	return 512;
}