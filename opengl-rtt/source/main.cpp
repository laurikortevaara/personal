#include <cassert>
#include <iostream>
#include <stdexcept>
#include <cmath>
#include <chrono>
#include <ctime>
#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Program.h"
#include "Texture.h"
#include "Shader.h"
#include "FrameBufferObject.h"
#include "ShadowMap.h"
#include "Object.h"
#include "Utils.h"

#include <fbxsdk.h>


// constants
//const glm::vec2 SCREEN_SIZE(1920, 1080);
const glm::vec2 SCREEN_SIZE(1024, 768);

// globals
GLFWwindow* gWindow = NULL;
GLuint color_tex = 0;
GLuint depth_tex = 0;
GLuint fbo = 0;

// FBX SDK
FbxManager* gFbxManager = NULL;

Texture* gTexture = NULL;
Texture* gTexture2 = NULL;
Texture* gTexture4 = NULL;
Texture* gTextureGround1 = NULL;
Texture* gTextureGround2 = NULL;
Texture* gTextureGround3 = NULL;

Program* gCurrentProgram = NULL;
Program* gProgramSimple = NULL;
Program* gProgramTexture = NULL;
Program* gProgramRTT = NULL;
Program* gProgramShadowMap = NULL;
Program* gProgramQuad = NULL;
GLuint   gCurrentTexture = NULL;
GLuint   gQuadTexture = NULL;
GLboolean gAnimate = true;

GLfloat  gZNear = 1.0f;
GLfloat  gZFar = 2000.0f;
GLfloat  gShadowDelta = -0.0005;
GLfloat  gJoystickDelta = 0.08;

glm::vec2 gJoystickLeft;
glm::vec2 gJoystickRight;

enum JOYSTICK_BUTTON{
    SELECT = 0,
    LEFT_ANALOG = 1,
    RIGHT_ANALOG = 2,
    START = 3,
    UP = 4,
    RIGHT = 5,
    DOWN = 6,
    LEFT = 7,
    L2 = 8,
    R2 = 9,
    L1 = 10,
    R1 = 11,
    TRIANGLE = 12,
    CIRCLE = 13,
    X = 14,
    SQUARE = 15,
    PS = 16
};

std::vector<Object*> gObjects;
Object* gQuad = NULL;

FrameBufferObject* gFrameBuffer = NULL;
ShadowMap* gShadowMap = NULL;

GLuint gVAO = 0;
GLuint gVBO = 0;
GLfloat gDegreesRotated = 0.0f;

GLuint gVAOQuad = 0;
GLuint gVBOquad = 0;

glm::vec3 gCameraPos(0,0,0);
GLfloat gCameraDistance = 3.0f;
GLfloat gCameraAngleX = 0.0f;
GLfloat gCameraAngleY = 0.0f;

static void renderToScreen();
static void renderModel();
static void renderToFbo();
static void renderQuad();

static glm::mat4 lookAt(glm::vec3 pos, glm::vec3 target, glm::vec3 up, GLfloat near, GLfloat far);
static glm::mat4 lookAtOrtho(glm::vec3 pos,glm::vec3 target,glm::vec3 up,GLfloat near,GLfloat far,GLfloat left=-5,GLfloat top=5,GLfloat right = 5,GLfloat bottom = -5);


long fibonacci(unsigned n)
{
    if (n < 2) return n;
    return fibonacci(n-1) + fibonacci(n-2);
}

void testTime()
{
    std::chrono::time_point<std::chrono::system_clock> start, end;
    start = std::chrono::system_clock::now();
    std::cout << "f(42) = " << fibonacci(42) << '\n';
    end = std::chrono::system_clock::now();
    
    std::chrono::duration<double> elapsed_seconds = end-start;
    std::time_t end_time = std::chrono::system_clock::to_time_t(end);
    
    std::cout << "finished computation at " << std::ctime(&end_time)
    << "elapsed time: " << elapsed_seconds.count() << "s\n";
}

static void renderQuad()
{
    if(gQuad==NULL)
    {
        gQuad = new Object();
        std::vector<Vertex> vertices({{-1,-1,0,0,0,-1,0,0},{1,-1,0,0,0,-1,1,0},{1,1,0,0,0,-1,1,1},
                                      {-1,-1,0,0,0,-1,0,0},{1,1,0,0,0,-1,1,1},{-1,1,0,0,0,-1,0,1}});
        gQuad->setMesh(vertices);
    }
    glViewport(0,0,500,500);
    glScissor(0,0,500,500);
    glEnable(GL_SCISSOR_TEST);
    
    gCurrentProgram = gProgramQuad;
    gCurrentProgram->use();
    
    // clear everything
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );

    GLfloat left =   1.0f;
    GLfloat right = -1.0f;
    GLfloat top =    1.0f;
    GLfloat bottom =-1.0f;
    glm::mat4 viewProj = lookAtOrtho({0,0,-1}, {0,0,0}, {0,1,0}, gZNear, gZFar,left,top,right,bottom);
    gCurrentProgram->setUniform("mvp", viewProj);
        
    GLint texture = gCurrentProgram->uniform("tex");
    GLint shadowTexture = gCurrentProgram->uniform("shadowMap");
    
    glActiveTexture(GL_TEXTURE0);
    glBindTexture( GL_TEXTURE_2D, gFrameBuffer->texture());
    glUniform1i(texture,0);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture( GL_TEXTURE_2D, gShadowMap->texture());
    glUniform1i(shadowTexture,1);
    
    gQuad->render();
    
    glScissor(0,0,SCREEN_SIZE.x,SCREEN_SIZE.y);
    glViewport(0,0,SCREEN_SIZE.x,SCREEN_SIZE.y);

    glDisable(GL_SCISSOR_TEST);
    glDisable(GL_STENCIL_TEST);
}

static void initializeQuad();


static void checkMem()
{
    /**
     GL_VENDOR
     Returns the company responsible for this GL implementation. This name does not change from release to release.
     
     GL_RENDERER
     Returns the name of the renderer. This name is typically specific to a particular configuration of a hardware platform. It does not change from release to release.
     
     GL_VERSION
     Returns a version or release number of the form OpenGL<space>ES<space><version number><space><vendor-specific information>.
     
     GL_SHADING_LANGUAGE_VERSION
     Returns a version or release number for the shading language of the form OpenGL<space>ES<space>GLSL<space>ES<space><version number><space><vendor-specific information>.
     
     GL_EXTENSIONS
     **/
    const GLchar* vendor = (const GLchar*)glGetString(GL_VENDOR);
    Utils::checkGLError();
    const GLchar* renderer = (const GLchar*)glGetString(GL_RENDERER);
    Utils::checkGLError();
    const GLchar* version = (const GLchar*)glGetString(GL_VERSION);
    Utils::checkGLError();
    const GLchar* shadingLangVersion = (const GLchar*)glGetString(GL_SHADING_LANGUAGE_VERSION);
    Utils::checkGLError();
    GLint numExtensions;
    
    std::vector<std::string> extensions;
    glGetIntegerv(GL_NUM_EXTENSIONS, &numExtensions);
    for(int i=0; i < numExtensions; ++i){
        extensions.push_back(std::string( (char*)(glGetStringi(GL_EXTENSIONS,i))));
    }
    Utils::checkGLError();

    std::string strVendor(vendor);
    std::string strRenderer(renderer);
    std::string strVersion(version);
    std::string strGLSLVersion(shadingLangVersion);
    std::string strExensions(extensions.empty()?"No extensions available":"");
    for( std::string strExtension : extensions)
        strExensions.append(strExtension+", ");
    
    /*
    int dedVideoMem=0,totalAvailableMem=0, curAvailableMem=0, evictionCount=0, evictedMem=0;
    Utils::checkGLError();
    glGetIntegerv(0x9047, &dedVideoMem);
    Utils::checkGLError();
    glGetIntegerv(0x9048, &totalAvailableMem);
    Utils::checkGLError();
    glGetIntegerv(0x9049, &curAvailableMem);
    Utils::checkGLError();
    glGetIntegerv(0x904A, &evictionCount);
    Utils::checkGLError();
    glGetIntegerv(0x904B, &evictedMem);
    Utils::checkGLError();*/
    
    std::string strDedicatedVieoMem = "";
    std::string strTotalAvailable = "";
    std::string strCurrentlyAvailable = "";
    std::string strEvictionCount = "";
    std::string strEvictedMem = "";
    Utils::checkGLError();
    std::stringstream is(strDedicatedVieoMem);
    //is << "Dedicated Video Memory: " << dedVideoMem / 1024 << " kb" << " (" << dedVideoMem / 1024 / 1024 << " MB)";
    strDedicatedVieoMem = is.str();
    is.str(std::string());
    is.flush();
    is.clear();
    Utils::checkGLError();
    is = std::stringstream(strTotalAvailable);
    //is << "Total Video Memory Available: " << totalAvailableMem / 1024 << " kb" << " (" << totalAvailableMem / 1024 / 1024 << " MB)";
    strTotalAvailable = is.str();
    is.str(std::string());
    is.flush();
    is.clear();
    
    Utils::checkGLError();
    is = std::stringstream(strCurrentlyAvailable);
    //is << "Current Video Memory Available: " << curAvailableMem / 1024 << " kb" << " (" << curAvailableMem / 1024 / 1024 << " MB)";
    strCurrentlyAvailable = is.str();
    is.str(std::string());
    is.flush();
    is.clear();
    
    Utils::checkGLError();
    std::cout << strDedicatedVieoMem << std::endl;
    std::cout << strTotalAvailable << std::endl;
    std::cout << strCurrentlyAvailable << std::endl;
    std::cout << strVendor << std::endl;
    std::cout << strRenderer << std::endl;
    std::cout << strVersion << std::endl;
    std::cout << strGLSLVersion << std::endl;
    std::cout << strExensions << std::endl;
    Utils::checkGLError();
    std::cout << "Is NVX_gpu_memory_info supported: " <<  glfwExtensionSupported("NVX_gpu_memory_info") << std::endl;
    Utils::checkGLError();
}


// loads the vertex shader and fragment shader, and links them to make the global gProgram
static void LoadShaders() {

    gProgramTexture = new Program({
        Shader(GL_VERTEX_SHADER,"shaders/passthrough_vertex.shader.vs"),
        Shader(GL_FRAGMENT_SHADER,"shaders/texture_fragment.shader.fs")});
    
    gProgramRTT = new Program({
        Shader(GL_VERTEX_SHADER,"shaders/RTT_vertex.shader.vs"),
        Shader(GL_FRAGMENT_SHADER,"shaders/RTT_fragment.shader.fs")});
    
    
    gProgramShadowMap = new Program(std::vector<Shader>({
        Shader(GL_VERTEX_SHADER,"shaders/ShadowMap_vertex.shader.vs"),
        Shader(GL_FRAGMENT_SHADER,"shaders/ShadowMap_fragment.shader.fs")}));
    
    gProgramSimple = new Program(std::vector<Shader>({
        Shader(GL_VERTEX_SHADER,"shaders/passthrough_vertex.shader.vs"),
        Shader(GL_FRAGMENT_SHADER,"shaders/basic_fragment.shader.fs")}));
    
    gProgramQuad = new Program(std::vector<Shader>({
        Shader(GL_VERTEX_SHADER,"shaders/quad_vs.shader"),
        Shader(GL_FRAGMENT_SHADER,"shaders/quad_fs.shader")}));
}

static glm::mat4 lookAt(glm::vec3 pos, glm::vec3 target, glm::vec3 up, GLfloat near, GLfloat far)
{
    //set the "projection" uniform in the vertex shader, because it's not going to change
    glm::mat4 projection = glm::perspective(glm::radians(60.0f), SCREEN_SIZE.x/SCREEN_SIZE.y, near, far);
    
    //set the "camera" uniform in the vertex shader, because it's also not going to change
    glm::mat4 view = glm::lookAt(pos, target, up);
    return projection * view;
}

static glm::mat4 lookAtOrtho(glm::vec3 pos, glm::vec3 target, glm::vec3 up, GLfloat near, GLfloat far,
            GLfloat left, GLfloat top, GLfloat right, GLfloat bottom)
{
    //set the "projection" uniform in the vertex shader, because it's not going to change
    glm::mat4 projection = glm::ortho(left,right,bottom,top, near, far);
    
    //set the "camera" uniform in the vertex shader, because it's also not going to change
    glm::mat4 view = glm::lookAt(pos, target, up);
    
    return projection * view;
}

static std::vector<Vertex> createCube(GLfloat s)
{
    return
    {{-s,-s,-s,0,-1,0,0,0},{ s,-s,-s,0,-1,0,s,0},{ s,-s, s,0,-1,0,s,s}, // bottom
    {-s,-s,-s,0,-1,0,0,0},{ s,-s, s,0,-1,0,s,s},{-s,-s, s,0,-1,0,0,s}, // bottom
    {-s, s,-s,0, 1,0,0,0},{ s, s, s,0, 1,0,s,s},{ s, s,-s,0, 1,0,s,0}, // top
    {-s, s,-s,0, 1,0,0,0},{-s, s, s,0, 1,0,0,s},{ s, s, s,0, 1,0,s,s}, // top
    {-s,-s,-s,-1,0,0,0,0},{-s, s, s,-1, 0,0,s,s},{-s, s,-s,-1,0,0,s,0}, // left
    {-s,-s,-s,-1,0,0,0,0},{-s,-s, s,-1, 0,0,0,s},{-s, s, s,-1,0,0,s,s}, // left
    { s,-s,-s, 1,0,0,0,0},{ s, s,-s, 1, 0,0,s,0},{ s, s, s,1,0,0,s,s}, // right
    { s,-s,-s, 1,0,0,0,0},{ s, s, s, 1, 0,0,s,s},{ s,-s, s,1,0,0,0,s}, // right
    {-s,-s,-s,0,0,-1,0,0},{ s, s,-s,0,0,-1,s,s},{ s,-s,-s,0,0,-1,s,0}, // front
    {-s,-s,-s,0,0,-1,0,0},{-s, s,-s,0,0,-1,0,s},{ s, s,-s,0,0,-1,s,s}, // front
    {-s,-s, s,0,0,1,0,0},{ s,-s, s,0,0,1,s,0},{ s, s, s,0,0,1,s,s}, // back
    {-s,-s, s,0,0,1,0,0},{ s, s, s,0,0,1,s,s},{-s, s, s,0,0,1,0,s}}; // back
}

static GLfloat rand(GLfloat minValue, GLfloat maxValue)
{
    return fmin(maxValue,fmax(minValue,rand()/(GLfloat)RAND_MAX*minValue + rand()/(GLfloat)RAND_MAX * maxValue));
}

GLboolean readFbx(std::vector<Vertex>& vertices)
{
    if(gFbxManager == nullptr)
    {
        gFbxManager = FbxManager::Create();
        
        FbxIOSettings* pIOsettings = FbxIOSettings::Create(gFbxManager, IOSROOT );
        gFbxManager->SetIOSettings(pIOsettings);
    }
    
    FbxImporter* pImporter = FbxImporter::Create(gFbxManager,"");
    FbxScene* pFbxScene = FbxScene::Create(gFbxManager,"");
    
    bool bSuccess = pImporter->Initialize("models/quad.fbx", -1, gFbxManager->GetIOSettings() );
    if(!bSuccess) return GL_FALSE;
    
    bSuccess = pImporter->Import(pFbxScene);
    if(!bSuccess) return GL_FALSE;
    
    pImporter->Destroy();
    
    FbxNode* pFbxRootNode = pFbxScene->GetRootNode();
    
    if(pFbxRootNode)
    {
        for(int i = 0; i < pFbxRootNode->GetChildCount(); i++)
        {
            FbxNode* pFbxChildNode = pFbxRootNode->GetChild(i);
            
            if(pFbxChildNode->GetNodeAttribute() == NULL)
                continue;
            
            FbxNodeAttribute::EType AttributeType = pFbxChildNode->GetNodeAttribute()->GetAttributeType();
            
            if(AttributeType != FbxNodeAttribute::eMesh)
                continue;
            
            FbxMesh* pMesh = (FbxMesh*) pFbxChildNode->GetNodeAttribute();
            
            FbxVector4* pVertices = pMesh->GetControlPoints();
            
            for (int j = 0; j < pMesh->GetPolygonCount(); j++)
            {
                int iNumVertices = pMesh->GetPolygonSize(j);
                assert( iNumVertices == 3 );
                
                FbxLayerElementNormal *Normals = pMesh->GetLayer( 0 )->GetNormals( );
                FbxLayerElement::EMappingMode MappingMode = pMesh->GetLayer( 0 )->GetNormals( )->GetMappingMode( );
                
                for (int k = 0; k < iNumVertices; k++)
                {
                    int iControlPointIndex = pMesh->GetPolygonVertex(j, k);
                    
                    Vertex vertex;
                    glm::vec3 v({ (float)pVertices[iControlPointIndex].mData[0],
                                  (float)pVertices[iControlPointIndex].mData[2],
                                  (float)pVertices[iControlPointIndex].mData[1]});
                    
                    vertex.x = v.x;
                    vertex.y = v.y;
                    vertex.z = v.z;
                    
                    FbxVector4 Normal = Normals->GetDirectArray( ).GetAt( k );
                    vertex.nx = ( float ) Normal.mData[0];
                    vertex.ny = ( float ) Normal.mData[2];
                    vertex.nz = ( float ) Normal.mData[1];

                    vertices.push_back( vertex );
                }
            }
        }
    }
    return true;
}



static void CreateObjects() {
    float y = -1.0f;
    float s = 1.5f;

/*    for(int i=0; i < 10; ++i)
    {
        Object* obj1 = new Object();
        std::vector<Vertex> vertices;
        GLfloat s = rand(0.3,1.5);
        vertices = createCube(s);

        obj1->setMesh(vertices);
        obj1->setTexture(gTexture);
        gObjects.push_back(obj1);
        obj1->setModelMatrix(glm::translate(glm::mat4(1),glm::vec3(rand(-100,100),rand(0,5),rand(-100,100))));
    }
    
    Object* obj2 = new Object();
    std::vector<Vertex> vertices2;
        vertices2 = {{-s,y,-s,0,1,0,0,0},{ s, y,s,0,1,0,s,s},{ s,y,-s,0,1,0,s,0},
                 {-s,y,-s,0,1,0,0,0},{ -s, y,s,0,1,0,0,s},{ s,y, s,0,1,0,s,s}};
    obj2->setMesh(vertices2);
    obj2->setTexture(gTexture2);
    gObjects.push_back(obj2);
  */
    
    Object* obj3 = new Object();
    std::vector<Vertex> vertices3;
    y = -2.0f;
    s = 1000.0f;

    vertices3 = {{-s,y,-s,0,1,0,0,0},{ s, y,s,0,1,0,s,s},{ s,y,-s,0,1,0,s,0},
                 {-s,y,-s,0,1,0,0,0},{ -s, y,s,0,1,0,0,s},{ s,y, s,0,1,0,s,s}};

    obj3->setMesh(vertices3);
    obj3->setTexture(gTextureGround2);
    gObjects.push_back(obj3);
    
    std::vector<Vertex> fbxVerts;
    if(readFbx(fbxVerts))
    {
        Object* obj4 = new Object();
        obj4->setMesh(fbxVerts);
        obj4->setTexture(gTexture4);
        gObjects.push_back(obj4);
    }
}


// loads the file "wooden-crate.jpg" into gTexture
static void LoadTexture() {
    gTexture = new Texture("textures/Stone_Floor_texture.png");
    gTexture2 = new Texture("textures/purple.png");
    gTexture4 = new Texture("textures/purple.png");
    
    gTextureGround1 = new Texture("textures/ground1.jpg");
    gTextureGround2 = new Texture("textures/ground2.jpg");
    gTextureGround3 = new Texture("textures/ground3.jpg");

    gTexture->printInfo();
    gTexture2->printInfo();
}

void initializeFbo()
{
    gFrameBuffer = new FrameBufferObject(SCREEN_SIZE.x,SCREEN_SIZE.y,GL_RGB);
}

void initializeShadowMap()
{
    gShadowMap = new ShadowMap(2048,2048, GL_RGBA);
}

static void renderToFbo()
{
    gCurrentProgram = gProgramRTT;
    gCurrentProgram->use();
    gFrameBuffer->use();
    
    glm::mat4 viewProj = lookAt(gCameraPos,{0,0,0},{0,0,-1}, gZNear, gZFar);
    
    GLfloat s= 10.0f;
    GLfloat left =   s;
    GLfloat right = -s;
    GLfloat top =    s;
    GLfloat bottom =-s;
    glm::mat4 viewProjOrtho = lookAtOrtho(gCameraPos, {0,0,0}, {0,0,1}, gZNear, gZFar, left,top,right,bottom);
    
    // clear everything
    glClearColor(0, 0, 0, 1); // black
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );
    
    for(auto model : gObjects)
    {
        glm::mat4 mvp = viewProjOrtho * model->modelMatrix() * model->rotateMatrix();
        //glActiveTexture(GL_TEXTURE0);
        //glBindTexture(GL_TEXTURE_2D,model->texture()->object());
    
        // set the "model" uniform in the vertex shader, based on the gDegreesRotated global
        if(gCurrentProgram->hasUniform("mvp"))
            gCurrentProgram->setUniform("mvp", mvp);
        

        model->render();
    }
    gCurrentProgram->unuse();
    gFrameBuffer->unuse();
}

static void renderToShadowMap()
{
    glPolygonOffset(1.1, 4.0);
    bool bShadow = true;
    if(bShadow){
        gCurrentProgram = gProgramShadowMap;
        gCurrentProgram->use();
        gShadowMap->use();
    }
    else{
        gCurrentProgram = gProgramRTT;
        gCurrentProgram->use();
        gFrameBuffer->use();
    }
    
    // clear everything
    glClearColor(0, 0, 0, 0); // black
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );
    
    for(auto model : gObjects)
    {
        glm::mat4 mvp = gShadowMap->getMVP() * model->modelMatrix() * model->rotateMatrix();
        
        // set the "model" uniform in the vertex shader, based on the gDegreesRotated global
        if(gCurrentProgram->hasUniform("mvp"))
            gCurrentProgram->setUniform("mvp", mvp);
        
        model->render();
    }
    if(bShadow)
        gShadowMap->unuse();
    else
        gFrameBuffer->unuse();
    
    gCurrentProgram->unuse();
    glPolygonOffset(0.0,0.0);
    //glDisable(GL_CULL_FACE);
}

void uninitializeFbo()
{
    //Delete resources
    glDeleteTextures(1, &color_tex);
    glDeleteTextures(1, &depth_tex);
    //Bind 0, which means render to back buffer, as a result, fb is unbound
    glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
    glDeleteFramebuffersEXT(1, &fbo);
    glViewport(0,0,SCREEN_SIZE.x, SCREEN_SIZE.y);
}

static void renderModel()
{
    // bind the VAO (the triangle)
    glBindVertexArray(gVAO);
    
    // draw the VAO
    glDrawArrays(GL_TRIANGLES, 0, 6*2*3);
    
    // unbind the VAO, the program and the texture
    glBindVertexArray(0);
}

// draws a single frame
static void renderToScreen()
{    
    gCurrentProgram = gProgramTexture;
    gCurrentProgram->use();
    
    glm::mat4 viewProj = lookAt(gCameraPos,{0,0,0},{0,1,0}, gZNear, gZFar);
    glm::mat4 viewProjOrtho = lookAtOrtho({0,0,-10}, {0,0,0}, {0,1,0}, gZNear, gZFar);
    
    // clear everything
    glClearColor(0, 0, 0, 1); // black
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );
    
    glm::mat4 biasMatrix(
                         0.5, 0.0, 0.0, 0.0,
                         0.0, 0.5, 0.0, 0.0,
                         0.0, 0.0, 0.5, 0.0,
                         0.5, 0.5, 0.5, 1.0
                         );
    GLint texture = gCurrentProgram->uniform("tex");
    GLint shadowTexture = gCurrentProgram->uniform("shadowMap");

    for(auto model : gObjects)
    {
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D,model->texture()->object());
        glUniform1i(texture,0);
        
        glActiveTexture(GL_TEXTURE1);
        glBindTexture( GL_TEXTURE_2D, gShadowMap->texture());
        glUniform1i(shadowTexture,1);

        glm::mat4 mvp =  viewProj * model->modelMatrix() * model->rotateMatrix();
        glm::mat4 depthBiasMVP = biasMatrix*gShadowMap->getMVP()*model->modelMatrix() * model->rotateMatrix();
        gCurrentProgram->setUniform("DepthBiasMVP", depthBiasMVP);
        gCurrentProgram->setUniform("shadowDelta", gShadowDelta);
        gCurrentProgram->setUniform("lightPos", gShadowMap->getLightPos());
        gCurrentProgram->setUniform("modelView", glm::lookAt(gCameraPos, glm::vec3(0,0,0), glm::vec3(0,1,0)));
        gCurrentProgram->setUniform("CameraPos", gCameraPos);
        
        // set the "model" uniform in the vertex shader, based on the gDegreesRotated global
        if(gCurrentProgram->hasUniform("mvp"))
            gCurrentProgram->setUniform("mvp", mvp);

        model->render();
    }
    gCurrentProgram->unuse();
}


// update the scene based on the time elapsed since last update
void Update(float secondsElapsed) {
    if(gAnimate){
        const GLfloat degreesPerSecond = 30.0f;
        gDegreesRotated += secondsElapsed * degreesPerSecond;
        
        glm::mat4 m;
        /*glm::mat4 m = gObjects[0]->modelMatrix();
        m = glm::translate(glm::mat4(1), glm::vec3(-2+gDegreesRotated*0.01f,gDegreesRotated*0.01f, 0.0));
        m = glm::rotate(m, gDegreesRotated*0.01f, glm::vec3(1.0,0.5,0.25));
        gObjects[0]->setModelMatrix(m);
         */
        
        m = gObjects[1]->modelMatrix();
        m = glm::translate(glm::mat4(1), glm::vec3(2,0, 0.0));
        gObjects[1]->setModelMatrix(m);
/*
        m = gObjects[2]->modelMatrix();
        m = glm::translate(glm::mat4(1), glm::vec3(0,gDegreesRotated*0.001f, 0.0));
        gObjects[2]->setModelMatrix(m);
 */
        glm::vec3 lightPos = gShadowMap->getLightPos();
        GLfloat distance = 3.0;
        lightPos.x = cos(gDegreesRotated*(3.14/180.0))*distance;
        lightPos.z = sin(gDegreesRotated*(3.14/180.0))*distance;
        distance = 3.0 + sin(gDegreesRotated*(3.14/180.0)/2.0)*1.5;
        gShadowMap->setLightPos(lightPos);
    }
  
    int buttonCount;
    const unsigned char* buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &buttonCount);
    /*std::stringstream ss;
    for(int i=0; i < buttonCount; ++i){
        std::string pressed = buttons[i] == (const unsigned char)GLFW_PRESS ? "(X)" : "( )";
        ss << "[" << i << "]:" << pressed << ", ";
    }
    std::cout << ss.str() << std::endl;*/

    int count;
    const float* axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &count);
    if(fabs(axes[0])<gJoystickDelta)
        gJoystickLeft.x = 0.0f;
    else
        gJoystickLeft.x = -axes[0];

    if(fabs(axes[1])<gJoystickDelta)
        gJoystickLeft.y = 0.0f;
    else
        gJoystickLeft.y = -axes[1];
    
    if(fabs(axes[2])<gJoystickDelta)
        gJoystickRight.x = 0.0f;
    else
        gJoystickRight.x = -axes[2];
    
    if(fabs(axes[3])<gJoystickDelta)
        gJoystickRight.y = 0.0f;
    else
        gJoystickRight.y = -axes[3];
    
    if(gJoystickRight.x != 0.0f || gJoystickRight.y != 0.0f){
        int i=0;
    }
    glm::mat4 m = gObjects[0]->modelMatrix();
    glm::mat4 r = gObjects[0]->rotateMatrix();
    GLfloat ytrans = 0.0f;
    if(buttons[R2]==GLFW_PRESS)
        ytrans += 0.1f;
    if(buttons[L2]==GLFW_PRESS)
        ytrans -= 0.1f;
    //m = glm::translate(m, glm::vec3(gJoystickRight.x*0.5f,ytrans, gJoystickRight.y*0.5f));
    gCameraAngleY += gJoystickRight.x*0.1;
    gCameraAngleX += gJoystickRight.y*0.1;
    gCameraDistance -= gJoystickLeft.y*0.1;
    
    
    gCameraPos.x = (cos(gCameraAngleY))*gCameraDistance;
    gCameraPos.y = gCameraAngleX;
    gCameraPos.z = sin(gCameraAngleY)*gCameraDistance;
    
    //r = glm::rotate(r, gJoystickLeft.y*0.05f, glm::vec3(1.0,0.0f,0.0f));
    //r = glm::rotate(r, gJoystickLeft.x*0.05f, glm::vec3(0.0f,1.0f,0.0f));
    gObjects[0]->setModelMatrix(m);
    gObjects[0]->setRotateMatrix(r);

    /*
     glm::vec3 lightPos = gShadowMap->getLightPos();
    lightPos.x += gJoystickRight.x;
    lightPos.z += gJoystickRight.y;
    gShadowMap->setLightPos(lightPos);
     */
    
}

void OnError(int errorCode, const char* msg) {
    throw std::runtime_error(msg);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    if (key == GLFW_KEY_SPACE && action == GLFW_RELEASE)
        gAnimate = !gAnimate;
    
    if( key == GLFW_KEY_F && action == GLFW_RELEASE) {
        Texture* t = new Texture();
        t->setObject(gFrameBuffer->texture());
        t->writeToFile("test_fbo.png", true);
        
        t->setObject(gShadowMap->texture());
        t->writeToFile("test_shadowMap.png", true);
        
        t->printInfo();
        delete t;
    }
    
    if(key == GLFW_KEY_F && action == GLFW_RELEASE)
        gQuadTexture = gFrameBuffer->texture();
    if(key == GLFW_KEY_S && action == GLFW_RELEASE)
        gQuadTexture = gShadowMap->texture();
}


// the program starts here
void AppMain() {
    // initialise GLFW
    glfwSetErrorCallback(OnError);
    if(!glfwInit())
        throw std::runtime_error("glfwInit failed");
    
    // open a window with GLFW
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    gWindow = glfwCreateWindow((int)SCREEN_SIZE.x, (int)SCREEN_SIZE.y, "OpenGL Tutorial", NULL, NULL);
    if(!gWindow)
        throw std::runtime_error("glfwCreateWindow failed. Can your hardware handle OpenGL 3.2?");
    
    // GLFW settings
    glfwMakeContextCurrent(gWindow);
    Utils::checkGLError();
    
    glfwSetKeyCallback(gWindow, key_callback);
    
    // initialise GLEW
    glewExperimental = GL_TRUE; //stops glew crashing on OSX :-/
    if(glewInit() != GLEW_OK)
        throw std::runtime_error("glewInit failed");
    
    // GLEW throws some errors, so discard all the errors so far
    Utils::checkGLError(false);
    
    checkMem();
    Utils::checkGLError();
    
    // print out some info about the graphics drivers
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
    std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
    Utils::checkGLError();
    
    // make sure OpenGL version 3.2 API is available
    if(!GLEW_VERSION_3_2)
        throw std::runtime_error("OpenGL 3.2 API is not available.");
    
    // OpenGL settings
    glFrontFace(GL_CCW);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    Utils::checkGLError();
    glDepthFunc(GL_LESS);
    Utils::checkGLError();
    //glEnable(GL_CULL_FACE);
    glEnable(GL_BLEND);
    Utils::checkGLError();
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    Utils::checkGLError();
    
    //initializeQuad();
    
    gFbxManager = FbxManager::Create();
    
    // load vertex and fragment shaders into opengl
    LoadShaders();
    
    // load the texture
    LoadTexture();
    
    CreateObjects();
    
    // create buffer and fill it with the points of the triangle
    //LoadCube();
    
    initializeFbo();
    initializeShadowMap();
    
    glViewport(0,0,SCREEN_SIZE.x, SCREEN_SIZE.y);
    
    // run while the window is open
    double lastTime = glfwGetTime();
    gQuadTexture = gTexture->object();
    
    const char* name = glfwGetJoystickName(GLFW_JOYSTICK_1);
    
    while(!glfwWindowShouldClose(gWindow)){
        // process pending events
        glfwPollEvents();
        
        // update the scene based on the time elapsed since last update
        double thisTime = glfwGetTime();
        Update((float)(thisTime - lastTime));
        lastTime = thisTime;
        
        glViewport(0,0,SCREEN_SIZE.x*2, SCREEN_SIZE.y*2);
        //renderToFbo();
        renderToShadowMap();        
        renderToScreen();
        
        //renderQuad();
        
        // swap the display buffers (displays what was just drawn)
        glfwSwapBuffers(gWindow);
    }
    
    uninitializeFbo();
    Utils::checkGLError();

    
    // clean up and exit
    glfwTerminate();
}

static void initializeQuad()
{
    std::string path1 = "shaders/passthrough_vertex.shader.vs";
    std::string path2 = "shaders/texture_fragment.shader.fs";
    std::string path3 = "shaders/quad_vs.shader";
    
    Shader VertexShader(GL_VERTEX_SHADER, path1);
    Shader FragmentShader(GL_FRAGMENT_SHADER, path2);
    
    Shader vs_quad(GL_VERTEX_SHADER,"shaders/quad_vs.shader");
    Shader fs_quad(GL_FRAGMENT_SHADER,"shaders/quad_fs.shader");
    
    std::vector<Shader> shaders;
    
    shaders.push_back(vs_quad);
    shaders.push_back(fs_quad);
    
    gProgramQuad = new Program(shaders);
    
    glGenVertexArrays(1, &gVAOQuad);
    glBindVertexArray(gVAOQuad);
    
    glGenBuffers(1, &gVBOquad);
    glBindBuffer(GL_ARRAY_BUFFER,gVBOquad);
    
    GLfloat vertexData[] {
        -1.0f, -1.0f, 0.0f,
         1.0f, -1.0f, 0.0f,
         1.0f,  1.0f, 0.0f,
        -1.0f, -1.0f, 0.0f,
         1.0f,  1.0f, 0.0f,
        -1.0f,  1.0f, 0.0f
    };
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);
    
    // connect the xyz to the "vert" attribute of the vertex shader
    glEnableVertexAttribArray(gProgramTexture->attrib("vert"));
    glVertexAttribPointer(gProgramTexture->attrib("vert"), 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), NULL);
    
    glBindVertexArray(0);
    
}


int main(int argc, char *argv[]) {
    try {
        //testTime();
        AppMain();
    } catch (const std::exception& e){
        std::cerr << "ERROR: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    
    return EXIT_SUCCESS;
}
