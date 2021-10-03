#include <fmt/core.h>
#include <sqlite/sqlite3.h>

static int callback(void* NotUsed, int argc, char** argv, char** azColName)
{
    int i;
    for (i = 0; i < argc; i++)
    {
        fmt::print("{} = {}\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    fmt::print("\n");
    return 0;
}

int main()
{
    fmt::print("Hello SQLite\n");
    sqlite3* db;
    char* zErrMsg = 0;
    int rc;
    char* sql;

    auto check_answer_and_print = [&](const auto msg) {
        if (rc != SQLITE_OK)
        {
            fmt::print("SQL error: {}\n", zErrMsg);
            sqlite3_free(zErrMsg);
        }
        else
        {
            fmt::print(msg);
        }
    };

    rc = sqlite3_open("test.db", &db);

    if (rc)
    {
        fmt::print("Can't open database: {}\n", sqlite3_errmsg(db));
        return EXIT_FAILURE;
    }
    else
    {
        fmt::print("Opened database successfully\n");
    }

    /* Create SQL statement */
    sql =
        "CREATE TABLE COMPANY("
        "ID INT PRIMARY KEY     NOT NULL,"
        "NAME           TEXT    NOT NULL,"
        "AGE            INT     NOT NULL,"
        "ADDRESS        CHAR(50),"
        "SALARY         REAL );";

    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
    check_answer_and_print("Table created successfully\n");

    /* Create SQL statement */
    sql =
        "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY) "
        "VALUES (1, 'Paul', 32, 'California', 20000.00 ); "
        "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY) "
        "VALUES (2, 'Allen', 25, 'Texas', 15000.00 ); "
        "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY)"
        "VALUES (3, 'Teddy', 23, 'Norway', 20000.00 );"
        "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY)"
        "VALUES (4, 'Mark', 25, 'Rich-Mond ', 65000.00 );";

    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
    check_answer_and_print("Records created successfully\n");

    /* Create SQL statement */
    sql = "SELECT * from COMPANY";

    /* Execute SQL statement */
    const char* data = "Callback function called";
    rc = sqlite3_exec(db, sql, callback, (void*)data, &zErrMsg);
    check_answer_and_print("Select done successfully\n");

    sqlite3_close(db);
    return EXIT_SUCCESS;
}