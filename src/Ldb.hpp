#include <iostream>
#include <sqlite3.h>

static int callback(void *NotUsed, int argc, char **argv, char **azColName);
void register_data(const char *Name, int Age);
void delete_data(int ID);
int get_next_id(sqlite3 *db);
