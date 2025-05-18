#pragma once
#include <algorithm>

struct BoundingBox {
        double minX, minY;
        double maxX, maxY;

        bool isValid() const { return minX <= maxX && minY <= maxY; }
        double width() const { return maxX - minX; }
        double height() const { return maxY - minY; }

        void expandToInclude(const BoundingBox& other) {
                minX = std::min(minX, other.minX);
                minY = std::min(minY, other.minY);
                maxX = std::max(maxX, other.maxX);
                maxY = std::max(maxY, other.maxY);
        }
};

