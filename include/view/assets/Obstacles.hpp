#pragma once

#include "view/assets/Obstacle.hpp"

class Ball : public Obstacle {
public:
    explicit Ball(int variant = 1);
};

class Bed : public Obstacle {
public:
    Bed();
};

class Plant : public Obstacle {
public:
    Plant();
};

class Sofa : public Obstacle {
public:
    explicit Sofa(int variant = 1);
    float getRotationFacingCenter(int row, int col, int boardRows, int boardCols) const override;
};

class SofaCoffe : public Obstacle {
public:
    SofaCoffe();
};

class TV : public Obstacle {
public:
    TV();
    float getRotationFacingCenter(int row, int col, int boardRows, int boardCols) const override;
};

class Table : public Obstacle {
protected:
    Table(const std::string& name, const std::string& texturePath);
};

class Desk : public Table {
public:
    Desk();
};

class DinnerTable : public Table {
public:
    DinnerTable();
};
