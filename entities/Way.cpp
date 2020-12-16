
#include "JsonHandlers/Serializers/EntityJsonSerializer.h"

#include "Way.h"

#include <utility>

Way::Way(const EntityId &id, const std::string &name, const std::string &description,
         const std::vector<std::string> &categoryTags, std::unique_ptr<Geometry> geometry, EntityId from, EntityId to,
         std::string direction, int speedLimit, bool tollRoad,
         std::vector<std::string> restricted) : Entity(id, name, description, categoryTags, std::move(geometry), "Way"),
                                                       from(std::move(from)), to(std::move(to)), direction(std::move(direction)), speedLimit(speedLimit),
                                                       tollRoad(tollRoad), restricted(std::move(restricted)) {}


rapidjson::Value Way::toJson(rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator> &allocator) {
    return entityJsonSerializer->toJson(this, allocator);
}

const EntityId &Way::getFrom() const {
    return from;
}

const EntityId &Way::getTo() const {
    return to;
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

std::pair<EntityId, EntityId> Way::getJunctions() const {
    return std::pair<EntityId, EntityId>(from, to);
}
