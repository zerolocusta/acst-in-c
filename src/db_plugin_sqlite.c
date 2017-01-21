/*
 * Copyright (c) 2014 Cesanta Software Limited
 * All rights reserved
 */

#include "db_plugin.h"
#include "sqlite3.h"

void *db_open(const char *db_path) {
  sqlite3 *db = NULL;
  if (sqlite3_open_v2(db_path, &db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE |
                                        SQLITE_OPEN_FULLMUTEX,
                      NULL) == SQLITE_OK) {
    sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS kv(key PRIMARY KEY, val);", 0,
                 0, 0);
  }
  return db;
}

void db_close(void **db_handle) {
  if (db_handle != NULL && *db_handle != NULL) {
    sqlite3_close(*db_handle);
    *db_handle = NULL;
  }
}