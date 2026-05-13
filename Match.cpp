#include "Match.h"
#include <iostream>

Match::Match(int id, int tournamentId, int team1Id, int team2Id,
    const std::string& matchDate, int team1Score, int team2Score,
    const std::string& stage, const std::string& status, const std::string& createdAt)
    : id(id), tournamentId(tournamentId), team1Id(team1Id), team2Id(team2Id),
    matchDate(matchDate), team1Score(team1Score), team2Score(team2Score),
    stage(stage), status(status), createdAt(createdAt) {
}

int Match::getId() const { return id; }
int Match::getTournamentId() const { return tournamentId; }
int Match::getTeam1Id() const { return team1Id; }
int Match::getTeam2Id() const { return team2Id; }
std::string Match::getMatchDate() const { return matchDate; }
int Match::getTeam1Score() const { return team1Score; }
int Match::getTeam2Score() const { return team2Score; }
std::string Match::getStage() const { return stage; }
std::string Match::getStatus() const { return status; }
std::string Match::getCreatedAt() const { return createdAt; }

void Match::setId(int id) { this->id = id; }
void Match::setTournamentId(int tournamentId) { this->tournamentId = tournamentId; }
void Match::setTeam1Id(int team1Id) { this->team1Id = team1Id; }
void Match::setTeam2Id(int team2Id) { this->team2Id = team2Id; }
void Match::setMatchDate(const std::string& matchDate) { this->matchDate = matchDate; }
void Match::setTeam1Score(int team1Score) { this->team1Score = team1Score; }
void Match::setTeam2Score(int team2Score) { this->team2Score = team2Score; }
void Match::setStage(const std::string& stage) { this->stage = stage; }
void Match::setStatus(const std::string& status) { this->status = status; }
void Match::setCreatedAt(const std::string& createdAt) { this->createdAt = createdAt; }

void Match::display() const {
    std::cout << "ID: " << id << ", Match: Team " << team1Id
        << " vs Team " << team2Id << ", Score: " << team1Score
        << "-" << team2Score << ", Status: " << status << std::endl;
}