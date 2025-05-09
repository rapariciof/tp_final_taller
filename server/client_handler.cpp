#include "client_handler.h"

#include <iostream>
#include <sstream>
#include <vector>

ClientHandler::ClientHandler(
    Socket socket, unsigned int game_room,
    ThreadSafeQueue<Command *> *commands_queue,
    BlockingThreadSafeQueue<Notification *> *notifications_queue,
    unsigned int hero_id, std::vector<ArgentumGame *> &games,
    std::string player_name, MessageCenter &message_center,
    const int seconds_for_proccesing_room_changes,
    const int nanoseconds_for_proccesing_attacks)
    : notifications_queue(notifications_queue), commands_queue(commands_queue) {
  this->peer_socket = std::move(socket);
  sender = new ClientNotificationSender(peer_socket, notifications_queue,
                                        std::ref(message_center), player_name);
  receiver = new ClientCommandReceiver(
      peer_socket, game_room, commands_queue, hero_id, std::ref(games),
      player_name, std::ref(message_center),
      seconds_for_proccesing_room_changes, nanoseconds_for_proccesing_attacks);
  sender->start();
  receiver->start();
}

ClientHandler::~ClientHandler() {
  this->sender->stop();
  this->receiver->stop();
  delete sender;
  this->peer_socket.close();
  delete receiver;
}

bool ClientHandler::is_alive() {
  return this->receiver->is_alive();  //|| this->sender->is_alive();
}
