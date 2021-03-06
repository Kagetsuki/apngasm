#ifndef _APNGASM_H_
#define _APNGASM_H_

#include <vector>
#include <string>
#include <zlib.h> // z_stream
#include "apngframe.h"
#include "apngasm-conf.h"
#include "apngasm-version.h"


namespace apngasm {

    namespace listener { class IAPNGAsmListener; }

    typedef struct { unsigned char *p; unsigned int size; int x, y, w, h, valid, filters; } OP;

    struct CHUNK { unsigned int size; unsigned char * p; };

    class APNGASM_DECLSPEC APNGAsm {
    public:
		//Construct APNGAsm object
		APNGAsm(void);
		
		//Construct APNGAsm object
		APNGAsm(const std::vector<APNGFrame> &frames);

		~APNGAsm(void);

		//Adds a frame from a file
		//Returns the frame number in the frame vector
		//Uses default delay of 10ms if not specified
		size_t addFrame(const std::string &filePath, unsigned delayNum = DEFAULT_FRAME_NUMERATOR, unsigned delayDen = DEFAULT_FRAME_DENOMINATOR);

		//Adds an APNGFrame object to the frame vector
		//Returns the frame number in the frame vector
		size_t addFrame(const APNGFrame &frame);
		
		//Adds an APNGFrame object to the frame vector
		APNGAsm& operator << (const APNGFrame &frame);

#ifdef APNG_WRITE_SUPPORTED
		//Assembles and outputs an APNG file
		//Returns the assembled file object
		//If no output path is specified only the file object is returned
		bool assemble(const std::string &outputPath);
#endif

#ifdef APNG_READ_SUPPORTED
		//Disassembles an APNG file
		//Returns the frame vector
		const std::vector<APNGFrame>& disassemble(const std::string &filePath);

		// Save png files.
		bool savePNGs(const std::string& outputDir) const;
#endif

#ifdef APNG_SPECS_SUPPORTED
		//Loads an animation spec from JSON or XML
		//Returns a frame vector with the loaded frames
		//Loaded frames are added to the end of the frame vector
		const std::vector<APNGFrame>& loadAnimationSpec(const std::string &filePath);

		// Save json file.
		bool saveJson(const std::string& outputPath, const std::string& imageDir="") const;

		// Save xml file.
		bool saveXml(const std::string& outputPath, const std::string& imageDir="") const;
#endif

		// Set APNGAsmListener.
		// If argument is NULL, set default APNGAsmListener.
		void setAPNGAsmListener(listener::IAPNGAsmListener* pListener=NULL);

		// Returns the vector of frames.
		const std::vector<APNGFrame>& getFrames() const;

		//Returns the number of frames
		size_t frameCount();

		//Throw away all frames, start over
		size_t reset();

    //Returns the version of APNGAsm
		const char* version(void) const;

    private:
    //apng frame vector
    std::vector<APNGFrame> _frames;

    // APNGAsm event listener.
    listener::IAPNGAsmListener* _pListener;

#ifdef APNG_WRITE_SUPPORTED
    unsigned char findCommonType(void);
    int upconvertToCommonType(unsigned char coltype);
    void dirtyTransparencyOptimization(unsigned char coltype);
    unsigned char downconvertOptimizations(unsigned char coltype, bool keep_palette, bool keep_coltype);

    bool save(const std::string &outputPath, unsigned char coltype, unsigned first, unsigned loops);

    void process_rect(unsigned char * row, int rowbytes, int bpp, int stride, int h, unsigned char * rows);
    void deflate_rect_fin(unsigned char * zbuf, unsigned int * zsize, int bpp, int stride, unsigned char * rows, int zbuf_size, int n);
    void deflate_rect_op(unsigned char *pdata, int x, int y, int w, int h, int bpp, int stride, int zbuf_size, int n);
    void get_rect(unsigned int w, unsigned int h, unsigned char *pimage1, unsigned char *pimage2, unsigned char *ptemp, unsigned int bpp, unsigned int stride, int zbuf_size, unsigned int has_tcolor, unsigned int tcolor, int n);

    void write_chunk(FILE * f, const char * name, unsigned char * data, unsigned int length);
    void write_IDATs(FILE * f, int frame, unsigned char * data, unsigned int length, unsigned int idat_size);

    OP              _op[6];
    z_stream        _op_zstream1;
    z_stream        _op_zstream2;
    unsigned char * _op_zbuf1;
    unsigned char * _op_zbuf2;
    z_stream        _fin_zstream;
    unsigned char * _row_buf;
    unsigned char * _sub_row;
    unsigned char * _up_row;
    unsigned char * _avg_row;
    unsigned char * _paeth_row;
    unsigned int    _next_seq_num;
#endif

#ifdef APNG_READ_SUPPORTED
    const std::vector<APNGFrame>& fileToFrames(const std::string &filePath, unsigned delayNum, unsigned delayDen);
    void compose_frame(unsigned char ** rows_dst, unsigned char ** rows_src, unsigned char bop, unsigned int x, unsigned int y, unsigned int w, unsigned int h);
    unsigned int read_chunk(FILE * f, CHUNK * pChunk);
    void recalc_crc(unsigned char * p, unsigned int size);

    std::vector<CHUNK>   _info_chunks;
#endif

    unsigned int    _width;
    unsigned int    _height;
    unsigned int    _size;
    rgb             _palette[256];
    unsigned char   _trns[256];
    unsigned int    _palsize;
    unsigned int    _trnssize;
	};	// class APNGAsm
	
}	// namespace apngasm

#endif  // _APNGASM_H_
