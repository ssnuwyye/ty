#include "Sport.h"
#include <iostream>

Sport::Sport(int id, const std::string& name, const std::string& icon,
    const std::string& color, const std::string& description)
    : id(id), name(name), icon(icon), color(color), description(description) {
}

int Sport::getId() const { return id; }
std::string Sport::getName() const { return name; }
std::string Sport::getIcon() const { return icon; }
std::string Sport::getColor() const { return color; }
std::string Sport::getDescription() const { return description; }

void Sport::setId(int id) { this->id = id; }
void Sport::setName(const std::string& name) { this->name = name; }
void Sport::setIcon(const std::string& icon) { this->icon = icon; }
void Sport::setColor(const std::string& color) { this->color = color; }
void Sport::setDescription(const std::string& description) { this->description = description; }

void Sport::display() const {
    std::cout << "ID: " << id << ", Sport: " << name
        << ", Icon: " << icon << ", Color: " << color << std::endl;
}