#include "Tournament.h"
#include <iostream>

Tournament::Tournament(int id, int sportId, const std::string& name,
    const std::string& startDate, const std::string& endDate,
    double prizePool, const std::string& status, const std::string& createdAt)
    : id(id), sportId(sportId), name(name), startDate(startDate),
    endDate(endDate), prizePool(prizePool), status(status), createdAt(createdAt) {
}

int Tournament::getId() const { return id; }
int Tournament::getSportId() const { return sportId; }
std::string Tournament::getName() const { return name; }
std::string Tournament::getStartDate() const { return startDate; }
std::string Tournament::getEndDate() const { return endDate; }
double Tournament::getPrizePool() const { return prizePool; }
std::string Tournament::getStatus() const { return status; }
std::string Tournament::getCreatedAt() const { return createdAt; }

void Tournament::setId(int id) { this->id = id; }
void Tournament::setSportId(int sportId) { this->sportId = sportId; }
void Tournament::setName(const std::string& name) { this->name = name; }
void Tournament::setStartDate(const std::string& startDate) { this->startDate = startDate; }
void Tournament::setEndDate(const std::string& endDate) { this->endDate = endDate; }
void Tournament::setPrizePool(double prizePool) { this->prizePool = prizePool; }
void Tournament::setStatus(const std::string& status) { this->status = status; }
void Tournament::setCreatedAt(const std::string& createdAt) { this->createdAt = createdAt; }

void Tournament::display() const {
    std::cout << "ID: " << id << ", Sport ID: " << sportId
        << ", Name: " << name << ", Status: " << status
        << ", Prize Pool: $" << prizePool << std::endl;
}