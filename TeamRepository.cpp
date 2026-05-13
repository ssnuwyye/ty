#include "TeamRepository.h"
#include "EncodingUtils.h"
#include <iostream>

TeamRepository::TeamRepository(DatabaseManager& dbManager) : db(dbManager) {}

bool TeamRepository::addTeam(const Team& team) {
    std::string name = ConvertToUTF8(team.getName());
    std::string tag = ConvertToUTF8(team.getTag());
    std::string logo = team.getLogo();
    std::string sportIdStr = std::to_string(team.getSportId());
    std::string winsStr = std::to_string(team.getWins());
    std::string lossesStr = std::to_string(team.getLosses());
    std::string pointsStr = std::to_string(team.getPoints());

    std::string sql = "INSERT INTO teams (sport_id, name, tag, logo, wins, losses, points) VALUES ($1, $2, $3, $4, $5, $6, $7)";
    const char* params[7] = { sportIdStr.c_str(), name.c_str(), tag.c_str(), logo.c_str(),
                              winsStr.c_str(), lossesStr.c_str(), pointsStr.c_str() };

    PGresult* res = PQexecParams(db.getConnection(), sql.c_str(), 7, NULL, params, NULL, NULL, 0);

    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        std::cerr << "Error adding team: " << PQerrorMessage(db.getConnection()) << std::endl;
        PQclear(res);
        return false;
    }
    PQclear(res);
    return true;
}

bool TeamRepository::updateTeam(const Team& team) {
    std::string name = ConvertToUTF8(team.getName());
    std::string tag = ConvertToUTF8(team.getTag());
    std::string logo = team.getLogo();
    std::string idStr = std::to_string(team.getId());
    std::string sportIdStr = std::to_string(team.getSportId());
    std::string winsStr = std::to_string(team.getWins());
    std::string lossesStr = std::to_string(team.getLosses());
    std::string pointsStr = std::to_string(team.getPoints());

    std::string sql = "UPDATE teams SET sport_id = $1, name = $2, tag = $3, logo = $4, wins = $5, losses = $6, points = $7 WHERE id = $8";
    const char* params[8] = { sportIdStr.c_str(), name.c_str(), tag.c_str(), logo.c_str(),
                              winsStr.c_str(), lossesStr.c_str(), pointsStr.c_str(), idStr.c_str() };

    PGresult* res = PQexecParams(db.getConnection(), sql.c_str(), 8, NULL, params, NULL, NULL, 0);

    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        std::cerr << "Error updating team: " << PQerrorMessage(db.getConnection()) << std::endl;
        PQclear(res);
        return false;
    }
    PQclear(res);
    return true;
}

bool TeamRepository::deleteTeam(int id) {
    std::string sql = "DELETE FROM teams WHERE id = $1";
    std::string idStr = std::to_string(id);
    const char* params[1] = { idStr.c_str() };

    PGresult* res = PQexecParams(db.getConnection(), sql.c_str(), 1, NULL, params, NULL, NULL, 0);

    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        std::cerr << "Error deleting team: " << PQerrorMessage(db.getConnection()) << std::endl;
        PQclear(res);
        return false;
    }
    PQclear(res);
    return true;
}

std::vector<Team> TeamRepository::getAllTeams() {
    std::vector<Team> teams;
    std::string sql = "SELECT t.id, t.sport_id, t.name, t.tag, t.logo, t.wins, t.losses, t.points, "
        "TO_CHAR(t.created_at, 'DD.MM.YYYY') as created_at FROM teams t ORDER BY t.points DESC";

    PGresult* res = PQexec(db.getConnection(), sql.c_str());

    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        std::cerr << "Error getting teams: " << PQerrorMessage(db.getConnection()) << std::endl;
        PQclear(res);
        return teams;
    }

    int rows = PQntuples(res);
    for (int i = 0; i < rows; i++) {
        std::string name = ConvertFromUTF8(PQgetvalue(res, i, 2));
        std::string tag = ConvertFromUTF8(PQgetvalue(res, i, 3));

        Team team(std::stoi(PQgetvalue(res, i, 0)),
            std::stoi(PQgetvalue(res, i, 1)),
            name, tag,
            PQgetvalue(res, i, 4),
            std::stoi(PQgetvalue(res, i, 5)),
            std::stoi(PQgetvalue(res, i, 6)),
            std::stoi(PQgetvalue(res, i, 7)),
            PQgetvalue(res, i, 8));
        teams.push_back(team);
    }
    PQclear(res);
    return teams;
}

std::vector<Team> TeamRepository::getTeamsBySport(int sportId) {
    std::vector<Team> teams;
    std::string sql = "SELECT id, sport_id, name, tag, logo, wins, losses, points, "
        "TO_CHAR(created_at, 'DD.MM.YYYY') as created_at FROM teams WHERE sport_id = $1 ORDER BY points DESC";
    std::string idStr = std::to_string(sportId);
    const char* params[1] = { idStr.c_str() };

    PGresult* res = PQexecParams(db.getConnection(), sql.c_str(), 1, NULL, params, NULL, NULL, 0);

    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        PQclear(res);
        return teams;
    }

    int rows = PQntuples(res);
    for (int i = 0; i < rows; i++) {
        std::string name = ConvertFromUTF8(PQgetvalue(res, i, 2));
        std::string tag = ConvertFromUTF8(PQgetvalue(res, i, 3));

        Team team(std::stoi(PQgetvalue(res, i, 0)),
            std::stoi(PQgetvalue(res, i, 1)),
            name, tag,
            PQgetvalue(res, i, 4),
            std::stoi(PQgetvalue(res, i, 5)),
            std::stoi(PQgetvalue(res, i, 6)),
            std::stoi(PQgetvalue(res, i, 7)),
            PQgetvalue(res, i, 8));
        teams.push_back(team);
    }
    PQclear(res);
    return teams;
}

std::vector<Team> TeamRepository::getTeamsByTournament(int tournamentId) {
    std::vector<Team> teams;
    std::string sql = "SELECT t.id, t.sport_id, t.name, t.tag, t.logo, t.wins, t.losses, t.points, "
        "TO_CHAR(t.created_at, 'DD.MM.YYYY') as created_at "
        "FROM teams t "
        "JOIN tournament_teams tt ON t.id = tt.team_id "
        "WHERE tt.tournament_id = $1 "
        "ORDER BY t.points DESC";
    std::string idStr = std::to_string(tournamentId);
    const char* params[1] = { idStr.c_str() };

    PGresult* res = PQexecParams(db.getConnection(), sql.c_str(), 1, NULL, params, NULL, NULL, 0);

    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        PQclear(res);
        return teams;
    }

    int rows = PQntuples(res);
    for (int i = 0; i < rows; i++) {
        std::string name = ConvertFromUTF8(PQgetvalue(res, i, 2));
        std::string tag = ConvertFromUTF8(PQgetvalue(res, i, 3));

        Team team(std::stoi(PQgetvalue(res, i, 0)),
            std::stoi(PQgetvalue(res, i, 1)),
            name, tag,
            PQgetvalue(res, i, 4),
            std::stoi(PQgetvalue(res, i, 5)),
            std::stoi(PQgetvalue(res, i, 6)),
            std::stoi(PQgetvalue(res, i, 7)),
            PQgetvalue(res, i, 8));
        teams.push_back(team);
    }
    PQclear(res);
    return teams;
}

bool TeamRepository::addTeamToTournament(int teamId, int tournamentId) {
    std::string sql = "INSERT INTO tournament_teams (tournament_id, team_id) VALUES ($1, $2) ON CONFLICT DO NOTHING";
    std::string tournamentIdStr = std::to_string(tournamentId);
    std::string teamIdStr = std::to_string(teamId);
    const char* params[2] = { tournamentIdStr.c_str(), teamIdStr.c_str() };

    PGresult* res = PQexecParams(db.getConnection(), sql.c_str(), 2, NULL, params, NULL, NULL, 0);

    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        std::cerr << "Error adding team to tournament: " << PQerrorMessage(db.getConnection()) << std::endl;
        PQclear(res);
        return false;
    }
    PQclear(res);
    return true;
}

bool TeamRepository::removeTeamFromTournament(int teamId, int tournamentId) {
    std::string sql = "DELETE FROM tournament_teams WHERE tournament_id = $1 AND team_id = $2";
    std::string tournamentIdStr = std::to_string(tournamentId);
    std::string teamIdStr = std::to_string(teamId);
    const char* params[2] = { tournamentIdStr.c_str(), teamIdStr.c_str() };

    PGresult* res = PQexecParams(db.getConnection(), sql.c_str(), 2, NULL, params, NULL, NULL, 0);

    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        std::cerr << "Error removing team from tournament: " << PQerrorMessage(db.getConnection()) << std::endl;
        PQclear(res);
        return false;
    }
    PQclear(res);
    return true;
}

bool TeamRepository::updateTeamStats(int id, int wins, int losses, int points) {
    std::string sql = "UPDATE teams SET wins = $1, losses = $2, points = $3 WHERE id = $4";
    std::string idStr = std::to_string(id);
    std::string winsStr = std::to_string(wins);
    std::string lossesStr = std::to_string(losses);
    std::string pointsStr = std::to_string(points);
    const char* params[4] = { winsStr.c_str(), lossesStr.c_str(), pointsStr.c_str(), idStr.c_str() };

    PGresult* res = PQexecParams(db.getConnection(), sql.c_str(), 4, NULL, params, NULL, NULL, 0);

    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        std::cerr << "Error updating team stats: " << PQerrorMessage(db.getConnection()) << std::endl;
        PQclear(res);
        return false;
    }
    PQclear(res);
    return true;
}

Team TeamRepository::getTeamById(int id) {
    std::string sql = "SELECT id, sport_id, name, tag, logo, wins, losses, points, "
        "TO_CHAR(created_at, 'DD.MM.YYYY') as created_at FROM teams WHERE id = $1";
    std::string idStr = std::to_string(id);
    const char* params[1] = { idStr.c_str() };

    PGresult* res = PQexecParams(db.getConnection(), sql.c_str(), 1, NULL, params, NULL, NULL, 0);

    if (PQresultStatus(res) != PGRES_TUPLES_OK || PQntuples(res) == 0) {
        PQclear(res);
        return Team();
    }

    std::string name = ConvertFromUTF8(PQgetvalue(res, 0, 2));
    std::string tag = ConvertFromUTF8(PQgetvalue(res, 0, 3));

    Team team(std::stoi(PQgetvalue(res, 0, 0)),
        std::stoi(PQgetvalue(res, 0, 1)),
        name, tag,
        PQgetvalue(res, 0, 4),
        std::stoi(PQgetvalue(res, 0, 5)),
        std::stoi(PQgetvalue(res, 0, 6)),
        std::stoi(PQgetvalue(res, 0, 7)),
        PQgetvalue(res, 0, 8));
    PQclear(res);
    return team;
}

bool TeamRepository::teamExists(int id) {
    std::string sql = "SELECT id FROM teams WHERE id = $1";
    std::string idStr = std::to_string(id);
    const char* params[1] = { idStr.c_str() };

    PGresult* res = PQexecParams(db.getConnection(), sql.c_str(), 1, NULL, params, NULL, NULL, 0);

    bool exists = (PQresultStatus(res) == PGRES_TUPLES_OK && PQntuples(res) > 0);
    PQclear(res);
    return exists;
}

std::string TeamRepository::getTeamDisplayName(int teamId) {
    std::string sql = "SELECT t.name, t.tag, s.name FROM teams t JOIN sports s ON t.sport_id = s.id WHERE t.id = $1";
    std::string idStr = std::to_string(teamId);
    const char* params[1] = { idStr.c_str() };

    PGresult* res = PQexecParams(db.getConnection(), sql.c_str(), 1, NULL, params, NULL, NULL, 0);

    if (PQresultStatus(res) == PGRES_TUPLES_OK && PQntuples(res) > 0) {
        std::string teamName = ConvertFromUTF8(PQgetvalue(res, 0, 0));
        std::string tag = ConvertFromUTF8(PQgetvalue(res, 0, 1));
        std::string result = teamName + " [" + tag + "]";
        PQclear(res);
        return result;
    }
    PQclear(res);
    return "Unknown";
}