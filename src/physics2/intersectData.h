#pragma once

class IntersectData {
   private:
    const bool m_doesIntersect;
    const float distance;

   public:
    IntersectData(const bool doesIntersect, const float distance);

    bool getDoesIntersect() const;
    float getDistance() const;
};
