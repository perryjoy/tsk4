#include "levels.h"
#include "anim.h"

class texture SNAKE::Tex;
class texture WALL::Tex;
class texture FOOD::Tex;

LEVEL::~LEVEL() {
   SNAKE::ClearTex();
   WALL::ClearTex();
   FOOD::ClearTex();
   Background.Delete();
};

LEVEL::LEVEL(std::string Filename, ANIM *Ani)
{
  FILE* F;
  int px = 0, py = 0;
  int w = 0, h = 0;
  char Buf[300];
  char c;
  texture tmp;
  F = fopen(Filename.c_str(), "r");
  if (F == NULL)
    throw "Error";
  fscanf(F, "%i %i", &px, &py); // Player start pos
  fscanf(F, "%s", Buf); // Player texture
  image Im;
  Im.LoadBMP(Buf);
  tmp = texture((char *)"Player", 4, Im);
  SNAKE::SetTex(tmp);
  Im.Free();
  
  fscanf(F, "%s", Buf); // Background texture
  Im.LoadBMP(Buf);
  Background = texture((char*)"Background", 4, Im);
  Im.Free();
  fscanf(F, "%s", Buf); // Wall texture
  Im.LoadBMP(Buf);
  tmp = texture((char *)"WALL", 4, Im);
  WALL::SetTex(tmp);
  Im.Free();
  fscanf(F, "%s", Buf); // FOOD texture
  Im.LoadBMP(Buf);
  tmp = texture((char *)"FOOD", 4, Im);
  FOOD::SetTex(tmp);
  Im.Free();
  fscanf(F, "%i %i", &w, &h); // Map Size
  Size = VEC2i(w, h);
  fscanf(F, "%c", &c); // backslash n
  map.resize(h);
  for (int i = 0; i < h; i++)
  {
    map[i].resize(w);
    for (int j = 0; j < w; j++) {
       UCHAR tmp;
       fscanf(F, "%c", &tmp);
       map[i][j] = TILE();
       map[i][j].type = TILE::TYPE(tmp - '0');
       if (map[i][j].type == TILE::WALLS) {
          WALL w(this);
          w.SetPos(j, i);
          pawns.push_back(new WALL(w));
          map[i][j].pawn = pawns[pawns.size() - 1];
       }
       else if (map[i][j].type == TILE::FOODS) {
          FOOD f(this);
          f.SetPos(j, i);
          pawns.push_back(new FOOD(f));
          map[i][j].pawn = pawns[pawns.size() - 1];
       }
       else {
          map[i][j].pawn = nullptr;
       }
    }
    fscanf(F, "%c", &c); // backslash n
  }
  fclose(F);
  SNAKE player(px, py, this);
  pawns.push_back(new SNAKE(player));
  ani = Ani;
}

VOID LEVEL::KillPawn(int x, int y)
{
   map[y][x].pawn->Kill();
   map[y][x].type = TILE::FREE;
}

BOOL LEVEL::CheckKey(char c)
{
   return ani->GetKeyboardData().keys[c];
}


VEC2i LEVEL::TryRespawn(VEC2i Pos, PAWN *p)
{
   int newX = rand() % (Size.x - 2) + 1;
   int newY = rand() % (Size.y - 2) + 1;
   if (map[newY][newX].type == TILE::FREE && map[newY][newX].pawn == nullptr) {
      map[Pos.y][Pos.x].pawn = nullptr;
      map[Pos.y][Pos.x].type = TILE::FREE;
      map[newY][newX].pawn = p;
      map[newY][newX].type = TILE::FOODS;
      return VEC2i(newX, newY);
   }
   return VEC2i(-1,-1);
}

VOID DrawTile(VEC2i Pos, VEC2i Size, VEC2f Tex1, VEC2f Tex2)
{
  VEC2f Siz(2.0 / Size.x, -2.0 / Size.y);
  VEC2f Pozz(((1.0 * Pos.x / Size.x) - 0.5) * 2, -((1.0 * Pos.y / Size.y) - 0.5) * 2);

  glBegin(GL_QUADS);
  //glBindTexture(GL_TEXTURE_2D, 0);
  //glColor3f(0.3, 0.5, 0.7);
  glTexCoord2f(Tex1.x, Tex1.y);
  glVertex2f(Pozz.x, Pozz.y + Siz.y);
  glTexCoord2f(Tex2.x, Tex1.y);
  glVertex2f(Pozz.x + Siz.x, Pozz.y + Siz.y);
  glTexCoord2f(Tex2.x, Tex2.y);
  glVertex2f(Pozz.x + Siz.x, Pozz.y);
  glTexCoord2f(Tex1.x, Tex2.y);
  glVertex2f(Pozz.x, Pozz.y);
  //glColor3f(1, 1, 1);
  glEnd();
}

VOID DrawTile2(VEC2i Pos, VEC2i Size, VEC2f Tex1, VEC2f Tex2, VEC2f Tex3, VEC2f Tex4)
{
  VEC2f Siz(2.0 / Size.x, -2.0 / Size.y);
  VEC2f Pozz(((1.0 * Pos.x / Size.x) - 0.5) * 2, -((1.0 * Pos.y / Size.y) - 0.5) * 2);

  glBegin(GL_QUADS);
  //glBindTexture(GL_TEXTURE_2D, 0);
  //glColor3f(0.3, 0.5, 0.7);
  glTexCoord2f(Tex1.x, Tex1.y);
  glVertex2f(Pozz.x, Pozz.y);
  glTexCoord2f(Tex2.x, Tex2.y);
  glVertex2f(Pozz.x + Siz.x, Pozz.y);
  glTexCoord2f(Tex3.x, Tex3.y);
  glVertex2f(Pozz.x + Siz.x, Pozz.y + Siz.y);
  glTexCoord2f(Tex4.x, Tex4.y);
  glVertex2f(Pozz.x, Pozz.y + Siz.y);
  //glColor3f(1, 1, 1);
  glEnd();
}

VOID WALL::Draw(VOID)
{
  WALL::Tex.Apply();
  DrawTile(Pos, lev->GetSize(), VEC2f(0, 0), VEC2f(1, 1));
};

SNAKE::SNAKE(int px, int py, LEVEL *l) : PAWN(l)
{
  Arr.push_back(VEC2i(px, py));
  Arr.push_back(VEC2i(px, py - 1));
  Arr.push_back(VEC2i(px, py - 2));
  Arr.push_back(VEC2i(px - 1, py - 2));
  Arr.push_back(VEC2i(px - 1, py - 1));
}

VOID SNAKE::Draw(VOID)
{
  VEC2i tail = Arr[Arr.size() - 1],
    endofbody = Arr[Arr.size() - 2],
    roundhead = Arr[0];
  SNAKE::Tex.Apply();
  if (tail.x > endofbody.x)
    DrawTile2(tail, lev->GetSize(), VEC2f(0.5, 0.5), VEC2f(1, 0.5), VEC2f(1, 0), VEC2f(0.5, 0));
  else if (tail.x < endofbody.x)
    DrawTile2(tail, lev->GetSize(), VEC2f(1, 0), VEC2f(0.5, 0), VEC2f(0.5, 0.5), VEC2f(1, 0.5));
  else if (tail.y > endofbody.y)
    DrawTile2(tail, lev->GetSize(), VEC2f(0.5, 0), VEC2f(0.5, 0.5), VEC2f(1, 0.5), VEC2f(1, 0));
  else
    DrawTile2(tail, lev->GetSize(), VEC2f(1, 0.5), VEC2f(1, 0), VEC2f(0.5, 0), VEC2f(0.5, 0.5));

  for (int i = Arr.size() - 2; i >= 1; i--)
  {
    if (Arr[i].x == Arr[(i - 1)].x && Arr[i].x == Arr[(i + 1)].x)
      DrawTile2(Arr[i], lev->GetSize(), VEC2f(0, 1), VEC2f(0.5, 1), VEC2f(0.5, 0.5), VEC2f(0, 0.5));
    else if (Arr[i].y == Arr[(i - 1)].y && Arr[i].y == Arr[(i + 1)].y)
      DrawTile2(Arr[i], lev->GetSize(), VEC2f(0.5, 1), VEC2f(0.5, 0.5), VEC2f(0, 0.5), VEC2f(0, 1));
    else if ((Arr[i].y < Arr[(i - 1)].y && Arr[i].x > Arr[(i + 1)].x) || (Arr[i].y < Arr[(i + 1)].y && Arr[i].x >Arr[(i - 1)].x))
      DrawTile2(Arr[i], lev->GetSize(), VEC2f(0, 0.5), VEC2f(0, 0), VEC2f(0.5, 0), VEC2f(0.5, 0.5));
    else if ((Arr[i].x < Arr[(i - 1)].x && Arr[i].y > Arr[(i + 1)].y) || (Arr[i].x < Arr[(i + 1)].x && Arr[i].y > Arr[(i - 1)].y))
      DrawTile2(Arr[i], lev->GetSize(), VEC2f(0, 0.5), VEC2f(0.5, 0.5), VEC2f(0.5, 0), VEC2f(0, 0));
    else if ((Arr[i].x < Arr[(i - 1)].x && Arr[i].y < Arr[(i + 1)].y) || (Arr[i].x < Arr[(i + 1)].x && Arr[i].y < Arr[(i - 1)].y))
      DrawTile2(Arr[i], lev->GetSize(), VEC2f(0, 0), VEC2f(0, 0.5), VEC2f(0.5, 0.5), VEC2f(0.5, 0));
    else if ((Arr[i].x > Arr[(i - 1)].x && Arr[i].y > Arr[(i + 1)].y) || (Arr[i].x > Arr[(i + 1)].x && Arr[i].y > Arr[(i - 1)].y))
      DrawTile2(Arr[i], lev->GetSize(), VEC2f(0.5, 0.5), VEC2f(0.5, 0), VEC2f(0, 0), VEC2f(0, 0.5));
  }
  /* Draw Head */
  if (Arr[0].x - Arr[1].x > 0)  //Right 
  {
    DrawTile2(roundhead, lev->GetSize(), VEC2f(0.5, 1), VEC2f(1, 1), VEC2f(1, 0.5), VEC2f(0.5, 0.5));
  }
  else if (Arr[0].x - Arr[1].x < 0) //left
  {
    DrawTile2(roundhead, lev->GetSize(), VEC2f(1, 1), VEC2f(0.5, 1), VEC2f(0.5, 0.5), VEC2f(1, 0.5));
  }
  else if (Arr[0].y - Arr[1].y > 0) //up
  {
    DrawTile2(roundhead, lev->GetSize(), VEC2f(0.5, 1), VEC2f(0.5, 0.5), VEC2f(1, 0.5), VEC2f(1, 1));
  }
  else //down
  {
    DrawTile2(roundhead, lev->GetSize(), VEC2f(1, 1), VEC2f(1, 0.5), VEC2f(0.5, 0.5), VEC2f(0.5, 1));
  }
};

VOID SNAKE::Response(VOID) {
  static double StartT = 1.0 * clock() / CLOCKS_PER_SEC;
  double CurT = 1.0 * clock() / CLOCKS_PER_SEC;
  if (lev->CheckKey('W'))
    CurVel = VEC2i(0, -1);
  if (lev->CheckKey('A'))
    CurVel = VEC2i(-1, 0);
  if (lev->CheckKey('D'))
    CurVel = VEC2i(1, 0);
  if (lev->CheckKey('S'))
    CurVel = VEC2i(0, 1);
  if ((CurVel.x || CurVel.y) && CurT - StartT > 0.2)
  {
    StartT = CurT;
    if (CurVel == Arr[1] - Arr[0])
      CurVel = Arr[0] - Arr[1];
    VEC2i NewPosHead = Arr[0] + CurVel;
    switch (lev->GetState(NewPosHead))
    {
    case TILE::WALLS:
      //no gameover
      break;
    case TILE::FOODS:
      Arr.push_back(Arr.back());
      for (int i = Arr.size() - 2; i > 0; i--)
        Arr[i] = Arr[i - 1];
      Arr[0] += CurVel;
      lev->KillPawn(NewPosHead.x, NewPosHead.y);
      break;
    case TILE::FREE:
      for (int i = Arr.size() - 1; i > 0; i--)
        Arr[i] = Arr[i - 1];
      Arr[0] += CurVel;
      break;
    default:
      break;
    }
  }
}

//VOID FOOD::Kill(VOID)
//{
//}
//
//VOID FOOD::Respawn(VOID)
//{
//}


VOID FOOD::Response(VOID)
{
   static double StartT = 1.0 * clock() / CLOCKS_PER_SEC;
   double CurT = 1.0 * clock() / CLOCKS_PER_SEC;
   VEC2i newPos;

   if (CurT - StartT >= 1.0) {
      if (!isExist && !(rand() % 3)) {
         newPos = lev->TryRespawn(Pos, this);
         if (newPos != VEC2i(-1, -1)) {
            Pos = newPos;
            isExist = true;
         }
      }
   }
}

VOID FOOD::Draw(VOID)
{
  FOOD::Tex.Apply();
  DrawTile(Pos, lev->GetSize(), VEC2f(1.0 * Type / 3, 0), VEC2f(1.0 * (Type + 1) / 3, 1));
};