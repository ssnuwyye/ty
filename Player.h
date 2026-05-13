#pragma once
#include <string>

class Player {
private:
    int id;
    int teamId;
    std::string nickname;
    std::string fullName;
    std::string role;
    std::string nationality;
    int rating;
    std::string createdAt;
public:
    Player(int id = 0, int teamId = 0, const std::string& nickname = "",
        const std::string& fullName = "", const std::string& role = "",
        const std::string& nationality = "", int rating = 1000,
        const std::string& createdAt = "");

    int getId() const;
    int getTeamId() const;
    std::string getNickname() const;
    std::string getFullName() const;
    std::string getRole() const;
    std::string getNationality() const;
    int getRating() const;
    std::string getCreatedAt() const;

    void setId(int id);
    void setTeamId(int teamId);
    void setNickname(const std::string& nickname);
    void setFullName(const std::string& fullName);
    void setRole(const std::string& role);
    void setNationality(const std::string& nationality);
    void setRating(int rating);
    void setCreatedAt(const std::string& createdAt);

    void display() const;
};