#include "Player.h"
#include <iostream>

Player::Player(int id, int teamId, const std::string& nickname,
    const std::string& fullName, const std::string& role,
    const std::string& nationality, int rating, const std::string& createdAt)
    : id(id), teamId(teamId), nickname(nickname), fullName(fullName),
    role(role), nationality(nationality), rating(rating), createdAt(createdAt) {
}

int Player::getId() const { return id; }
int Player::getTeamId() const { return teamId; }
std::string Player::getNickname() const { return nickname; }
std::string Player::getFullName() const { return fullName; }
std::string Player::getRole() const { return role; }
std::string Player::getNationality() const { return nationality; }
int Player::getRating() const { return rating; }
std::string Player::getCreatedAt() const { return createdAt; }

void Player::setId(int id) { this->id = id; }
void Player::setTeamId(int teamId) { this->teamId = teamId; }
void Player::setNickname(const std::string& nickname) { this->nickname = nickname; }
void Player::setFullName(const std::string& fullName) { this->fullName = fullName; }
void Player::setRole(const std::string& role) { this->role = role; }
void Player::setNationality(const std::string& nationality) { this->nationality = nationality; }
void Player::setRating(int rating) { this->rating = rating; }
void Player::setCreatedAt(const std::string& createdAt) { this->createdAt = createdAt; }

void Player::display() const {
    std::cout << "ID: " << id << ", Player: " << nickname
        << " (" << fullName << "), Role: " << role
        << ", Rating: " << rating << std::endl;
}