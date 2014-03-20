rm -rf ./cocos2d-x/build/build

xcodebuild -target 'chipmunk Mac' -project cocos2d-x/build/cocos2d_libs.xcodeproj -arch x86_64 -arch i386
xcodebuild -target 'cocos2dx Mac' -project cocos2d-x/build/cocos2d_libs.xcodeproj -arch x86_64 -arch i386
xcodebuild -target 'cocos2dx-extensions Mac'  -project cocos2d-x/build/cocos2d_libs.xcodeproj -arch x86_64 -arch i386
xcodebuild -target 'CocosDenshion Mac' -project cocos2d-x/build/cocos2d_libs.xcodeproj -arch x86_64 -arch i386
xcodebuild -target 'jsbindings Mac' -project Cocos2d-JS/frameworks/js-bindings/bindings/proj.ios_mac/cocos2d_js_bindings.xcodeproj -arch x86_64 -arch i386


rm -rf ./cocos2d-x/libsMac
mkdir ./cocos2d-x/libsMac

cp ./cocos2d-x/build/build/Release/"libcocos2dx Mac.a" ./cocos2d-x/libsMac/"libcocos2dx Mac.a"
cp ./cocos2d-x/build/build/Release/"libcocos2dx-extensions Mac.a" ./cocos2d-x/libsMac/"libcocos2dx-extensions Mac.a"
cp ./cocos2d-x/build/build/Release/"libCocosDenshion Mac.a" ./cocos2d-x/libsMac/"libCocosDenshion Mac.a"
cp ./Cocos2d-JS/frameworks/js-bindings/bindings/proj.ios_mac/build/Release/"libjsbindings Mac.a" ./cocos2d-x/libsMac/"libjsbindings Mac.a"

rm -rf ./cocos2d-x/build/build
rm -rf ./Cocos2d-JS/frameworks/js-bindings/bindings/proj.ios_mac/build

