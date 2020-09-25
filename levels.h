#ifndef __LEVELS_H_
#define __LEVELS_H_

#include "def.h"
#include "texture.h"
#include <ctime>

class ANIM;
class LEVEL;

class PAWN {
protected:
   LEVEL *lev = NULL;
   BOOL isExist = true;
public:
   PAWN(LEVEL *l) : lev(l)
   {
   }
   virtual VOID Draw(VOID) = 0;
   virtual VOID Response(VOID) = 0;
   BOOL IsExist(VOID)
   {
      return isExist;
   }
   virtual VOID Kill(VOID)
   {
      isExist = false;
   }
   virtual VOID Respawn(VOID)
   {
      isExist = true;
   }
};

struct TILE
{
  enum TYPE
  {
    FREE = 0,
    WALLS = 1,
    FOODS = 2
  } type;
  PAWN *pawn;
};

class LEVEL {

   ANIM *ani = nullptr;
   std::vector<PAWN *> pawns;
   std::vector<std::vector<TILE>> map;
   VEC2i Size;
   texture Background;
public:
   LEVEL(std::string Filename, ANIM *Ani);
   VOID Draw(VOID)
   {
      Background.Apply();

      glBegin(GL_QUADS);

      glTexCoord2f(0, 0);
      glVertex2f(-1, 1);
      glTexCoord2f(1, 0);
      glVertex2f(1, 1);
      glTexCoord2f(1, 1);
      glVertex2f(1, -1);
      glTexCoord2f(0, 1);
      glVertex2f(-1, -1);
      glEnd();

      for (auto &a : pawns)
         if (a->IsExist())
            a->Draw();
   }
   VOID Response(VOID)
   {
      for (auto &a : pawns)
         a->Response();
   }
   VOID KillPawn(int x, int y);
   TILE::TYPE GetState(VEC2i v)
   {
      return map[v.y][v.x].type;
   }
   BOOL CheckKey(char c);

   VEC2i GetSize(VOID)
   {
      return Size;
   }

   VEC2i TryRespawn(VEC2i pos, PAWN *p);
   ~LEVEL();
};

class SNAKE : public PAWN {
   static texture Tex;
   std::vector<VEC2i> Arr;
   VEC2i CurVel = VEC2i(0);
public:
   SNAKE(int x, int y, LEVEL *l);
   ~SNAKE() { };
   virtual VOID Draw(VOID);
   virtual VOID Response(VOID);
   static VOID SetTex(texture t)
   {
      Tex = t;
   }
   static VOID ClearTex(VOID)
   {
      Tex.Delete();
   }
};

class WALL : public PAWN {
   static texture Tex;
   VEC2i Pos;
public:
   WALL(LEVEL *l) : PAWN(l) { };
   ~WALL() { };
   virtual VOID Draw(VOID);
   virtual VOID Response(VOID) { };
   static VOID SetTex(texture t)
   {
      Tex = t;
   }
   static VOID ClearTex(VOID)
   {
      Tex.Delete();
   }

   VOID SetPos(int x, int y)
   {
      Pos = VEC2i(x, y);
   }
};

class FOOD : public PAWN {
   static texture Tex;
   UINT Type = 0;
   VEC2i Pos;
public:
   FOOD(LEVEL *l) : PAWN(l) 
   {
      Type = rand() % 3;
   }
   ~FOOD() { }
   virtual VOID Draw(VOID);
   virtual VOID Response(VOID);
   /*VOID Kill(VOID);
   VOID Respawn(VOID);*/
   static VOID SetTex(texture t)
   {
      Tex = t;
   }
   static VOID ClearTex(VOID)
   {
      Tex.Delete();
   }

   VOID SetPos(int x, int y)
   {
      Pos = VEC2i(x, y);
   }
};

#endif // __LEVELS_H_
