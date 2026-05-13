#pragma once
#include "Tournament.h"
#include "Match.h"
#include "DatabaseManager.h"
#include <vector>

class TournamentRepository {
private:
    DatabaseManager& db;
public:
    TournamentRepository(DatabaseManager& dbManager);
    bool addTournament(const Tournament& tournament);
    bool updateTournament(const Tournament& tournament);
    bool deleteTournament(int id);
    Tournament getTournamentById(int id);
    std::vector<Tournament> getAllTournaments();
    std::vector<Tournament> getTournamentsBySport(int sportId);
    std::vector<Tournament> getTournamentsByStatus(const std::string& status);
    std::vector<Match> getUpcomingMatchesBySport(int sportId, int limit = 5);
    bool updateTournamentStatus(int id, const std::string& status);
    bool tournamentExists(int id);
    std::string getTournamentDisplayName(int tournamentId);
};