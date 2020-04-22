//
// Created by andrea on 4/22/20.
//

#ifndef CHESS_DB_H
#define CHESS_DB_H

#include <sqlite_modern_cpp.h>
#include <string>
using std::string;

namespace database {
class Database {
 public:
  Database(const std::string& db_path);

 private:
  sqlite::database db_;
};
}

#endif  // CHESS_DB_H
