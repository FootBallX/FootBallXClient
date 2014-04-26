rm -rf ./cocos2d-x/build/build
rm -rf ./Cocos2d-JS/frameworks/js-bindings/bindings/proj.ios_mac/build

xcodebuild -target 'chipmunk Mac' -project cocos2d-x/build/cocos2d_libs.xcodeproj -arch x86_64
xcodebuild -target 'cocos2dx Mac' -project cocos2d-x/build/cocos2d_libs.xcodeproj -arch x86_64
xcodebuild -target 'cocos2dx-extensions Mac'  -project cocos2d-x/build/cocos2d_libs.xcodeproj -arch x86_64
xcodebuild -target 'CocosDenshion Mac' -project cocos2d-x/build/cocos2d_libs.xcodeproj -arch x86_64
xcodebuild -target 'jsbindings Mac' -project Cocos2d-JS/frameworks/js-bindings/bindings/proj.ios_mac/cocos2d_js_bindings.xcodeproj -arch x86_64

xcodebuild -target 'chipmunk Mac' -project cocos2d-x/build/cocos2d_libs.xcodeproj -configuration Debug -arch x86_64
xcodebuild -target 'cocos2dx Mac' -project cocos2d-x/build/cocos2d_libs.xcodeproj -configuration Debug -arch x86_64
xcodebuild -target 'cocos2dx-extensions Mac'  -project cocos2d-x/build/cocos2d_libs.xcodeproj -configuration Debug -arch x86_64
xcodebuild -target 'CocosDenshion Mac' -project cocos2d-x/build/cocos2d_libs.xcodeproj -configuration Debug -arch x86_64
xcodebuild -target 'jsbindings Mac' -project Cocos2d-JS/frameworks/js-bindings/bindings/proj.ios_mac/cocos2d_js_bindings.xcodeproj -configuration Debug -arch x86_64


rm -rf ./cocos2d-x/libsMac
mkdir ./cocos2d-x/libsMac
rm -rf ./cocos2d-x/libsMacDebug
mkdir ./cocos2d-x/libsMacDebug

cp ./cocos2d-x/build/build/Release/"libcocos2dx Mac.a" ./cocos2d-x/libsMac/"libcocos2dx Mac.a"
cp ./cocos2d-x/build/build/Release/"libcocos2dx-extensions Mac.a" ./cocos2d-x/libsMac/"libcocos2dx-extensions Mac.a"
cp ./cocos2d-x/build/build/Release/"libCocosDenshion Mac.a" ./cocos2d-x/libsMac/"libCocosDenshion Mac.a"
cp ./Cocos2d-JS/frameworks/js-bindings/bindings/proj.ios_mac/build/Release/"libjsbindings Mac.a" ./cocos2d-x/libsMac/"libjsbindings Mac.a"

cp ./cocos2d-x/build/build/Debug/"libcocos2dx Mac.a" ./cocos2d-x/libsMacDebug/"libcocos2dx Mac.a"
cp ./cocos2d-x/build/build/Debug/"libcocos2dx-extensions Mac.a" ./cocos2d-x/libsMacDebug/"libcocos2dx-extensions Mac.a"
cp ./cocos2d-x/build/build/Debug/"libCocosDenshion Mac.a" ./cocos2d-x/libsMacDebug/"libCocosDenshion Mac.a"
cp ./Cocos2d-JS/frameworks/js-bindings/bindings/proj.ios_mac/build/Debug/"libjsbindings Mac.a" ./cocos2d-x/libsMacDebug/"libjsbindings Mac.a"

rm -rf ./cocos2d-x/build/build
rm -rf ./Cocos2d-JS/frameworks/js-bindings/bindings/proj.ios_mac/build

