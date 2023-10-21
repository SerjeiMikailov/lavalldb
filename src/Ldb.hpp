#include <iostream>
#include <sqlite3.h>

static int callback(void *NotUsed, int argc, char **argv, char **azColName);
void register_data(int ID, const char *Name, int Age);