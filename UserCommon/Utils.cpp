#include <cstring>
#include <memory>
#include "Utils.h"

double Utils::kmh_to_mm(int speed) {
    return static_cast<double>(speed) * 1000 / 60;
}

Minutes Utils::calculateTime(Meters distance, int speed) {
    return Minutes(static_cast<double>(distance / Utils::kmh_to_mm(speed)));
}

std::vector<std::string> Utils::split(const char *array, const char *delimiters) {
    std::vector<std::string> vector;
    auto arrayCopy = std::make_unique<char[]>(strlen(array));
    strcpy(arrayCopy.get(), array);
    char *token = strtok(arrayCopy.get(), delimiters);
    while (token != nullptr) {
        vector.push_back(std::string(token));
        token = strtok(nullptr, delimiters);
    }
    return vector;
}

bool Utils::isWayRestricted(const AbstractWay &way, const Restrictions &restrictions) {
    for (std::string restriction : restrictions.getRestrictions()) {
        if ((restriction == "highway" && way.isHighway()) || (restriction == "toll" && way.isToll())) {
            return true;
        }
    }
    return false;
}

Minutes Utils::getWayDuration(Meters length, int speed) {
    return Minutes(static_cast<double>(length) / kmh_to_mm(speed));
}

