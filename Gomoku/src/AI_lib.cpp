#include "AI_lib.h"
#include "macros.h"
#include <iostream>
void bitboardtomatrix(unsigned  int pxboard[],unsigned int poboard[],char board[][SIZE]) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (pxboard[i] & (1 << (SIZE - j - 1))) {
                board[i][j] = X_ID ;
            } else if (poboard[i] & (1 << (SIZE - j - 1))) {
                board[i][j] = O_ID ;
            } else {
                board[i][j] = EMPTY_ID;
            }
        }
    }
}
void matrixtobitboard(unsigned  int pxboard[],unsigned int poboard[],char board[][SIZE]) {
    int xrow;
    int orow;
    for (int i = 0; i < SIZE; i++) {
        xrow = 0;
        orow = 0;
        for (int j = 0; j < SIZE; j++) {
            if (board[i][j] != EMPTY_ID) {
                if (board[i][j] == X_ID ) {
                    xrow += 1 << (SIZE - j - 1);
                } else {
                    orow += 1 << (SIZE - j - 1);
                }
            }
        }
        pxboard[i] = xrow;
        poboard[i] = orow;
    }
}

void addtobitboard(int i,int j,unsigned int bitboard[]) {
    bitboard[i] |= (1 <<(SIZE - j - 1));
}
void unmovetobitboard(int i,int j,unsigned int bitboard[]) {
    bitboard[i] ^= (1 << (SIZE - j - 1));
}

vector<array<int,3>> getListBitboard(unsigned int xboard[],unsigned int oboard[]) {
    unsigned int zeromap;
    unsigned int zeroup;
    unsigned int zerodown;
    unsigned int zero;
    vector<array<int,3>> movelist;
    movelist.reserve(50);
    for (int i = 0; i < SIZE; i++) {
        zeromap = ~(xboard[i] | oboard[i]) ;
        zeroup = ~(xboard[i - 1] | oboard[i - 1]) ;
        zerodown = ~(xboard[i + 1] | oboard[i + 1]);
        for (int j = 0; j < SIZE; j++) {
            zero = zeromap & (1 << (SIZE - j - 1));
            //if bit is empty
            if (zero) {
                //horizontal check for 1
                if (j > 0 && !((zero << 1) & zeromap)) {
                    movelist.push_back({i, j,0});
                    continue;
                }
                if (j < SIZE - 1 && !((zero >> 1) & zeromap)) {
                    movelist.push_back({i, j,0});
                    continue;
                }
                if (i > 0) {
                    if (!(zero & zeroup)) {
                        movelist.push_back({i, j,0});
                        continue;
                        //upper left
                    } else if (!((zero << 1) & zeroup) && j > 0) {
                        movelist.push_back({i, j,0});
                        continue;
                        //upper right
                    } else if (!((zero >> 1) & zeroup) && j < SIZE - 1) {
                         movelist.push_back({i, j,0});
                        continue;
                    }
                } if (i < SIZE - 1) {
                    // bottom
                    if (!(zero & zerodown)) {
                        movelist.push_back({i, j,0});
                        continue;
                        // bottom left
                    } else if (!((zero << 1) & zerodown) && j > 0) {
                         movelist.push_back({i, j,0});
                        continue;
                        // bottom right
                    } else if (!((zero >> 1) & zerodown) && j < SIZE - 1) {
                         movelist.push_back({i, j,0});
                        continue;
                    }

                }

            }
        }
    }
    return movelist;
}


void rotate90(unsigned int bitboard[]) {
    unsigned int i_mask;
    unsigned int j_mask;
    for (int i = 0, i_mask = 0b0000000001; i < SIZE; ++i, i_mask <<= 1) {
        for (int j = SIZE - 1, j_mask = 0b1000000000; j > i; --j, j_mask >>= 1) {
            if (((bitboard[i] & j_mask) != 0) != ((bitboard[j] & i_mask) != 0)) {
                bitboard[i] ^= j_mask;
                bitboard[j] ^= i_mask;
            }
        }
    }
}

bool check_empty(unsigned int x_b[],unsigned int o_b[],int x,int y){
    if( (~(x_b[x]|o_b[x]))&(1<<(SIZE-y-1) ) ){
        return true;
    }
    return false;
}
char getwinnerbitboard(unsigned int x_b[],unsigned int  o_b[]) {
    unsigned int x_b_rotated90[SIZE];
    unsigned int o_b_rotated90[SIZE];
    for(int i=0;i<SIZE;i++){
        x_b_rotated90[i]=x_b[i];
        o_b_rotated90[i]=o_b[i];
    }
    rotate90(x_b_rotated90);
    rotate90(o_b_rotated90);
    for (int i = 0; i < SIZE - 4; i++) {
        //for x

        //vertical check
        if ((x_b[i] & x_b[i + 1] & x_b[i + 2] & x_b[i + 3] & x_b[i + 4])) {
            return X_ID;
        }

        //horizontal check
        if ((x_b_rotated90[i] & x_b_rotated90[i + 1] & x_b_rotated90[i + 2] & x_b_rotated90[i + 3] & x_b_rotated90[i + 4])) {
            return X_ID;
        }

        //diagonal check right
        if (x_b[i] & (x_b[i + 1] >> 1) & (x_b[i + 2] >> 2) & (x_b[i + 3] >> 3) & (x_b[i + 4] >> 4)) {
            return X_ID;
        }

        //diagonal check left
        if (x_b[i] & (x_b[i + 1] << 1) & (x_b[i + 2] << 2) & (x_b[i + 3] << 3) & (x_b[i + 4] << 4)) {
            return X_ID;
        }

        //for o

        //vertical check
        if ((o_b[i] & o_b[i + 1] & o_b[i + 2] & o_b[i + 3] & o_b[i + 4])) {
            return O_ID;
        }

        //horizontal check
        if ((o_b_rotated90[i] & o_b_rotated90[i + 1] & o_b_rotated90[i + 2] & o_b_rotated90[i + 3] & o_b_rotated90[i + 4])) {
            return O_ID;
        }

        //diagonal check right
        if (o_b[i] & (o_b[i + 1] >> 1) & (o_b[i + 2] >> 2) & (o_b[i + 3] >> 3) & (o_b[i + 4] >> 4)) {
            return O_ID;
        }

        //diagonal check left
        if (o_b[i] & (o_b[i + 1] << 1) & (o_b[i + 2] << 2) & (o_b[i + 3] << 3) & (o_b[i + 4] << 4)) {
            return O_ID;
        }
    }
    return EMPTY_ID;
}

/*
void ai_bitboard(unsigned int x_b[],unsigned int o_b[],int depth,int move_count,char ai) {
    int bestscore = -INT_MAX;
    int score;
    int x_coordinate = -1;
    int y_coordinate = -1;
    std::vector<std::array<int,3>> movelist = getListBitboard(x_b,o_b);
    moveorder(x_b,o_b,ai,true,movelist);
    const int len = (int)movelist.size();
    unsigned int* ai_bitboard=x_b;
    if(ai==O_ID){
        ai_bitboard=o_b;
    }

    //std::cout<<"\n";

    for (int i = 0; i < len; i++) {
        int x_c=movelist[i][0];
        int y_c=movelist[i][1];
        addtobitboard(x_c,y_c,ai_bitboard);
        score = alphabeta_bitboard(x_b,o_b, depth, false, move_count, ai, -INT_MAX,INT_MAX);
        std::cout<<x_c<<" "<<y_c<<" score "<<score<<" score_s "<<movelist[i][2]<<" bestscore "<<bestscore<<"\n";
        unmovetobitboard(x_c,y_c,ai_bitboard);
        if (score > bestscore) {
            bestscore = score;
            x_coordinate = x_c;
            y_coordinate = y_c;
        }

    }

    if (x_coordinate != -1 && y_coordinate != -1) {
        std::cout<<"Best Move "<<x_coordinate<<" "<<y_coordinate<<std::endl;
        //make_move(x_coordinate, y_coordinate);
    }
}
*/
void moveorder(unsigned int x_b[],unsigned int o_b[],char ai,bool ismaximising,std::vector<std::array<int,3>>& movelist){
    int len=(int)movelist.size();
    if(ai==X_ID){
        for(int i=0;i<len;i++){
            addtobitboard(movelist[i][0],movelist[i][1],x_b);
            movelist[i][2]=get_score(x_b,o_b,ai);
            unmovetobitboard(movelist[i][0],movelist[i][1],x_b);
        }
    }else{
        for(int i=0;i<len;i++){
            addtobitboard(movelist[i][0],movelist[i][1],o_b);
            movelist[i][2]=get_score(x_b,o_b,ai);
            unmovetobitboard(movelist[i][0],movelist[i][1],o_b);
        }
    }
    if(ismaximising){
        std::sort(movelist.begin(),movelist.end(),comparator_descending);
    }else{
        std::sort(movelist.begin(),movelist.end(),comparator_ascending);
    }
}

inline bool comparator_descending(std::array<int,3> a,std::array<int,3> b){
    return (a[2]>b[2]);
}

inline bool comparator_ascending(std::array<int,3> a,std::array<int,3> b){
    return (a[2]<b[2]);
}


int alphabeta_bitboard(unsigned int x_b[],unsigned int o_b[],int depth,bool isMaximising,int move_count,char ai,int alpha,int beta,float bestrate) {
    //if depth is 0 or game is over evaluate
    int multiplier = 1;
    const char win = getwinnerbitboard(x_b,o_b);
    if (win != EMPTY_ID) {
        if (win != ai) {
            multiplier = -1;
        }
        return (winnerscore+depth)* multiplier;
    }
    if (depth == 0) {
        return get_score(x_b,o_b, ai);
    }

    if (move_count == SIZE*SIZE) {
        return 0;
    }
    int bestscore;
    int score;
    std::vector<std::array<int,3>> movelist;
    movelist = getListBitboard(x_b,o_b);
    moveorder(x_b,o_b,ai,isMaximising,movelist);
    int len =(int) movelist.size();
    float z=len*bestrate;
    len=(int)z;
    unsigned int* b=x_b;
    unsigned int* c=o_b;
    if(ai==O_ID){
        b=o_b;
        c=x_b;
    }
    if (isMaximising == true) {
        //ai turn

        bestscore = -INT_MAX;
        for (int i = 0; i < len; i++) {
            addtobitboard(movelist[i][0], movelist[i][1], b );
            score = alphabeta_bitboard(x_b, o_b, depth - 1, !isMaximising, move_count + 1, ai, alpha, beta,bestrate);
            unmovetobitboard(movelist[i][0], movelist[i][1], b );
            if(score>bestscore){
                bestscore=score;
            }
            //alpha=Math.max(alpha,bestscore);
            alpha=std::max(alpha,bestscore);

            if(alpha>=beta){
                break;
            }

        }
        return bestscore;
    } else {
        //opponent turn
        bestscore = INT_MAX;
        for (int i = 0; i < len; i++) {
            addtobitboard(movelist[i][0], movelist[i][1],c);
            score = alphabeta_bitboard(x_b, o_b, depth - 1, !isMaximising, move_count + 1, ai, alpha, beta,bestrate);
            unmovetobitboard(movelist[i][0], movelist[i][1], c);
            if(score<bestscore){
                bestscore=score;
            }
            //beta=Math.min(beta,bestscore);
            beta=std::min(beta,bestscore);
            if(alpha>=beta){
                break;
            }
        }
        return bestscore;
    }
}

int evaluate_bitboard_combined(unsigned int x_b[],unsigned int o_b[],char ai) {
    unsigned int  index_h;
    unsigned int  index_v;
    unsigned int  zero_h;
    unsigned int zero_v;
    unsigned int count_h = 0;
    unsigned int count_v = 0;
    char type_h = EMPTY_ID;
    char type_v = EMPTY_ID;
    int open_h = 0;
    int open_v = 0;
    int score_h = 0;
    int score_v = 0;
    for (int i = 0; i < SIZE; i++) {
        zero_h = ~(x_b[i] | o_b[i]);
        index_v = 1 << (SIZE - i - 1);
        for (int j = 0; j < SIZE; j++) {
            index_h = 1 << (SIZE - j - 1);
            zero_v = ~(x_b[j] | o_b[j]);

            //horizontal
            //if zero
            if (zero_h & index_h) {
                if (count_h > 0) {
                    open_h += 1;
                    score_h += addthreat(count_h, open_h, type_h, ai);
                    count_h = 0;
                    open_h = 1;
                } else {
                    open_h = 1;
                }
                type_h = EMPTY_ID;
            }



            else if (index_h & x_b[i]) {  //if x
                if (type_h == X_ID) {
                    count_h += 1;
                } else {
                    if (count_h > 0) {
                        score_h += addthreat(count_h, open_h, type_h, ai);
                        count_h = 1;
                        open_h = 0;
                    } else {
                        count_h = 1;
                    }
                    type_h = X_ID;
                }

            } else { // if o
                if (type_h == O_ID) {
                    count_h += 1;
                } else {
                    if (count_h > 0) {
                        score_h += addthreat(count_h, open_h, type_h, ai);
                        count_h = 1;
                        open_h = 0;
                    } else {
                        count_h = 1;
                    }
                    type_h = O_ID;
                }

            }


            //vertical
            if (zero_v & index_v) {
                if (count_v > 0) {
                    open_v += 1;
                    score_v += addthreat(count_v, open_v, type_v, ai);
                    count_v = 0;
                    open_v = 1;
                } else {
                    open_v = 1;
                }
                type_v = EMPTY_ID;
            }



            else if (index_v & x_b[j]) {  //if x
                if (type_v == X_ID) {
                    count_v += 1;
                } else {
                    if (count_v > 0) {
                        score_v += addthreat(count_v, open_v, type_v, ai);
                        count_v = 1;
                        open_v = 0;
                    } else {
                        count_v = 1;
                    }
                    type_v = X_ID;
                }

            } else { // if o
                if (type_v == O_ID) {
                    count_v += 1;
                } else {
                    if (count_v > 0) {
                        score_v += addthreat(count_v, open_v, type_v, ai);
                        count_v = 1;
                        open_v = 0;
                    } else {
                        count_v = 1;
                    }
                    type_v = O_ID;
                }

            }


        }


        if (count_h > 0) {
            score_h += addthreat(count_h, open_h, type_h, ai);
        }
        count_h = 0;
        open_h = 0;

        if (count_v > 0) {
            score_v += addthreat(count_v, open_v, type_v, ai);
        }
        count_v = 0;
        open_v = 0;

    }
    return score_h+score_v;
}

int evaluate_bitboard_diagonal_right(unsigned int x_b[],unsigned int o_b[],char ai) {
    int score = 0;
    int open = 0;
    int counter = 0;
    char type = EMPTY_ID;
    unsigned int index;
    for (int i = 0; i < SIZE - 4; i++) {
        int x_c = 0;
        int y_c = i;
        while (x_c < SIZE && y_c < SIZE) {
            index = 1 << (SIZE - y_c - 1);

            if (x_b[x_c] & index) { //if x
                if (type == X_ID) {
                    counter += 1;
                } else {
                    if (counter > 0) {
                        score += addthreat(counter, open, type, ai);
                        counter = 1;
                        open = 0;
                    } else {
                        counter = 1;
                    }
                    type = X_ID;
                }
            } else if (o_b[x_c] & index) {
                if (type == O_ID) {
                    counter += 1;
                } else {
                    if (counter > 0) {
                        score += addthreat(counter, open, type, ai);
                        counter = 1;
                        open = 0;
                    } else {
                        counter = 1;
                    }
                    type = O_ID;
                }
            } else {
                if (counter > 0) {
                    open += 1;
                    score += addthreat(counter, open, type, ai);
                    counter = 0;
                    open = 1;
                } else {
                    open = 1;
                }
                type = EMPTY_ID;
            }
            x_c += 1;
            y_c += 1;
        }
        if (counter > 0) {

            score += addthreat(counter, open, type, ai);
        }
        counter = 0;
        open = 0;

    }


    for (int i = 1; i < SIZE - 4; i++) {
        int x_c = i;
        int y_c = 0;
        while (x_c < SIZE && y_c < SIZE) {
            index = 1 << (SIZE - y_c - 1);
            if (x_b[x_c] & index) {
                if (type == X_ID) {
                    counter += 1;
                } else {
                    if (counter > 0) {

                        score += addthreat(counter, open, type, ai);
                        counter = 1;
                        open = 0;
                    } else {
                        counter = 1;
                    }
                    type = X_ID;
                }
            } else if (o_b[x_c] & index) {
                if (type == O_ID) {
                    counter += 1;
                } else {
                    if (counter > 0) {
                        score += addthreat(counter, open, type, ai);
                        counter = 1;
                        open = 0;
                    } else {
                        counter = 1;
                    }
                    type = O_ID;
                }
            } else {
                if (counter > 0) {
                    open += 1;
                    score += addthreat(counter, open, type, ai);
                    counter = 0;
                    open = 1;
                } else {
                    open = 1;
                }
                type = EMPTY_ID;
            }
            x_c += 1;
            y_c += 1;
        }
        if (counter > 0) {

            score += addthreat(counter, open, type, ai);
        }
        counter = 0;
        open = 0;
    }
    return score;
}


int evaluate_bitboard_diagonal_left(unsigned int x_b[],unsigned int o_b[],char ai) {
    int score = 0;
    int open = 0;
    int counter = 0;
    char type = EMPTY_ID;
    unsigned int index;
    for (int i = SIZE - 1; i >= 4; i--) {
        int x_c = 0;
        int y_c = i;
        while (x_c < SIZE && y_c >= 0) {
            index = 1 << (SIZE - y_c - 1);
            if (x_b[x_c] & index) {
                if (type == X_ID) {
                    counter += 1;
                } else {
                    if (counter > 0) {
                        score += addthreat(counter, open, type, ai);
                        counter = 1;
                        open = 0;
                    } else {
                        counter = 1;
                    }
                    type = X_ID;
                }
            } else if (o_b[x_c] & index) {
                if (type == O_ID) {
                    counter += 1;
                } else {
                    if (counter > 0) {
                        score += addthreat(counter, open, type, ai);
                        counter = 1;
                        open = 0;
                    } else {
                        counter = 1;
                    }
                    type = O_ID;
                }
            } else {
                if (counter > 0) {
                    open += 1;
                    score += addthreat(counter, open, type, ai);
                    counter = 0;
                    open = 1;
                } else {
                    open = 1;
                }
                type = EMPTY_ID;
            }
            x_c += 1;
            y_c -= 1;
        }
        if (counter > 0) {
            score += addthreat(counter, open, type, ai);
        }
        counter = 0;
        open = 0;

    }


    for (int i = 1; i < SIZE - 4; i++) {
        int x_c = i;
        int y_c = SIZE - 1;
        while (x_c < SIZE && y_c >= 0) {
            index = 1 << (SIZE - y_c - 1);
            if (x_b[x_c] & index) {
                if (type == X_ID) {
                    counter += 1;
                } else {
                    if (counter > 0) {
                        score += addthreat(counter, open, type, ai);
                        counter = 1;
                        open = 0;
                    } else {
                        counter = 1;
                    }
                    type = X_ID;
                }
            } else if (o_b[x_c] & index) {
                if (type == O_ID) {
                    counter += 1;
                } else {
                    if (counter > 0) {
                        score += addthreat(counter, open, type, ai);
                        counter = 1;
                        open = 0;
                    } else {
                        counter = 1;
                    }
                    type = O_ID;
                }
            } else {
                if (counter > 0) {
                    open += 1;
                    score += addthreat(counter, open, type, ai);
                    counter = 0;
                    open = 1;
                } else {
                    open = 1;
                }
                type = EMPTY_ID;
            }
            x_c += 1;
            y_c -= 1;
        }
        if (counter > 0) {

            score += addthreat(counter, open, type, ai);
        }
        counter = 0;
        open = 0;
    }
    return score;
}

int get_score(unsigned int x_b[],unsigned int o_b[],char ai) {
    return evaluate_bitboard_combined(x_b, o_b, ai) + evaluate_bitboard_diagonal_right(x_b, o_b, ai) + evaluate_bitboard_diagonal_left(x_b, o_b, ai);
}

int addthreat(int counter,int open,char type,char ai) {
    int multiplier = 1;
    if (type != ai) {
        multiplier = -1;
    }
    if (counter == 5) {
        return winnerscore * multiplier;
    }
    if (open == 0) {
        return 0;
    }
    if (counter == 4) {
        if (open == 2) {
            return openfour * multiplier;
        } else {
            return closedfour * multiplier;
        }
    }

    if (counter == 3) {
        if (open == 2) {
            return openthree * multiplier;
        } else {
            return closedthree * multiplier;
        }
    }
    if (counter == 2) {
        if (open == 2) {
            return opentwo * multiplier;
        } else {
            return closedtwo * multiplier;
        }
    } else {
        if (open == 2) {
            return openone * multiplier;
        } else {
            return closedone * multiplier;
        }
    }
}

vector<array<int,2>> winnerbuttons(unsigned int x_b[],unsigned int o_b[],vector<array<int,2>>& winnerlist){
    char board[SIZE][SIZE];
    bitboardtomatrix(x_b,o_b,board);
    for (int i = 2; i < SIZE - 2; i++) {
        for (int j = 2; j < SIZE - 2; j++) {
            if (board[i][j] == board[i - 1][j] &&
                board[i][j] == board[i - 2][j] &&
                board[i][j] == board[i + 1][j] &&
                board[i][j] == board[i + 2][j] &&
                board[i][j] != EMPTY_ID
            ) {
                winnerlist.push_back({i-2,j});
                winnerlist.push_back({i-1,j});
                winnerlist.push_back({i,j});
                winnerlist.push_back({i+1,j});
                winnerlist.push_back({i+2,j});
                return winnerlist;
            } else if (
                board[i][j] == board[i][j - 2] &&
                board[i][j] == board[i][j - 1] &&
                board[i][j] == board[i][j + 1] &&
                board[i][j] == board[i][j + 2] &&
                board[i][j] != EMPTY_ID
            ) {
                winnerlist.push_back({i,j-2});
                winnerlist.push_back({i,j-1});
                winnerlist.push_back({i,j});
                winnerlist.push_back({i,j+1});
                winnerlist.push_back({i,j+2});
                return winnerlist;
            } else if (
                board[i][j] == board[i - 2][j - 2] &&
                board[i][j] == board[i - 1][j - 1] &&
                board[i][j] == board[i + 1][j + 1] &&
                board[i][j] == board[i + 2][j + 2] &&
                board[i][j] != EMPTY_ID
            ) {
                winnerlist.push_back({i-2,j-2});
                winnerlist.push_back({i-1,j-1});
                winnerlist.push_back({i,j});
                winnerlist.push_back({i+1,j+1});
                winnerlist.push_back({i+2,j+2});
                return winnerlist;
            } else if (
                board[i][j] == board[i - 2][j + 2] &&
                board[i][j] == board[i - 1][j + 1] &&
                board[i][j] == board[i + 1][j - 1] &&
                board[i][j] == board[i + 2][j - 2] &&
                board[i][j] != EMPTY_ID
            ) {
                winnerlist.push_back({i-2,j+2});
                winnerlist.push_back({i-1,j+1});
                winnerlist.push_back({i,j});
                winnerlist.push_back({i+1,j-1});
                winnerlist.push_back({i+2,j-2});
                return winnerlist;
            }
        }
    }
    for (int i = 2; i < SIZE - 2; i++) {
        if (board[0][i] == board[0][i - 2] &&
            board[0][i] == board[0][i - 1] &&
            board[0][i] == board[0][i + 1] &&
            board[0][i] == board[0][i + 2] &&
            board[0][i] != EMPTY_ID
        ) {
            winnerlist.push_back({0,i-2});
            winnerlist.push_back({0,i-1});
            winnerlist.push_back({0,i});
            winnerlist.push_back({0,i+1});
            winnerlist.push_back({0,i+2});
            return winnerlist;
        } else if (board[1][i] == board[1][i - 2] &&
            board[1][i] == board[1][i - 1] &&
            board[1][i] == board[1][i + 1] &&
            board[1][i] == board[1][i + 2] &&
            board[1][i] != EMPTY_ID
        ) {
            winnerlist.push_back({1,i-2});
            winnerlist.push_back({1,i-1});
            winnerlist.push_back({1,i});
            winnerlist.push_back({1,i+1});
            winnerlist.push_back({1,i+2});
            return winnerlist;
        } else if (board[i][0] == board[i - 2][0] &&
            board[i][0] == board[i - 1][0] &&
            board[i][0] == board[i + 1][0] &&
            board[i][0] == board[i + 2][0] &&
            board[i][0] != EMPTY_ID
        ) {
            winnerlist.push_back({i-2,0});
            winnerlist.push_back({i-1,0});
            winnerlist.push_back({i,0});
            winnerlist.push_back({i+1,0});
            winnerlist.push_back({i+2,0});
            return winnerlist;
        } else if (board[i][1] == board[i - 2][1] &&
            board[i][1] == board[i - 1][1] &&
            board[i][1] == board[i + 1][1] &&
            board[i][1] == board[i + 2][1] &&
            board[i][1] != EMPTY_ID
        ) {
            winnerlist.push_back({i-2,1});
            winnerlist.push_back({i-1,1});
            winnerlist.push_back({i,1});
            winnerlist.push_back({i+1,1});
            winnerlist.push_back({i+2,1});
            return winnerlist;
        }
        else if (board[SIZE - 1][i] == board[SIZE - 1][i - 2] &&
            board[SIZE - 1][i] == board[SIZE - 1][i - 1] &&
            board[SIZE - 1][i] == board[SIZE - 1][i + 1] &&
            board[SIZE - 1][i] == board[SIZE - 1][i + 2] &&
            board[SIZE - 1][i] != EMPTY_ID
        ) {
            winnerlist.push_back({SIZE-1,i-2});
            winnerlist.push_back({SIZE-1,i-1});
            winnerlist.push_back({SIZE-1,i});
            winnerlist.push_back({SIZE-1,i+1});
            winnerlist.push_back({SIZE-1,i+2});
            return winnerlist;
        } else if (board[SIZE - 2][i] == board[SIZE - 2][i - 2] &&
            board[SIZE - 2][i] == board[SIZE - 2][i - 1] &&
            board[SIZE - 2][i] == board[SIZE - 2][i + 1] &&
            board[SIZE - 2][i] == board[SIZE - 2][i + 2] &&
            board[SIZE - 2][i] != EMPTY_ID
        ) {
            winnerlist.push_back({SIZE-2,i-2});
            winnerlist.push_back({SIZE-2,i-1});
            winnerlist.push_back({SIZE-2,i});
            winnerlist.push_back({SIZE-2,i+1});
            winnerlist.push_back({SIZE-2,i+2});
            return winnerlist;
        } else if (board[i][SIZE - 1] == board[i - 2][SIZE - 1] &&
            board[i][SIZE - 1] == board[i - 1][SIZE - 1] &&
            board[i][SIZE - 1] == board[i + 1][SIZE - 1] &&
            board[i][SIZE - 1] == board[i + 2][SIZE - 1] &&
            board[i][SIZE - 1] != EMPTY_ID
        ) {
            winnerlist.push_back({i-2,SIZE-1});
            winnerlist.push_back({i-1,SIZE-1});
            winnerlist.push_back({i,SIZE-1});
            winnerlist.push_back({i+1,SIZE-1});
            winnerlist.push_back({i+2,SIZE-1});
            return winnerlist;
        } else if (board[i][SIZE - 2] == board[i - 2][SIZE - 2] &&
            board[i][SIZE - 2] == board[i - 1][SIZE - 2] &&
            board[i][SIZE - 2] == board[i + 1][SIZE - 2] &&
            board[i][SIZE - 2] == board[i + 2][SIZE - 2] &&
            board[i][SIZE - 2] != EMPTY_ID
        ) {
            winnerlist.push_back({i-2,SIZE-2});
            winnerlist.push_back({i-1,SIZE-2});
            winnerlist.push_back({i,SIZE-2});
            winnerlist.push_back({i+1,SIZE-2});
            winnerlist.push_back({i+2,SIZE-2});
            return winnerlist;
        }
    }
    return winnerlist;
}
