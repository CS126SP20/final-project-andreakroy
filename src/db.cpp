//
// Created by andrea on 4/22/20.
//

#include "db.h"

namespace database {
Database::Database(const std::string &db_path) : db_(db_path) {
  db_ << "CREATE TABLE if not exists games (\n"
             "  game_id INTEGER NOT NULL,\n"
             "  move STRING NOT NULL,\n"
             "  number INTEGER NOT NULL,\n"
             "  color STRING NOT NULL\n"
             ")";
}
}  // namespace database
