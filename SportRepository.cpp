#include "SportRepository.h"
#include "EncodingUtils.h"
#include <iostream>

SportRepository::SportRepository(DatabaseManager& dbManager) : db(dbManager) {}

bool SportRepository::addSport(const Sport& sport) {
    std::string name = ConvertToUTF8(sport.getName());
    std::string icon = ConvertToUTF8(sport.getIcon());
    std::string color = ConvertToUTF8(sport.getColor());
    std::string description = ConvertToUTF8(sport.getDescription());

    std::string sql = "INSERT INTO sports (name, icon, color, description) VALUES ($1, $2, $3, $4)";
    const char* params[4] = { name.c_str(), icon.c_str(), color.c_str(), description.c_str() };

    PGresult* res = PQexecParams(db.getConnection(), sql.c_str(), 4, NULL, params, NULL, NULL, 0);

    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        std::cerr << "Error adding sport: " << PQerrorMessage(db.getConnection()) << std::endl;
        PQclear(res);
        return false;
    }
    PQclear(res);
    return true;
}

bool SportRepository::updateSport(const Sport& sport) {
    std::string name = ConvertToUTF8(sport.getName());
    std::string icon = ConvertToUTF8(sport.getIcon());
    std::string color = ConvertToUTF8(sport.getColor());
    std::string description = ConvertToUTF8(sport.getDescription());
    std::string idStr = std::to_string(sport.getId());

    std::string sql = "UPDATE sports SET name = $1, icon = $2, color = $3, description = $4 WHERE id = $5";
    const char* params[5] = { name.c_str(), icon.c_str(), color.c_str(), description.c_str(), idStr.c_str() };

    PGresult* res = PQexecParams(db.getConnection(), sql.c_str(), 5, NULL, params, NULL, NULL, 0);

    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        std::cerr << "Error updating sport: " << PQerrorMessage(db.getConnection()) << std::endl;
        PQclear(res);
        return false;
    }
    PQclear(res);
    return true;
}

bool SportRepository::deleteSport(int id) {
    std::string sql = "DELETE FROM sports WHERE id = $1";
    std::string idStr = std::to_string(id);
    const char* params[1] = { idStr.c_str() };

    PGresult* res = PQexecParams(db.getConnection(), sql.c_str(), 1, NULL, params, NULL, NULL, 0);

    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        std::cerr << "Error deleting sport: " << PQerrorMessage(db.getConnection()) << std::endl;
        PQclear(res);
        return false;
    }
    PQclear(res);
    return true;
}

std::vector<Sport> SportRepository::getAllSports() {
    std::vector<Sport> sports;
    PGresult* res = PQexec(db.getConnection(), "SELECT id, name, icon, color, description FROM sports ORDER BY id");

    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        std::cerr << "Error getting sports: " << PQerrorMessage(db.getConnection()) << std::endl;
        PQclear(res);
        return sports;
    }

    int rows = PQntuples(res);
    for (int i = 0; i < rows; i++) {
        std::string name = ConvertFromUTF8(PQgetvalue(res, i, 1));
        std::string icon = ConvertFromUTF8(PQgetvalue(res, i, 2));
        std::string description = ConvertFromUTF8(PQgetvalue(res, i, 4));

        Sport sport(std::stoi(PQgetvalue(res, i, 0)), name, icon, PQgetvalue(res, i, 3), description);
        sports.push_back(sport);
    }
    PQclear(res);
    return sports;
}

Sport SportRepository::getSportById(int id) {
    std::string sql = "SELECT id, name, icon, color, description FROM sports WHERE id = $1";
    std::string idStr = std::to_string(id);
    const char* params[1] = { idStr.c_str() };

    PGresult* res = PQexecParams(db.getConnection(), sql.c_str(), 1, NULL, params, NULL, NULL, 0);

    if (PQresultStatus(res) != PGRES_TUPLES_OK || PQntuples(res) == 0) {
        PQclear(res);
        return Sport();
    }

    std::string name = ConvertFromUTF8(PQgetvalue(res, 0, 1));
    std::string icon = ConvertFromUTF8(PQgetvalue(res, 0, 2));
    std::string description = ConvertFromUTF8(PQgetvalue(res, 0, 4));

    Sport sport(std::stoi(PQgetvalue(res, 0, 0)), name, icon, PQgetvalue(res, 0, 3), description);
    PQclear(res);
    return sport;
}

bool SportRepository::sportExists(int id) {
    std::string sql = "SELECT id FROM sports WHERE id = $1";
    std::string idStr = std::to_string(id);
    const char* params[1] = { idStr.c_str() };

    PGresult* res = PQexecParams(db.getConnection(), sql.c_str(), 1, NULL, params, NULL, NULL, 0);

    bool exists = (PQresultStatus(res) == PGRES_TUPLES_OK && PQntuples(res) > 0);
    PQclear(res);
    return exists;
}

std::string SportRepository::getSportName(int sportId) {
    std::string sql = "SELECT name FROM sports WHERE id = $1";
    std::string idStr = std::to_string(sportId);
    const char* params[1] = { idStr.c_str() };

    PGresult* res = PQexecParams(db.getConnection(), sql.c_str(), 1, NULL, params, NULL, NULL, 0);

    if (PQresultStatus(res) == PGRES_TUPLES_OK && PQntuples(res) > 0) {
        std::string name = ConvertFromUTF8(PQgetvalue(res, 0, 0));
        PQclear(res);
        return name;
    }
    PQclear(res);
    return "Unknown";
}