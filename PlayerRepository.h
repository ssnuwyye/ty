#pragma once
#include "Player.h"
#include "DatabaseManager.h"
#include <vector>

class PlayerRepository {
private:
    DatabaseManager& db;
public:
    PlayerRepository(DatabaseManager& dbManager);
    bool addPlayer(const Player& player);
    bool updatePlayer(const Player& player);
    bool deletePlayer(int id);
    Player getPlayerById(int id);
    std::vector<Player> getAllPlayers();
    std::vector<Player> getPlayersByTeam(int teamId);
    bool updatePlayerRating(int id, int rating);
    bool playerExists(int id);
    std::string getPlayerDisplayName(int playerId);
};