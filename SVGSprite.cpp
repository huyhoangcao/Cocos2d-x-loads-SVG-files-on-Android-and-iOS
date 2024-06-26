#include "SVGSprite.h"

USING_NS_CC;

SVGSprite* SVGSprite::create(std::string path, float scale, std::string unit, float dpi)
{
	SVGSprite* ref = new (std::nothrow) SVGSprite();
	if(ref && ref->init(path, scale, unit, dpi))
	{
		ref->autorelease();
		return ref;
	}else
	{
		CC_SAFE_DELETE(ref);
		ref = nullptr;
		return nullptr;
	}
}

bool SVGSprite::init(std::string path, float scale, std::string unit, float dpi)
{
	std::ostringstream os;
	os << path << "_" << scale << "_" << unit << "_" << dpi;
	Texture2D* texture = TextureCache::getInstance()->getTextureForKey(os.str());
	if(texture != nullptr) return Sprite::initWithTexture(texture);
	else {
		NSVGimage *image = NULL;
		NSVGrasterizer *rast = NULL;
		unsigned char* img = NULL;
		int w, h;
		//--- parse svg from file

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

		//--- init rasterizer
		w = (int)image->width * scale;
		h = (int)image->height * scale;
		rast = nsvgCreateRasterizer();
		if (rast == NULL) {
			log("Could not init Rasterizer.\n");
			nsvgDeleteRasterizer(rast);
			nsvgDelete(image);
			return false;
		}
		//--- create image buffer
		img = (unsigned char*)malloc(w*h * 4);
		if (img == NULL) {
			log("Could not alloc image buffer.\n");
			nsvgDeleteRasterizer(rast);
			nsvgDelete(image);
			return false;
		}

		log("rasterizing image %d x %d\n", w, h);
		nsvgRasterize(rast, image, 0, 0, scale, img, w, h, w * 4);

		Image* imageA = new Image();
		imageA->initWithRawData(img, w*h * 4, w, h, 4);

		texture = TextureCache::getInstance()->addUIImage(imageA, os.str());

		return Sprite::initWithTexture(texture);
	}
}
