//
// Created by 张程易 on 31/10/2017.
//

#include "StandardMaterial.h"

StandardMaterial StandardMaterial::instance;

void StandardMaterial::use(Shader *shader) {
    shader->setFloat("material.shininess", 20.0f);
    shader->setFloat("gradient", Config::FogGradient);
    shader->setFloat("density", Config::FogDensity);
}

Material * StandardMaterial::getInstance() {
    return &instance;
}
