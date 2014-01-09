//
//  TheForceAppDelegate.h
//  TheForce
//
//  Created by Ming Zhao on 2/1/13.
//  Copyright __MyCompanyName__ 2013. All rights reserved.
//

#ifndef  _APP_DELEGATE_H_
#define  _APP_DELEGATE_H_

#include "CCApplication.h"

/**
@brief    The cocos2d Application.

The reason for implement as private inheritance is to hide some interface call by Director.
*/
class AppDelegate : private cocos2d::Application
{
public:
    AppDelegate();
    virtual ~AppDelegate();


    /**
    @brief    Implement Director and Scene init code here.
    @return true    Initialize success, app continue.
    @return false   Initialize failed, app terminate.
    */
    virtual bool applicationDidFinishLaunching();

    /**
    @brief  The function be called when the application enter background
    @param  the pointer of the application
    */
    virtual void applicationDidEnterBackground();

    /**
    @brief  The function be called when the application enter foreground
    @param  the pointer of the application
    */
    virtual void applicationWillEnterForeground();
    
protected:
    /**
     @brief The funciton be called when set up multiple resolution
     @param
     */
    void setupMultipleResolutionSupport();
    
    /**
     @brief The funciton be called when prepare configuration files
     @param
     */
    void setupConfigurationFiles();
};

#endif // _APP_DELEGATE_H_

