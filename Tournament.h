#pragma once
#include <string>

class Tournament {
private:
    int id;
    int sportId;
    std::string name;
    std::string startDate;
    std::string endDate;
    double prizePool;
    std::string status;
    std::string createdAt;
public:
    Tournament(int id = 0, int sportId = 0, const std::string& name = "",
        const std::string& startDate = "", const std::string& endDate = "",
        double prizePool = 0.0, const std::string& status = "upcoming",
        const std::string& createdAt = "");

    int getId() const;
    int getSportId() const;
    std::string getName() const;
    std::string getStartDate() const;
    std::string getEndDate() const;
    double getPrizePool() const;
    std::string getStatus() const;
    std::string getCreatedAt() const;

    void setId(int id);
    void setSportId(int sportId);
    void setName(const std::string& name);
    void setStartDate(const std::string& startDate);
    void setEndDate(const std::string& endDate);
    void setPrizePool(double prizePool);
    void setStatus(const std::string& status);
    void setCreatedAt(const std::string& createdAt);

    void display() const;
};