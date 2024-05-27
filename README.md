# Cocos2d-x-loads-SVG-files-on-Android-and-iOS
Cocos2d-x loads SVG files on Android and iOS

The code used is from the following libraries:

    CocosSVGSprite by namkazt: https://github.com/namkazt/CocosSVGSprite
    nanosvg by memononen: https://github.com/memononen/nanosvg

The code has been slightly modified from namkazt's CocosSVGSprite library to be able to load SVG files on Android and iOS.

/*
This code encounters an error when building on Android and iOS. The error occurs because the SVG file cannot be opened. It needs to be switched to use the Cocos2d-x code to open and read the SVG file, as shown in the code snippet below

image = nsvgParseFromFile(path.c_str(), unit.c_str(), dpi);
if (image == NULL) {
    log("Could not open SVG image.\n");
    nsvgDeleteRasterizer(rast);
    nsvgDelete(image);
    return false;
}*/

cocos2d::Data data = cocos2d::FileUtils::getInstance()->getDataFromFile(path);
if (data.isNull()) {
    cocos2d::log("Failed to open file: %s", path.c_str());
    return false;
}
const unsigned char* rawBytes = data.getBytes();
char* buffer = reinterpret_cast<char*>(const_cast<unsigned char*>(rawBytes));
image = nsvgParse(buffer, unit.c_str(), dpi);
