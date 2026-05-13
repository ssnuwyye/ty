#pragma once
#include <string>

class Match {
private:
    int id;
    int tournamentId;
    int team1Id;
    int team2Id;
    std::string matchDate;
    int team1Score;
    int team2Score;
    std::string stage;
    std::string status;
    std::string createdAt;
public:
    Match(int id = 0, int tournamentId = 0, int team1Id = 0, int team2Id = 0,
        const std::string& matchDate = "", int team1Score = 0, int team2Score = 0,
        const std::string& stage = "group", const std::string& status = "scheduled",
        const std::string& createdAt = "");

    int getId() const;
    int getTournamentId() const;
    int getTeam1Id() const;
    int getTeam2Id() const;
    std::string getMatchDate() const;
    int getTeam1Score() const;
    int getTeam2Score() const;
    std::string getStage() const;
    std::string getStatus() const;
    std::string getCreatedAt() const;

    void setId(int id);
    void setTournamentId(int tournamentId);
    void setTeam1Id(int team1Id);
    void setTeam2Id(int team2Id);
    void setMatchDate(const std::string& matchDate);
    void setTeam1Score(int team1Score);
    void setTeam2Score(int team2Score);
    void setStage(const std::string& stage);
    void setStatus(const std::string& status);
    void setCreatedAt(const std::string& createdAt);

    void display() const;
};