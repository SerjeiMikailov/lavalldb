#include <iostream>
#include <sqlite3.h>

using namespace std;

static int callback(void *NotUsed, int argc, char **argv, char **azColName){
    for(int i = 0; i < argc; i++){
        cout << azColName[i] << ": " << (argv[i] ? argv[i] : "NULL") << endl;
    }
    return 0;
}

int main() {
    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;

    rc = sqlite3_open("database.db", &db);

    if(rc) {
        cout << "Error opening the database: " << sqlite3_errmsg(db) << endl;
        return rc;
    } else {
        cout << "Database opened successfully." << endl;
    }

    const char *sql = "CREATE TABLE Data ("
                      "ID INT PRIMARY KEY NOT NULL,"
                      "Name TEXT NOT NULL,"
                      "Age INT NOT NULL);";

    rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);

    if(rc != SQLITE_OK) {
        cout << "Error creating table: " << sqlite3_errmsg(db) << endl;
        sqlite3_free(zErrMsg);
    } else {
        cout << "Table created successfully." << endl;
    }

    sql = "INSERT INTO Data (ID, Name, Age) VALUES (1, 'Example', 30);";

    rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);

    if(rc != SQLITE_OK) {
        cout << "Error inserting data: " << sqlite3_errmsg(db) << endl;
        sqlite3_free(zErrMsg);
    } else {
        cout << "Data inserted successfully." << endl;
    }

    sqlite3_close(db);

    return 0;
}
