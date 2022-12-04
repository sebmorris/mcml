#include "photon.hpp"

Photon::Photon() : position_(CartVec(0, 0, 0)), direction_(CartVec(0, 0, -1)) {  }
Photon::Photon(CartVec pos, CartVec dir) : position_(pos), direction_(dir) {  }

void Photon::step(const double size) {
    positionHistory_.push(position_);
    position_ += size * direction_;
}

double Photon::unstep() {
    double step = (position_.z() - positionHistory_.top().z()) / direction_.z();
    position_ = positionHistory_.top();
    positionHistory_.pop();

    return step;
}

void Photon::stepToHeight(const double height) {
    positionHistory_.push(position_);
    step((height - position_.z()) / direction_.z());
}

void Photon::setDirection(double x, double y, double z) {
    setDirection(CartVec(x, y, z));
}

void Photon::setDirection(const CartVec& direction) {
    direction_ = direction;
}

double Photon::weight() const {
    return weight_;
}

const CartVec& Photon::position() const {
    return position_;
}

const CartVec& Photon::direction() const {
    return direction_;
}

void Photon::incrementWeight(double inc) {
    weight_ += inc;
}

void Photon::setWeight(double weight) {
    weight_ = weight;
}