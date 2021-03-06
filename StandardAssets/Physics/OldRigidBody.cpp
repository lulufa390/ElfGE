//
// Created by 张程易 on 31/10/2017.
//

#include "OldRigidBody.h"

void OldRigidBody::onCollisionEnter(Collider *collider, glm::vec3 info) {
    if (!isTrigger && !isFace) {
        auto target = collider->getGameObject();
        auto rb = target->getComponent<OldRigidBody>();
        if (rb == nullptr) return;
        glm::vec3 directVector;
        if (rb->isFace) directVector = getGameObject()->getWorldForward();
        else directVector = glm::normalize(target->getWorldPosition() - getGameObject()->getWorldPosition());

        float va = glm::dot(directVector, velocity);
        float vb = glm::dot(directVector, rb->velocity);
        float ma = mass;
        float mb = rb->mass;
        if (fabs(va) < 1e-4 && fabs(vb) < 1e-4) return;
        if ((va >= 0 && vb <= 0)
            || (va >= 0 && vb >= 0 && va >= vb)
            || (va <= 0 && vb <= 0 && va >= vb)) {

            printf("pz\n");
            float p = ma * va + mb * vb;
            float m = ma + mb;
            float vc = (collisionK * mb * (vb - va) + p) / m;
            float vd = (collisionK * ma * (va - vb) + p) / m;

            velocity += (vc - va) * directVector;
            rb->velocity += (vd - vb) * directVector;
            onCollision = true;

        }
    }
    GameScript::onCollisionEnter(collider, info);
}

void OldRigidBody::update() {
    velocity += (float) Utility::deltaTime / mass * force;
    if (useGravity) velocity += (float) Utility::deltaTime * g;
    getGameObject()->transform.translate(velocity);
}

void OldRigidBody::onCollisionExit(Collider *collider) {
    if (!isTrigger) {
        onCollision = false;

    }
    GameScript::onCollisionExit(collider);
}

void OldRigidBody::start() {

}

OldRigidBody::OldRigidBody(float mass, const glm::vec3 &velocity, const glm::vec3 &force, float collisionK, bool isTrigger,
                     bool useGravity) : mass(mass), collisionK(collisionK), isTrigger(isTrigger),
                                        useGravity(useGravity), velocity(velocity), force(force) {}

std::string OldRigidBody::getName() {
    return "RigidBody";
}

