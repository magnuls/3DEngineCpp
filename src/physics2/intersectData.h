#pragma once

// Holds the data if we have intersected and distance between the objects
namespace Physics {
struct IntersectData {
    float distance;
    bool m_doesIntersect;

    IntersectData(bool doesIntersect, float dist)
        : distance(dist), m_doesIntersect(doesIntersect) {};
};
}  // namespace Physics
