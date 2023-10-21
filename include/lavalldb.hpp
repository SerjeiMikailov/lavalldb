#include <iostream>
#include <sqlite3.h>

static int callback(void *NotUsed, int argc, char **argv, char **azColName)
{
    for (int i = 0; i < argc; i++)
    {
        std::cout << azColName[i] << ": " << (argv[i] ? argv[i] : "NULL") << std::endl;
    }
    return 0;
}

int get_next_id(sqlite3 *db)
{
    const char *sql = "SELECT MAX(ID) FROM Data;";
    sqlite3_stmt *stmt;
    int nextID = 1;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) == SQLITE_OK)
    {
        if (sqlite3_step(stmt) == SQLITE_ROW)
        {
            nextID = sqlite3_column_int(stmt, 0) + 1;
        }
        sqlite3_finalize(stmt);
    }

    return nextID;
}

void register_data(const char *Name, int Age)
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

    const char *createTableSQL = "CREATE TABLE IF NOT EXISTS Data (ID INTEGER PRIMARY KEY AUTOINCREMENT, Name TEXT NOT NULL, Age INT NOT NULL);";

    rc = sqlite3_exec(db, createTableSQL, callback, 0, &zErrMsg);

    if (rc != SQLITE_OK)
    {
        std::cout << "Error creating table: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_free(zErrMsg);
        sqlite3_close(db);
        return;
    }

    const char *sql = "INSERT INTO Data (Name, Age) VALUES (?, ?);";
    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);

    if (rc != SQLITE_OK)
    {
        std::cout << "Error preparing SQL statement: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return;
    }

    sqlite3_bind_text(stmt, 1, Name, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, Age);

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


void delete_data(int ID)
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

    const char *sql = "DELETE FROM Data WHERE ID = ?;";

    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);

    if (rc != SQLITE_OK)
    {
        std::cout << "Error preparing SQL statement: " << sqlite3_errmsg(db) << std::endl;
        sqlite3_close(db);
        return;
    }

    sqlite3_bind_int(stmt, 1, ID);

    rc = sqlite3_step(stmt);

    if (rc != SQLITE_DONE)
    {
        std::cout << "Error deleting data: " << sqlite3_errmsg(db) << std::endl;
    }
    else
    {
        std::cout << "Data deleted successfully." << std::endl;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
}
