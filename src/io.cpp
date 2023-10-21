#include "Ldb.hpp"

static int callback(void *NotUsed, int argc, char **argv, char **azColName)
{
    for (int i = 0; i < argc; i++)
    {
        std::cout << azColName[i] << ": " << (argv[i] ? argv[i] : "NULL") << std::endl;
    }
    return 0;
}

void register_data(int ID, const char *Name, int Age)
{
    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;

    rc = sqlite3_open("../database.db", &db);

    if (rc)
    {
        std::cout << "Error opening the database: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return;
    }
    else
    {
        std::cout << "Database opened successfully." << std::endl;
    }

    const char *sql = "CREATE TABLE IF NOT EXISTS Data ("
                      "ID INT PRIMARY KEY NOT NULL,"
                      "Name TEXT NOT NULL,"
                      "Age INT NOT NULL);";

    rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);

    if (rc != SQLITE_OK)
    {
        std::cout << "Error creating table: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_free(zErrMsg);
        sqlite3_close(db);
        return;
    }
    else
    {
        std::cout << "Table created successfully." << std::endl;
    }

    // SQL statement
    sqlite3_stmt *stmt;
    sql = "INSERT INTO Data (ID, Name, Age) VALUES (?, ?, ?);";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);

    if (rc != SQLITE_OK)
    {
        std::cout << "Error preparing SQL statement: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return;
    }

    // parameters toSQL statement
    sqlite3_bind_int(stmt, 1, ID);
    sqlite3_bind_text(stmt, 2, Name, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 3, Age);

    // execute 
    rc = sqlite3_step(stmt);

    if (rc != SQLITE_DONE)
    {
        std::cout << "Error inserting data: " << sqlite3_errmsg(db) << std::endl;
    }
    else
    {
        std::cout << "Data inserted successfully." << std::endl;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
}