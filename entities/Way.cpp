
#include "JsonHandlers/Serializers/EntityJsonSerializer.h"

#include "Way.h"

Way::Way(const std::string &id, const std::string &name, const std::string &description,
         const std::vector<std::string> &categoryTags, const std::string &from, const std::string &to,
         const std::vector<Coordinates> &curves, const std::string &direction, int speedLimit, bool tollRoad,
         const std::vector<std::string> &restricted) : Entity(id, name, description, categoryTags), from(from), to(to),
                                                       curves(curves), direction(direction),
                                                       speedLimit(speedLimit),
                                                       tollRoad(tollRoad), restricted(restricted) {}

rapidjson::Value Way::toJson(rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator> &allocator) {
    return entityJsonSerializer->toJson(this, allocator);
}

const EntityId &Way::getFrom() const {
    return from;
}

const EntityId &Way::getTo() const {
    return to;
}

const std::vector<Coordinates> &Way::getCurves() const {
    return curves;
}

const std::string &Way::getDirection() const {
    return direction;
}

int Way::getSpeedLimit() const {
    return speedLimit;
}

bool Way::isTollRoad() const {
    return tollRoad;
}

const std::vector<std::string> &Way::getRestricted() const {
    return restricted;
}
