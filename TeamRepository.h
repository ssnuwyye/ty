#pragma once
#include "Team.h"
#include "DatabaseManager.h"
#include <vector>

class TeamRepository {
private:
    DatabaseManager& db;
public:
    TeamRepository(DatabaseManager& dbManager);
    bool addTeam(const Team& team);
    bool updateTeam(const Team& team);
    bool deleteTeam(int id);
    Team getTeamById(int id);
    std::vector<Team> getAllTeams();
    std::vector<Team> getTeamsBySport(int sportId);
    std::vector<Team> getTeamsByTournament(int tournamentId);
    bool addTeamToTournament(int teamId, int tournamentId);
    bool removeTeamFromTournament(int teamId, int tournamentId);
    bool updateTeamStats(int id, int wins, int losses, int points);
    bool teamExists(int id);
    std::string getTeamDisplayName(int teamId);
};