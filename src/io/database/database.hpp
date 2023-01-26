#ifndef DATABASE_H

#include <iostream>
#include <string>
#include <sqlite3.h>
#include <stdexcept>
//#include <sqlar.c>

class Database {
    friend int empty_callback(void*, int, char**, char**);
    private:
        const std::string filepath_;
        int rc_;
        sqlite3* db_;
        char *errorMessage_;

        const std::string CREATE_TABLE_ = u8"CREATE TABLE IF NOT EXISTS archive (" \
            "id INTEGER PRIMARY KEY AUTOINCREMENT," \
            "last_modification_time INT NOT NULL," \
            "original_size INT64 NOT NULL," \
            "data BLOB NOT NULL" \
        ");"; // original_size to know what type needed for storage

        const std::string INSERT_BLOB_ = u8"INSERT INTO archive (last_modification_time, original_size, data) " \
            //"VALUES (CURRENT_TIMESTAMP, sqlar_compress(?0));";
            "VALUES (CURRENT_TIMESTAMP, ?, ?);";
        sqlite3_stmt* INSERT_BLOB_PREPARED_; // will be populated 
        
        void exec_without_return(const std::string&);
    public:
        Database() = delete;
        Database(std::string);
        ~Database();
        void archive(const std::string&);
};
int empty_callback(void*, int, char**, char**);


#endif