#ifndef MAIN_H
#define MAIN_H
#include "wx/wx.h"
#include "macros.h"
#include "player.h"
#include "AI_lib.h"
#include <thread>
#include <chrono>
#include <mutex>
class main:public wxFrame
{
    public:
        //constructor

        main();
        ~main();


        player PlayerX;
        player PlayerO;
        unsigned int x_b[SIZE];
        unsigned int o_b[SIZE];
        char board[SIZE][SIZE];
        bool turn;
        char winner;
        int move_count;
        float bestRateNum;

        struct listofmove{
            std::vector<array<int,3>> movedivide;
            int p[64];
        };
        //components
        wxButton** buttonlist;
        wxChoice* starter;
        wxPanel* settings_panel=nullptr;
        wxChoice* playerX_mode=nullptr;
        wxChoice* playerO_mode=nullptr;
        wxButton* reset=nullptr;
        wxButton* startGame=nullptr;
        wxSlider* xSlider=nullptr;
        wxSlider* oSlider=nullptr;
        wxPanel* MainPanel=nullptr;
        wxStaticText* xSliderText=nullptr;
        wxStaticText* oSliderText=nullptr;
        wxStaticText* xTitle=nullptr;
        wxStaticText* oTitle=nullptr;
        wxPanel* outerBar=nullptr;
        wxPanel* innerBar=nullptr;
        wxSlider* bestRate=nullptr;
        wxStaticText* bestRateText=nullptr;

        //add events
        void onButtonClicked( wxCommandEvent &evt);
        void onReset(wxCommandEvent &evt);
        void onStart(wxCommandEvent &evt);
        void onSliderChange_X(wxCommandEvent &evt);
        void onSliderChange_O(wxCommandEvent &evt);
        void onTypeChange_X(wxCommandEvent &evt);
        void onTypeChange_O(wxCommandEvent &evt);
        void onStarterChanged(wxCommandEvent &evt);
        void onBestRateChanged(wxCommandEvent &evt);
        wxDECLARE_EVENT_TABLE();

        //functions
        void UpdateBoard(wxButton**,int);
        void DisableSettings();
        void EnableSettings();
        void disableButtons();
        void enableButtons();
        void Game_Handler(int,int);
        int human_move(int,int);
        void ai_move(unsigned int[],unsigned int[],int,int ,char);
        void make_move(int,int);
        void reset_game();
        void start_game();
        void end_game();
        void get_winner_buttons();
        void ai_first_move();


};

#endif // MAIN_H
