rm -rf ./cocos2d-x/build/build

xcodebuild -target 'chipmunk iOS' -project cocos2d-x/build/cocos2d_libs.xcodeproj -sdk iphonesimulator7.1
xcodebuild -target 'cocos2dx iOS' -project cocos2d-x/build/cocos2d_libs.xcodeproj -sdk iphonesimulator7.1
xcodebuild -target 'cocos2dx-extensions iOS'  -project cocos2d-x/build/cocos2d_libs.xcodeproj -sdk iphonesimulator7.1
xcodebuild -target 'CocosDenshion iOS' -project cocos2d-x/build/cocos2d_libs.xcodeproj -sdk iphonesimulator7.1
xcodebuild -target 'jsbindings iOS' -project Cocos2d-JS/frameworks/js-bindings/bindings/proj.ios_mac/cocos2d_js_bindings.xcodeproj -sdk iphonesimulator7.1

rm -rf ./lib_cocos2dx_ios_sim

mkdir ./lib_cocos2dx_ios_sim

cp ./cocos2d-x/build/build/Release-iphonesimulator/"libcocos2dx iOS.a" ./lib_cocos2dx_ios_sim/"libcocos2dx iOS.a"
cp ./cocos2d-x/build/build/Release-iphonesimulator/"libcocos2dx-extensions iOS.a" ./lib_cocos2dx_ios_sim/"libcocos2dx-extensions iOS.a"
cp ./cocos2d-x/build/build/Release-iphonesimulator/"libCocosDenshion iOS.a" ./lib_cocos2dx_ios_sim/"libCocosDenshion iOS.a"
cp ./Cocos2d-JS/frameworks/js-bindings/bindings/proj.ios_mac/build/Release-iphonesimulator/"libjsbindings iOS.a" ./lib_cocos2dx_ios_sim/"libjsbindings iOS.a"

rm -rf ./cocos2d-x/build/build
rm -rf ./Cocos2d-JS/frameworks/js-bindings/bindings/proj.ios_mac/build

xcodebuild -target 'chipmunk iOS' -project cocos2d-x/build/cocos2d_libs.xcodeproj -sdk iphoneos7.1
xcodebuild -target 'cocos2dx iOS' -project cocos2d-x/build/cocos2d_libs.xcodeproj -sdk iphoneos7.1
xcodebuild -target 'cocos2dx-extensions iOS'  -project cocos2d-x/build/cocos2d_libs.xcodeproj -sdk iphoneos7.1
xcodebuild -target 'CocosDenshion iOS' -project cocos2d-x/build/cocos2d_libs.xcodeproj -sdk iphoneos7.1
xcodebuild -target 'jsbindings iOS' -project Cocos2d-JS/frameworks/js-bindings/bindings/proj.ios_mac/cocos2d_js_bindings.xcodeproj -sdk iphoneos7.1

rm -rf ./lib_cocos2dx_ios
mkdir ./lib_cocos2dx_ios


cp ./cocos2d-x/build/build/Release-iphoneos/"libcocos2dx iOS.a" ./lib_cocos2dx_ios/"libcocos2dx iOS.a"
cp ./cocos2d-x/build/build/Release-iphoneos/"libcocos2dx-extensions iOS.a" ./lib_cocos2dx_ios/"libcocos2dx-extensions iOS.a"
cp ./cocos2d-x/build/build/Release-iphoneos/"libCocosDenshion iOS.a" ./lib_cocos2dx_ios/"libCocosDenshion iOS.a"
cp ./Cocos2d-JS/frameworks/js-bindings/bindings/proj.ios_mac/build/Release-iphoneos/"libjsbindings iOS.a" ./lib_cocos2dx_ios/"libjsbindings iOS.a"

rm -rf ./cocos2d-x/build/build
rm -rf ./Cocos2d-JS/frameworks/js-bindings/bindings/proj.ios_mac/build

rm -rf ./cocos2d-x/libs
mkdir ./cocos2d-x/libs

lipo -create ./lib_cocos2dx_ios/"libcocos2dx iOS.a"  ./lib_cocos2dx_ios_sim/"libcocos2dx iOS.a"  -output ./cocos2d-x/libs/"libcocos2dx iOS.a"
lipo -create ./lib_cocos2dx_ios/"libcocos2dx-extensions iOS.a" ./lib_cocos2dx_ios_sim/"libcocos2dx-extensions iOS.a"  -output ./cocos2d-x/libs/"libcocos2dx-extensions iOS.a"
lipo -create ./lib_cocos2dx_ios/"libCocosDenshion iOS.a" ./lib_cocos2dx_ios_sim/"libCocosDenshion iOS.a"  -output ./cocos2d-x/libs/"libCocosDenshion iOS.a"
lipo -create ./lib_cocos2dx_ios/"libjsbindings iOS.a" ./lib_cocos2dx_ios_sim/"libjsbindings iOS.a"  -output ./cocos2d-x/libs/"libjsbindings iOS.a"

rm -rf ./lib_cocos2dx_ios_sim
rm -rf ./lib_cocos2dx_ios




