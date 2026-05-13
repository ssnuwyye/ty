#pragma once
#include <string>

class Sport {
private:
    int id;
    std::string name;
    std::string icon;
    std::string color;
    std::string description;
public:
    Sport(int id = 0, const std::string& name = "",
        const std::string& icon = "", const std::string& color = "",
        const std::string& description = "");

    int getId() const;
    std::string getName() const;
    std::string getIcon() const;
    std::string getColor() const;
    std::string getDescription() const;

    void setId(int id);
    void setName(const std::string& name);
    void setIcon(const std::string& icon);
    void setColor(const std::string& color);
    void setDescription(const std::string& description);

    void display() const;
};