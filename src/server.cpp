//
// Created by andrea on 4/19/20.
//
#include "server/mongoose.h"
#include "server.h"
#include <string>

bool Server::Init(int port) {
  struct mg_mgr mgr;
  struct mg_connection* connection;

  std::string s = std::to_string(port);
  static char const *port_chr = s.c_str();

  mg_mgr_init(&mgr, nullptr);
  //connection = mg_bind(&mgr, port_chr, EventHandler());
  if (connection == nullptr) {
    return false;
  }
  mg_set_protocol_http_websocket(connection);
  for (;;) {
    mg_mgr_poll(&mgr, 1000);
  }
  return true;
}