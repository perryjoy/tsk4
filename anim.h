#pragma once
#include "win.h"
#include "render.h"
#include "levels.h"

class ANIM : public virtual WIN, public virtual RENDER {
   std::vector<LEVEL *> levels;

public:
   ANIM(VOID) : WIN(), RENDER(this)
   {
   }

   ~ANIM(VOID)
   {
      for (auto &i : levels) {
         delete i;
      }
   }

   VOID AddLevel(LEVEL *lev)
   {
      levels.push_back(lev);
   }

   UINT GetW(VOID)
   {
     return RENDER::w;
   }

   UINT GetH(VOID)
   {
     return RENDER::h;
   }

   virtual VOID Paint(VOID)
   {
      RENDER::StartFrame();
      for (auto &i : levels)
         i->Draw();
      RENDER::EndFrame();
   }
   virtual VOID Timer(VOID)
   {
      for (auto &i : levels)
         i->Response();
   }

   VOID Run(VOID)
   {
      WIN::Run();
   }
};