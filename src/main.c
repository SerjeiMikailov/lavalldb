#include <stdio.h>
#include <stdlib.h>
#include <sqlite3.h>

static int callback(void *data, int argc, char **argv, char **azColName) {
  int i;
  for(i = 0; i<argc; i++) {
    printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
  }
  printf("\n");
  return 0;
}

int main(int argc, char* argv[]) {
  sqlite3 *db;
  char *zErrMsg = 0;
  int rc;
  
  rc = sqlite3_open("database.db", &db);
  if(rc) {
    fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
    return rc;
  } else {
    fprintf(stdout, "Opened database successfully\n");
  }

  const char *create_table_sql = "CREATE TABLE IF NOT EXISTS USERS("
                                    "ID INT PRIMARY KEY     NOT NULL,"
                                    "NAME           TEXT    NOT NULL,"
                                    "AGE            INT     NOT NULL);";

  rc = sqlite3_exec(db, create_table_sql, 0, 0, &zErrMsg);
  if(rc != SQLITE_OK) {
    fprintf(stderr, "SQL error: %s\n", zErrMsg);
    sqlite3_free(zErrMsg);
    sqlite3_close(db);
    return rc;
  } else {
    fprintf(stdout, "Table created successfully\n");
  }

  int id;
  char name[100];
  int age;

  printf("Enter ID: ");
  scanf("%d", &id);
  printf("Enter Name: ");
  scanf("%s", name);
  printf("Enter Age: ");
  scanf("%d", &age);

  char insert_data_sql[200];
  snprintf(insert_data_sql, sizeof(insert_data_sql), "INSERT INTO USERS (ID, NAME, AGE) VALUES (%d, '%s', %d);", id, name, age);

  rc = sqlite3_exec(db, insert_data_sql, 0, 0, &zErrMsg);
  if(rc != SQLITE_OK) {
    fprintf(stderr, "SQL error: %s\n", zErrMsg);
    sqlite3_free(zErrMsg);
    sqlite3_close(db);
    return rc;
  } else {
    fprintf(stdout, "Data inserted successfully\n");
  }

  sqlite3_close(db);

  return 0;
}
