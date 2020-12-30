#!/bin/bash
set -x

if [ -z "$1" ]
then
    echo "Usage: $0 <target_ip> [target port] [target path]"
    exit 1
fi

TARGET_IP=$1
TARGET_PORT=$2
TARGET_PATH=$3

JTOOL="jtool"
OPTOOL="/Users/vcorneci/ios_sandboxing/Tools/optool/optool"

APP_NAME="Sample"
PROJECTS_PATH="/Users/vcorneci/ios_sandboxing/Xcode_projects"
PROJECT_PATH="$PROJECTS_PATH/$APP_NAME"

SCHEME_NAME="Sample"
CONFIGURATION="Release"
ARCHIVE_PATH="$PROJECT_PATH/build/$APP_NAME.xcarchive"

RESOURCES=$(pwd)/resources

# Build phase
cd $PROJECT_PATH
/usr/bin/xcodebuild -scheme $APP_NAME \
                    -configuration $CONFIGURATION clean archive -archivePath $ARCHIVE_PATH \
                    CODE_SIGNING_REQUIRED=NO

cd $ARCHIVE_PATH"/Products/Applications/$APP_NAME.app"
cp $RESOURCES/ent.xml .

cp $RESOURCES/sandbox.bin .

echo $PWD
# Copy injected framework
mkdir -p Frameworks
cp -r $RESOURCES/InjectingFramework.framework/InjectingFramework Frameworks/InjectingFramework
# Inject a new DYLD_LOAD command in the target Mach-O binary
$OPTOOL install -c load -p '@rpath/InjectingFramework' -t $APP_NAME

# Sign phase
JDEBUG=1 $JTOOL -arch arm64 --sign --ent ent.xml --inplace $APP_NAME
JDEBUG=1 $JTOOL -arch arm64 --sign --inplace Frameworks/InjectingFramework

# Archive
cd ..
tar -cvf $APP_NAME.tar $APP_NAME.app

# Copy to device
if [ -z "$TARGET_PORT" ]
then
	scp $APP_NAME.tar root@$TARGET_IP:$TARGET_PATH
    # Run script on device
    ssh root@$TARGET_IP './install_sample.sh'
else
	scp -P $TARGET_PORT $APP_NAME.tar root@$TARGET_IP:$TARGET_PATH
    # Run script on device
    ssh -P $TARGET_PORT root@$TARGET_IP './install_sample.sh'
fi
