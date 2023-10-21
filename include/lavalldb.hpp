#include <iostream>
#include <map>
#include <string>
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

void create_table(sqlite3 *db)
{
    const char *createTableSQL = "CREATE TABLE IF NOT EXISTS Data (ID INTEGER PRIMARY KEY AUTOINCREMENT, Name TEXT NOT NULL, Age INT NOT NULL, Occupation TEXT);";

    char *zErrMsg = 0;
    int rc = sqlite3_exec(db, createTableSQL, callback, 0, &zErrMsg);

    if (rc != SQLITE_OK)
    {
        cout << "Error creating table: " << sqlite3_errmsg(db) << endl;
        sqlite3_free(zErrMsg);
    }
    else
    {
        cout << "Table created successfully." << endl;
    }
}

void register_data(const map<string, string>& columnData)
{
    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;

    rc = sqlite3_open("database.db", &db);

    if (rc)
    {
        cout << "Error opening the database: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        return;
    }

    create_table(db);

    // Montar a consulta SQL dinamicamente com base nos nomes das colunas
    string columns;
    string placeholders;
    for (const auto& pair : columnData)
    {
        columns += pair.first + ", ";
        placeholders += "?, ";
    }
    // Remover a vírgula extra no final das strings columns e placeholders
    columns = columns.substr(0, columns.length() - 2);
    placeholders = placeholders.substr(0, placeholders.length() - 2);

    string sql = "INSERT INTO Data (" + columns + ") VALUES (" + placeholders + ");";

    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0);

    if (rc != SQLITE_OK)
    {
        cout << "Error preparing SQL statement: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        return;
    }

    // Bind dos valores aos placeholders
    int i = 1;
    for (const auto& pair : columnData)
    {
        rc = sqlite3_bind_text(stmt, i++, pair.second.c_str(), -1, SQLITE_STATIC);
        if (rc != SQLITE_OK)
        {
            cout << "Error binding parameter: " << sqlite3_errmsg(db) << endl;
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            return;
        }
    }

    rc = sqlite3_step(stmt);

    if (rc != SQLITE_DONE)
    {
        cout << "Error inserting data: " << sqlite3_errmsg(db) << endl;
    }
    else
    {
        cout << "Data inserted successfully." << endl;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
}