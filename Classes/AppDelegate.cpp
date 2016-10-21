#include "AppDelegate.h"
#include "HelloWorldScene.h"

USING_NS_CC;

AppDelegate::AppDelegate() {
    
}

AppDelegate::~AppDelegate()
{
}

bool AppDelegate::applicationDidFinishLaunching() {
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLViewImpl::create("Hello World");
        glview->setFrameSize(640, 480);
        director->setOpenGLView(glview);
    }
    
    auto scene = HelloWorld::createScene();
    director->runWithScene(scene);
    
    return true;
}

void AppDelegate::applicationDidEnterBackground() {
}

void AppDelegate::applicationWillEnterForeground() {
}

void AppDelegate::initGLContextAttrs(){
    GLContextAttrs gl_context_attrs = {8,8,8,8,24,8};
    GLView::setGLContextAttrs(gl_context_attrs);
}
