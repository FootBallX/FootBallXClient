//
//  CMessageBoxLayer.h
//  GameX
//
//  Created by 马 俊 on 13-11-21.
//
//

#ifndef __GameX__CMessageBoxLayer__
#define __GameX__CMessageBoxLayer__

#include "CBaseLayer.h"

class CMessageBoxLayer : public CBaseLayer
{
public:
    enum BUTTON_TYPE
    {
        _NONE = 0,
        _OK = 1,
        _YES = 2,
        _NO = 4,
        _CANCEL = 8,
    };
    
    
    CREATE_FUNC(CMessageBoxLayer);
    CMessageBoxLayer();
    virtual ~CMessageBoxLayer();
    
    virtual bool init();
    
    virtual bool onTouchBegan(Touch *touch, Event *event) override;
    
    virtual bool addButton(const char* t, BUTTON_TYPE btnType);
    virtual void setMsg(const char* msg);
    virtual void setTitle(const char* title);
    virtual void setCallback(function<bool(int)> f);
    
    virtual void doModal();
    virtual void closeModal();
protected:
    virtual void onButtonClicked(Object* sender, Control::EventType type);
    virtual void adjustBoxSize();
    
    function<bool(int)> m_callback;

private:
    const int TAG_THIS = 2000;
    const int TAG_BKG = 1980;
    const int TAG_TITLE = 1981;
    const int TAG_MSG = 1982;
    
    const int Z_BKG = 10;
    const int Z_MSG = 20;
    const int Z_TITLE = 20;
    const int Z_BUTTON = 20;
    
    const char* FONT_NAME = "Helvetica";
    const int FONT_SIZE = 12 * CCBReader::getResolutionScale();
    const char* BKG_FILENAME = "huds/info_bkg.png";
    const float INSET = 15 * CCBReader::getResolutionScale();
    
    const float V_GAP = 10 * CCBReader::getResolutionScale();
    const float H_GAP = 5 * CCBReader::getResolutionScale();
    
    Size m_boxSize;
    unsigned int m_buttonMask = BUTTON_TYPE::_NONE;
    vector<ControlButton*> m_buttons;
};

#endif /* defined(__GameX__CMessageBoxLayer__) */
