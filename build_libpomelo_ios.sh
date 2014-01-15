#用这个编译libpomelo库以及它依赖的uv和jansson
#编译以后会把ios版和模拟器版合并成一个文件后放入lib目录。
#libpomelo本身提供的两个build命令连续使用的话，会造成第二个编译的无法正常使用
#所以这里在编译好一个后彻底删除build目录再编译第二个。

rm -rf ./libpomelo/build
rm -rf ./libpomelo/deps/jansson/build
rm -rf ./libpomelo/deps/uv/build

xcodebuild -project ./libpomelo/deps/jansson/jansson.xcodeproj -sdk iphonesimulator6.1 -arch i386
xcodebuild -project ./libpomelo/deps/uv/uv.xcodeproj -sdk iphonesimulator6.1 -arch i386
xcodebuild -project ./libpomelo/pomelo.xcodeproj -sdk iphonesimulator6.1 -arch i386

rm -rf ./libpomelo/lib_ios_sim
mkdir ./libpomelo/lib_ios_sim

cp ./libpomelo/build/Default-iphonesimulator/libpomelo.a ./libpomelo/lib_ios_sim/libpomelo.a
cp ./libpomelo/deps/jansson/build/Default-iphonesimulator/libjansson.a ./libpomelo/lib_ios_sim/libjansson.a
cp ./libpomelo/deps/uv/build/Default-iphonesimulator/libuv.a ./libpomelo/lib_ios_sim/libuv.a

rm -rf ./libpomelo/build
rm -rf ./libpomelo/deps/jansson/build
rm -rf ./libpomelo/deps/uv/build

xcodebuild -project ./libpomelo/deps/jansson/jansson.xcodeproj
xcodebuild -project ./libpomelo/deps/uv/uv.xcodeproj
xcodebuild -project ./libpomelo/pomelo.xcodeproj

rm -rf ./libpomelo/lib_ios
mkdir ./libpomelo/lib_ios
cp ./libpomelo/build/Default-iphoneos/libpomelo.a ./libpomelo/lib_ios/libpomelo.a
cp ./libpomelo/deps/jansson/build/Default-iphoneos/libjansson.a ./libpomelo/lib_ios/libjansson.a
cp ./libpomelo/deps/uv/build/Default-iphoneos/libuv.a ./libpomelo/lib_ios/libuv.a

rm -rf ./libpomelo/build
rm -rf ./libpomelo/deps/jansson/build
rm -rf ./libpomelo/deps/uv/build

lipo -create ./libpomelo/lib_ios/libpomelo.a  ./libpomelo/lib_ios_sim/libpomelo.a  -output ./libpomelo/lib/libpomelo.a
lipo -create ./libpomelo/lib_ios/libjansson.a ./libpomelo/lib_ios_sim/libjansson.a  -output ./libpomelo/lib/libjansson.a
lipo -create ./libpomelo/lib_ios/libuv.a ./libpomelo/lib_ios_sim/libuv.a  -output ./libpomelo/lib/libuv.a

rm -rf ./libpomelo/lib_ios_sim
rm -rf ./libpomelo/lib_ios

