#include "transform_utils.h"
#include <algorithm>

SDL_Point worldToScreen(double x, double y, const Transform& t) {
	int sx = static_cast<int>(x * t.scale + t.offsetX);
	int sy = t.screenH - static_cast<int>(y * t.scale + t.offsetY);
	return { sx, sy };
}

Transform makeTransform(const BoundingBox& box, int screenW, int screenH, double padding) {
        double worldW = box.maxX - box.minX;
        double worldH = box.maxY - box.minY;

        // shrink the drawable area by padding * 2
        double scaleX = (screenW - 2*padding) / worldW;
        double scaleY = (screenH - 2*padding) / worldH;
        double scale = std::min(scaleX, scaleY);

        double offsetX = padding - box.minX * scale;
        double offsetY = padding - box.minY * scale;

        return { scale, offsetX, offsetY, screenH };
}

