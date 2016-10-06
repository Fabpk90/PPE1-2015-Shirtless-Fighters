#ifndef CREDITS_H_INCLUDED
#define CREDITS_H_INCLUDED

struct MenuCredits{

    Image i_Background;

    void Init()
    {
        i_Background = CreateImage("data/backgrounds_menu/3.png");
    }

    int Draw()
    {
        while (!KeyGetOneShot(SDLK_ESCAPE) && !StickGetClick(0, 7))
        {
            DrawImage(i_Background);
            RefreshScreen();
        }
        return 0;
    }
};

#endif // CREDITS_H_INCLUDED
