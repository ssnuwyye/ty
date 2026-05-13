#include "MatchRepository.h"
#include "EncodingUtils.h"
#include "TeamRepository.h"
#include <iostream>

MatchRepository::MatchRepository(DatabaseManager& dbManager) : db(dbManager) {}

bool MatchRepository::addMatch(const Match& match) {
    std::string tournamentIdStr = std::to_string(match.getTournamentId());
    std::string team1IdStr = std::to_string(match.getTeam1Id());
    std::string team2IdStr = std::to_string(match.getTeam2Id());
    std::string team1ScoreStr = std::to_string(match.getTeam1Score());
    std::string team2ScoreStr = std::to_string(match.getTeam2Score());

    // Конвертируем строки в UTF-8
    std::string matchDate = ConvertToUTF8(match.getMatchDate());
    std::string stage = ConvertToUTF8(match.getStage());
    std::string status = ConvertToUTF8(match.getStatus());

    std::string sql = "INSERT INTO matches (tournament_id, team1_id, team2_id, match_date, team1_score, team2_score, stage, status) VALUES ($1, $2, $3, $4, $5, $6, $7, $8)";
    const char* params[8] = { tournamentIdStr.c_str(), team1IdStr.c_str(), team2IdStr.c_str(),
                              matchDate.c_str(), team1ScoreStr.c_str(), team2ScoreStr.c_str(),
                              stage.c_str(), status.c_str() };

    PGresult* res = PQexecParams(db.getConnection(), sql.c_str(), 8, NULL, params, NULL, NULL, 0);

    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        std::cerr << "Error adding match: " << PQerrorMessage(db.getConnection()) << std::endl;
        PQclear(res);
        return false;
    }
    PQclear(res);
    return true;
}

bool MatchRepository::updateMatch(const Match& match) {
    std::string idStr = std::to_string(match.getId());
    std::string tournamentIdStr = std::to_string(match.getTournamentId());
    std::string team1IdStr = std::to_string(match.getTeam1Id());
    std::string team2IdStr = std::to_string(match.getTeam2Id());
    std::string team1ScoreStr = std::to_string(match.getTeam1Score());
    std::string team2ScoreStr = std::to_string(match.getTeam2Score());

    std::string matchDate = ConvertToUTF8(match.getMatchDate());
    std::string stage = ConvertToUTF8(match.getStage());
    std::string status = ConvertToUTF8(match.getStatus());

    std::string sql = "UPDATE matches SET tournament_id = $1, team1_id = $2, team2_id = $3, match_date = $4, team1_score = $5, team2_score = $6, stage = $7, status = $8 WHERE id = $9";
    const char* params[9] = { tournamentIdStr.c_str(), team1IdStr.c_str(), team2IdStr.c_str(),
                              matchDate.c_str(), team1ScoreStr.c_str(), team2ScoreStr.c_str(),
                              stage.c_str(), status.c_str(), idStr.c_str() };

    PGresult* res = PQexecParams(db.getConnection(), sql.c_str(), 9, NULL, params, NULL, NULL, 0);

    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        std::cerr << "Error updating match: " << PQerrorMessage(db.getConnection()) << std::endl;
        PQclear(res);
        return false;
    }
    PQclear(res);
    return true;
}

bool MatchRepository::deleteMatch(int id) {
    std::string sql = "DELETE FROM matches WHERE id = $1";
    std::string idStr = std::to_string(id);
    const char* params[1] = { idStr.c_str() };

    PGresult* res = PQexecParams(db.getConnection(), sql.c_str(), 1, NULL, params, NULL, NULL, 0);

    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        std::cerr << "Error deleting match: " << PQerrorMessage(db.getConnection()) << std::endl;
        PQclear(res);
        return false;
    }
    PQclear(res);
    return true;
}

std::vector<Match> MatchRepository::getAllMatches() {
    std::vector<Match> matches;
    std::string sql = "SELECT m.id, m.tournament_id, m.team1_id, m.team2_id, "
        "TO_CHAR(m.match_date, 'DD.MM.YYYY HH24:MI') as match_date, "
        "m.team1_score, m.team2_score, m.stage, m.status, "
        "TO_CHAR(m.created_at, 'DD.MM.YYYY') as created_at "
        "FROM matches m ORDER BY m.match_date DESC";

    PGresult* res = PQexec(db.getConnection(), sql.c_str());

    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        std::cerr << "Error getting matches: " << PQerrorMessage(db.getConnection()) << std::endl;
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

std::vector<Match> MatchRepository::getMatchesByTournament(int tournamentId) {
    std::vector<Match> matches;
    std::string sql = "SELECT id, tournament_id, team1_id, team2_id, "
        "TO_CHAR(match_date, 'DD.MM.YYYY HH24:MI') as match_date, "
        "team1_score, team2_score, stage, status, "
        "TO_CHAR(created_at, 'DD.MM.YYYY') as created_at "
        "FROM matches WHERE tournament_id = $1 ORDER BY match_date";
    std::string idStr = std::to_string(tournamentId);
    const char* params[1] = { idStr.c_str() };

    PGresult* res = PQexecParams(db.getConnection(), sql.c_str(), 1, NULL, params, NULL, NULL, 0);

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

std::vector<Match> MatchRepository::getMatchesByStatus(const std::string& status) {
    std::vector<Match> matches;
    std::string sql = "SELECT id, tournament_id, team1_id, team2_id, "
        "TO_CHAR(match_date, 'DD.MM.YYYY HH24:MI') as match_date, "
        "team1_score, team2_score, stage, status, "
        "TO_CHAR(created_at, 'DD.MM.YYYY') as created_at "
        "FROM matches WHERE status = $1 ORDER BY match_date";
    const char* params[1] = { status.c_str() };

    PGresult* res = PQexecParams(db.getConnection(), sql.c_str(), 1, NULL, params, NULL, NULL, 0);

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

bool MatchRepository::updateMatchScore(int id, int team1Score, int team2Score) {
    std::string sql = "UPDATE matches SET team1_score = $1, team2_score = $2 WHERE id = $3";
    std::string idStr = std::to_string(id);
    std::string team1ScoreStr = std::to_string(team1Score);
    std::string team2ScoreStr = std::to_string(team2Score);
    const char* params[3] = { team1ScoreStr.c_str(), team2ScoreStr.c_str(), idStr.c_str() };

    PGresult* res = PQexecParams(db.getConnection(), sql.c_str(), 3, NULL, params, NULL, NULL, 0);

    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        std::cerr << "Error updating match score: " << PQerrorMessage(db.getConnection()) << std::endl;
        PQclear(res);
        return false;
    }
    PQclear(res);
    return true;
}

bool MatchRepository::updateMatchStatus(int id, const std::string& status) {
    std::string sql = "UPDATE matches SET status = $1 WHERE id = $2";
    std::string idStr = std::to_string(id);
    std::string statusUtf8 = ConvertToUTF8(status);
    const char* params[2] = { statusUtf8.c_str(), idStr.c_str() };

    PGresult* res = PQexecParams(db.getConnection(), sql.c_str(), 2, NULL, params, NULL, NULL, 0);

    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        std::cerr << "Error updating match status: " << PQerrorMessage(db.getConnection()) << std::endl;
        PQclear(res);
        return false;
    }
    PQclear(res);
    return true;
}

Match MatchRepository::getMatchById(int id) {
    std::string sql = "SELECT id, tournament_id, team1_id, team2_id, "
        "TO_CHAR(match_date, 'DD.MM.YYYY HH24:MI') as match_date, "
        "team1_score, team2_score, stage, status, "
        "TO_CHAR(created_at, 'DD.MM.YYYY') as created_at "
        "FROM matches WHERE id = $1";
    std::string idStr = std::to_string(id);
    const char* params[1] = { idStr.c_str() };

    PGresult* res = PQexecParams(db.getConnection(), sql.c_str(), 1, NULL, params, NULL, NULL, 0);

    if (PQresultStatus(res) != PGRES_TUPLES_OK || PQntuples(res) == 0) {
        PQclear(res);
        return Match();
    }

    Match match(std::stoi(PQgetvalue(res, 0, 0)),
        std::stoi(PQgetvalue(res, 0, 1)),
        std::stoi(PQgetvalue(res, 0, 2)),
        std::stoi(PQgetvalue(res, 0, 3)),
        PQgetvalue(res, 0, 4),
        std::stoi(PQgetvalue(res, 0, 5)),
        std::stoi(PQgetvalue(res, 0, 6)),
        PQgetvalue(res, 0, 7),
        PQgetvalue(res, 0, 8),
        PQgetvalue(res, 0, 9));
    PQclear(res);
    return match;
}

bool MatchRepository::matchExists(int id) {
    std::string sql = "SELECT id FROM matches WHERE id = $1";
    std::string idStr = std::to_string(id);
    const char* params[1] = { idStr.c_str() };

    PGresult* res = PQexecParams(db.getConnection(), sql.c_str(), 1, NULL, params, NULL, NULL, 0);

    bool exists = (PQresultStatus(res) == PGRES_TUPLES_OK && PQntuples(res) > 0);
    PQclear(res);
    return exists;
}

std::string MatchRepository::getMatchDisplayName(int matchId) {
    TeamRepository teamRepo(db);
    Match match = getMatchById(matchId);
    if (match.getId() == 0) return "Unknown";

    std::string team1 = teamRepo.getTeamDisplayName(match.getTeam1Id());
    std::string team2 = teamRepo.getTeamDisplayName(match.getTeam2Id());
    return team1 + " vs " + team2;
}