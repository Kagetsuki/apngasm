#ifndef _APNGFRAME_H_
#define _APNGFRAME_H_

#include <string>
#include <vector>

namespace apngasm {

  const unsigned DEFAULT_FRAME_NUMERATOR = 100;
  const unsigned DEFAULT_FRAME_DENOMINATOR = 1000;

  typedef struct { unsigned char r, g, b; } rgb;
  typedef struct { unsigned char r, g, b, a; } rgba;

  //Individual APNG frame
  class APNGFrame
  {
  public:
    // Raw pixel data
    unsigned char* pixels(unsigned char* setPixels = NULL);
    unsigned char* _pixels;
    
    // Width and Height
    unsigned int width(unsigned int setWidth = 0);
    unsigned int height(unsigned int setHeight = 0);
    unsigned int _width;
    unsigned int _height;

    // PNG color type
    unsigned char colorType(unsigned char setColorType = 255);
    unsigned char _colorType;
    
    // Palette into
    rgb* palette(rgb* setPalette = NULL);
    rgb _palette[256];
    
    //Transparency info
    unsigned char* transparency(unsigned char* setTransparency = NULL);
    unsigned char _transparency[256];
    
    //Sizes for palette and transparency records
    int paletteSize(int setPaletteSize = 0);
    int _paletteSize;

    int transparencySize(int setTransparencySize = 0);
    int _transparencySize;
    
    //Delay is numerator/denominator ratio, in seconds
    unsigned int delayNum(unsigned int setDelayNum = 0);
    unsigned int _delayNum;

    unsigned int delayDen(unsigned int setDelayDen = 0);
    unsigned int _delayDen;

    unsigned char** rows(unsigned char** setRows = NULL);
    unsigned char ** _rows;

    // Init empty frame for filling in manually
    APNGFrame();
    // Init frame from a PNG image file
    APNGFrame(const std::string &filePath, unsigned delayNum = DEFAULT_FRAME_NUMERATOR, unsigned delayDen = DEFAULT_FRAME_DENOMINATOR);
    // Init frame 
    APNGFrame(rgb *pixels, unsigned int width, unsigned int height, unsigned delayNum = DEFAULT_FRAME_NUMERATOR, unsigned delayDen = DEFAULT_FRAME_DENOMINATOR);
    APNGFrame(rgb *pixels, unsigned int width, unsigned int height, rgb *trns_color = NULL, unsigned delayNum = DEFAULT_FRAME_NUMERATOR, unsigned delayDen = DEFAULT_FRAME_DENOMINATOR);
    APNGFrame(rgba *pixels, unsigned int width, unsigned int height, unsigned delayNum = DEFAULT_FRAME_NUMERATOR, unsigned delayDen = DEFAULT_FRAME_DENOMINATOR);

    // Save frame to a PNG image file.
    // Return true if save succeeded.
    bool save(const std::string& outPath) const;

    // for Ruby.
    std::vector<unsigned char>& pixels(const std::vector<unsigned char>& setPixels = std::vector<unsigned char>());
    std::vector<rgb>& palette(const std::vector<rgb>& setPalette = std::vector<rgb>());
    std::vector<unsigned char>& transparency(const std::vector<unsigned char>& setTransparency = std::vector<unsigned char>());
    std::vector<unsigned char*>& rows(const std::vector<unsigned char*>& setRows = std::vector<unsigned char*>());

  private:
    // for Ruby.
    std::vector<unsigned char> _tmpPixels;
    std::vector<rgb> _tmpPalette;
    std::vector<unsigned char> _tmpTransparency;
    std::vector<unsigned char*> _tmpRows;

  };  // class APNGFrame

} // namespace apngasm

#endif /* _APNGFRAME_H_ */
