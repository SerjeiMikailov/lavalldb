#include <iostream>
#include <map>
#include <string>
#include <sqlite3.h>

using namespace std;


/*


    ---> To run with success you will need: SQLite3

    Install:

    Windows: "choco install sqlite" (With Chocolatey)
    Or
    https://www.sqlite.org/download.html

    Arch Linux:
    sudo pacman -S sqlite

    Ubuntu:
    sudo apt install sqlite3

*/


// Callback function to display query results
static int callback(void *NotUsed, int argc, char **argv, char **azColName)
{
    for (int i = 0; i < argc; i++)
    {
        cout << azColName[i] << ": " << (argv[i] ? argv[i] : "NULL") << endl;
    }
    return 0;
}

// Function to create a table in the database
void create_table(sqlite3 *db)
{
    // SQL statement to create 'Data' table with ID, Name, Age, and Occupation columns
    const char *createTableSQL = "CREATE TABLE IF NOT EXISTS Data (ID INTEGER PRIMARY KEY AUTOINCREMENT, Name TEXT NOT NULL, Age INT NOT NULL, Occupation TEXT);";

    char *zErrMsg = 0;
    int rc = sqlite3_exec(db, createTableSQL, callback, 0, &zErrMsg);

    // Check if the table creation was successful
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

// Function to register data in the 'Data' table dynamically
void register_data(const map<string, string>& columnData)
{
    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;

    // Open the database file named 'database.db'
    rc = sqlite3_open("database.db", &db);

    // Check if the database connection was successful
    if (rc)
    {
        cout << "Error opening the database: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        return;
    }

    // Create the 'Data' table if it does not exist
    create_table(db);

    // Dynamically construct SQL query based on column names provided in 'columnData' map
    string columns;
    string placeholders;
    for (const auto& pair : columnData)
    {
        columns += pair.first + ", ";
        placeholders += "?, ";
    }
    // Remove the trailing comma and space in 'columns' and 'placeholders' strings
    columns = columns.substr(0, columns.length() - 2);
    placeholders = placeholders.substr(0, placeholders.length() - 2);

    // Construct the SQL query for inserting data
    string sql = "INSERT INTO Data (" + columns + ") VALUES (" + placeholders + ");";

    sqlite3_stmt *stmt;
    rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, 0);

    // Check if preparing the SQL statement was successful
    if (rc != SQLITE_OK)
    {
        cout << "Error preparing SQL statement: " << sqlite3_errmsg(db) << endl;
        sqlite3_close(db);
        return;
    }

    // Bind values to the placeholders in the SQL query
    int i = 1;
    for (const auto& pair : columnData)
    {
        rc = sqlite3_bind_text(stmt, i++, pair.second.c_str(), -1, SQLITE_STATIC);
        // Check if binding the parameter was successful
        if (rc != SQLITE_OK)
        {
            cout << "Error binding parameter: " << sqlite3_errmsg(db) << endl;
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            return;
        }
    }

    // Execute the SQL query to insert data into the 'Data' table
    rc = sqlite3_step(stmt);

    // Check if data insertion was successful
    if (rc != SQLITE_DONE)
    {
        cout << "Error inserting data: " << sqlite3_errmsg(db) << endl;
    }
    else
    {
        cout << "Data inserted successfully." << endl;
    }

    // Finalize the SQL statement and close the database connection
    sqlite3_finalize(stmt);
    sqlite3_close(db);
}
