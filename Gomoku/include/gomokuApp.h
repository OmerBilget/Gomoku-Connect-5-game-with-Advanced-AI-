#ifndef GOMOKUAPP_H
#define GOMOKUAPP_H

#include "wx/wx.h"
#include "main.h"

class gomokuApp : public wxApp
{
    public:
        gomokuApp();
        ~gomokuApp();
    private:

        main* FRAME=nullptr;
    public :
        virtual bool OnInit();
};

#endif // GOMOKUAPP_H
