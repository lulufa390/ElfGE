//
// Created by 张程易 on 30/10/2017.
//

#ifndef ELFGE_RUNTIME_H
#define ELFGE_RUNTIME_H


#include "Scene.h"
#include "../Utility.h"
#include "../../Assets/Entry.h"

class Runtime {
    static Scene * scene;
public:

    static Scene *getScene();

    static void setScene(Scene *scene);

    static Camera *getCamera();

    static void setCamera(Camera *camera);

    static void start();

};


#endif //ELFGE_RUNTIME_H
