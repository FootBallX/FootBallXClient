#!/usr/bin/python
# coding=gb2312
import os
import re
import glob
import plistlib
import sys
import subprocess
import time

customClass = ''
attribute_code = ''
function_code=''

function_cpp_code_list = []

menu_selector_code=''
control_selector_code=''
member_variable_code=''

attribute_list=[]
function_list=[]


def check_attribute(cur_dict):
    global attribute_code;
    global member_variable_code;
    global attribute_list;

    baseClass = cur_dict['baseClass']
    customClass = cur_dict['customClass']
    memberVarAssignmentName = cur_dict['memberVarAssignmentName']
    memberVarAssignmentType = cur_dict['memberVarAssignmentType']

    #check 
    if memberVarAssignmentType == 1 and memberVarAssignmentName == '':
        print("ERROR!!!! [" + cur_dict['displayName']  + "] no set assignment name!");
    if memberVarAssignmentType != 1 and memberVarAssignmentName != '':
        print("ERROR!!!! [" + cur_dict['displayName'] + " : " + memberVarAssignmentName + "] assignment target not set 'Doc root var' !");
    if memberVarAssignmentType == 1 and memberVarAssignmentName != '':
        if attribute_list.count(memberVarAssignmentName) != 0:
            print("ERROR!!!! [" + cur_dict['displayName']  + "] Duplicate definition assignment: " + memberVarAssignmentName);
            # time.sleep(5);
        attribute_list.append(memberVarAssignmentName);

    if memberVarAssignmentType == 1 and memberVarAssignmentName != '':
        if customClass:
            baseClass = customClass;
        if baseClass == 'CCBFile':
            baseClass = cur_dict['displayName'];
            
        baseClass = class_name_convert(baseClass);

        attribute_code = attribute_code + '    ' + baseClass + '* m_' + memberVarAssignmentName + ' = nullptr;\n';
        member_variable_code = member_variable_code + \
        '    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "' + memberVarAssignmentName + '", ' + baseClass + '*, this->m_' + memberVarAssignmentName + ');\n'

def check_funtion(cur_dict):
    global function_code;
    global function_cpp_code_list;
    global menu_selector_code;
    global control_selector_code;
    global customClass;
    global function_list;

    customClass = class_name_convert(customClass);
    
    properties = cur_dict['properties']
    if cur_dict['baseClass'] == 'CCControlButton':
        for prop in properties:
            if prop['name'] == 'ccControl':
                functionName = prop['value'][0];
                targetType = prop['value'][1];

                # check
                if functionName == '' and targetType == 1:
                    print("ERROR!!!! [" + cur_dict['displayName'] + "] no set Selector!");
                if functionName != '' and targetType != 1:
                    print("ERROR!!!! [" + cur_dict['displayName'] + " : " + functionName + "] selector target not set 'Document root' !");
                if functionName != '' and targetType == 1:
                    if function_list.count(functionName) != 0:
                        print("ERROR!!!! [" + cur_dict['displayName']  + "] Duplicate definition Selector: " + functionName);
                    function_list.append(functionName);


                if functionName != '' and targetType == 1:
                    function_code = function_code + \
                    '    void ' + functionName+ '(Ref* sender, Control::EventType event);\n'

                    control_selector_code = control_selector_code + \
                    '    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "' + functionName + '", ' + customClass + '::' + functionName + ');\n'

                    function_cpp_code_list.append('void ' + customClass + '::' + functionName + '(Ref* sender, Control::EventType event)');

    if cur_dict['baseClass'] == 'CCMenuItemImage' :
        for prop in properties:
            if prop['name'] == 'block' and functionName != '':
                functionName = prop['value'][0];
                if functionName != '':
                    function_code = function_code + \
                    '    void _' + functionName+ '(CCObject* pSender);\n'

                    menu_selector_code = menu_selector_code + \
                    '        CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "' + functionName + '", ' + customClass + '::_' + functionName + ');\n'

                    function_cpp_code_list.append('void ' + customClass + '::_' + functionName + '(cocos2d::CCObject *pSender)');


def check_font(cur_dict):
    if cur_dict['baseClass'] == 'CCLabelTTF':
        properties = cur_dict['properties'];
        for prop in properties:
            if prop['name'] == 'fontName':
                value = prop['value'];
                if value.find('KaiTi.ttf') == -1 and value.find('SimHei.ttf') == -1:
                    print("ERROR!!!! [" + cur_dict['displayName'] + "] set font error!");

def check_children(cur_dict):
    check_attribute(cur_dict);
    check_funtion(cur_dict);
    #check_font(cur_dict);

    children = cur_dict['children'];
    for child in children:
        check_children(child);

def check_callbackChannel(cur_dict):
    sequences = cur_dict['sequences'];
    for item in sequences:
        if item['autoPlay'] == True:
            callbackChannel = item['callbackChannel'];
            keyframes = callbackChannel['keyframes'];
            for frame in keyframes:
                funName = frame['value'][0];
                integer = frame['value'][1];
                if integer == 1:
                    print(111);


def replace_code(m_file, start, end, code):
    result = ''
    m_file_list = m_file.split('\n');

    is_start = False;
    is_finded = False;
    for line in m_file_list:
        if not is_start or is_finded:
            result = result + line + '\n'

        if not is_start and not is_finded and line.find(start) != -1 :
            is_start = True;
            result = result + code;
            continue;
        
        if is_start and line.find(end) != -1:
            result = result + line + '\n'
            is_start = False;
            is_finded = True;
            
    return result;

def remove_code(m_file, start, end):
    result = ''
    m_file_list = m_file.split('\n');

    is_start = False;
    is_finded = False;
    for line in m_file_list:
        if not is_start and not is_finded and line.find(start) != -1 :
            is_start = True;

        if not is_start or is_finded:
            result = result + line + '\n'
        
        if is_start and line.find(end) != -1:
            is_start = False;
            is_finded = True;
    return result;

def class_name_convert(name):
    if name == 'CCLayer':
        return 'Layer';
    elif name == 'CCSprite':
        return 'Sprite';
    elif name == 'CCScale9Sprite':
        return 'Scale9Sprite';
    elif name == 'CCNode':
        return 'Node';
    elif name == 'CCControlButton':
        return 'ControlButton';
    elif name == 'CCLabelTTF':
        return 'Label';
    elif name == 'CCEditBox':
        return 'EditBox';
    
    return name;
        
        
def main(ccb_file, output_dir, className):
    global customClass;
    global attribute_code;
    global member_variable_code;
    global function_code;
    global function_cpp_code_list;
    global menu_selector_code;
    global control_selector_code;
    print('\n\n');
    print('========================================  START BUILD  ========================================');
    print('CCB FILE:' + ccb_file);
    print('OUTPUT:' + output_dir);

    rootObj = plistlib.readPlist(ccb_file);

    customClass = rootObj['nodeGraph']['customClass'];
    
    if len(className) != 0:
        customClass = className;
        

    print('Layer Class:' + customClass);
    print('\n');

    check_children(rootObj['nodeGraph']);
    check_callbackChannel(rootObj);

    replace_file = replace_temp;
    replace_file = replace_file.replace('SampleLayer', customClass);
    replace_file = replace_code(replace_file, 'create_attribute_start', 'create_attribute_end', attribute_code);
    replace_file = replace_code(replace_file, 'create_function_start', 'create_function_end', function_code);

    # if menu_selector_code != '':
        
    # else:
        # replace_file = remove_code(replace_file, 'onResolveCCBCCMenuItemSelector', '}');
    #replace_file = replace_code(replace_file, 'onResolveCCBCCMenuItemSelector', 'return NULL', '    {\n' + menu_selector_code);
    #replace_file = replace_code(replace_file, 'onResolveCCBCCControlSelector', 'return NULL', '    {\n' + control_selector_code);
    #replace_file = replace_code(replace_file, 'onAssignCCBMemberVariable', 'return false', '    {\n' + member_variable_code);
    replace_file = replace_file.replace('    //----create_attribute_start\n', '');
    replace_file = replace_file.replace('    //----create_attribute_end\n', '');
    replace_file = replace_file.replace('    //----create_function_start\n', '');
    replace_file = replace_file.replace('    //----create_function_end\n', '');

    if not os.path.exists(output_dir) : 
        os.mkdir(output_dir);

    h_file_path = output_dir + customClass + '.h';
    if os.path.isfile(h_file_path):
        temp_file = open(h_file_path, "rU");
        h_file = temp_file.read();
        temp_file.close();
    else:
        h_file = h_file_temp;

    try:
        h_file = h_file.replace('SampleLayer', customClass);
    except Exception: 
        h_file = h_file.decode("utf-8-sig");
        h_file = h_file.replace('SampleLayer', customClass);
        pass

    h_file = replace_code(h_file, 'AUTO CREATE CODE START', 'AUTO CREATE CODE END', replace_file);
    h_file = h_file[0:-1];

    f = file(h_file_path, 'w')
    f.write(h_file);
    f.close();

    cpp_file_path = output_dir + customClass + '.cpp';
    if os.path.isfile(cpp_file_path):
        temp_file = open(cpp_file_path, 'rU');
        cpp_file = temp_file.read();
        temp_file.close();
    else:
        cpp_file = cpp_file_temp;
        cpp_file = cpp_file.replace('SampleLayer', customClass);

    for fun_code in function_cpp_code_list:
        if cpp_file.find(fun_code) == -1:
            cpp_file = cpp_file + fun_code + '\n{\n\n}\n\n\n';

    cpp_file = replace_code(cpp_file, '// AUTO_GEN_MENU_ITEM_BEGIN', '// AUTO_GEN_MENU_ITEM_END', menu_selector_code);
    cpp_file = replace_code(cpp_file, '// AUTO_GEN_CONTROL_BEGIN', '// AUTO_GEN_CONTROL_END', control_selector_code);
    cpp_file = replace_code(cpp_file, '// AUTO_GEN_VAR_BEGIN', '// AUTO_GEN_VAR_END', member_variable_code);
    
    f = file(cpp_file_path, 'w')
    f.write(cpp_file);
    f.close();

    print_load_code = print_load_code_temp;
    print_load_code = print_load_code.replace('SampleLayer', customClass);
    base=os.path.basename(ccb_file);
    print_load_code = print_load_code.replace('login_select_server', os.path.splitext(base)[0]);
    print(print_load_code);

        

h_file_temp ="""
#ifndef FootballX_SampleLayer_
#define FootballX_SampleLayer_

#include "Common.h"
#include "CBaseLayer.h"


class SampleLayer
: public CBaseLayer
, public cocosbuilder::CCBSelectorResolver
, public cocosbuilder::CCBMemberVariableAssigner
, public cocosbuilder::NodeLoaderListener
{
public:

//========AUTO CREATE CODE START========
//========AUTO CREATE CODE END========

#endif  // FootballX_SampleLayer_
"""

cpp_file_temp = """

#include "SampleLayer.h"

static class SampleLayerRegister
{
public:
    SampleLayerRegister()
    {
        cocosbuilder::NodeLoaderLibrary::getInstance()->registerNodeLoader( "SampleLayer", SampleLayerLoader::loader());
    }
} __reg;

SampleLayer::SampleLayer()
{
}

SampleLayer::~SampleLayer()
{
}

void SampleLayer::onEnter()
{
    CBaseLayer::onEnter();
}

void SampleLayer::onExit()
{
    CBaseLayer::onExit();
}

SEL_MenuHandler SampleLayer::onResolveCCBCCMenuItemSelector(Ref* pTarget, const char* pSelectorName)
{
    // AUTO_GEN_MENU_ITEM_BEGIN
    // AUTO_GEN_MENU_ITEM_END
    return nullptr;
}



Control::Handler SampleLayer::onResolveCCBCCControlSelector(Ref* pTarget, const char* pSelectorName)
{
    // AUTO_GEN_CONTROL_BEGIN
    // AUTO_GEN_CONTROL_END
    return nullptr;
}



bool SampleLayer::onAssignCCBMemberVariable(Ref* pTarget, const char* pMemberVariableName, Node * pNode)
{
    // AUTO_GEN_VAR_BEGIN
    // AUTO_GEN_VAR_END
	return false;
}



void SampleLayer::onNodeLoaded(Node * pNode, cocosbuilder::NodeLoader * pNodeLoader)
{
    
}



"""

replace_temp = """
public:
    CREATE_FUNC(SampleLayer);
    
    SampleLayer();
    virtual ~SampleLayer();

    virtual void onEnter() override;
    virtual void onExit() override;

    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(Ref* pTarget, const char* pSelectorName) override;
    virtual Control::Handler onResolveCCBCCControlSelector(Ref* pTarget, const char* pSelectorName) override;
    virtual bool onAssignCCBMemberVariable(Ref* pTarget, const char* pMemberVariableName, Node * pNode) override;
    virtual void onNodeLoaded(Node * pNode, cocosbuilder::NodeLoader * pNodeLoader) override;
    
protected:
    //----create_attribute_start
    CCNode* m_myNode;
    CCControlButton* m_myButton;
    //----create_attribute_end
    
    //----create_function_start
    //----create_function_end

};

class SampleLayerLoader : public cocosbuilder::LayerLoader
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(SampleLayerLoader, loader);
    
protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(SampleLayer);
};

"""

print_load_code_temp="""
//---------------------------------Copy code for load ccb-------------------------------------

static SampleLayer* createSampleLayer()
{
    CCNodeLoaderLibrary* ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
    ccNodeLoaderLibrary->registerCCNodeLoader("SampleLayer", SampleLayerLoader::loader());
    CCBReader* reader = new cocos2d::extension::CCBReader(ccNodeLoaderLibrary);
    reader->autorelease();
    SampleLayer* result = (SampleLayer*)CCBUtil::readNodeGraphFromFile(reader, "login_select_server.ccbi", false);
    return result;
}
//------------------------------------------End------------------------------------------------
"""

if len(sys.argv) == 3:
    main(sys.argv[1], sys.argv[2], '');
else:
    main(sys.argv[1], sys.argv[2], sys.argv[3]);
    
print ('Done!')