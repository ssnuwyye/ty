#pragma once
#include <string>

class Team {
private:
    int id;
    int sportId;
    int tournamentId;
    std::string name;
    std::string tag;
    std::string logo;
    int wins;
    int losses;
    int points;
    std::string createdAt;
public:
    Team(int id = 0, int tournamentId = 0, const std::string& name = "",
        const std::string& tag = "", const std::string& logo = "🏢",
        int wins = 0, int losses = 0, int points = 0,
        const std::string& createdAt = "");

    int getId() const;
    int getSportId() const;           // Новый метод
    void setSportId(int sportId);
    int getTournamentId() const;
    std::string getName() const;
    std::string getTag() const;
    std::string getLogo() const;
    int getWins() const;
    int getLosses() const;
    int getPoints() const;
    std::string getCreatedAt() const;

    void setId(int id);
    void setTournamentId(int tournamentId);
    void setName(const std::string& name);
    void setTag(const std::string& tag);
    void setLogo(const std::string& logo);
    void setWins(int wins);
    void setLosses(int losses);
    void setPoints(int points);
    void setCreatedAt(const std::string& createdAt);

    void display() const;
};