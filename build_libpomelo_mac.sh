rm -rf ./libpomelo/build
rm -rf ./libpomelo/deps/jansson/build
rm -rf ./libpomelo/deps/uv/build

xcodebuild -project ./libpomelo/deps/jansson/janssonMac.xcodeproj -arch x86_64
xcodebuild -project ./libpomelo/deps/uv/uvMac.xcodeproj -arch x86_64
xcodebuild -target "libpomelo" -project ./libpomelo/pomeloMac.xcodeproj -arch x86_64

rm -rf ./libpomelo/libMac
mkdir ./libpomelo/libMac

cp ./libpomelo/build/Default/libpomelo.a ./libpomelo/libMac/libpomelo.a
cp ./libpomelo/deps/jansson/build/Default/libjansson.a ./libpomelo/libMac/libjansson.a
cp ./libpomelo/deps/uv/build/Default/libuv.a ./libpomelo/libMac/libuv.a

rm -rf ./libpomelo/build
rm -rf ./libpomelo/deps/jansson/build
rm -rf ./libpomelo/deps/uv/build


