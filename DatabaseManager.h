#pragma once
#include <string>
#include <libpq-fe.h>

class DatabaseManager {
private:
    PGconn* conn;
    std::string connectionString;
public:
    DatabaseManager(const std::string& connString);
    ~DatabaseManager();
    bool connect();
    void disconnect();
    PGconn* getConnection();
    bool isConnected() const;
};