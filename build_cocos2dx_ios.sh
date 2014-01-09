rm -rf ./cocos2d-x/build

xcodebuild -target 'chipmunk iOS' -project cocos2d-x/cocos2d_libs.xcodeproj -sdk iphonesimulator6.1 -arch i386
xcodebuild -target 'cocos2dx iOS' -project cocos2d-x/cocos2d_libs.xcodeproj -sdk iphonesimulator6.1 -arch i386
xcodebuild -target 'cocos2dx-extensions iOS'  -project cocos2d-x/cocos2d_libs.xcodeproj -sdk iphonesimulator6.1 -arch i386
xcodebuild -target 'CocosDenshion iOS' -project cocos2d-x/cocos2d_libs.xcodeproj -sdk iphonesimulator6.1 -arch i386

rm -rf ./lib_cocos2dx_ios_sim

mkdir ./lib_cocos2dx_ios_sim

cp ./cocos2d-x/build/Release-iphonesimulator/"libcocos2dx iOS.a" ./lib_cocos2dx_ios_sim/"libcocos2dx iOS.a"
cp ./cocos2d-x/build/Release-iphonesimulator/"libcocos2dx-extensions iOS.a" ./lib_cocos2dx_ios_sim/"libcocos2dx-extensions iOS.a"
cp ./cocos2d-x/build/Release-iphonesimulator/"libCocosDenshion iOS.a" ./lib_cocos2dx_ios_sim/"libCocosDenshion iOS.a"

rm -rf ./cocos2d-x/build


xcodebuild -target 'chipmunk iOS' -project cocos2d-x/cocos2d_libs.xcodeproj -sdk iphoneos6.1 -arch armv7 -arch armv7s
xcodebuild -target 'cocos2dx iOS' -project cocos2d-x/cocos2d_libs.xcodeproj -sdk iphoneos6.1 -arch armv7 -arch armv7s
xcodebuild -target 'cocos2dx-extensions iOS'  -project cocos2d-x/cocos2d_libs.xcodeproj -sdk iphoneos6.1 -arch armv7 -arch armv7s
xcodebuild -target 'CocosDenshion iOS' -project cocos2d-x/cocos2d_libs.xcodeproj -sdk iphoneos6.1 -arch armv7 -arch armv7s


rm -rf ./lib_cocos2dx_ios
mkdir ./lib_cocos2dx_ios


cp ./cocos2d-x/build/Release-iphoneos/"libcocos2dx iOS.a" ./lib_cocos2dx_ios/"libcocos2dx iOS.a"
cp ./cocos2d-x/build/Release-iphoneos/"libcocos2dx-extensions iOS.a" ./lib_cocos2dx_ios/"libcocos2dx-extensions iOS.a"
cp ./cocos2d-x/build/Release-iphoneos/"libCocosDenshion iOS.a" ./lib_cocos2dx_ios/"libCocosDenshion iOS.a"

rm -rf ./cocos2d-x/build

rm -rf ./cocos2d-x/libs
mkdir ./cocos2d-x/libs

lipo -create ./lib_cocos2dx_ios/"libcocos2dx iOS.a"  ./lib_cocos2dx_ios_sim/"libcocos2dx iOS.a"  -output ./cocos2d-x/libs/"libcocos2dx iOS.a"
lipo -create ./lib_cocos2dx_ios/"libcocos2dx-extensions iOS.a" ./lib_cocos2dx_ios_sim/"libcocos2dx-extensions iOS.a"  -output ./cocos2d-x/libs/"libcocos2dx-extensions iOS.a"
lipo -create ./lib_cocos2dx_ios/"libCocosDenshion iOS.a" ./lib_cocos2dx_ios_sim/"libCocosDenshion iOS.a"  -output ./cocos2d-x/libs/"libCocosDenshion iOS.a"

rm -rf ./lib_cocos2dx_ios_sim
rm -rf ./lib_cocos2dx_ios




