#pragma once
#include "Match.h"
#include "DatabaseManager.h"
#include <vector>

class MatchRepository {
private:
    DatabaseManager& db;
public:
    MatchRepository(DatabaseManager& dbManager);
    bool addMatch(const Match& match);
    bool updateMatch(const Match& match);
    bool deleteMatch(int id);
    Match getMatchById(int id);
    std::vector<Match> getAllMatches();
    std::vector<Match> getMatchesByTournament(int tournamentId);
    std::vector<Match> getMatchesByStatus(const std::string& status);
    bool updateMatchScore(int id, int team1Score, int team2Score);
    bool updateMatchStatus(int id, const std::string& status);
    bool matchExists(int id);
    std::string getMatchDisplayName(int matchId);
};