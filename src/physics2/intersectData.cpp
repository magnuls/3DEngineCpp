#include "intersectData.h"

IntersectData::IntersectData(const bool doesIntersect, const float distance)
    : m_doesIntersect(doesIntersect), distance(distance) {};

bool IntersectData::getDoesIntersect() const { return m_doesIntersect; }
float IntersectData::getDistance() const { return distance; }
