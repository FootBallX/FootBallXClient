rm -rf ./cocos2d-x/build/build

xcodebuild -target 'chipmunk Mac' -project cocos2d-x/build/cocos2d_libs.xcodeproj -sdk macosx10.8 -arch x86_64
xcodebuild -target 'cocos2dx Mac' -project cocos2d-x/build/cocos2d_libs.xcodeproj -sdk macosx10.8 -arch x86_64
xcodebuild -target 'cocos2dx-extensions Mac'  -project cocos2d-x/build/cocos2d_libs.xcodeproj -sdk macosx10.8 -arch x86_64
xcodebuild -target 'CocosDenshion Mac' -project cocos2d-x/build/cocos2d_libs.xcodeproj -sdk macosx10.8 -arch x86_64


rm -rf ./cocos2d-x/libsMac
mkdir ./cocos2d-x/libsMac

cp ./cocos2d-x/build/build/Release/"libcocos2dx Mac.a" ./cocos2d-x/libsMac/"libcocos2dx Mac.a"
cp ./cocos2d-x/build/build/Release/"libcocos2dx-extensions Mac.a" ./cocos2d-x/libsMac/"libcocos2dx-extensions Mac.a"
cp ./cocos2d-x/build/build/Release/"libCocosDenshion Mac.a" ./cocos2d-x/libsMac/"libCocosDenshion Mac.a"


rm -rf ./cocos2d-x/build/build

