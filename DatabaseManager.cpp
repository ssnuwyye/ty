#include "DatabaseManager.h"
#include <iostream>

DatabaseManager::DatabaseManager(const std::string& connString)
    : conn(nullptr), connectionString(connString) {
}

DatabaseManager::~DatabaseManager() {
    disconnect();
}

bool DatabaseManager::connect() {
    conn = PQconnectdb(connectionString.c_str());

    if (PQstatus(conn) != CONNECTION_OK) {
        std::cerr << "Connection error: " << PQerrorMessage(conn) << std::endl;
        return false;
    }

    PGresult* res = PQexec(conn, "SET client_encoding TO 'UTF8'");
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        std::cerr << "Error setting encoding: " << PQerrorMessage(conn) << std::endl;
    }
    else {
        std::cout << "Client encoding set to UTF8" << std::endl;
    }
    PQclear(res);

    return true;
}

void DatabaseManager::disconnect() {
    if (conn) {
        PQfinish(conn);
        conn = nullptr;
    }
}

PGconn* DatabaseManager::getConnection() {
    return conn;
}

bool DatabaseManager::isConnected() const {
    return conn && PQstatus(conn) == CONNECTION_OK;
}