#include <node.h>
#include <node_buffer.h>
#include <iostream>

using namespace std;

namespace rgbaToBmp {

    using v8::Exception;
    using v8::FunctionCallbackInfo;
    using v8::Isolate;
    using v8::Local;
    using v8::Object;
    using v8::String;
    using v8::Value;

    /**** BMP file header structure ****/
    typedef struct {
        unsigned int   bfSize;           /* Size of file */
        unsigned short bfReserved1;      /* Reserved */
        unsigned short bfReserved2;      /* ... */
        unsigned int   bfOffBits;        /* Offset to bitmap data */
    } BITMAPFILEHEADER;

    /**** BMP file info structure ****/
    typedef struct {
        unsigned int   biSize;           /* Size of info header */
        int            biWidth;          /* Width of image */
        int            biHeight;         /* Height of image */
        unsigned short biPlanes;         /* Number of color planes */
        unsigned short biBitCount;       /* Number of bits per pixel */
        unsigned int   biCompression;    /* Type of compression to use */
        unsigned int   biSizeImage;      /* Size of image data */
        int            biXPelsPerMeter;  /* X pixels per meter */
        int            biYPelsPerMeter;  /* Y pixels per meter */
        unsigned int   biClrUsed;        /* Number of colors used */
        unsigned int   biClrImportant;   /* Number of important colors */
    } BITMAPINFOHEADER;

    unsigned char* rgbaToBmp32(unsigned char *rgbbuf,int width,int height, int &size) {
        BITMAPFILEHEADER bfh;
        BITMAPINFOHEADER bih;
        unsigned short bfType=0x4d42;           
        bfh.bfReserved1 = 0;
        bfh.bfReserved2 = 0;
        bfh.bfSize = 2+sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER)+width*height*4;
        bfh.bfOffBits = 0x36;

        bih.biSize = sizeof(BITMAPINFOHEADER);
        bih.biWidth = width;
        bih.biHeight = height;
        bih.biPlanes = 1;
        bih.biBitCount = 32;
        bih.biCompression = 0;
        bih.biSizeImage = 0;
        bih.biXPelsPerMeter =0;
        bih.biYPelsPerMeter =0;
        bih.biClrUsed = 0;
        bih.biClrImportant = 0;
        
        unsigned char *tran=new unsigned char[width*height*4];
        int lineSize=width*4;
        int dataSize=height*lineSize;

        for(int i=0;i<dataSize;i+=lineSize) {
            memcpy(tran+dataSize-lineSize-i,rgbbuf+i,lineSize);
        }

        size=sizeof(bfType)+sizeof(bfh)+sizeof(bih)+width*height*4;
        unsigned char* out=new unsigned char[size];
        unsigned char *tmp=out;
        memcpy(out,&bfType,sizeof(bfType));
        out+=sizeof(bfType);
        memcpy(out,&bfh, sizeof(bfh));
        out+=sizeof(bfh);
        memcpy(out,&bih, sizeof(bih));
        out+=sizeof(bih);
        memcpy(out,tran,width*height*4);

        delete[] tran;

        return tmp;
    }

    void GCFreeDataInMemory(char *mem, void *hit) {
        cout<<"release..."<<endl;
        delete[] mem;
    }

    void rgbaToBmp(const FunctionCallbackInfo<Value>& args) { 
        Isolate* isolate = args.GetIsolate();
        if (!args[0] -> IsObject()) {
            isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate,"param invalid")));
            return;
        }
        if (!args[1] -> IsNumber() || !args[1] -> IsNumber()) {
            isolate->ThrowException(Exception::TypeError(String::NewFromUtf8(isolate,"param invalid")));
            return;
        }
        unsigned char* rgbBuffer;
        unsigned int width;
        unsigned int height;
        rgbBuffer = (unsigned char*) node::Buffer::Data(args[0]->ToObject());
        width = args[1]->Uint32Value();
        height = args[2]->Uint32Value();
        int size=width*height*4;
        unsigned char tmp;
        for(int i=0;i<size;i+=4){
            tmp=rgbBuffer[i];
            rgbBuffer[i]=rgbBuffer[i+2];
            rgbBuffer[i+2]=tmp;
        }
        unsigned char *bgrbuffer=rgbaToBmp32(rgbBuffer,width,height,size);
        args.GetReturnValue().Set(node::Buffer::New(isolate, (char*)bgrbuffer, (size_t)size, GCFreeDataInMemory, NULL).ToLocalChecked()); 
    }

    void init(Local<Object> exports) {
        NODE_SET_METHOD(exports, "rgbaToBmp", rgbaToBmp);
    }

    NODE_MODULE(NODE_GYP_MODULE_NAME, init)
} 
