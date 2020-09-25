#ifndef __IMAGE_H_
#define __IMAGE_H_

#include "def.h"

typedef struct
{
  BYTE B, G, R, A;
} color4;

class image {
   friend class texture;

   std::vector<color4> Pixels;
   UINT W, H;
public:
   image() : W(0), H(0)
   {
   }
   BOOL LoadBMP(std::string FileName);

   VOID Free(VOID)
   {
      Pixels.clear();
   }

   ~image()
   {
   };
};


#endif // __IMAGE_H_
