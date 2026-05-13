#include "TournamentRepository.h"
#include "EncodingUtils.h"
#include <iostream>
#include <sstream>
#include <iomanip>

TournamentRepository::TournamentRepository(DatabaseManager& dbManager) : db(dbManager) {}

// Вспомогательная функция для форматирования double с точкой
static std::string formatDouble(double value) {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2) << value;
    std::string result = oss.str();
    // Заменяем запятую на точку (на случай русской локали)
    for (char& c : result) {
        if (c == ',') c = '.';
    }
    return result;
}

bool TournamentRepository::addTournament(const Tournament& tournament) {
    std::string name = ConvertToUTF8(tournament.getName());
    std::string sportIdStr = std::to_string(tournament.getSportId());
    std::string prizePoolStr = formatDouble(tournament.getPrizePool());
    std::string startDate = ConvertToUTF8(tournament.getStartDate());
    std::string endDate = ConvertToUTF8(tournament.getEndDate());
    std::string status = ConvertToUTF8(tournament.getStatus());

    std::string sql = "INSERT INTO tournaments (sport_id, name, start_date, end_date, prize_pool, status) VALUES ($1, $2, $3, $4, $5, $6)";
    const char* params[6] = { sportIdStr.c_str(), name.c_str(), startDate.c_str(),
                              endDate.c_str(), prizePoolStr.c_str(), status.c_str() };

    PGresult* res = PQexecParams(db.getConnection(), sql.c_str(), 6, NULL, params, NULL, NULL, 0);

    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        std::cerr << "Error adding tournament: " << PQerrorMessage(db.getConnection()) << std::endl;
        PQclear(res);
        return false;
    }
    PQclear(res);
    return true;
}

bool TournamentRepository::updateTournament(const Tournament& tournament) {
    std::string name = ConvertToUTF8(tournament.getName());
    std::string idStr = std::to_string(tournament.getId());
    std::string sportIdStr = std::to_string(tournament.getSportId());
    std::string prizePoolStr = formatDouble(tournament.getPrizePool());
    std::string startDate = ConvertToUTF8(tournament.getStartDate());
    std::string endDate = ConvertToUTF8(tournament.getEndDate());
    std::string status = ConvertToUTF8(tournament.getStatus());

    std::string sql = "UPDATE tournaments SET sport_id = $1, name = $2, start_date = $3, end_date = $4, prize_pool = $5, status = $6 WHERE id = $7";
    const char* params[7] = { sportIdStr.c_str(), name.c_str(), startDate.c_str(),
                              endDate.c_str(), prizePoolStr.c_str(), status.c_str(), idStr.c_str() };

    PGresult* res = PQexecParams(db.getConnection(), sql.c_str(), 7, NULL, params, NULL, NULL, 0);

    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        std::cerr << "Error updating tournament: " << PQerrorMessage(db.getConnection()) << std::endl;
        PQclear(res);
        return false;
    }
    PQclear(res);
    return true;
}

bool TournamentRepository::deleteTournament(int id) {
    std::string sql = "DELETE FROM tournaments WHERE id = $1";
    std::string idStr = std::to_string(id);
    const char* params[1] = { idStr.c_str() };

    PGresult* res = PQexecParams(db.getConnection(), sql.c_str(), 1, NULL, params, NULL, NULL, 0);

    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        std::cerr << "Error deleting tournament: " << PQerrorMessage(db.getConnection()) << std::endl;
        PQclear(res);
        return false;
    }
    PQclear(res);
    return true;
}

std::vector<Tournament> TournamentRepository::getAllTournaments() {
    std::vector<Tournament> tournaments;
    std::string sql = "SELECT id, sport_id, name, TO_CHAR(start_date, 'DD.MM.YYYY'), "
        "TO_CHAR(end_date, 'DD.MM.YYYY'), prize_pool, status, TO_CHAR(created_at, 'DD.MM.YYYY') "
        "FROM tournaments ORDER BY start_date DESC";

    PGresult* res = PQexec(db.getConnection(), sql.c_str());

    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        std::cerr << "Error getting tournaments: " << PQerrorMessage(db.getConnection()) << std::endl;
        PQclear(res);
        return tournaments;
    }

    int rows = PQntuples(res);
    for (int i = 0; i < rows; i++) {
        std::string name = ConvertFromUTF8(PQgetvalue(res, i, 2));

        Tournament tournament(std::stoi(PQgetvalue(res, i, 0)),
            std::stoi(PQgetvalue(res, i, 1)),
            name,
            PQgetvalue(res, i, 3),
            PQgetvalue(res, i, 4),
            std::stod(PQgetvalue(res, i, 5)),
            PQgetvalue(res, i, 6),
            PQgetvalue(res, i, 7));
        tournaments.push_back(tournament);
    }
    PQclear(res);
    return tournaments;
}

std::vector<Tournament> TournamentRepository::getTournamentsBySport(int sportId) {
    std::vector<Tournament> tournaments;
    std::string sql = "SELECT id, sport_id, name, TO_CHAR(start_date, 'DD.MM.YYYY'), "
        "TO_CHAR(end_date, 'DD.MM.YYYY'), prize_pool, status, TO_CHAR(created_at, 'DD.MM.YYYY') "
        "FROM tournaments WHERE sport_id = $1 ORDER BY start_date DESC";
    std::string idStr = std::to_string(sportId);
    const char* params[1] = { idStr.c_str() };

    PGresult* res = PQexecParams(db.getConnection(), sql.c_str(), 1, NULL, params, NULL, NULL, 0);

    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        PQclear(res);
        return tournaments;
    }

    int rows = PQntuples(res);
    for (int i = 0; i < rows; i++) {
        std::string name = ConvertFromUTF8(PQgetvalue(res, i, 2));

        Tournament tournament(std::stoi(PQgetvalue(res, i, 0)),
            std::stoi(PQgetvalue(res, i, 1)),
            name,
            PQgetvalue(res, i, 3),
            PQgetvalue(res, i, 4),
            std::stod(PQgetvalue(res, i, 5)),
            PQgetvalue(res, i, 6),
            PQgetvalue(res, i, 7));
        tournaments.push_back(tournament);
    }
    PQclear(res);
    return tournaments;
}

std::vector<Tournament> TournamentRepository::getTournamentsByStatus(const std::string& status) {
    std::vector<Tournament> tournaments;
    std::string sql = "SELECT id, sport_id, name, TO_CHAR(start_date, 'DD.MM.YYYY'), "
        "TO_CHAR(end_date, 'DD.MM.YYYY'), prize_pool, status, TO_CHAR(created_at, 'DD.MM.YYYY') "
        "FROM tournaments WHERE status = $1 ORDER BY start_date";
    const char* params[1] = { status.c_str() };

    PGresult* res = PQexecParams(db.getConnection(), sql.c_str(), 1, NULL, params, NULL, NULL, 0);

    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        PQclear(res);
        return tournaments;
    }

    int rows = PQntuples(res);
    for (int i = 0; i < rows; i++) {
        std::string name = ConvertFromUTF8(PQgetvalue(res, i, 2));

        Tournament tournament(std::stoi(PQgetvalue(res, i, 0)),
            std::stoi(PQgetvalue(res, i, 1)),
            name,
            PQgetvalue(res, i, 3),
            PQgetvalue(res, i, 4),
            std::stod(PQgetvalue(res, i, 5)),
            PQgetvalue(res, i, 6),
            PQgetvalue(res, i, 7));
        tournaments.push_back(tournament);
    }
    PQclear(res);
    return tournaments;
}

std::vector<Match> TournamentRepository::getUpcomingMatchesBySport(int sportId, int limit) {
    std::vector<Match> matches;
    std::string sql = "SELECT m.id, m.tournament_id, m.team1_id, m.team2_id, "
        "TO_CHAR(m.match_date, 'DD.MM.YYYY HH24:MI') as match_date, "
        "m.team1_score, m.team2_score, m.stage, m.status, "
        "TO_CHAR(m.created_at, 'DD.MM.YYYY') as created_at "
        "FROM matches m "
        "JOIN tournaments t ON m.tournament_id = t.id "
        "WHERE t.sport_id = $1 AND m.status = 'scheduled' "
        "AND m.match_date > NOW() "
        "ORDER BY m.match_date ASC "
        "LIMIT $2";

    std::string sportIdStr = std::to_string(sportId);
    std::string limitStr = std::to_string(limit);
    const char* params[2] = { sportIdStr.c_str(), limitStr.c_str() };

    PGresult* res = PQexecParams(db.getConnection(), sql.c_str(), 2, NULL, params, NULL, NULL, 0);

    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        PQclear(res);
        return matches;
    }

    int rows = PQntuples(res);
    for (int i = 0; i < rows; i++) {
        Match match(std::stoi(PQgetvalue(res, i, 0)),
            std::stoi(PQgetvalue(res, i, 1)),
            std::stoi(PQgetvalue(res, i, 2)),
            std::stoi(PQgetvalue(res, i, 3)),
            PQgetvalue(res, i, 4),
            std::stoi(PQgetvalue(res, i, 5)),
            std::stoi(PQgetvalue(res, i, 6)),
            PQgetvalue(res, i, 7),
            PQgetvalue(res, i, 8),
            PQgetvalue(res, i, 9));
        matches.push_back(match);
    }
    PQclear(res);
    return matches;
}

bool TournamentRepository::updateTournamentStatus(int id, const std::string& status) {
    std::string sql = "UPDATE tournaments SET status = $1 WHERE id = $2";
    std::string idStr = std::to_string(id);
    std::string statusUtf8 = ConvertToUTF8(status);
    const char* params[2] = { statusUtf8.c_str(), idStr.c_str() };

    PGresult* res = PQexecParams(db.getConnection(), sql.c_str(), 2, NULL, params, NULL, NULL, 0);

    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        std::cerr << "Error updating tournament status: " << PQerrorMessage(db.getConnection()) << std::endl;
        PQclear(res);
        return false;
    }
    PQclear(res);
    return true;
}

Tournament TournamentRepository::getTournamentById(int id) {
    std::string sql = "SELECT id, sport_id, name, TO_CHAR(start_date, 'DD.MM.YYYY'), "
        "TO_CHAR(end_date, 'DD.MM.YYYY'), prize_pool, status, TO_CHAR(created_at, 'DD.MM.YYYY') "
        "FROM tournaments WHERE id = $1";
    std::string idStr = std::to_string(id);
    const char* params[1] = { idStr.c_str() };

    PGresult* res = PQexecParams(db.getConnection(), sql.c_str(), 1, NULL, params, NULL, NULL, 0);

    if (PQresultStatus(res) != PGRES_TUPLES_OK || PQntuples(res) == 0) {
        PQclear(res);
        return Tournament();
    }

    std::string name = ConvertFromUTF8(PQgetvalue(res, 0, 2));

    Tournament tournament(std::stoi(PQgetvalue(res, 0, 0)),
        std::stoi(PQgetvalue(res, 0, 1)),
        name,
        PQgetvalue(res, 0, 3),
        PQgetvalue(res, 0, 4),
        std::stod(PQgetvalue(res, 0, 5)),
        PQgetvalue(res, 0, 6),
        PQgetvalue(res, 0, 7));
    PQclear(res);
    return tournament;
}

bool TournamentRepository::tournamentExists(int id) {
    std::string sql = "SELECT id FROM tournaments WHERE id = $1";
    std::string idStr = std::to_string(id);
    const char* params[1] = { idStr.c_str() };

    PGresult* res = PQexecParams(db.getConnection(), sql.c_str(), 1, NULL, params, NULL, NULL, 0);

    bool exists = (PQresultStatus(res) == PGRES_TUPLES_OK && PQntuples(res) > 0);
    PQclear(res);
    return exists;
}

std::string TournamentRepository::getTournamentDisplayName(int tournamentId) {
    std::string sql = "SELECT t.name, s.name FROM tournaments t JOIN sports s ON t.sport_id = s.id WHERE t.id = $1";
    std::string idStr = std::to_string(tournamentId);
    const char* params[1] = { idStr.c_str() };

    PGresult* res = PQexecParams(db.getConnection(), sql.c_str(), 1, NULL, params, NULL, NULL, 0);

    if (PQresultStatus(res) == PGRES_TUPLES_OK && PQntuples(res) > 0) {
        std::string tournamentName = ConvertFromUTF8(PQgetvalue(res, 0, 0));
        std::string sportName = ConvertFromUTF8(PQgetvalue(res, 0, 1));
        std::string result = tournamentName + " (" + sportName + ")";
        PQclear(res);
        return result;
    }
    PQclear(res);
    return "Unknown";
}