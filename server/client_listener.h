#ifndef CLIENT_LISTENER_H
#define CLIENT_LISTENER_H

#include <list>
#include <vector>

#include "../util/json/json-forwards.h"
#include "../util/json/json.h"
#include "../util/thread_safe_queue.h"
#include "argentum_game.h"
#include "client_handler.h"
#include "common_socket.h"
#include "message_center.h"
#include "players_saver_sender.h"
#include "protocol.h"
#include "starting_info_notification.h"
#include "thread.h"

class ClientListener : public Thread {
 public:
  /*Crea un nuevo ClientListener que escuchara en el puerto port
  y utilizara numbers para asignarle a cada cliente que se conecte*/
  explicit ClientListener(const char *port, const char *entities_cfg_file);

  ~ClientListener() override;
  /*Escucha nuevos clientes en el puerto 'port'.
  Por cada nuevo cliente que se conecte al puerto, crea a un nuevo
  ClientHandler al cual se le asigna un numero del vector numbers
  utilizando RoundRobin para que juege al juego GuessNumberGame */
  void run() override;
  // Deja de escuchar por nuevos clientes
  void stop_listening();

 private:
  MessageCenter message_center;
  std::vector<ArgentumGame *> game_rooms;
  std::vector<ThreadSafeQueue<Command *> *> queues_commands;
  Socket server_socket;
  std::list<ClientHandler *> clients;
  PlayersSaverSender *players_saver_sender;
  PlayersSaverSender *players_saver_sender_periodically;
  StartingInfoNotification *create_start_notification(unsigned int hero_id,
                                                      unsigned int initial_map);
  /*Remueve a los clientes del vector clients
   que hayan muerto o hayan terminado de jugar
  */
  void garbage_collector();
  unsigned int entities_ids = 0;
  int seconds_for_proccesing_room_changes;
  int nanoseconds_for_proccesing_attacks;
  FilesHandler files_handler;
};

#endif  // CLIENTLISTENER_H
