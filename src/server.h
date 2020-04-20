//
// Created by andrea on 4/19/20.
//

#ifndef FINALPROJECT_SERVER_H
#define FINALPROJECT_SERVER_H
#include "server/mongoose.h"

static struct mg_serve_http_opts opts;

class Server {
  int port_;
  bool Init(int port);
  void EventHandler(struct mg_connection *connection, int ev, void *p);
 public:
  Server(int port);
};

#endif  // FINALPROJECT_SERVER_H
