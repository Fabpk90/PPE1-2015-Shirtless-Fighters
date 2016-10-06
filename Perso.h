#ifndef PERSO_H_INCLUDED
#define PERSO_H_INCLUDED

#include <vector>
#include <string>

#include "graphic.h"
#include "Arme.h"
#include "util/gameConst.h"
#include "Gun.h"

#include "util/gameConst.h"
#include "util/spawn.h"


typedef struct Perso
{

    /*
    * Collisions work like this: press a button check if he can move, then move or not
    * the bottom collision detection is done before each end of the loop, before the update method
    */

    Image imgRun[8];
    Image imgJump;
    Image imgIdle;
    Image imgFall;
    Image imgBlood;
    Image imgAttack;

    int idImage = 0;

    std::string name;

    unsigned int animTimeStart = Time();
    unsigned int animTimeEnd = Time();

    //the player number, (0 player 1)
    int numPlayer;

    //indicate where the player cannot go
    bool isCollidingRight = false, isCollidingLeft = false, isCollidingBottom = false, isCollidingTop = false;

    //0: right, 1: left, 2: top, 3: down, used for attacks
    int movementDirection = RIGHT;

    //0: right, 1: left, used for imgs anim
    int direction = RIGHT;

    //thanks to them we can check the collisions, bottom left is gave by the imgpos
    Vec2 topLeft, topRight, bottomRight, bottomLeft;
    Vec2 Left, Right, Top, Bottom;

    Vec2 imgPosition = CreateVec2(0.0f, 0.0f);

    //float needed to simulate gravity, for jumps and fall, up = jump, down = fall
    float up = 0.0f, down = -4.0f;

    float movementSpeed = MOVEMENTSPEED;
    float jumpHeight = JUMPHEIGHT;

    bool isPlayerMoving = false;

    Arme weapon;
    bool isAttacking = false;
    bool canAttack = true;
    unsigned int timeDeath = 1;

    std::vector<Vec2> *respawnPoint = nullptr;
    std::vector<unsigned int> *respawnPointTimer = nullptr;

    bool isInvicible = false;

    unsigned int animTimeWeapon = 0;

    int animTimeWeaponWaiting = 300;
    int timeBeforeStrike = 300;

    int score = 0;
    int nbKill = 0;
    int nbDeath = 0;

    float jumpDestination = 0;
    bool isPlayerJumping = false;
    int jumpCount = 0;
    bool canJump = true;

    Vec2 movementVector = CreateVec2(0.0f, 0.0f);

    float gravity = 2.0f;

    Gun gun;

    void Init(char* imgPath, Vec2 pos, int num)
    {
        numPlayer = num;
        setPosition(pos);

        loadImage(imgPath);

        //needed for the 3 collider point ( the 4 is the imgPos)
        calculateColliderPos();
    }

    void Init(char * imgPath, int num)
    {
        numPlayer = num;
        loadImage(imgPath);
    }

    void Init(char* imgPath, float x, float y, int num)
    {
        numPlayer = num;
        setPosition(x, y);

        loadImage(imgPath);

        calculateColliderPos();
    }


    Image getImage()
    {
        if(movementVector.x == 0.0f && movementVector.y == 0.0f)//idle
        {
            if(isAttacking)
            {
                 if(direction == RIGHT)//going right
                    ImgSetScale(imgAttack, -1.0f, 1.0f);
                else//going left
                    ImgSetScale(imgAttack, 1.0f, 1.0f);

                ImgSetPosition(imgAttack, imgPosition);
                return (imgAttack);
            }
            else
            {
                if(direction == RIGHT)
                {
                    ImgSetScale(imgIdle, -1.0f, 1.0f);
                }
                else
                    ImgSetScale(imgIdle, 1.0f, 1.0f);
                ImgSetPosition(imgIdle, imgPosition);
                return (imgIdle);
            }
        }
        else if(isPlayerJumping)//jump
        {
            if(direction == RIGHT)
            {
                ImgSetScale(imgJump, -1.0f, 1.0f);
            }
            else
                ImgSetScale(imgJump, 1.0f, 1.0f);

            ImgSetPosition(imgJump, imgPosition);
            return (imgJump);
        }
        else if(movementVector.y < 0.0f)//falling
        {
            if(direction == RIGHT)
            {
                ImgSetScale(imgFall, -1.0f, 1.0f);
            }
            else
                ImgSetScale(imgFall, 1.0f, 1.0f);

            ImgSetPosition(imgFall, imgPosition);
            return (imgFall);
        }
        else //normal
        {
            //if he's attacking
            if(!isAttacking)
            {
                 if(direction == RIGHT)//going right
                    ImgSetScale(getCurrentImage(), -1.0f, 1.0f);
                else//going left
                    ImgSetScale(getCurrentImage(), 1.0f, 1.0f);

                ImgSetPosition(getCurrentImage(), imgPosition);
                return (getCurrentImage());
            }
            else
            {

                if(direction == RIGHT)//going right
                    ImgSetScale(imgAttack, -1.0f, 1.0f);
                else//going left
                    ImgSetScale(imgAttack, 1.0f, 1.0f);

                ImgSetPosition(imgAttack, imgPosition);
                return (imgAttack);

            }
        }

    }


    void setSpawnPoints( std::vector<Vec2> *respawnPoints, std::vector<unsigned int> *respawnPointTimers)
    {
        respawnPoint = respawnPoints;
        respawnPointTimer = respawnPointTimers;
    }

    void loadImage(char* imgPath)
    {

        gun.Init();
        weapon.Init(2, imgPath);

        char txt[50];

        for (int i=0; i<8; i++)
        {
            sprintf(txt,"%srun%d-%d.png",imgPath, i, numPlayer);
            imgRun[i]=CreateImage(txt);

        }

        sprintf(txt,"%sidle-%d.png",imgPath, numPlayer);
        imgIdle = CreateImage(txt);

        sprintf(txt,"%sjump-%d.png",imgPath, numPlayer);
        imgJump = CreateImage(txt);

        sprintf(txt,"%sfall-%d.png",imgPath, numPlayer);
        imgFall = CreateImage(txt);

        sprintf(txt,"%sblood.png",imgPath);
        imgBlood = CreateImage(txt);

        sprintf(txt,"%sattack-%d.png",imgPath, numPlayer);
        imgAttack = CreateImage(txt);
    }

    void setMovementSpeed(float value)
    {
        movementSpeed = value;
    }

    void setJumpHeight(float value)
    {
        jumpHeight = value;
    }

    void setGravity(float newGravity)
    {
        gravity = newGravity;
    }

    void setPosition(float x, float y)
    {
        imgPosition.x = x;
        imgPosition.y = y;

         for (int i=0; i<8; i++)
        {
            ImgSetPosition(imgRun[i], GetPosition());
        }

        ImgSetPosition(imgIdle, GetPosition());
        ImgSetPosition(imgJump, GetPosition());
        ImgSetPosition(imgFall, GetPosition());
        //ImgSetPosition(imgBlood, GetPosition());
        ImgSetPosition(imgAttack, GetPosition());

        calculateColliderPos();
    }

    void setPosition(Vec2 pos)
    {
        setPosition(pos.x, pos.y);
    }

    void addPosition(float x, float y)
    {
        imgPosition.x += x;
        imgPosition.y += y;
    }

    void addPosition(Vec2 pos)
    {
        addPosition(pos.x, pos.y);
    }

    void Scored()
    {
        score++;
    }

    void calculateColliderPos()
    {
        topRight = CreateVec2((GetPosition().x + 30) / 32, (GetPosition().y + 64) / 32);
        topLeft = CreateVec2((GetPosition().x + 2) / 32, (GetPosition().y + 64) / 32);

        Top = CreateVec2((GetPosition().x + 16) / 32, (GetPosition().y + 62) / 32);

        bottomRight = CreateVec2((GetPosition().x + 30) / 32, (GetPosition().y) / 32);
        bottomLeft = CreateVec2((GetPosition().x + 2) / 32, (GetPosition().y ) / 32);

        Bottom = CreateVec2((GetPosition().x + 16) / 32, (GetPosition().y) / 32);

        Right = CreateVec2((GetPosition().x + 40) / 32, (GetPosition().y + 32) / 32);
        Left = CreateVec2((GetPosition().x - 10) / 32, (GetPosition().y + 32) / 32);
    }

    Vec2 GetPosition()
    {
        return imgPosition;
    }

    Vec2 GetImagePosition()
    {
        return ImgGetPosition(getCurrentImage());
    }

    //does not work if the player is jumping
    Image getCurrentImage()
    {
        if(movementVector.x == 0.0f && movementVector.y == 0.0f)//idle
        {
           return imgIdle;
        }
        else if(movementVector.y > 0.0f)//jump
        {
            return imgJump;
        }
        else if(movementVector.y < 0.0f)//falling
        {
            return imgFall;
        }
        else //normal
        {
            //if he's attacking
            if(isAttacking)
            {
                return imgAttack;
            }
            else
            {
                return imgRun[idImage];
            }
        }
    }

    void goRight()
    {
        isPlayerMoving = true;

        movementVector.x += MOVEMENTSPEED;
        movementDirection = RIGHT;
        direction = RIGHT;
    }

    void goLeft()
    {
        isPlayerMoving = true;

        movementVector.x -= MOVEMENTSPEED;
        movementDirection = LEFT;
        direction = LEFT;
    }

    //if the player is not jumping, he jump and can't jump again until it touches the ground or he stops
    //the jump destination is used as a to reach position for the jump
    void goUp()
    {
        isPlayerMoving = true;
        isCollidingBottom = false;

        movementDirection = TOP;

        if(jumpCount < 2)
        {
            down = 0.0f;
            jumpCount++;
            isPlayerJumping = true;
            jumpDestination = GetPosition().y + jumpHeight;
        }
    }


    bool checkLeftCollision(int matMap[40][23])
    {
        if (matMap[(int)(Left.x)][(int)(Left.y)] != 1 && matMap[(int)(topLeft.x)][(int)(topLeft.y)] != 1)
        {
            if(!isCollidingBottom)//in the air?
            {
                bottomLeft = CreateVec2((GetPosition().x - 10) / 32, (GetPosition().y ) / 32);

                if(matMap[(int)(bottomLeft.x)][(int)(bottomLeft.y)] != 1)
                {
                    bottomLeft = CreateVec2((GetPosition().x ) / 32, (GetPosition().y ) / 32);
                    isPlayerMoving = false;
                    return false;
                }
                else
                {
                    bottomLeft = CreateVec2((GetPosition().x) / 32, (GetPosition().y ) / 32);
                    isPlayerMoving = true;
                    return true;
                }
            }
            isPlayerMoving = false;
            return false;
        }
        isPlayerMoving = true;
        return true;
    }

    bool checkTopCollision(int matMap[40][23])
    {
        if(matMap[(int)(topLeft.x)][(int)(topLeft.y)] != 1 && matMap[(int)(topRight.x)][(int)(topRight.y)] != 1)
        {
            isPlayerMoving = false;
            return false;
        }
        isPlayerMoving = true;
        return true;
    }

    bool checkBottomCollision(int matMap[40][23])
    {
        isCollidingBottom = false;
        if (matMap[(int)(bottomLeft.x)][(int)(bottomLeft.y)] == 1 || matMap[(int)(bottomRight.x)][(int)(bottomRight.y)] == 1)
        {
            jumpCount = 0;
            isCollidingBottom = true;

            Vec2 pos = GetPosition();

            pos.y += ((bottomLeft.y ) * 32) - imgPosition.y;

            setPosition(pos);
            calculateColliderPos();
            isPlayerMoving = false;
            return true;
        }
        //in the air here

        Bottom = CreateVec2( (GetPosition().x + 16) / 32, (GetPosition().y - 4) / 32);

        if(matMap[(int)Bottom.x][(int)Bottom.y] != 1)
        {
            return false;
        }
        else//colliding
        {
            jumpCount = 0;
            isCollidingBottom = true;

            Vec2 pos = GetPosition();

            //cause collision is detected 4 y down
            pos.y += 4;

            setPosition(pos);
            calculateColliderPos();
            isPlayerMoving = false;

            return true;
        }

        isPlayerMoving = true;

        return false;
    }

    //check if the player is colliding with something, if he is he gets blocked
    //return the bool and set it to the collision bool
    bool checkRightCollision(int matMap[40][23])
    {

        if (matMap[(int)(Right.x)][(int)(Right.y)] != 1 && matMap[(int)(topRight.x)][(int)(topRight.y)] != 1)
        {
            if(!checkBottomCollision(matMap))
            {
                bottomRight = CreateVec2((GetPosition().x + 42) / 32, (GetPosition().y ) / 32);

                if(matMap[(int)(bottomRight.x)][(int)(bottomRight.y)] != 1 && matMap[(int)(Right.x)][(int)(Right.y)] != 1)
                {
                    bottomRight = CreateVec2((GetPosition().x + 32) / 32, (GetPosition().y ) / 32);
                    return false;
                }
                else
                {
                    bottomRight = CreateVec2((GetPosition().x + 32) / 32, (GetPosition().y ) / 32);
                    isPlayerMoving = true;
                    return true;
                }
            }
            return false;
        }

        /*Vec2 pos = GetPosition();

        pos.x -= Right.x - (int)Right.x ;

        setPosition(pos);
        calculateColliderPos();*/

        isPlayerMoving = false;

        return true;
    }

    void DrawCurrentImage(Image img)
    {
        //if the player is dead then we make the img blink
        if(isInvicible)
        {
            if(Time() % 5 == 0)
                DrawImage(img);
        }
        else
            DrawImage(img);
    }

    //called each frame to move the player if he can (checks collisions)
    void update(float deltaTime, int matMap[40][23], std::vector<Perso> *perso)
    {

        //if the player is jumping, check if he has reached the destination,
        //if not he goes up if yes he stops and can jump again
        if(isPlayerJumping)
        {
            down -= .5f;

            //if the player has reached the jump dest then he stops
            if(GetPosition().y >= jumpDestination || checkTopCollision(matMap))
            {
                isPlayerJumping = false;
                down = 0.0f;
            }
            else//if not, he goes up
            {
                movementVector.y += ((deltaTime) * 4) + down;
                canJump = false;
                //isPlayerJumping = true;
            }
        }
        else//if not jumping he goes down thanks to gravity
        {
            if(!isCollidingBottom)//if he doesn't touch the ground
            {
                movementDirection = DOWN;

                movementVector.y -= (gravity * deltaTime) ;
                //setPosition(GetPosition().x, GetPosition().y - (2 * deltaTime));
            }//if he does he can jump again
            else
            {
                canJump = true;
                movementDirection = direction;
                //movementVector = CreateVec2(0.0f, 0.0f);
            }
        }

        //moves the player, if it needs to
        addPosition(movementVector);

          //updating the animation
        animTimeEnd = Time();

        isPlayerMoving = true;

        if(animTimeEnd - animTimeStart >= 1000 / 7)
        {
            idImage = (idImage + 1) % 8;
            animTimeStart = animTimeEnd;
        }

        DrawCurrentImage(getImage());

        //check if the invicible time is over
        if(isInvicible && Time() > timeDeath )
        {
            canAttack = true;
            isInvicible = false;
        }

        if(Time() < timeDeath)
        {
            DrawImage(imgBlood);
        }

        calculateColliderPos();
        gun.actuBullet(Right * 32, Left * 32, matMap, movementDirection);

        if(isAttacking)
        {
            //update weapon pos according to direction
            //redo the logic, we already know which direction is, no need to send it
            switch(movementDirection)
            {
                case LEFT:
                    weapon.Draw(Left * 32, movementDirection);
                    break;

                case DOWN:
                    weapon.Draw(Bottom * 32, movementDirection);
                    break;

                case RIGHT:
                    weapon.Draw(Right * 32, movementDirection);
                    break;

                case TOP:
                    weapon.Draw(Top * 32, movementDirection);
                    break;
            }

            //if he attacks check if the weapon touches other players
            for(unsigned int i = 0; i < perso->size(); i++)
            {
                //if it collides with someone
                if(ImgGetCollision(weapon.getCurrentImage(), perso->at(i).getCurrentImage()))
                {
                    //if it's not the player itself, and if the player is not attacking a invicble player
                    if(perso->at(i).numPlayer != numPlayer && !perso->at(i).isInvicible)
                    {
                        Scored();
                        perso->at(i).Die(getRandomSpawnPoint(*respawnPoint, respawnPointTimer));
                        perso->at(i).gun.reload();
                        nbKill++;
                    }
                }
            }

            //the anim time is finished the player can attack again
            if(Time() >= animTimeWeapon)
            {
                isAttacking = false;
            }
        }
            for(unsigned int i = 0; i < perso->size(); i++)
            {
                //checking for bullets collision
                for (int j = 0; j < 4; j++)
                {
                    if(ImgGetCollision(gun.bullet[j], perso->at(i).getCurrentImage()))
                    {
                        Scored();
                        perso->at(i).Die(getRandomSpawnPoint(*respawnPoint, respawnPointTimer));
                        nbKill++;
                    }
                }
            }

        movementVector = CreateVec2(0.0f, 0.0f);
    }

    void Die(Vec2 spawnPoint)
    {
        if(score - 1 < 0)
            score = 0;
        else
            score--;

        canAttack = false;
        isInvicible = true;

        timeDeath = Time() + 1000;

       ImgSetPosition(imgBlood, GetPosition());
       setPosition(spawnPoint);

       nbDeath++;
    }

    void Die()
    {
        if(score - 1 < 0)
            score = 0;
        else
            score--;

        canAttack = false;
        isInvicible = true;

        timeDeath = Time() + 500;

       ImgSetPosition(imgBlood, GetPosition());
       setPosition(getRandomSpawnPoint(*respawnPoint, respawnPointTimer));

       nbDeath++;

    }

    void Attack()
    {

        if(!isAttacking && canAttack)
        {
            animTimeWeapon = Time() + animTimeWeaponWaiting;
            isAttacking = true;
            weapon.Attack();
        }

        /*
            attack(0.3sec) wait(0.2 sec)
        */
    }

    void Shoot()
    {
        gun.Bullet(movementDirection, Right * 32, Left * 32);
    }

    void Free()
    {
        for (int i=0; i<7; i++)
        {
            FreeImage(imgRun[i]);
        }

        FreeImage(imgJump);
        FreeImage(imgFall);
        FreeImage(imgIdle);
        FreeImage(imgBlood);

        //weapon.Free();
    }

} Perso;

#endif // PERSO_H_INCLUDED


