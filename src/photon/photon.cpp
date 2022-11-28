#include "photon.hpp"

const double ALIVE = 1;
const double DEAD = 0;

Photon::Photon() : position(CartVec()), direction(CartVec(0, 0, -1)), weight(ALIVE) {}
Photon::Photon(CartVec pos, CartVec dir) : position(pos), direction(CartVec(0, 0, -1)), weight(ALIVE) {}
Photon::Photon(CartVec pos, CartVec dir, double weight) : position(pos), direction(dir), weight(weight) {}

void Photon::step(const double size) {
    directionHistory.push(position);
    position += size * direction;
}

double Photon::unstep() {
    double step = (position.z - directionHistory.top().z) / direction.z;
    position = directionHistory.top();
    directionHistory.pop();

    return step;
}

void Photon::stepToHeight(const double height) {
    directionHistory.push(position);
    step((height - position.z) / direction.z);
}

void Photon::changeDirection(double x, double y, double z) {
    direction.x = x;
    direction.y = y;
    direction.z = z;
}