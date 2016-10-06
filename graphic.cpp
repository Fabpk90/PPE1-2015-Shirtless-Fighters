#include "graphic.h"
#include <string>
#include <vector>
#include <assert.h>
#include <iostream>

//#define PRINT_DEBUG
#define M_KEY_SIZE 500
#define M_FLASH_CURSOR 1000 //nombre de milliseconde pour fair clignoter le curseur d'écriture
#define M_PI 3.1415926535897932384626433832795

struct ImageLoaded
{
    GLuint id;
    std::string name;
    Vec2 size;
};
struct MyImage
{
        Vec2 position;
        float rotate;
        Vec2 scale;
        Color color;
        struct ImageLoaded* imgLoaded;
        GLuint fragShader, vertShader;
};
struct MyText
{
    std::string text;
    Vec2 position;
    float size;
    Color color;
    bool isAlterable;
    bool drawing;
    int nbMaxChar;
};
struct Stick
{
    std::vector<bool> button;
    std::vector<bool> buttonOld;
    std::vector<int> hat;
    std::vector<float> axis;
    SDL_Joystick* joystick;
};

GLuint loadTexture(const char * filename, Vec2 *size,bool useMipMap = true);//charge la texture avec openGL
GLuint loadTexture(SDL_Surface* picture_surface, Vec2 *size,bool useMipMap = true);//charge la texture avec openGL
struct ImageLoaded* LoadLetter(char letter);
SDL_Surface * flipSurface(SDL_Surface * surface);
struct ImageLoaded* GetImageId(std::string name);//permet de recupere l'id openGL d'une image (evite les chargement multiple)
bool isLoaded(struct MyImage* img);
bool isLoaded(struct ImageLoaded* tex);
bool isLoaded(struct MyText* text);
bool fileExist(std::string name);
void TextSetCharactere(Text txt, SDLKey c);

///variables globale
Vec2 M_winSize;
SDL_Event M_event;
int M_timeAfterRefresh;
std::vector<struct ImageLoaded*> M_tabImageLoaded;
std::vector<struct MyImage*> M_tabImage;
bool M_key[M_KEY_SIZE];
bool M_keyOld[M_KEY_SIZE];
Vec2 M_mousePosition;
bool M_mouseClickLeft;
bool M_mouseOldClickLeft;
bool M_mouseClickRight;
bool M_mouseOldClickRight;
TTF_Font* M_font;
std::vector<struct Stick*> M_stick;
std::vector<struct MyText*> M_tabText;
int M_idTextFocus;
struct ImageLoaded* M_tabCar[256];//tableau de caractere pour les ecriture
int M_timeCursor;//ce temps permet de fair clignoter le cursor
MyText* M_txtFPS;
int M_timerFPS;
int M_nbFrame;
int M_timeCPUFree;


///implementation des fonctions
Vec2 CreateVec2(float x, float y)
{
    Vec2 v = {x,y};
    return v;
}

float Distance(Vec2 point1, Vec2 point2)
{
    return sqrt((point1.x-point2.x)*(point1.x-point2.x) + (point1.y-point2.y)*(point1.y-point2.y));
}

Color CreateColor(int red, int green, int blue)
{
    return CreateColor(red, green, blue, 255);
}

Color CreateColor(int red, int green, int blue, int alpha)
{
    Color c = {(char)(red%256), (char)(green%256), (char)(blue%256), (char)(alpha%256)};
    return c;
}

struct ImageLoaded* GetImageId(std::string name)
{
    //on cherche l'image dans les image déjà charger
    for(int i=0; i < (int)M_tabImageLoaded.size(); i++)
    {
        if (strcmp(name.c_str(), M_tabImageLoaded[i]->name.c_str())==0)
            return M_tabImageLoaded[i];
    }
    //si on a pas trouver l'image dans les image déjà charger alors on l'ajoute
    struct ImageLoaded* img = new struct ImageLoaded;
    img->id = loadTexture(name.c_str(), &img->size);
    img->name = name;
    M_tabImageLoaded.push_back(img);
    return img;
}

Image CreateImage(char *name)
{
    if (!fileExist(name))
    {
        std::cerr << "impossible de charge le fichier : " << name;
        assert(!"vous tenter de charge une image qui n'éxiste pas. allez voir le fichier ereur");
    }
    struct MyImage* img = new struct MyImage;
    img->imgLoaded = GetImageId(name);
    img->position = CreateVec2(0.0f,0.0f);
    img->rotate = 0.0f;
    img->scale = CreateVec2(1.0f,1.0f);
    img->color = CreateColor(255,255,255,255);
    img->fragShader = 0;
    img->vertShader = 0;
    M_tabImage.push_back(img);
    return img;
}

void FreeImage(Image img)
{
    //si l'image n'est pas en mémore on sort un énorme assert
    if (!isLoaded(img))
    {
        //std::cerr << "file : " <<  __FILE__ << std::endl << "line : " <<  __LINE__;
        assert(!"Impossible de libéré une image qui n'éxiste pas");
        return ;
    }

#ifdef PRINT_DEBUG
    printf(" M_tabImageLoaded = ");
    for (int i=0; i < (int) M_tabImageLoaded.size(); i++)
        printf("%d,", M_tabImageLoaded[i]->id);
    printf("\n");
    printf(" M_tabImage.size = %d\n", M_tabImage.size());
#endif // PRINT_DEBUG
    //on compte le nombre d'image charge qui utilise la même texture que l'image en parametre
    int useTex = 0;
    int idImg = -1;
    //on libère les shader
    ///if (img->fragShader) glDeleteShader(img->fragShader);
    ///if (img->vertShader) glDeleteShader(img->vertShader);

    for (int i=0; i < (int)M_tabImage.size(); i++)
    {
        if (M_tabImage[i]->imgLoaded == img->imgLoaded)
            useTex++;
        if (M_tabImage[i] == img)
            idImg = i;
    }
    //si il y a qu'une seul image qui utilise cette texture alors on la libère
    if (useTex == 1)
    {
        glDeleteTextures(1,&img->imgLoaded->id);
        //on vire la texture du tableau des texture
        for (int i=0; i < (int)M_tabImageLoaded.size(); i++)
        {
            if (img->imgLoaded == M_tabImageLoaded[i])
            {
                M_tabImageLoaded.erase(M_tabImageLoaded.begin()+i);
                i--;
            }
        }
        delete img->imgLoaded;
    }
    if (idImg >= 0)
    {
        M_tabImage.erase(M_tabImage.begin()+idImg);
        delete img;
    }
#ifdef PRINT_DEBUG
    //on fait des impression pour verifier les état de variable
    printf(" id = %d\n", idImg);
    printf(" useTex = %d\n", useTex);
    printf(" M_tabImageLoaded = ");
    for (int i=0; i < (int) M_tabImageLoaded.size(); i++)
        printf("%d,", M_tabImageLoaded[i]->id);
    printf("\n");
    printf(" M_tabImage.size = %d\n\n\n", M_tabImage.size());
#endif // PRINT_DEBUG
}

void ImgSetPosition(Image img, Vec2 pos)
{
    img->position = pos;
}

void ImgSetPosition(Image img, int x, int y)
{
    img->position.x = x;
    img->position.y = y;
}

void ImgSetRotation(Image img, float angle)
{
    img->rotate = angle;
}

void ImgSetScale(Image img, float scale)
{
    img->scale.x = img->scale.y = scale;
}

void ImgSetScale(Image img, float scaleX, float scaleY)
{
    img->scale.x = scaleX;
    img->scale.y = scaleY;
}

void ImgSetScale(Image img, Vec2 scale)
{
    img->scale.x = scale.x;
    img->scale.y = scale.y;
}

void ImgSetColor(Image img, Color color)
{
    img->color = color;
}

Vec2 ImgGetPosition(Image img)
{
    return img->position;
}

float ImgGetRotation(Image img)
{
    return img->rotate;
}

Vec2 ImgGetScale(Image img)
{
    return img->scale;
}

Color ImgGetColor(Image img)
{
    return img->color;
}

Vec2 ImgGetSize(Image img)
{
    return img->imgLoaded->size;
}

Vec2 ImgGetRealSize(Image img)
{
    return img->imgLoaded->size*img->scale;
}

/**
convertie un point dans un repère donnée
*/
Vec2 PointInSpace(Vec2 point, Vec2 pos, Vec2 size, float rot, Vec2 scale)
{
    ///on calcul le point P2 qui est le point point mais translater pour etre dans le repaire du rectangle
    Vec2 P2 = point-(pos+(size/2));

    ///on calcul le point P3 qui es le point P2 mais tournée pour etre totalement dans le repère du rectangle
    float dP2 = Distance(CreateVec2(0,0), P2);//taille du vecteur P2
    if(dP2 == 0) return CreateVec2(0,0);
    float rotP2 = acos(P2.x/dP2); //attentione acos est en radian
    if(P2.y<0)
        rotP2 = 2*M_PI-rotP2;
    float rotP3 = rotP2-rot/180*M_PI;
    Vec2 P3 = CreateVec2(cos(rotP3)*dP2, sin(rotP3)*dP2);
    return P3/scale;

}

/**
permet de savoir si un point est a l'interieur d'un rectangle
prend en parametre la position du point ainsi que position du point en bas a gauche du rectangle sa taille et sa rotation
*/
bool PointInRect(Vec2 point, Vec2 pos, Vec2 size, float rot, Vec2 scale)
{
    ///on convertie le point dans l'espace du cube
    Vec2 P3 = PointInSpace(point, pos, size, rot, scale);

    ///on regarde su P3 est dans le rectangle
    if (P3.x > -size.x/2 &&
        P3.x < size.x/2 &&
        P3.y > -size.y/2 &&
        P3.y < size.y/2)
            return true;
    return false;
}

/**
la position du point voulu dans le repar normale
prend en parametre l'image est le numero du point
1   2

0   3
*/
Vec2 ImgGetPoint(Image img, int numPoint)
{
    Vec2 P;//point que l'on doit convertir
    if (numPoint==0)
        P = CreateVec2(-1,-1)*(img->imgLoaded->size/2)*img->scale;
    else if (numPoint==1)
        P = CreateVec2(-1,1)*(img->imgLoaded->size/2)*img->scale;
    else if (numPoint==2)
        P = CreateVec2(1,1)*(img->imgLoaded->size/2)*img->scale;
    else
        P = CreateVec2(1,-1)*(img->imgLoaded->size/2)*img->scale;
    float dP = Distance(CreateVec2(0,0), P);//taille du vecteur P
    if(dP == 0) return P+(img->position+(img->imgLoaded->size/2));
    float rotP = acos(P.x/dP);
    if (numPoint == 0 || numPoint == 3)
        rotP = 2*M_PI-rotP;
    float rotP2 = rotP+img->rotate/180*M_PI;
    //printf("rotP = %f, rotP2 = %f\n", rotP*180/M_PI, rotP2*180/M_PI);
    Vec2 P2 = CreateVec2(cos(rotP2)*dP, sin(rotP2)*dP);
    //printf("dP2 = %f, dp = %f\n", Distance(CreateVec2(0,0), P2), Distance(CreateVec2(0,0), P));
    return P2+(img->position+(img->imgLoaded->size/2));
}

bool IntersectionSegments(Vec2 P1, Vec2 P2, Vec2 P3, Vec2 P4)
{
    //si les droite sont vertical alors c'est facile (cas a par pour evité les division par 0
    if (P1.x==P2.x)
    {
        if (P3.x==P4.x)
            return false;
        float A2 = (P3.y-P4.y)/(P3.x-P4.x);
        float B2 = P3.y-A2*P3.x;
        float y = A2*P1.x+B2;
        if (P1.x<=fmax(P3.x,P4.x) &&
            P1.x>=fmin(P3.x,P4.x) &&
            y<=fmax(P1.y,P2.y) &&
            y>=fmin(P1.y,P2.y) )
            return true;
        return false;
    }
    if (P3.x==P4.x)
    {
        float A1 = (P1.y-P2.y)/(P1.x-P2.x);
        float B1 = P1.y-A1*P1.x;
        float y = A1*P3.x+B1;
        if (P3.x<=fmax(P1.x,P2.x) &&
            P3.x>=fmin(P1.x,P2.x) &&
            y<=fmax(P3.y,P4.y) &&
            y>=fmin(P3.y,P4.y) )
            return true;
        return false;
    }
    //si aucune des deux droite n'est vertical
    float A1 = (P1.y-P2.y)/(P1.x-P2.x);
    float B1 = P1.y-A1*P1.x;
    float A2 = (P3.y-P4.y)/(P3.x-P4.x);
    float B2 = P3.y-A2*P3.x;
    if (A1==A2) //si les deux droite sont parallele
        return false;
    float x = (B2 - B1) / (A1 - A2);//le point d'intersection des duex droite
    if (x<=fmax(P1.x,P2.x) &&
        x>=fmin(P1.x,P2.x) &&
        x<=fmax(P3.x,P4.x) &&
        x>=fmin(P3.x,P4.x) )
            return true;
    return false;

}

bool ImgGetCollision(Image img, Image img2)
{
    Vec2 PImg[4], PImg2[4];
    for (int i=0; i<4; i++)
    {
        PImg[i] = ImgGetPoint(img, i);
        PImg2[i] = ImgGetPoint(img2, i);
    }
    //en premier on regarde si un des point de chaque forme est a l'interieur de l'autre (au cas ou une forme englobe l'autre)
    for (int i=0; i<4; i++)
    {
        if (PointInRect(PImg[i], img2->position, img2->imgLoaded->size, img2->rotate, img2->scale) ||
            PointInRect(PImg2[i], img->position, img->imgLoaded->size, img->rotate, img->scale) )
            return true;
    }


    //on copare les 8 segment et on regarde leur point d'intercection
    for (int i=0; i<4; i++)
    {
        for (int j=0; j<4; j++)
        {
            if(IntersectionSegments(PImg[i], PImg[(i+1)%4], PImg2[j], PImg2[(j+1)%4]))
                return true;
        }
    }

    return false;
}

bool PointInImgLite(Vec2 point, Image img)
{

    ///on regarde su P3 est dans le rectangle
    if (point.x > img->position.x-img->imgLoaded->size.x*(img->scale.x-1.0f)/2.0f &&
        point.x < img->position.x+img->imgLoaded->size.x*(img->scale.x+1.0f)/2.0f &&
        point.y > img->position.y-img->imgLoaded->size.y*(img->scale.y-1.0f)/2.0f &&
        point.y < img->position.y+img->imgLoaded->size.y*(img->scale.y+1.0f)/2.0f)
            return true;
    return false;
}

bool ImgGetCollisionLite(Image img, Image img2)
{
    if (img2->position.x+img2->imgLoaded->size.x*(img2->scale.x+1.0f)/2.0f < img->position.x-img->imgLoaded->size.x*(img->scale.x-1.0f)/2.0f ||
        img2->position.x+img2->imgLoaded->size.x*(img2->scale.x-1.0f)/2.0f > img->position.x+img->imgLoaded->size.x*(img->scale.x+1.0f)/2.0f ||
        img2->position.y+img2->imgLoaded->size.y*(img2->scale.y+1.0f)/2.0f < img->position.y-img->imgLoaded->size.y*(img->scale.y-1.0f)/2.0f ||
        img2->position.y+img2->imgLoaded->size.y*(img2->scale.y-1.0f)/2.0f > img->position.y+img->imgLoaded->size.y*(img->scale.y+1.0f)/2.0f )
        return false;

    return true;
}

Text CreateText(const char* text, Color color, int size)
{
    struct MyText* txt = new struct MyText;
    txt->color = color;
    txt->position = {0,0};
    txt->size = size;
    txt->text = text;
    txt->isAlterable = false;
    txt->drawing = false;
    txt->nbMaxChar = -1;
    M_tabText.push_back(txt);
    return txt;
}

void FreeText(Text text)
{
    for (int i=0; i<(int)M_tabText.size();i++)
    {
        if (text == M_tabText[i])
        {
            M_tabText.erase(M_tabText.begin()+i);
            i--;
        }
    }
    delete text;
}

void TextSetText(Text txt, const char* text, ...)
{
    char dest[1024];
    va_list argptr;
    va_start(argptr, text);
    vsprintf(dest, text, argptr);
    va_end(argptr);
    txt->text = dest;
}

void TextSetPosition(Text txt, Vec2 position)
{
    txt->position = position;
}

void TextSetColor(Text txt, Color color)
{
    txt->color = color;
}

void TextSetSize(Text txt, int size)
{
    txt->size = size;
}

const char* TextGetText(Text txt)
{
    return txt->text.c_str();
}

Vec2 TextGetPosition(Text txt)
{
    return txt->position;
}

Color TextGetColor(Text txt)
{
    return txt->color;
}

int TextGetSize(Text txt)
{
    return txt->size;
}

void TextSetAlterable(Text txt, bool alterable)
{
    txt->isAlterable = alterable;
}

bool TextGetAlterable(Text txt)
{
    return txt->isAlterable;
}

void TextSetFocus(Text txt)
{
    for(int i =0; i<(int)M_tabText.size(); i++)
    {
        if (M_tabText[i] == txt)
        {
            M_idTextFocus = i;
        }
    }
}

void TextShootFocus()
{
    M_idTextFocus = -1;
}

bool TextGetFocus(Text txt)
{
    if(M_idTextFocus>=0 && M_idTextFocus<(int)M_tabText.size() && M_tabText[M_idTextFocus] == txt)
    {
        return true;
    }
    return false;
}

void TextSetLimitEditable(Text txt, unsigned int nbMaxChar)
{
    if (txt)
    {
        txt->nbMaxChar = nbMaxChar;
        if (txt->text.size() > nbMaxChar)
            txt->text = txt->text.substr(0, nbMaxChar);
    }
}

void CreateWindow(int width, int height, char* name, bool fullScreen)
{
    M_idTextFocus = -1;
    M_nbFrame = 0;
    M_timeCPUFree = 0;
    //on initialise le tableau key
    for (int i=0; i < M_KEY_SIZE; i++)
    {
        M_key[i] = false;
        M_keyOld[i] = false;
    }
    //on initialize la SDL
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK);
    //initialisation des joystick
    SDL_JoystickEventState(SDL_ENABLE);
    for(int i=0; i<SDL_NumJoysticks(); i++)
    {
        struct Stick* s = new struct Stick;
        s->joystick = SDL_JoystickOpen(i);
        int nbButton = SDL_JoystickNumButtons(s->joystick);
        for(int i=0; i<nbButton; i++)
        {
            s->button.push_back(false);
            s->buttonOld.push_back(false);
        }
        int nbAxis = SDL_JoystickNumAxes(s->joystick);
        for(int i=0; i<nbAxis; i++)
            s->axis.push_back(0);
        int nbHat = SDL_JoystickNumHats(s->joystick);
        for(int i=0; i<nbHat; i++)
            s->hat.push_back(SDL_HAT_CENTERED);
        M_stick.push_back(s);
    }
    //on défini un titre a la fenetre
    SDL_WM_SetCaption(name,NULL);
    //on ouvre une fenetre en 32 bits et en mode openGL
    SDL_SetVideoMode(width, height, 32, (fullScreen?SDL_OPENGL|SDL_FULLSCREEN:SDL_OPENGL));
    SDL_EnableUNICODE(1);
    //on sauvegarde les dimention de la fenetre
    M_winSize.x = width;
    M_winSize.y = height;
    //on initialise les nombre aléatoire
    srand(time(NULL));
    M_timeAfterRefresh = 0;
    //on désactive la lumiere
    glDisable(GL_LIGHTING);
    //on parametr l'affichage de l'alpha
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //on charge la matrice de projection
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity( );
    gluOrtho2D(0,width,0,height);
    //initalisation de sdl TTF
    if(TTF_Init() == -1)
    {
        std::cerr << "Erreur d'initialisation de TTF_Init :" << TTF_GetError() << std::endl;
        exit(EXIT_FAILURE);
    }
    M_font = TTF_OpenFont("arial.ttf", 100);
    for (int i=0; i < 256; i++)
        M_tabCar[i] = LoadLetter((char)i);
    //init des variablespour afficher des FPS
    M_txtFPS = CreateText("", CreateColor(255,255,255), 25);
    M_timerFPS = 0;
}

int GetSizeTextX(Text txt)
{
    int size = 0;
    for (int i=0; i<(int)txt->text.size(); i++)
    {
        int car = (int)(unsigned char)txt->text[i];
        size += M_tabCar[car]->size.x*txt->size/M_tabCar[car]->size.y;
    }
    return size;
}

Vec2 WindowGetSize()
{
    return M_winSize;
}

void RefreshScreen(bool printFPS)
{
    //on imprime les FPS
    if (SDL_GetTicks() - M_timerFPS >= 1000)
    {
        M_timerFPS = SDL_GetTicks();
        char txt[100];
        sprintf(txt, "FPS : %d  CPU Free : %d", M_nbFrame, M_timeCPUFree);
        TextSetText(M_txtFPS, txt);
        M_nbFrame = 0;
        M_timeCPUFree = 0;
    }
    if(printFPS)
        DrawText(M_txtFPS);
    M_nbFrame++;

    //on fini de dessiner la scene
    glFlush();
    //on switch les buffer
    SDL_GL_SwapBuffers();

    //on attand pour ne rafrechir que 60 fois par seconde
    int time = fmax(1000/60.0 - (SDL_GetTicks() - M_timeAfterRefresh), 0.0f);
    M_timeCPUFree += time;
    SDL_Delay(time);
    M_timeAfterRefresh = SDL_GetTicks();
    //on clear le nouveau buffer
    glClear(GL_COLOR_BUFFER_BIT);
    //on recupere les evenement
    for (int i=0; i < M_KEY_SIZE; i++)
        M_keyOld[i] = M_key[i];
    for (int i= 0; i < (int)M_stick.size(); i++)
    {
        for (int j=0; j < (int)M_stick[i]->button.size(); j++)
            M_stick[i]->button[j] = M_stick[i]->buttonOld[j];
    }
    M_mouseOldClickLeft = M_mouseClickLeft;
    M_mouseOldClickRight = M_mouseClickRight;
    while(SDL_PollEvent(&M_event))
    {
        switch(M_event.type)
        {
            case SDL_QUIT:
            {
                SDL_Quit();
                exit(EXIT_SUCCESS);
                break;
            }
            case SDL_KEYDOWN:
            {
                if(M_idTextFocus>=0 && M_idTextFocus<(int)M_tabText.size() && M_tabText[M_idTextFocus]->isAlterable)
                {
                    TextSetCharactere(M_tabText[M_idTextFocus], M_event.key.keysym.sym);
                }
                M_key[M_event.key.keysym.sym] = true;
                break;
            }
            case SDL_KEYUP:
            {
                M_key[M_event.key.keysym.sym] = false;
                break;
            }
            case SDL_MOUSEMOTION:
            {
                M_mousePosition.x = M_event.motion.x;
                M_mousePosition.y = M_winSize.y-M_event.motion.y;
                break;
            }
            case SDL_MOUSEBUTTONDOWN:
            {
                if (M_event.button.button == SDL_BUTTON_LEFT)
                    M_mouseClickLeft = true;
                else if (M_event.button.button == SDL_BUTTON_RIGHT)
                    M_mouseClickRight = true;
                //si on a clicer sur un text alors on lui donne le focus
                for (int i=0; i<(int)M_tabText.size(); i++)
                {
                    if (M_mousePosition.y > M_tabText[i]->position.y &&
                        M_mousePosition.y < M_tabText[i]->position.y + M_tabText[i]->size &&
                        M_mousePosition.x > M_tabText[i]->position.x &&
                        M_mousePosition.x < M_tabText[i]->position.x + GetSizeTextX(M_tabText[i]) + 50 &&
                        M_tabText[i]->drawing &&
                        M_tabText[i]->isAlterable)
                    {
                        M_idTextFocus = i;
                    }
                    //printf("clis de la souris (%.0f, %.0f)", M_mousePosition.x, M_mousePosition.y);
                }

                break;
            }
            case SDL_MOUSEBUTTONUP:
            {
                if (M_event.button.button == SDL_BUTTON_LEFT)
                    M_mouseClickLeft = false;
                else if (M_event.button.button == SDL_BUTTON_RIGHT)
                    M_mouseClickRight = false;
                break;
            }
            case SDL_JOYBUTTONDOWN:
            {
                M_stick[M_event.jbutton.which]->button[M_event.jbutton.button] = true;
                break;
            }
            case SDL_JOYBUTTONUP:
            {
                M_stick[M_event.jbutton.which]->button[M_event.jbutton.button] = false;
                break;
            }
            case SDL_JOYAXISMOTION:
            {
                M_stick[M_event.jaxis.which]->axis[M_event.jaxis.axis] = M_event.jaxis.value/32768.0;
                break;
            }
            case SDL_JOYHATMOTION:
            {
                M_stick[M_event.jhat.which]->hat[M_event.jhat.hat] = M_event.jhat.value;
                break;
            }
        }
    }
    //on dit que tout les text ne sont pas afficher
    for (int i=0; i<(int)M_tabText.size(); i++)
        M_tabText[i]->drawing = false;
    //on augment le temps du curseur
    M_timeCursor = (M_timeCursor + 1000/60)%M_FLASH_CURSOR;
}

void DrawTriangles(Vec2 p1, Vec2 p2, Vec2 p3, Color c)
{
    //on affiche l'image
    glBegin(GL_TRIANGLES);
        glColor4ub(c.r,c.g,c.b,c.a);
        glVertex2d(p1.x,p1.y);
        glVertex2d(p2.x,p2.y);
        glVertex2d(p3.x,p3.y);
    glEnd();
}

void DrawRect(Vec2 p0, Vec2 p1, Vec2 p2, Vec2 p3, Color c)
{
    //on affiche l'image
    glBegin(GL_QUADS);
        glColor4ub(c.r,c.g,c.b,c.a);
        glVertex2d(p3.x,p3.y);
        glVertex2d(p2.x,p2.y);
        glVertex2d(p1.x,p1.y);
        glVertex2d(p0.x,p0.y);
    glEnd();
}

void DrawLine(Vec2 p0, Vec2 p1, Color col, float thickness)
{
    glLineWidth(thickness);
    glBegin(GL_LINE_STRIP);
        glColor4ub(col.r,col.g,col.b,col.a);
        glVertex2d(p0.x,p0.y);
        glVertex2d(p1.x,p1.y);
    glEnd();
}

void DrawImage(Image img)
{
    //si l'image n'est pas en mémore on sort un énorme assert
    if (!isLoaded(img))
    {
        assert(!"Impossible de d'afficher une image qui nest pas charger ou alors qui a été libéré");
        return ;
    }
    //on applique les transformation de la matrice
    glPushMatrix();
    glTranslatef(img->imgLoaded->size.x/2+img->position.x,
                 img->imgLoaded->size.y/2+img->position.y, 0.0f);
    glScalef(img->scale.x, img->scale.y, 1.0f);
    glRotatef(img->rotate, 0,0,1);
    //on active les texture openGL et on applique la bonne texture
    glEnable(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, img->imgLoaded->id);
    //on affiche l'image
    glBegin(GL_QUADS);
        glColor4ub(img->color.r,img->color.g,img->color.b, img->color.a);
        glTexCoord2d(0,1);  glVertex2d(-img->imgLoaded->size.x/2,
                                       img->imgLoaded->size.y/2);
        glTexCoord2d(0,0);  glVertex2d(-img->imgLoaded->size.x/2,
                                       -img->imgLoaded->size.y/2);
        glTexCoord2d(1,0);  glVertex2d(img->imgLoaded->size.x/2,
                                       -img->imgLoaded->size.y/2);
        glTexCoord2d(1,1);  glVertex2d(img->imgLoaded->size.x/2,
                                       img->imgLoaded->size.y/2);
    glEnd();
    //on desactive les texture
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}

void DrawText(Text txt)
{
    //si l'image n'est pas en mémore on sort un énorme assert
    if (!isLoaded(txt))
    {
        assert(!"Impossible de d'afficher un text qui nest pas charger ou alors qui a été libéré");
        return ;
    }
    txt->drawing = true;
    //on active les texture openGL et on applique la bonne texture
    glEnable(GL_TEXTURE_2D);
    int cursor = 0;
    for (int i=0; i<(int)txt->text.size(); i++)
    {
        int car = (int)(unsigned char)txt->text[i];
        glBindTexture(GL_TEXTURE_2D, M_tabCar[car]->id);
        Vec2 size = {M_tabCar[car]->size.x*txt->size/M_tabCar[car]->size.y, txt->size};
        //on affiche l'image
        glBegin(GL_QUADS);
            glColor4ub(txt->color.r,txt->color.g,txt->color.b, txt->color.a);
            glTexCoord2d(0,1);  glVertex2d(txt->position.x+cursor,
                                           txt->position.y+size.y);
            glTexCoord2d(0,0);  glVertex2d(txt->position.x+cursor,
                                           txt->position.y);
            glTexCoord2d(1,0);  glVertex2d(txt->position.x+cursor+size.x,
                                           txt->position.y);
            glTexCoord2d(1,1);  glVertex2d(txt->position.x+cursor+size.x,
                                           txt->position.y+size.y);
        glEnd();
        cursor+=size.x;
    }
    //on affiche le curseur si le text est focus

    if(M_idTextFocus>=0 && M_idTextFocus<(int)M_tabText.size() && M_tabText[M_idTextFocus] == txt && M_timeCursor > M_FLASH_CURSOR/2.0 && txt->isAlterable)
    {
        glBindTexture(GL_TEXTURE_2D, M_tabCar['|']->id);
        Vec2 size = {M_tabCar['|']->size.x*txt->size/M_tabCar['|']->size.y, txt->size};
        //on affiche l'image
        glBegin(GL_QUADS);
            glColor4ub(txt->color.r,txt->color.g,txt->color.b, txt->color.a);
            glTexCoord2d(0,1);  glVertex2d(txt->position.x+cursor,
                                           txt->position.y+size.y);
            glTexCoord2d(0,0);  glVertex2d(txt->position.x+cursor,
                                           txt->position.y);
            glTexCoord2d(1,0);  glVertex2d(txt->position.x+cursor+size.x,
                                           txt->position.y);
            glTexCoord2d(1,1);  glVertex2d(txt->position.x+cursor+size.x,
                                           txt->position.y+size.y);
        glEnd();
        cursor+=size.x;
    }

    //on desactive les texture
    glDisable(GL_TEXTURE_2D);
}

bool KeyGet(int key)
{
    if (key>=M_KEY_SIZE || key < 0)
        return false;
    return M_key[key];
}

bool KeyGetOneShot(int key)
{
    if (key>=M_KEY_SIZE || key < 0)
        return false;
    if (M_key[key] && !M_keyOld[key])
        return true;
    return false;
}

Vec2 MouseGetPosition()
{
    return M_mousePosition;
}

bool MouseGetClickLeft()
{
    return M_mouseClickLeft;
}

bool MouseGetClickLeftOneShot()
{
    if (M_mouseClickLeft && !M_mouseOldClickLeft)
        return true;
    return false;
}

bool MouseGetClickRight()
{
    return M_mouseClickRight;
}

bool MouseGetClickRightOneShot()
{
    if (M_mouseClickRight && !M_mouseOldClickRight)
        return true;
    return false;
}

int StickGetNbConnect()
{
    return (int)M_stick.size();
}

const char * StickGetName(int numStick)
{
    return SDL_JoystickName(numStick);
}

bool StickGetClick(int numStick, int numButton)
{
    if (numStick< 0 || numStick>=(int)M_stick.size())
        return false;
    if (numButton<0 || numButton>=(int)M_stick[numStick]->button.size())
        return false;
    return M_stick[numStick]->button[numButton];
}

bool StickGetClickOneShot(int numStick, int numButton)
{
    if (numStick< 0 || numStick>=(int)M_stick.size())
        return false;
    if (numButton<0 || numButton>=(int)M_stick[numStick]->button.size())
        return false;
    if(M_stick[numStick]->button[numButton] && !M_stick[numStick]->buttonOld[numButton])
        return true;
    return false;
}

int StickGetStateHat(int numStick, int numHat)
{
    if (numStick< 0 || numStick>=(int)M_stick.size())
        return false;
    if (numHat<0 || numHat>=(int)M_stick[numStick]->hat.size())
        return false;
    return M_stick[numStick]->hat[numHat];
}

float StickGetAxis(int numStick, int numAxis)
{
    if (numStick< 0 || numStick>=(int)M_stick.size())
        return 0.0f;
    if (numAxis<0 || numAxis>=(int)M_stick[numStick]->axis.size())
        return 0.0f;
    return M_stick[numStick]->axis[numAxis];
}

int Rand(int min, int max)
{
    return rand()%(max-min)+min;
}

float fRand()
{
    return ((float) rand())/RAND_MAX;
}

struct ImageLoaded* LoadLetter(char letter)
{
    std::string str(1, letter);
    //std::cout << (int)letter << " = " << str << std::endl;
    SDL_Surface* surface = TTF_RenderText_Solid(M_font, str.c_str(), {255,255,255});
    struct ImageLoaded* img = new struct ImageLoaded;
    img->id = loadTexture(surface, &img->size);
    img->name = str;
    return img;
}

GLuint loadTexture(const char * filename, Vec2 *size,bool useMipMap)
{
    GLuint glID;
    SDL_Surface * picture_surface = NULL;
    SDL_Surface *gl_surface = NULL;
    SDL_Surface * gl_fliped_surface = NULL;
    Uint32 rmask, gmask, bmask, amask;

    picture_surface = IMG_Load(filename);
    if (picture_surface == NULL)
        return 0;

    (*size).x = picture_surface->w;
    (*size).y = picture_surface->h;


#if SDL_BYTEORDER == SDL_BIG_ENDIAN

    rmask = 0xff000000;
    gmask = 0x00ff0000;
    bmask = 0x0000ff00;
    amask = 0x000000ff;
#else

    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = 0xff000000;
#endif

    SDL_PixelFormat format = *(picture_surface->format);
    format.BitsPerPixel = 32;
    format.BytesPerPixel = 4;
    format.Rmask = rmask;
    format.Gmask = gmask;
    format.Bmask = bmask;
    format.Amask = amask;

    gl_surface = SDL_ConvertSurface(picture_surface,&format,SDL_SWSURFACE);

    gl_fliped_surface = flipSurface(gl_surface);

    glGenTextures(1, &glID);

    glBindTexture(GL_TEXTURE_2D, glID);


    if (useMipMap)
    {

        gluBuild2DMipmaps(GL_TEXTURE_2D, 4, gl_fliped_surface->w,
                          gl_fliped_surface->h, GL_RGBA,GL_UNSIGNED_BYTE,
                          gl_fliped_surface->pixels);

        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,
                        GL_LINEAR_MIPMAP_LINEAR);

    }
    else
    {
        glTexImage2D(GL_TEXTURE_2D, 0, 4, gl_fliped_surface->w,
                     gl_fliped_surface->h, 0, GL_RGBA,GL_UNSIGNED_BYTE,
                     gl_fliped_surface->pixels);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    }
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);


    SDL_FreeSurface(gl_fliped_surface);
    SDL_FreeSurface(gl_surface);
    SDL_FreeSurface(picture_surface);

    return glID;
}

GLuint loadTexture(SDL_Surface* picture_surface, Vec2 *size,bool useMipMap)
{
    GLuint glID;
    SDL_Surface *gl_surface = NULL;
    SDL_Surface * gl_fliped_surface = NULL;
    Uint32 rmask, gmask, bmask, amask;

    if (picture_surface == NULL)
        return 0;

    (*size).x = picture_surface->w;
    (*size).y = picture_surface->h;


#if SDL_BYTEORDER == SDL_BIG_ENDIAN

    rmask = 0xff000000;
    gmask = 0x00ff0000;
    bmask = 0x0000ff00;
    amask = 0x000000ff;
#else

    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = 0xff000000;
#endif

    SDL_PixelFormat format = *(picture_surface->format);
    format.BitsPerPixel = 32;
    format.BytesPerPixel = 4;
    format.Rmask = rmask;
    format.Gmask = gmask;
    format.Bmask = bmask;
    format.Amask = amask;

    gl_surface = SDL_ConvertSurface(picture_surface,&format,SDL_SWSURFACE);

    gl_fliped_surface = flipSurface(gl_surface);

    glGenTextures(1, &glID);

    glBindTexture(GL_TEXTURE_2D, glID);


    if (useMipMap)
    {

        gluBuild2DMipmaps(GL_TEXTURE_2D, 4, gl_fliped_surface->w,
                          gl_fliped_surface->h, GL_RGBA,GL_UNSIGNED_BYTE,
                          gl_fliped_surface->pixels);

        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,
                        GL_LINEAR_MIPMAP_LINEAR);

    }
    else
    {
        glTexImage2D(GL_TEXTURE_2D, 0, 4, gl_fliped_surface->w,
                     gl_fliped_surface->h, 0, GL_RGBA,GL_UNSIGNED_BYTE,
                     gl_fliped_surface->pixels);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    }
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);


    SDL_FreeSurface(gl_fliped_surface);
    SDL_FreeSurface(gl_surface);
    SDL_FreeSurface(picture_surface);

    return glID;
}

SDL_Surface * flipSurface(SDL_Surface * surface)
{
    int current_line,pitch;
    SDL_Surface * fliped_surface = SDL_CreateRGBSurface(SDL_SWSURFACE,
                                   surface->w,surface->h,
                                   surface->format->BitsPerPixel,
                                   surface->format->Rmask,
                                   surface->format->Gmask,
                                   surface->format->Bmask,
                                   surface->format->Amask);



    SDL_LockSurface(surface);
    SDL_LockSurface(fliped_surface);

    pitch = surface->pitch;
    for (current_line = 0; current_line < surface->h; current_line ++)
    {
        memcpy(&((unsigned char* )fliped_surface->pixels)[current_line*pitch],
               &((unsigned char* )surface->pixels)[(surface->h - 1  -
                                                    current_line)*pitch],
               pitch);
    }

    SDL_UnlockSurface(fliped_surface);
    SDL_UnlockSurface(surface);
    return fliped_surface;
}

bool isLoaded(struct MyImage* img)
{
    if (img == NULL)
        return false;
    for(int i=0; i<(int)M_tabImage.size(); i++)
    {
        if (img == M_tabImage[i])
            return true;
    }
    return false;
}

bool isLoaded(struct ImageLoaded* tex)
{
    if (tex == NULL)
        return false;
    for(int i=0; i<(int)M_tabImageLoaded.size(); i++)
    {
        if (tex == M_tabImageLoaded[i])
            return true;
    }
    return false;

}

bool isLoaded(struct MyText* text)
{
    if (text == NULL)
        return false;
    for(int i=0; i<(int)M_tabText.size(); i++)
    {
        if (text == M_tabText[i])
            return true;
    }
    return false;

}

bool fileExist(std::string name)
{
    FILE *file = fopen(name.c_str(), "r");
    if (file)
    {
        fclose(file);
        return true;
    }
    return false;
}

void TextSetCharactere(Text txt, SDLKey c)
{
    //suprimer
    if (c==SDLK_BACKSPACE && txt->text.size())    txt->text.erase(txt->text.begin()+txt->text.size()-1);

    //si on a depassé la limite de caractere alors on ne fait rien
    if (txt->nbMaxChar >= 0 && (int)txt->text.size() >= txt->nbMaxChar)
        return ;

    //si maj est enfoncer
    if (M_key[SDLK_RSHIFT] || M_key[SDLK_LSHIFT])
    {
        //les lettre
        if (c>= SDLK_a && c<=SDLK_z) txt->text += c-32;
        //les bouton au dessus des lettre
        if (c==SDLK_0)      txt->text += '0';
        if (c==SDLK_1)      txt->text += '1';
        if (c==SDLK_2)      txt->text += '2';
        if (c==SDLK_3)      txt->text += '3';
        if (c==SDLK_4)      txt->text += '4';
        if (c==SDLK_5)      txt->text += '5';
        if (c==SDLK_6)      txt->text += '6';
        if (c==SDLK_7)      txt->text += '7';
        if (c==SDLK_8)      txt->text += '8';
        if (c==SDLK_9)      txt->text += '9';
    }
    else
    {
        if (c>= SDLK_a && c<=SDLK_z) txt->text += c;
        //les bouton au dessus des lettre
        if (c==SDLK_0)      txt->text += 'à';
        //if (c==SDLK_1)      txt->text += '&';
        if (c==SDLK_2)      txt->text += 'é';
        //if (c==SDLK_3)      txt->text += '"';
        //if (c==SDLK_4)      txt->text += '\'';
        //if (c==SDLK_5)      txt->text += '(';
        if (c==SDLK_6)      txt->text += '-';
        if (c==SDLK_7)      txt->text += 'è';
        if (c==SDLK_8)      txt->text += '_';
        if (c==SDLK_9)      txt->text += 'ç';
    }

    //le pavé numérique
    if (c==SDLK_KP0)      txt->text += '0';
    if (c==SDLK_KP1)      txt->text += '1';
    if (c==SDLK_KP2)      txt->text += '2';
    if (c==SDLK_KP3)      txt->text += '3';
    if (c==SDLK_KP4)      txt->text += '4';
    if (c==SDLK_KP5)      txt->text += '5';
    if (c==SDLK_KP6)      txt->text += '6';
    if (c==SDLK_KP7)      txt->text += '7';
    if (c==SDLK_KP8)      txt->text += '8';
    if (c==SDLK_KP9)      txt->text += '9';
    //la barre d'éspace
    if (c==SDLK_SPACE)    txt->text += ' ';
    //entrée
    if (c==SDLK_RETURN)
    {
        M_idTextFocus = -1;
    }

    //printf("%s\n", txt->text.c_str());
}

unsigned int Time()
{
    return SDL_GetTicks();

}
