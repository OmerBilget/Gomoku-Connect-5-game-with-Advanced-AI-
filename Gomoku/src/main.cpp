#include "main.h"
#include <string>


#include <iostream>
//=================================



#define BOARD_SIZE 50
#define BOARD_BUTTON_ID 10000
#define RESET_ID 200
#define START_ID 100
#define XSLIDER_ID 125
#define OSLIDER_ID 127
#define PLAYERXTYPE_ID 179
#define PLAYEROTYPE_ID 190
#define STARTER_ID 273
#define BESTRATE_ID 1777


//=================================
wxBEGIN_EVENT_TABLE(main,wxFrame)
    EVT_BUTTON(BOARD_BUTTON_ID,onButtonClicked)
    EVT_BUTTON(RESET_ID,onReset)
    EVT_BUTTON(START_ID,onStart)
    EVT_SLIDER(XSLIDER_ID,onSliderChange_X)
    EVT_SLIDER(OSLIDER_ID,onSliderChange_O)
    EVT_CHOICE(PLAYERXTYPE_ID,onTypeChange_X)
    EVT_CHOICE(PLAYEROTYPE_ID,onTypeChange_O)
    EVT_CHOICE(STARTER_ID,onStarterChanged)
    EVT_SLIDER(BESTRATE_ID,onBestRateChanged)
wxEND_EVENT_TABLE()



//=================================
main::main(): wxFrame(nullptr,wxID_ANY,"SDSDAASDA",wxPoint(30,30),wxSize(780,600),wxDEFAULT_FRAME_STYLE & ~wxRESIZE_BORDER)
,PlayerX(X_ID,HUMAN,3)
,PlayerO(O_ID,HUMAN,3)
{
    for(int i=0;i<SIZE;i++){
        x_b[i]=0;
        o_b[i]=0;
    }
    bitboardtomatrix(x_b,o_b,board);
    turn=TURN_OF_X;
    winner=EMPTY_ID;
    move_count=0;
    bestRateNum=1;

    //main
    MainPanel=new wxPanel(this,1000);
    MainPanel->SetBackgroundColour(wxColor(160,160,160));
    buttonlist=new wxButton*[SIZE*SIZE]; //generate board button

    for(int i=0;i<SIZE;i++){
        for(int j=0;j<SIZE;j++){
            buttonlist[i*SIZE+j]=new wxButton(MainPanel,BOARD_BUTTON_ID+(i*SIZE+j),"",wxPoint(30+j*BOARD_SIZE,30+i*BOARD_SIZE),wxSize(BOARD_SIZE,BOARD_SIZE));
            buttonlist[i*SIZE+j]->Bind(wxEVT_COMMAND_BUTTON_CLICKED,&main::onButtonClicked,this);
            buttonlist[i*SIZE+j]->SetBackgroundColour(wxColor(125,125,125));

        }
    }
    settings_panel=new wxPanel(MainPanel,100,wxPoint(SIZE*BOARD_SIZE+50,30),wxSize(200,SIZE*BOARD_SIZE));
    settings_panel->SetBackgroundColour(RGB(100,100,100));

    xTitle=new wxStaticText(settings_panel,1999,"PLAYER X",wxPoint(0,0),wxSize(200,25),wxALIGN_CENTRE_HORIZONTAL);

    playerX_mode=new wxChoice(settings_panel,PLAYERXTYPE_ID,wxPoint(0,25),wxSize(100,40));
    playerX_mode->Append(_("AI"));
    playerX_mode->Append(_("HUMAN"));
    playerX_mode->SetSelection(1);
    playerX_mode->Bind(wxEVT_CHOICE,main::onTypeChange_X,this);

    oTitle=new wxStaticText(settings_panel,1978,"PLAYER O",wxPoint(0,100),wxSize(200,25),wxALIGN_CENTRE_HORIZONTAL);

    playerO_mode=new wxChoice(settings_panel,PLAYEROTYPE_ID,wxPoint(0,125),wxSize(100,40));
    playerO_mode->Append(_("AI"));
    playerO_mode->Append(_("HUMAN"));
    playerO_mode->SetSelection(1);
    playerO_mode->Bind(wxEVT_CHOICE,&main::onTypeChange_O,this);

    xSlider=new wxSlider(settings_panel,XSLIDER_ID,2,1,6,wxPoint(0,60),wxSize(150,25));
    xSliderText=new wxStaticText(settings_panel,178,"223123",wxPoint(xSlider->GetPosition().x+xSlider->GetSize().GetX(),xSlider->GetPosition().y+5),wxSize(50,20));
    xSlider->Bind(wxEVT_SLIDER,&main::onSliderChange_X,this);
    xSliderText->SetLabel(std::to_string(xSlider->GetValue()) );

    oSlider=new wxSlider(settings_panel,OSLIDER_ID,2,1,6,wxPoint(0,160),wxSize(150,25));
    oSliderText=new wxStaticText(settings_panel,178,"223123",wxPoint(oSlider->GetPosition().x+xSlider->GetSize().GetX(),oSlider->GetPosition().y+5),wxSize(50,20));
    oSlider->Bind(wxEVT_SLIDER,&main::onSliderChange_O,this);
    oSliderText->SetLabel(std::to_string(oSlider->GetValue()) );

    starter=new wxChoice(settings_panel,STARTER_ID,wxPoint(0,450),wxSize(100,40));
    starter->Append(_("X STARTS"));
    starter->Append(_("O STARTS"));
    starter->SetSelection(0);
    starter->Bind(wxEVT_CHOICE,main::onStarterChanged,this);

    reset=new wxButton(settings_panel,RESET_ID,"RESET",wxPoint(50,400),wxSize(50,50));
    reset->Bind(wxEVT_COMMAND_BUTTON_CLICKED,&main::onReset,this);
    startGame=new wxButton(settings_panel,START_ID,"START",wxPoint(0,400),wxSize(50,50));
    startGame->Bind(wxEVT_COMMAND_BUTTON_CLICKED,&main::onStart,this);

    outerBar=new wxPanel(settings_panel,176,wxPoint(15,300),wxSize(170,24));
    outerBar->SetBackgroundColour(wxColor(70,70,70));
    innerBar=new wxPanel(outerBar,7594,wxPoint(2,2),wxSize(0,20));
    innerBar->SetBackgroundColour(wxColor(0,255,0));

    bestRate=new wxSlider(settings_panel,BESTRATE_ID,100,10,100,wxPoint(0,200),wxSize(150,25));
    bestRateText=new wxStaticText(settings_panel,178,"100",wxPoint(bestRate->GetPosition().x+bestRate->GetSize().GetX(),bestRate->GetPosition().y+5),wxSize(50,20));
    bestRate->Bind(wxEVT_SLIDER,&main::onBestRateChanged,this);
    EnableSettings();
    disableButtons();



}

//delete
main::~main()
{
    delete[]buttonlist;
}


//On Board Button Clicked
void main::onButtonClicked(wxCommandEvent &evt){
   int x=(evt.GetId()-BOARD_BUTTON_ID)/SIZE;
   int y=(evt.GetId()-BOARD_BUTTON_ID)%SIZE;
   if((PlayerX.type==AI && PlayerO.type==AI) || winner!=EMPTY_ID){
     return;
   }
   Game_Handler(x,y);
}


//On Reset Button Clicked
void main::onReset(wxCommandEvent &evt){
    reset_game();
}

//On Start Button Clicked
void main::onStart(wxCommandEvent &evt){
    start_game();
}

//on Slider Change
void main::onSliderChange_X(wxCommandEvent &evt){
    xSliderText->SetLabel(std::to_string(xSlider->GetValue()) );
    PlayerX.depth=xSlider->GetValue();
}
void main::onSliderChange_O(wxCommandEvent &evt){
    oSliderText->SetLabel(std::to_string(oSlider->GetValue()) );
    PlayerO.depth=oSlider->GetValue();
}


void main::UpdateBoard(wxButton** board,int index){
    int x=index/SIZE;
    int y=index%SIZE;
   if(turn==TURN_OF_X){
         buttonlist[x*SIZE+y]->SetLabel(X_ID);
         board[index]->SetBackgroundColour(wxColor(220,0,250));
   }else{
         buttonlist[x*SIZE+y]->SetLabel(O_ID);
         board[index]->SetBackgroundColour(wxColor(255,200,0));
   }
}


void main::Game_Handler(int x,int y){
    if(PlayerX.type==AI && PlayerO.type==AI){
        return;
    }
    if(turn==TURN_OF_X){
        if(PlayerX.type==HUMAN){
            int is_player_moved= human_move(x,y);
            if(is_player_moved==0){
                return;
            }
            if(PlayerO.type==AI){
                ai_move(x_b,o_b,PlayerO.depth,move_count,O_ID);
            }
        }else{

        }
    }else{
        if(PlayerO.type==HUMAN){
            int is_player_moved= human_move(x,y);
            if(is_player_moved==0){
                return;
            }
            if(PlayerX.type==AI){
                 ai_move(x_b,o_b,PlayerX.depth,move_count,X_ID);
            }
        }else{

        }
    }
}

int main::human_move(int x,int y){
    if(check_empty(x_b,o_b,x,y)==false){
        return 0;
    }
    make_move(x,y);
    return 1;
}

void main::onTypeChange_X(wxCommandEvent &evt){
    int i=playerX_mode->GetSelection();
    if(i==0){
        PlayerX.type=AI;
    }else{
        PlayerX.type=HUMAN;
    }
}


void main::onTypeChange_O(wxCommandEvent &evt){
    int i=playerO_mode->GetSelection();
    if(i==0){
        PlayerO.type=AI;
    }else{
        PlayerO.type=HUMAN;
    }
}

void main::onStarterChanged(wxCommandEvent &evt){
    int i=starter->GetSelection();
    if(i==0){
        turn=TURN_OF_X;
    }else{
        turn=TURN_OF_O;
    }
}

void main::DisableSettings(){
    playerX_mode->Disable();
    playerO_mode->Disable();
    xSlider->Disable();
    oSlider->Disable();
    startGame->Disable();
    reset->Enable();
    starter->Disable();
}

void main::EnableSettings(){
    playerX_mode->Enable();
    playerO_mode->Enable();
    xSlider->Enable();
    oSlider->Enable();
    startGame->Enable();
    reset->Disable();
    starter->Enable();
}


void main::disableButtons(){
    for(int i=0;i<SIZE*SIZE;i++){
         buttonlist[i]->Disable();
    }
}

void main::enableButtons(){
    for(int i=0;i<SIZE*SIZE;i++){
         buttonlist[i]->Enable();
    }
}


void main::ai_move(unsigned int x_b[],unsigned int o_b[],int depth,int move_count,char ai) {
    int bestscore = -INT_MAX;
    int score;
    int x_coordinate = -1;
    int y_coordinate = -1;

    std::vector<std::array<int,3>> movelist = getListBitboard(x_b,o_b);
    moveorder(x_b,o_b,ai,true,movelist);
    int len = (int)movelist.size();
    float z=len*bestRateNum;
    len=int(z);
    int complete=outerBar->GetSize().GetX()-4;
    float completerate=0;
    float interval=complete/len;
    innerBar->SetSize(0,20);
    unsigned int* ai_bitboard=x_b;
    if(ai==O_ID){
        ai_bitboard=o_b;
    }

    //std::cout<<"\n";

    for (int i = 0; i < len; i++) {
        int x_c=movelist[i][0];
        int y_c=movelist[i][1];
        addtobitboard(x_c,y_c,ai_bitboard);
        score = alphabeta_bitboard(x_b,o_b, depth, false, move_count, ai, -INT_MAX,INT_MAX,1);
        unmovetobitboard(x_c,y_c,ai_bitboard);
        if (score > bestscore) {
            bestscore = score;
            x_coordinate = x_c;
            y_coordinate = y_c;
        }
        completerate+=interval;
        innerBar->SetSize(completerate,20);

    }
    innerBar->SetSize(complete,20);
    if (x_coordinate != -1 && y_coordinate != -1) {
        make_move(x_coordinate,y_coordinate);
    }
}


void main::make_move(int x,int y){
   if(winner==EMPTY_ID){
      if(turn==TURN_OF_X){
          addtobitboard(x,y,x_b);
      }else{
          addtobitboard(x,y,o_b);
      }
      UpdateBoard(buttonlist,x*SIZE+y);
      winner=getwinnerbitboard(x_b,o_b);
      if(winner!=EMPTY_ID){
        end_game();
        return;
      }
      turn=!turn;
      move_count+=1;
   }
}

void main::reset_game(){
    for(int i=0;i<SIZE*SIZE;i++){
         buttonlist[i]->SetLabel("");
         buttonlist[i]->SetBackgroundColour(wxColor(125,125,125));
    }
    EnableSettings();
    disableButtons();
    innerBar->SetSize(0,20);
}

void main::start_game(){
    DisableSettings();
    enableButtons();
    for(int i=0;i<SIZE;i++){
        x_b[i]=0;
        o_b[i]=0;
    }
    winner=EMPTY_ID;
    move_count=0;
    int i=starter->GetSelection();
    if(i==0){
        turn=TURN_OF_X;
    }else{
        turn=TURN_OF_O;
    }
    if(turn==TURN_OF_X && PlayerX.type==AI){
        ai_first_move();
    }else if(turn==TURN_OF_O &&PlayerO.type==AI){
        ai_first_move();
    }
}

void main::end_game(){
    get_winner_buttons();
}

void main::get_winner_buttons(){
    vector<array<int,2>> winnerlist;
    winnerlist=winnerbuttons(x_b,o_b,winnerlist);
    int len=winnerlist.size();
    char board[SIZE][SIZE];
    bitboardtomatrix(x_b,o_b,board);
    for(int i=0;i<SIZE;i++){
        for(int j=0;j<SIZE;j++){
            if(board[i][j]==EMPTY_ID){
                buttonlist[i*SIZE+j]->SetBackgroundColour(wxColour(100,100,100));
            }else if(board[i][j]==X_ID){
                buttonlist[i*SIZE+j]->SetBackgroundColour(wxColour(150,0,170));
            }else{
                buttonlist[i*SIZE+j]->SetBackgroundColour(wxColour(170,150,0));
            }
        }
    }
    if(turn==TURN_OF_X){
        for(int i=0;i<len;i++){
            buttonlist[winnerlist[i][0]*SIZE+winnerlist[i][1]]->SetBackgroundColour(wxColor(240,100,255));
        }
    }else{
        for(int i=0;i<len;i++){
            buttonlist[winnerlist[i][0]*SIZE+winnerlist[i][1]]->SetBackgroundColour(wxColor(255,220,100));
        }
    }
}

void main::ai_first_move(){
    make_move(5,5);
}




void main::onBestRateChanged(wxCommandEvent &evt){
    bestRateNum=((float)bestRate->GetValue()/(float)bestRate->GetMax());
    bestRateText->SetLabel(std::to_string(bestRateNum) );
}


