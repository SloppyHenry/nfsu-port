#pragma once
// Table.cpp: Graph (Stückweise lineare Kurve über bVector2-Punkte), hier nur GetValue (WP-30.3 ergänzt den Rest).
#include <cmath>

#include "game/base/bmath.hpp"

namespace nfsu {

struct Graph {
    bVector2* Points = nullptr;
    int NumPoints = 0;

    // orig: 0x59cf80 Graph::GetValue: vor dem ersten Punkt y des ersten, ab dem letzten Punkt y des letzten
    float GetValue(float x) const {
        int n = NumPoints;
        if (n > 1) {
            if (x < Points[0].x) return Points[0].y;
            if (Points[n - 1].x <= x) return Points[n - 1].y;
            for (int i = 0; i < n - 1; ++i) {
                if (Points[i].x <= x && x < Points[i + 1].x) {
                    float dy = Points[i + 1].y - Points[i].y;
                    float dx = Points[i + 1].x - Points[i].x;
                    if (std::fabs(dx) <= 1e-06f) return dy * 0.5f + Points[i].y;
                    return ((x - Points[i].x) / dx) * dy + Points[i].y;
                }
            }
        }
        return Points[0].y;
    }

    // orig: 0x59d040 Graph::GetInverse: x zu gegebenem y (y muss über x steigen)
    float GetInverse(float y) const {
        int n = NumPoints;
        if (n > 1) {
            if (y < Points[0].y) return Points[0].x;
            if (Points[n - 1].y <= y) return Points[n - 1].x;
            for (int i = 0; i < n - 1; ++i) {
                if (Points[i].y <= y && y < Points[i + 1].y) {
                    double dx = static_cast<double>(Points[i + 1].x) - Points[i].x;
                    double dy = static_cast<double>(Points[i + 1].y) - Points[i].y;
                    if (1e-06 < std::fabs(dy)) return static_cast<float>(((y - static_cast<double>(Points[i].y)) / dy) * dx + Points[i].x);
                    return static_cast<float>(dx * 0.5 + Points[i].x);
                }
            }
        }
        return Points[0].x;
    }
};

}  // namespace nfsu
