#pragma once
#include "Sport.h"
#include "DatabaseManager.h"
#include <vector>

class SportRepository {
private:
    DatabaseManager& db;
public:
    SportRepository(DatabaseManager& dbManager);
    bool addSport(const Sport& sport);
    bool updateSport(const Sport& sport);
    bool deleteSport(int id);
    Sport getSportById(int id);
    std::vector<Sport> getAllSports();
    bool sportExists(int id);
    std::string getSportName(int sportId);
};