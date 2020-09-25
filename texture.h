#pragma once

#include "def.h"
#include "Image.h"

class texture {
   char *Name;
   UINT W, H;
   UINT Id;
public:
   texture(VOID) : W(0), H(0), Id(-1)
   {
   }

   texture(CHAR *Name, INT BytesPerPixel, image &Im)
   {
      this->Name = Name;
      W = Im.W;
      H = Im.H;
      glGenTextures(1, &Id);
      glBindTexture(GL_TEXTURE_2D, Id);
      //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
      //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
      glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

      gluBuild2DMipmaps(GL_TEXTURE_2D, BytesPerPixel, W, H,
         BytesPerPixel == 4 ? GL_BGRA_EXT : BytesPerPixel == 3 ? GL_BGR_EXT : GL_LUMINANCE,
         GL_UNSIGNED_BYTE, &Im.Pixels[0]);
      glBindTexture(GL_TEXTURE_2D, 0);
   }
   VOID Apply(VOID)
   {
      glBindTexture(GL_TEXTURE_2D, Id);
   }

   VOID Delete(VOID)
   {
      if (Id == -1)
         return;
      glDeleteTextures(1, &Id);
      Id = -1;
   }

   ~texture()
   {
   }
};