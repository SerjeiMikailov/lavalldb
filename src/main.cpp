#include <iostream>
#include <sqlite3.h>

using namespace std;

static int callback(void *NotUsed, int argc, char **argv, char **azColName)
{
    for (int i = 0; i < argc; i++)
    {
        cout << azColName[i] << ": " << (argv[i] ? argv[i] : "NULL") << endl;
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
        cout << "Error opening the database: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        return;
    }
    else
    {
        cout << "Database opened successfully." << endl;
    }

    const char *sql = "CREATE TABLE IF NOT EXISTS Data ("
                      "ID INT PRIMARY KEY NOT NULL,"
                      "Name TEXT NOT NULL,"
                      "Age INT NOT NULL);";

    rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);

    if (rc != SQLITE_OK)
    {
        cout << "Error creating table: " << sqlite3_errmsg(db) << endl;
        sqlite3_free(zErrMsg);
        sqlite3_close(db);
        return;
    }
    else
    {
        cout << "Table created successfully." << endl;
    }

    // Prepare the SQL statement
    sqlite3_stmt *stmt;
    sql = "INSERT INTO Data (ID, Name, Age) VALUES (?, ?, ?);";
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, 0);

    if (rc != SQLITE_OK)
    {
        cout << "Error preparing SQL statement: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        return;
    }

    // Bind parameters to the SQL statement
    sqlite3_bind_int(stmt, 1, ID);
    sqlite3_bind_text(stmt, 2, Name, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 3, Age);

    // Execute the statement
    rc = sqlite3_step(stmt);

    if (rc != SQLITE_DONE)
    {
        cout << "Error inserting data: " << sqlite3_errmsg(db) << endl;
    }
    else
    {
        cout << "Data inserted successfully." << endl;
    }

    // Finalize the statement and close the database
    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

int main()
{
    // Example usage of the register_data function
    register_data(2, "Cleius", 30);
    register_data(3, "Enesvildo", 650);

    return 0;
}
