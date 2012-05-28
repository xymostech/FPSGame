#include "packet.h"

void packet_send_connect(int sock, struct client *to, struct client *conn) {
	unsigned char buffer[4];

	data_pack_int16(buffer, 1);
	data_pack_int16(buffer+2, conn->id);

	client_sendpacket(to, sock, buffer, 4);
}

void packet_send_disconnect(int sock, struct client *to, struct client *disconn) {
	unsigned char buffer[4];

	data_pack_int16(buffer, 2);
	data_pack_int16(buffer+2, disconn->id);

	client_sendpacket(to, sock, buffer, 4);
}

void packet_send_id(int sock, struct client *to) {
	unsigned char buffer[4];

	data_pack_int16(buffer, 3);
	data_pack_int16(buffer+2, to->id);

	client_sendpacket(to, sock, buffer, 4);
}

void packet_send_respawn(int sock, struct client *to) {
	unsigned char buffer[14];
	
	data_pack_int16(buffer, 4);
	data_pack_float32(buffer+2,  to->player.x);
	data_pack_float32(buffer+6,  to->player.y);
	data_pack_float32(buffer+10, to->player.z);

	client_sendpacket(to, sock, buffer, 14);
}

void packet_send_position(int sock, struct client *to, struct client *pos) {
	unsigned char buffer[20];
	
	data_pack_int16(buffer, 5);
	data_pack_int16(buffer+2, pos->id);
	data_pack_float32(buffer+4,  pos->player.x);
	data_pack_float32(buffer+8,  pos->player.y);
	data_pack_float32(buffer+12, pos->player.z);
	data_pack_float32(buffer+16, pos->player.yvel);

	client_sendpacket(to, sock, buffer, 20);
}
