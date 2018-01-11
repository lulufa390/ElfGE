//
// Created by sky on 2017/12/21.
//

#include "GrassRenderer.h"
static void dumpGLErrorLog(int l) {
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR) {
        std::cout<<"glError: "<<err<<" line_num:"<<l<<std::endl;
    }
}
void GrassRenderer::initBladeIndices(){
    int seg;
    unsigned int vc1 = 0;
    unsigned int vc2 = BLADE_VERTS;
    int i=0;
    // blade front side
    for (seg = 0; seg < BLADE_SEGS; ++seg) {
        index[i++] = vc1 + 0; // tri 1
        index[i++] = vc1 + 1;
        index[i++] = vc1 + 2;
        index[i++] = vc1 + 2; // tri 2
        index[i++] = vc1 + 1;
        index[i++] = vc1 + 3;
        vc1 += 2;
    }
    // blade back side
    for (seg = 0; seg < BLADE_SEGS; ++seg) {
        index[i++] = vc2 + 2; // tri 1
        index[i++] = vc2 + 1;
        index[i++] = vc2 + 0;
        index[i++] = vc2 + 3; // tri 2
        index[i++] = vc2 + 1;
        index[i++] = vc2 + 2;
        vc2 += 2;
    }
    for(int i=0;i<index.size();i++){
//        std::cout<<index[i]<<std::endl;
    }
}

void GrassRenderer::initBladeShapeVerts(){
    float noise = 0;
    // to be generated by simplex algorithm
    for (int i = 0; i < numBlades; ++i) {
//            noise = fabs(simplex(offset[i*4+0] * 0.03, offset[i*4+1] * 0.03));
        noise = noise * noise * noise;
        noise *= 5.0;
        shape[i*4+0] = BLADE_WIDTH + randomGen() * BLADE_WIDTH * 0.5 ;// width
        shape[i*4+1] = (float)(BLADE_HEIGHT_MIN + pow(randomGen(), 4.0) * (BLADE_HEIGHT_MAX - BLADE_HEIGHT_MIN) + // height
                       noise);
//        shape[i*4+1] = BLADE_HEIGHT_MIN;
        shape[i*4+2] = 0.0 + randomGen() * 0.3; // lean
//        shape[i*4+3] = 0.05 + randomGen() * 0.3; // curve
        shape[i*4+3] = randomGen() * 0.3; // curve
//        std::cout<<shape[i*4+0]<<" "<<shape[i*4+1]<<" "<<shape[i*4+2]<<" "<<shape[i*4+3]<<" "<<i<<std::endl;
    }
}
void GrassRenderer::initBladeOffsetVerts() {
    for (int i = 0; i < numBlades; ++i) {
        offset[i*4+0] = (randomGen() - 0.5) * 2  * radius; // x
        offset[i*4+1] = (randomGen() - 0.5) * 2 * radius; // y
        offset[i*4+2] = 0.0; // z
        offset[i*4+3] = (float)(M_PI * 2.0 * randomGen()); // rot
//        std::cout<<offset[i*4+0]<<" "<<offset[i*4+1]<<" "<<offset[i*4+2]<<" "<<offset[i*4+3]<<" "<<i<<std::endl;
    }
}
void GrassRenderer::initBladeIndexVerts() {
    for (int i = 0; i < vindex.size(); ++i) {
        vindex[i] = i;
    }
}

float GrassRenderer::randomGen() {  // generate a random value from 0 to 1
    static std::random_device rd;  //Will be used to obtain a seed for the random number engine
    static std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    static std::uniform_real_distribution<float> dis(0, 1.0);
    return dis(gen);
}

void GrassRenderer::prepare() {
    static float time = 0;
   //printf("hello");
    dumpGLErrorLog(__LINE__);
    shader->use();
    dumpGLErrorLog(__LINE__);
    shader->setFloat("time",(float)time);
    time = fmod(time + Utility::deltaTime,20000000000);
//    time+=Utility::deltaTime;
    dumpGLErrorLog(__LINE__);
    GameObject * cameraT = Runtime::getCamera()->getGameObject();
    shader->setVec3("camDir",cameraT->getWorldPosition());
    dumpGLErrorLog(__LINE__);
    glm::vec2 v1,v2;
    v1.x = cameraT->getWorldPosition().x;
    v1.y = cameraT->getWorldPosition().z;
    v2.x = cameraT->getWorldForward().x;
    v2.y = cameraT->getWorldForward().z;
    v2 = glm::normalize(v2)*radius;
//    shader->setVec2("drawPos",v1 + v2);
    shader->setVec2("drawPos",glm::vec2(0,0));
    dumpGLErrorLog(__LINE__);
    shader->setMat4("modelViewMatrix",Runtime::getCamera()->getViewMatrix()*
                                      getGameObject()->getModelMatrix());
//                                      glm::mat4(1));
    dumpGLErrorLog(__LINE__);
    shader->setMat4("projectionMatrix",Runtime::getCamera()->getProjectionMatrix());
    shader->setInt("map",0);
    shader->setInt("heightMap",1);

    dumpGLErrorLog(__LINE__);
    shader->setFloat("BLADE_HEIGHT_TALL",BLADE_HEIGHT_MAX*1.5);
    shader->setFloat("BLADE_SEGS",BLADE_SEGS);
    shader->setFloat("PATCH_SIZE",radius*2);
    shader->setFloat("TRANSITION_LOW",transitionLow);
    shader->setFloat("TRANSITION_HIGH",transitionHigh);
    shader->setVec3("heightMapScale",heightMapScale);
    shader->setVec3("grassColor",grassColor);
    shader->setVec3("fogColor",fogColor);
    shader->setFloat("fogNear",1.0);
    shader->setFloat("fogFar",fogFar);
    shader->setFloat("grassFogFar",grassFogFar);
    shader->setVec3("fogColor",fogColor);
    shader->setFloat("windIntensity",windIntensity);

    dumpGLErrorLog(__LINE__);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D,texture);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, heightMap);
}

void GrassRenderer::bindVertice() {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &vindexVBO);
    glGenBuffers(1, &shapeVBO);
    glGenBuffers(1, &offsetVBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, vindexVBO);
    glBufferData(GL_ARRAY_BUFFER, vindex.size() * sizeof(float), &vindex[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, shapeVBO);
    glBufferData(GL_ARRAY_BUFFER, shape.size() * sizeof(float), &shape[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, offsetVBO);
    glBufferData(GL_ARRAY_BUFFER, offset.size() * sizeof(float), &offset[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, index.size() * sizeof(GLuint), &index[0], GL_STATIC_DRAW);


   // printf("hello");
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER,vindexVBO);
    glVertexAttribPointer(0, 1, GL_FLOAT, GL_FALSE,0 , 0);
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER,shapeVBO);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE,0,  0);
    glVertexAttribDivisor(1,1);
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER,offsetVBO);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 0,  0);
    glVertexAttribDivisor(2,1);
    glBindVertexArray(0);
}



void GrassRenderer::updateGraphics(RenderLayer renderLayer) {
    if(renderLayer != RenderLayer::WORLD) return;
    dumpGLErrorLog(__LINE__);
    prepare();
    dumpGLErrorLog(__LINE__);
    glBindVertexArray(VAO);
    glDrawElementsInstanced(GL_TRIANGLES,(GLsizei) index.size(),GL_UNSIGNED_INT,0,numBlades);
//    glDrawElements(GL_TRIANGLES,(GLsizei)12 ,GL_UNSIGNED_INT,0);
    glBindVertexArray(0);
}