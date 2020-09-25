#include "anim.h"

#include <list>

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInctance, CHAR *cmdLine, INT showCmd)
{
   ANIM anim;
   anim.AddLevel(new LEVEL("default.txt", &anim));
   anim.Run();

   return 0x1e;
}
