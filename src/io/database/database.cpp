#include "database.hpp"

using std::string;
using std::cout;

Database::Database(string filepath) : filepath_(filepath) {
    int rc = sqlite3_open(filepath_.c_str(), &db_);
    if (rc != SQLITE_OK /* SUCCESS */) {
        throw std::runtime_error("Could not connect to the database");
    }

    exec_without_return(CREATE_TABLE_);

    rc = sqlite3_prepare_v2(db_, INSERT_BLOB_.c_str(), INSERT_BLOB_.size(), &INSERT_BLOB_PREPARED_, NULL);
    if (rc != SQLITE_OK) {
        cout << rc << std::endl;
        throw std::runtime_error("Something went wrong with preparing INSERT statement");
    }
}

Database::~Database() {
    sqlite3_close(db_);
}

int empty_callback(void* NotUsed, int argc, char** argv, char** azColName) { return 0; }

void Database::exec_without_return(const string&) {
    int result = sqlite3_exec(db_, CREATE_TABLE_.c_str(), empty_callback, 0, &errorMessage_);

    if (result != SQLITE_OK) {
        cout << errorMessage_ << std::endl;
        throw std::runtime_error("Something went wrong running SQL");
    }
}

void Database::archive(const string& message) {
    const void* blob = message.c_str();
    const size_t size = message.length();

    int rc = sqlite3_bind_int64(INSERT_BLOB_PREPARED_, 1, size);
    if (rc != SQLITE_OK) {
        cout << rc << std::endl;
        throw std::runtime_error("Something went wrong binding the size");
    }

    rc = sqlite3_bind_blob(INSERT_BLOB_PREPARED_, 2, blob, size, SQLITE_TRANSIENT /* must be copied - sqlite manages the data */);
    if (rc != SQLITE_OK) {
        throw std::runtime_error("Something went wrong binding the blob");
    }

    rc = sqlite3_step(INSERT_BLOB_PREPARED_);
    if (rc != SQLITE_DONE) {
        cout << rc << std::endl;
        throw std::runtime_error("Something went wrong stepping INSERT");
    }

    rc = sqlite3_reset(INSERT_BLOB_PREPARED_);
    if (rc != SQLITE_OK) {
        throw std::runtime_error("Something went wrong resetting INSERT prepared statement");
    }
}