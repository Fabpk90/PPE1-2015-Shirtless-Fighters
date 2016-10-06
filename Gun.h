#ifndef GUN_H_INCLUDED
#define GUN_H_INCLUDED

#include <cstring>
#include <cstdlib>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include "graphic.h"
#include "util/gameConst.h"


typedef struct Gun
{

    Image bullet[4];
    Image impact[4];
    int balle=0;
    Image smoke=CreateImage("data/smoke.png");
    int sens[4];
    int collision[4];
    int comptB=0;
    int comptI[4];
    //int chest=1;
    //Image openChest=CreateImage("data/openChest.png");
    int timerB=0;
    int timer=0;


    void Init()
    {
        for (int i=0; i<4; i++)
        {
            bullet[i]=CreateImage("data/fireball.png");
            collision[i]=0;
            comptI[i]=0;
            impact[i]=CreateImage("data/impact.png");
        }
    }



    void Bullet(int movementDirection,Vec2 Right,Vec2 Left)
    {
        if (balle<3)
        {

            balle=balle+1;
            ImgSetScale(bullet[balle],2.0);

            if(movementDirection==0)
            {
                ImgSetPosition(bullet[balle],Right);
                sens[balle]=1;
            }
            if(movementDirection==1)
            {
                ImgSetPosition(bullet[balle],Left.x - 32, Left.y);
                sens[balle]=0;
            }
        }
        else
        {
            comptB=40;
        }
    }

    void actuBullet(Vec2 Right,Vec2 Left,int Mat[40][23],int direction)
    {
        if (balle>=0 && balle<4)
        {
            Vec2 posBullet;
            for (int i=0; i<=balle; i++)
            {
                if(collision[i]!=1)
                {

                    if(sens[i]==1)
                    {
                        ImgSetPosition(bullet[i],ImgGetPosition(bullet[i]).x+25,ImgGetPosition(bullet[i]).y);
                        posBullet=CreateVec2(ImgGetPosition(bullet[i]).x+21,ImgGetPosition(bullet[i]).y+16);
                    }
                    else
                    {
                        ImgSetScale(bullet[i],-2,2);
                        ImgSetPosition(bullet[i],ImgGetPosition(bullet[i]).x-25,ImgGetPosition(bullet[i]).y);
                        posBullet=CreateVec2(ImgGetPosition(bullet[i]).x-21,ImgGetPosition(bullet[i]).y-16);
                    }

                    if (ImgGetPosition(bullet[i]).x<1300 && ImgGetPosition(bullet[i]).x>0 && ImgGetPosition(bullet[i]).y<900 && ImgGetPosition(bullet[i]).y>0)
                    {

                        if (Mat[(int)posBullet.x/32][(int)posBullet.y/32]==1)
                        {
                            collision[i]=1;
                            comptI[i]=15;
                            if(sens[i]==1)
                            {
                                ImgSetPosition(impact[i],ImgGetPosition(bullet[i]).x-25,ImgGetPosition(bullet[i]).y);
                            }
                            else
                            {
                                ImgSetPosition(impact[i],ImgGetPosition(bullet[i]));
                            }

                            ImgSetPosition(bullet[i], 0.0f, 0.0f);

                        }


                        DrawImage(bullet[i]);
                    }
                }

                if(comptI[i]>0)
                {
                    DrawImage(impact[i]);
                    comptI[i] =comptI[i]-1;
                }


                if(comptB>0)
                {
                    if(direction==RIGHT)
                    {
                        ImgSetScale(smoke,0.7,0.7);
                        ImgSetPosition(smoke,Right-20);
                    }
                    else if(direction==LEFT)
                    {
                        ImgSetScale(smoke,-0.7,0.7);
                        ImgSetPosition(smoke,Left-20);
                    }

                    DrawImage(smoke);
                    comptB =comptB-1;
                }
            }
        }
        /*if (chest==0)
        {
            timer+=1;
            if (timer-timerB>600)
            {
                chest=1;
                timerB=timer;
            }
        }*/
    }


    void reload()
    {
        balle=0;
        //chest=0;
        for (int i=0; i<4; i++)
        {
            collision[i]=0;
        }
    }

    /*int OpenChest()
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
    }*/

    void Free()
    {
         for (int i=0; i<4; i++)
        {
            FreeImage(bullet[i]);
            FreeImage(impact[i]);
        }
        FreeImage(smoke);
        //FreeImage(openChest);
    }


} Gun;

#endif // GUN_H_INCLUDED
