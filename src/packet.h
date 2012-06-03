#ifndef PACKET_H
#define PACKET_H

#include "client.h"

void packet_send_connect(int, struct client*, struct client*);
void packet_send_disconnect(int, struct client*, struct client*);
void packet_send_id(int, struct client*);
void packet_send_respawn(int, struct client*);
void packet_send_position(int, struct client*, struct client*);
void packet_send_hit(int, struct client*, struct client*);

#endif /* PACKET_H */
