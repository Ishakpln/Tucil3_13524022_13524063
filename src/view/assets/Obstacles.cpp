#include "view/assets/Obstacles.hpp"
#include <cmath>
#include <string>

namespace {
    float snapToPerpendicularAngle(float angle) {
        float snapped = std::round(angle / 90.0f) * 90.0f;

        if (snapped >= 360.0f) snapped -= 360.0f;
        if (snapped <= -360.0f) snapped += 360.0f;
        if (std::abs(snapped) < 0.001f) snapped = 0.0f;

        return snapped;
    }

    float rotationFromDownVectorToCenter(int row, int col, int widthTiles, int heightTiles, int boardRows, int boardCols) {
        const float objectCenterX = col + widthTiles / 2.0f;
        const float objectCenterY = row + heightTiles / 2.0f;
        const float boardCenterX = boardCols / 2.0f;
        const float boardCenterY = boardRows / 2.0f;

        const float dx = boardCenterX - objectCenterX;
        const float dy = boardCenterY - objectCenterY;

        if (dx == 0.0f && dy == 0.0f) {
            return 0.0f;
        }

        float angle = std::atan2(dx, dy) * RAD2DEG;

        return snapToPerpendicularAngle(angle);
    }
}

Ball::Ball(int variant)
    : Obstacle("Ball" + std::to_string(variant), "./assets/components/Ball/Ball" + std::to_string(variant) + ".png") {}

Bed::Bed()
    : Obstacle("Bed", "./assets/components/Bed/Bed1.png") {}

Plant::Plant()
    : Obstacle("Plant", "./assets/components/Plant/Plant1.png") {}

Sofa::Sofa(int variant)
    : Obstacle("Sofa" + std::to_string(variant), "./assets/components/Sofa/Sofa" + std::to_string(variant) + ".png") {}

float Sofa::getRotationFacingCenter(int row, int col, int boardRows, int boardCols) const {
    return rotationFromDownVectorToCenter(row, col, getWidthInTiles(), getHeightInTiles(), boardRows, boardCols);
}

SofaCoffe::SofaCoffe()
    : Obstacle("SofaCoffe", "./assets/components/SofaCoffe/Sofa.png") {}

TV::TV()
    : Obstacle("TV", "./assets/components/TV/TV Backdrop.png") {}

float TV::getRotationFacingCenter(int row, int col, int boardRows, int boardCols) const {
    return rotationFromDownVectorToCenter(row, col, getWidthInTiles(), getHeightInTiles(), boardRows, boardCols);
}

Table::Table(const std::string& name, const std::string& texturePath)
    : Obstacle(name, texturePath) {}

Desk::Desk()
    : Table("Desk", "./assets/components/Table/Desk.png") {}

DinnerTable::DinnerTable()
    : Table("DinnerTable", "./assets/components/Table/Dinner1.png") {}
