#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED


#include <cstring>
#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>

#include "graphic.h"
/*
typedef struct Player
{
    Image img[10];
    unsigned int timer=0;
    int imgActu=0;
    char txt[100];
    float y=4;


    unsigned int animSpeed=50;
    Vec2 deplacement =CreateVec2(840,500);
    Vec2 dte =CreateVec2(-1.42f,1.42f);
    Vec2 gch =CreateVec2(1.42f,1.42f);
    Vec2 up =CreateVec2(0,0);
    Vec2 down =CreateVec2(0,-4);

    Vec2 htGc =CreateVec2(0,0);
    Vec2 htBs =CreateVec2(0,0);
    Vec2 Gc =CreateVec2(0,0);
    int jumpCount=0;
    //speed of the player
    float vitesse =5;
    const int CstVit =10;//vitesse max joueur

    Image shield=CreateImage("data/shield.png");
    Image bullet[4];
    int balle=0;
    Image smoke=CreateImage("data/smoke.png");
    int sens[4];
    int comptB=0;
    int defense=0;
    int chest=1;
    Image openChest=CreateImage("data/openChest.png");
    int playerSpeed;

//Fonctions de bases
    void Init()
    {
        for (int i=0; i<10; i++)
        {
            sprintf(txt,"data/Run (%d).png",i+1);
            img[i]=CreateImage(txt);
            ImgSetScale(img[i],1.42f,1.42f);
            ImgSetPosition(img[i],840,500);
            if (i<4)
            {
                bullet[i]=CreateImage("data/fireball.png");
            }
        }
        playerSpeed=0;

    }
    void Actu()
    {
        //find the right img of the anim to play
        if (Time()-timer>animSpeed)
        {
            imgActu=(imgActu+1)%10;
            timer=Time();
        }
        ImgSetPosition(img[imgActu],deplacement-ImgGetSize(img[imgActu])/2);

        /*htGc =(0,0);
        htBs =(0,0);
        bsDt =(0,0);
        bsGc =(0,0);*/

    }

    void Draw()
    {
        DrawImage(img[imgActu]);
    }

//Fonctions Gravité/collision
    void plafond()
    {
        if (ImgGetPosition(img[imgActu]).y > 10)
        {
            deplacement= deplacement+down;
            down.y -=0.5;
        }
    }

    void plafondB()
    {
        if (ImgGetPosition(img[imgActu]).y > 10)
        {
            deplacement= deplacement+down;
            up.y=0;
        }
    }

    void sol()
    {
        deplacement= deplacement+up;
        down.y=-4;
        jumpCount=0;

        if(up.y<0)
        {
            up.y=0;
        }
        if (up.y>0)
        {
            up.y-=0.5;
        }
        up.x=0;
    }

    void Gravite()
    {
        if (ImgGetPosition(img[imgActu]).y > 10)
        {
            deplacement= deplacement+up+down;
            if (down.y>-15)
                down.y -=0.3;
        }

        if(up.x>0)
            up.x-=1;

        if(up.x<0)
            up.x+=1;

        if (up.y>0)
        {
            up.y+=-1;
        }
    }

    void Shield()
    {
        if(KeyGetOneShot(SDLK_c))
        {
            if (defense==0)
                defense=1;
            else
                defense=0;
        }
        if (defense==1)
        {
            DrawImage(shield);
            ImgSetScale(shield,1.75);
            ImgSetColor(shield,CreateColor(100,100,150,120));
            ImgSetPosition(shield,deplacement-ImgGetSize(shield)/2);
        }
    }

    void Jump()
    {
        if (jumpCount<1)
        {
            up.y =20;
            down.y=-4;
            jumpCount+=1;
        }

    }

    void WallJumpLeft()
    {
        up.y +=2;
        up.x -=2.5;
        playerSpeed=0;
        down.y=-4;
    }

    void WallJumpRight()
    {
        up.y +=2;
        up.x +=2.5;
        playerSpeed=0;
        down.y=-4;
    }

//Fonctions déplacement
    void Gauche()
    {
        if (ImgGetScale(img[1])==dte)
        {
            animSpeed=70;
            playerSpeed=0;
        }
        for (int i=0; i<10; i++)
        {
            ImgSetScale(img[i],1.42,1.42);
        }
        if (deplacement.x>=10 && deplacement.x<=1280)
        {
            if (playerSpeed>-MAXSPEED)
            {
                deplacement.x=deplacement.x+playerSpeed;
                playerSpeed-=0.8;
            }
            else
            {
                deplacement.x+=playerSpeed;
            }
        }

        if (animSpeed>15)
            animSpeed=animSpeed-1;


    }
    void Droite()
    {
        if (ImgGetScale(img[1])==gch)
        {
            animSpeed=70;
            playerSpeed=0;
        }
        for (int i=0; i<10; i++)
        {
            ImgSetScale(img[i],-1.42,1.42);
        }

        //checks if the player is within the screen res
        if (deplacement.x>=10 && deplacement.x<=1280)
        {
            if (playerSpeed<MAXSPEED)
            {
                deplacement.x+=playerSpeed;
                playerSpeed+=0.8;
            }
            else
            {
                deplacement.x+=playerSpeed;
            }
        }

        if (animSpeed>15)
            animSpeed=animSpeed-1;
    }

    void Bullet()
    {
        if (balle<3)
        {

            balle++;
            ImgSetScale(bullet[balle],2.0);
            ImgSetPosition(bullet[balle],deplacement);
            if(ImgGetScale(img[imgActu]).x==-1.42f)
            {
                sens[balle]=1;
            }
            else
            {
                sens[balle]=0;
            }
        }
        else
        {
            comptB=40;
        }
    }

    void actuBullet()
    {
        if (balle>=0)
        {
            for (int i=0; i<=balle; i++)
            {
                if(sens[i]==1)
                {
                    ImgSetPosition(bullet[i],ImgGetPosition(bullet[i]).x+25,ImgGetPosition(bullet[i]).y);
                }
                else
                {
                    ImgSetScale(bullet[i],-2,2);
                    ImgSetPosition(bullet[i],ImgGetPosition(bullet[i]).x-25,ImgGetPosition(bullet[i]).y);
                }
                DrawImage(bullet[i]);

            }

            if(comptB>0)
            {
                ImgSetPosition(smoke,deplacement.x,deplacement.y-5);
                ImgSetScale(smoke,0.7);
                DrawImage(smoke);
                comptB =comptB-1;
            }
        }
    }

    void reload()
    {
        if (chest==1 && balle>0)
        {
            balle=0;
            chest=0;
            ImgSetPosition(openChest,deplacement.x-26,deplacement.y-28);
        }
    }

    int OpenChest()
    {
        if(chest==0)
        {
            DrawImage(openChest);
            return 1;
        }
        else
        {
            return 0;
        }
    }

//Fonction de renvoie
    Image getImg()
    {
        return img[imgActu];
    }

    Vec2 GetPosition()
    {
        return deplacement;
    }

    /*Vec2 GetBullet()
     {
         return img
     }*/

} Player;
*/


#endif // PLAYER_H_INCLUDED

