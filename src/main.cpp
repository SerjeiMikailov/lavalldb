//#include "Ldb.hpp"
#include "../include/lavalldb.hpp"

int main()
{
    map<string, string> columnData;
    columnData["Name"] = "John Doe";
    columnData["Age"] = "30";
    columnData["Occupation"] = "Engineer";

    register_data(columnData);

    return 0;
}

