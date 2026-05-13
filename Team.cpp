#include "Team.h"
#include <iostream>

Team::Team(int id, int tournamentId, const std::string& name,
    const std::string& tag, const std::string& logo,
    int wins, int losses, int points, const std::string& createdAt)
    : id(id), tournamentId(tournamentId), name(name), tag(tag), logo(logo),
    wins(wins), losses(losses), points(points), createdAt(createdAt) {
}

int Team::getId() const { return id; }
int Team::getSportId() const { return sportId; }
std::string Team::getName() const { return name; }
std::string Team::getTag() const { return tag; }
std::string Team::getLogo() const { return logo; }
int Team::getWins() const { return wins; }
int Team::getLosses() const { return losses; }
int Team::getPoints() const { return points; }
std::string Team::getCreatedAt() const { return createdAt; }


void Team::setId(int id) { this->id = id; }
void Team::setName(const std::string& name) { this->name = name; }
void Team::setTag(const std::string& tag) { this->tag = tag; }
void Team::setLogo(const std::string& logo) { this->logo = logo; }
void Team::setWins(int wins) { this->wins = wins; }
void Team::setLosses(int losses) { this->losses = losses; }
void Team::setPoints(int points) { this->points = points; }
void Team::setCreatedAt(const std::string& createdAt) { this->createdAt = createdAt; }

void Team::display() const {
    std::cout << "ID: " << id << ", Team: " << name
        << " [" << tag << "], W/L: " << wins << "/" << losses
        << ", Points: " << points << std::endl;
}