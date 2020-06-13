#ifndef SERVER_PROTOCOL_H
#define SERVER_PROTOCOL_H

#include <string>

#include "common_socket.h"

namespace ServerProtocol {
/*Recibe un request del cliente y lo interpreta creando un Comando
POST: El comando devuelto debe ser eliminado por el usuario*/
unsigned char receive_command(const Socket& skt);
/*Envia respuesta a el comando recibido*/
void send_response_to_command(const Socket& skt, const unsigned char* message,
                              const uint16_t* size);
unsigned int receive_room_no(const Socket& skt);
}  // namespace ServerProtocol

#endif  // SERVER_PROTOCOL_H
