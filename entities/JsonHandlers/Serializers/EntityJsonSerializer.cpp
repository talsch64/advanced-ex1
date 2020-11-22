#include <iostream>
#include "EntityJsonSerializer.h"
#include "../../Entity.h"
#include "../../POI.h"
#include "../../Junction.h"
#include "../../Way.h"
#include "../../geometry/PointList.h"

rapidjson::Value EntityJsonSerializer::entityToJson(Entity *entity, rapidjson::Document::AllocatorType &allocator) {
    rapidjson::Value json;
    json.SetObject();
    json = setString(std::move(json), "type", entity->getType(), allocator);
    json = setString(std::move(json), "id", entity->getId(), allocator);
    json = setString(std::move(json), "name", entity->getName(), allocator);
    if (entity->getDescription()) {
        json = setString(std::move(json), "description", entity->getDescription().value(), allocator);
    }
    json = setStringVector(std::move(json), "category_tags", entity->getCategoryTags(), allocator);
    return json;
}

rapidjson::Value EntityJsonSerializer::toJson(POI *entity, rapidjson::Document::AllocatorType &allocator) {
    rapidjson::Value entityJson = entityToJson(entity, allocator);
    entityJson = setStringVector(std::move(entityJson), "accessibility", entity->getAccessibility(), allocator);
    Geometry *geometry = entity->getGeometry().get();
    rapidjson::Value geometryJson = geometry->toJson(allocator);
    entityJson.AddMember("geometry", geometryJson, allocator);

    return entityJson;
}

rapidjson::Value EntityJsonSerializer::toJson(Junction *entity, rapidjson::Document::AllocatorType &allocator) {
    rapidjson::Value json = entityToJson(entity, allocator);
    Geometry *geometry = entity->getGeometry().get();
    rapidjson::Value geometryJson = geometry->toJson(allocator);
    json.AddMember("coordinates", geometryJson, allocator);
    return json;
}

rapidjson::Value
EntityJsonSerializer::toJson(Way *entity, rapidjson::Document::AllocatorType &allocator) {
    rapidjson::Value json = entityToJson(entity, allocator);
    json = setString(std::move(json), "direction", entity->getDirection(), allocator);

    rapidjson::Value speedLimit;
    speedLimit.SetInt(entity->getSpeedLimit());
    json.AddMember("speed_limit", speedLimit, allocator);

    rapidjson::Value tollRoad;
    tollRoad.SetBool(entity->isTollRoad());
    json.AddMember("toll_road", tollRoad, allocator);

    json = setString(std::move(json), "from", entity->getFrom(), allocator);

    json = setString(std::move(json), "to", entity->getTo(), allocator);

    json = setStringVector(std::move(json), "restricted", entity->getRestricted(), allocator);

    Geometry *geometry = entity->getGeometry().get();
    rapidjson::Value curves = geometry->toJson(allocator);
    json.AddMember("curves", curves, allocator);

    return json;
}

rapidjson::Value
EntityJsonSerializer::setString(rapidjson::Value json, const rapidjson::GenericStringRef<char> &name, const std::string& str,
                                rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator> &allocator) {
    rapidjson::Value val;
    val.SetString(str.c_str(), str.length(), allocator);
    json.AddMember(name, val, allocator);
    return json;
}

rapidjson::Value EntityJsonSerializer::setStringVector(rapidjson::Value json, const rapidjson::GenericStringRef<char>& name,
                                                       const std::vector<std::string> &vec,
                                                       rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator> &allocator) {
    rapidjson::Value arr;
    arr.SetArray();
    for (const std::string& str : vec) {
        rapidjson::Value elVal;
        elVal.SetString(str.c_str(), str.length(), allocator);
        arr.PushBack(elVal, allocator);
    }
    json.AddMember(name, arr, allocator);
    return json;
}
