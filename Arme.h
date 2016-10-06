#ifndef ARME_H_INCLUDED
#define ARME_H_INCLUDED

#include "graphic.h"
#include "util/gameConst.h"



typedef struct Arme
{
    Image animWeapon[2];

    int idImg = 0;
    int nbImgs;

    void Init(int nbImg, char* imgPath)
    {
        nbImgs = nbImg;
        char arrayString[50];

        for(int i = 0; i < nbImg; i++)
        {
            //generate the imgpath
            sprintf(arrayString, "%sarme%d.png", imgPath, i);

            //load weapon's images
            animWeapon[i] = CreateImage(arrayString);
        }
    }

    void Attack()
    {
        idImg = Rand(0, nbImgs);
    }

    void Draw(Vec2 pos, int direction)
    {

        switch(direction)
        {
        case TOP:
            ImgSetRotation(getCurrentImage(), 90.0f);
            ImgSetScale(getCurrentImage(), -2.0f, -2.0f);
            ImgSetPosition(getCurrentImage(), pos);
            break;

        case RIGHT:
            ImgSetRotation(getCurrentImage(), 0.0f);
            ImgSetScale(getCurrentImage(), -2.0f, 2.0f);
            ImgSetPosition(getCurrentImage(), pos);
            break;

        case DOWN:
            ImgSetRotation(getCurrentImage(), 270.0f);
            ImgSetScale(getCurrentImage(), 2.0f, -2.0f);
            ImgSetPosition(getCurrentImage(), pos);
            break;

        case LEFT:
            ImgSetRotation(getCurrentImage(), 0.0f);
            ImgSetScale(getCurrentImage(), 2.0f, 2.0f);
            pos.x -= 32;
            ImgSetPosition(getCurrentImage(), pos);
            break;
        }

        DrawImage(getCurrentImage());

    }

    Image getCurrentImage()
    {
        return animWeapon[idImg];
    }

    void Free()
    {
        for(int i = 0; i < nbImgs; i++)
        {
            FreeImage(animWeapon[i]);
        }
    }


} Arme;

#endif // ARME_H_INCLUDED
