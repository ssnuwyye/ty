#include "PlayerRepository.h"
#include "EncodingUtils.h"
#include "TeamRepository.h"
#include <iostream>

PlayerRepository::PlayerRepository(DatabaseManager& dbManager) : db(dbManager) {}

bool PlayerRepository::addPlayer(const Player& player) {
    std::string nickname = ConvertToUTF8(player.getNickname());
    std::string fullName = ConvertToUTF8(player.getFullName());
    std::string role = ConvertToUTF8(player.getRole());
    std::string nationality = ConvertToUTF8(player.getNationality());
    std::string teamIdStr = player.getTeamId() > 0 ? std::to_string(player.getTeamId()) : "NULL";
    std::string ratingStr = std::to_string(player.getRating());

    std::string sql = "INSERT INTO players (team_id, nickname, full_name, role, nationality, rating) VALUES ($1, $2, $3, $4, $5, $6)";
    const char* params[6] = { player.getTeamId() > 0 ? teamIdStr.c_str() : nullptr, nickname.c_str(), fullName.c_str(),
                              role.c_str(), nationality.c_str(), ratingStr.c_str() };

    PGresult* res = PQexecParams(db.getConnection(), sql.c_str(), 6, NULL, params, NULL, NULL, 0);

    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        std::cerr << "Error adding player: " << PQerrorMessage(db.getConnection()) << std::endl;
        PQclear(res);
        return false;
    }
    PQclear(res);
    return true;
}

bool PlayerRepository::updatePlayer(const Player& player) {
    std::string nickname = ConvertToUTF8(player.getNickname());
    std::string fullName = ConvertToUTF8(player.getFullName());
    std::string role = ConvertToUTF8(player.getRole());
    std::string nationality = ConvertToUTF8(player.getNationality());
    std::string idStr = std::to_string(player.getId());
    std::string teamIdStr = player.getTeamId() > 0 ? std::to_string(player.getTeamId()) : "NULL";
    std::string ratingStr = std::to_string(player.getRating());

    std::string sql = "UPDATE players SET team_id = $1, nickname = $2, full_name = $3, role = $4, nationality = $5, rating = $6 WHERE id = $7";
    const char* params[7] = { player.getTeamId() > 0 ? teamIdStr.c_str() : nullptr, nickname.c_str(), fullName.c_str(),
                              role.c_str(), nationality.c_str(), ratingStr.c_str(), idStr.c_str() };

    PGresult* res = PQexecParams(db.getConnection(), sql.c_str(), 7, NULL, params, NULL, NULL, 0);

    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        std::cerr << "Error updating player: " << PQerrorMessage(db.getConnection()) << std::endl;
        PQclear(res);
        return false;
    }
    PQclear(res);
    return true;
}

bool PlayerRepository::deletePlayer(int id) {
    std::string sql = "DELETE FROM players WHERE id = $1";
    std::string idStr = std::to_string(id);
    const char* params[1] = { idStr.c_str() };

    PGresult* res = PQexecParams(db.getConnection(), sql.c_str(), 1, NULL, params, NULL, NULL, 0);

    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        std::cerr << "Error deleting player: " << PQerrorMessage(db.getConnection()) << std::endl;
        PQclear(res);
        return false;
    }
    PQclear(res);
    return true;
}

std::vector<Player> PlayerRepository::getAllPlayers() {
    std::vector<Player> players;
    std::string sql = "SELECT p.id, p.team_id, p.nickname, p.full_name, p.role, p.nationality, p.rating, "
        "TO_CHAR(p.created_at, 'DD.MM.YYYY') as created_at FROM players p ORDER BY p.rating DESC";

    PGresult* res = PQexec(db.getConnection(), sql.c_str());

    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        std::cerr << "Error getting players: " << PQerrorMessage(db.getConnection()) << std::endl;
        PQclear(res);
        return players;
    }

    int rows = PQntuples(res);
    for (int i = 0; i < rows; i++) {
        std::string nickname = ConvertFromUTF8(PQgetvalue(res, i, 2));
        std::string fullName = ConvertFromUTF8(PQgetvalue(res, i, 3));
        std::string role = ConvertFromUTF8(PQgetvalue(res, i, 4));
        std::string nationality = ConvertFromUTF8(PQgetvalue(res, i, 5));

        Player player(std::stoi(PQgetvalue(res, i, 0)),
            PQgetisnull(res, i, 1) ? 0 : std::stoi(PQgetvalue(res, i, 1)),
            nickname, fullName, role, nationality,
            std::stoi(PQgetvalue(res, i, 6)),
            PQgetvalue(res, i, 7));
        players.push_back(player);
    }
    PQclear(res);
    return players;
}

std::vector<Player> PlayerRepository::getPlayersByTeam(int teamId) {
    std::vector<Player> players;
    std::string sql = "SELECT id, team_id, nickname, full_name, role, nationality, rating, "
        "TO_CHAR(created_at, 'DD.MM.YYYY') as created_at FROM players WHERE team_id = $1 ORDER BY rating DESC";
    std::string idStr = std::to_string(teamId);
    const char* params[1] = { idStr.c_str() };

    PGresult* res = PQexecParams(db.getConnection(), sql.c_str(), 1, NULL, params, NULL, NULL, 0);

    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        PQclear(res);
        return players;
    }

    int rows = PQntuples(res);
    for (int i = 0; i < rows; i++) {
        std::string nickname = ConvertFromUTF8(PQgetvalue(res, i, 2));
        std::string fullName = ConvertFromUTF8(PQgetvalue(res, i, 3));
        std::string role = ConvertFromUTF8(PQgetvalue(res, i, 4));
        std::string nationality = ConvertFromUTF8(PQgetvalue(res, i, 5));

        Player player(std::stoi(PQgetvalue(res, i, 0)),
            PQgetisnull(res, i, 1) ? 0 : std::stoi(PQgetvalue(res, i, 1)),
            nickname, fullName, role, nationality,
            std::stoi(PQgetvalue(res, i, 6)),
            PQgetvalue(res, i, 7));
        players.push_back(player);
    }
    PQclear(res);
    return players;
}

bool PlayerRepository::updatePlayerRating(int id, int rating) {
    std::string sql = "UPDATE players SET rating = $1 WHERE id = $2";
    std::string idStr = std::to_string(id);
    std::string ratingStr = std::to_string(rating);
    const char* params[2] = { ratingStr.c_str(), idStr.c_str() };

    PGresult* res = PQexecParams(db.getConnection(), sql.c_str(), 2, NULL, params, NULL, NULL, 0);

    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        std::cerr << "Error updating player rating: " << PQerrorMessage(db.getConnection()) << std::endl;
        PQclear(res);
        return false;
    }
    PQclear(res);
    return true;
}

Player PlayerRepository::getPlayerById(int id) {
    std::string sql = "SELECT id, team_id, nickname, full_name, role, nationality, rating, "
        "TO_CHAR(created_at, 'DD.MM.YYYY') as created_at FROM players WHERE id = $1";
    std::string idStr = std::to_string(id);
    const char* params[1] = { idStr.c_str() };

    PGresult* res = PQexecParams(db.getConnection(), sql.c_str(), 1, NULL, params, NULL, NULL, 0);

    if (PQresultStatus(res) != PGRES_TUPLES_OK || PQntuples(res) == 0) {
        PQclear(res);
        return Player();
    }

    std::string nickname = ConvertFromUTF8(PQgetvalue(res, 0, 2));
    std::string fullName = ConvertFromUTF8(PQgetvalue(res, 0, 3));
    std::string role = ConvertFromUTF8(PQgetvalue(res, 0, 4));
    std::string nationality = ConvertFromUTF8(PQgetvalue(res, 0, 5));

    Player player(std::stoi(PQgetvalue(res, 0, 0)),
        PQgetisnull(res, 0, 1) ? 0 : std::stoi(PQgetvalue(res, 0, 1)),
        nickname, fullName, role, nationality,
        std::stoi(PQgetvalue(res, 0, 6)),
        PQgetvalue(res, 0, 7));
    PQclear(res);
    return player;
}

bool PlayerRepository::playerExists(int id) {
    std::string sql = "SELECT id FROM players WHERE id = $1";
    std::string idStr = std::to_string(id);
    const char* params[1] = { idStr.c_str() };

    PGresult* res = PQexecParams(db.getConnection(), sql.c_str(), 1, NULL, params, NULL, NULL, 0);

    bool exists = (PQresultStatus(res) == PGRES_TUPLES_OK && PQntuples(res) > 0);
    PQclear(res);
    return exists;
}

std::string PlayerRepository::getPlayerDisplayName(int playerId) {
    std::string sql = "SELECT nickname, full_name FROM players WHERE id = $1";
    std::string idStr = std::to_string(playerId);
    const char* params[1] = { idStr.c_str() };

    PGresult* res = PQexecParams(db.getConnection(), sql.c_str(), 1, NULL, params, NULL, NULL, 0);

    if (PQresultStatus(res) == PGRES_TUPLES_OK && PQntuples(res) > 0) {
        std::string nickname = ConvertFromUTF8(PQgetvalue(res, 0, 0));
        std::string fullName = ConvertFromUTF8(PQgetvalue(res, 0, 1));
        std::string result = fullName.empty() ? nickname : nickname + " (" + fullName + ")";
        PQclear(res);
        return result;
    }
    PQclear(res);
    return "Unknown";
}