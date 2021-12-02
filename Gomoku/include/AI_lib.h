#ifndef AI_LIB_H
#define AI_LIB_H

#include "macros.h"
#include <vector>
#include <array>
#include <algorithm>
#include <limits>
#include <cmath>
using std::vector;
using std::array;

void bitboardtomatrix(unsigned  int [],unsigned int [],char[][SIZE]);  // working
void matrixtobitboard(unsigned  int pxboard[],unsigned  int poboard[],char board[][SIZE]);  //working
void addtobitboard(int,int,unsigned int[]); //working
void unmovetobitboard(int,int,unsigned int bitboard[]); //working

vector<array<int,3>> getListBitboard(unsigned int[],unsigned int[]);
void rotate90(unsigned int[]);//working
char getwinnerbitboard(unsigned int[],unsigned int[]);//working
bool check_empty(unsigned int[],unsigned int[],int,int);

int evaluate_bitboard_combined(unsigned int [],unsigned int [],char);
int evaluate_bitboard_diagonal_right(unsigned int [],unsigned int [],char );
int evaluate_bitboard_diagonal_left(unsigned int [],unsigned int [],char );

void ai_bitboard(unsigned int [],unsigned int [],int,int,char);
int alphabeta_bitboard(unsigned int[],unsigned int [],int,bool,int,char,int,int,float);

int get_score(unsigned int[],unsigned int[],char);
int addthreat(int,int,char,char);

void moveorder(unsigned int[],unsigned int[],char,bool,vector<array<int,3>>&);
bool comparator_descending(std::array<int,3> ,std::array<int,3> );
bool comparator_ascending(std::array<int,3> ,std::array<int,3> );

vector<array<int,2>> winnerbuttons(unsigned int [],unsigned int [],vector<array<int,2>>& );
#endif // AI_LIB_H
