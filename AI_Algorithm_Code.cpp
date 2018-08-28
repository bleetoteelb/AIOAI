// Samsung Go Tournament Form C (g++-4.8.3)
//hi
/*
[AI 코드 작성 방법]
1. char info[]의 배열 안에					"TeamName:자신의 팀명,Department:자신의 소속"					순서로 작성합니다.
( 주의 ) Teamname:과 Department:는 꼭 들어가야 합니다.
"자신의 팀명", "자신의 소속"을 수정해야 합니다.
2. 아래의 myturn() 함수 안에 자신만의 AI 코드를 작성합니다.
3. AI 파일을 테스트 하실 때는 "육목 알고리즘대회 툴"을 사용합니다.
4. 육목 알고리즘 대회 툴의 연습하기에서 바둑돌을 누른 후, 자신의 "팀명" 이 들어간 알고리즘을 추가하여 테스트 합니다.
[변수 및 함수]
myturn(int cnt) : 자신의 AI 코드를 작성하는 메인 함수 입니다.
int cnt (myturn()함수의 파라미터) : 돌을 몇 수 둬야하는지 정하는 변수, cnt가 1이면 육목 시작 시  한 번만  두는 상황(한 번), cnt가 2이면 그 이후 돌을 두는 상황(두 번)
int  x[0], y[0] : 자신이 둘 첫 번 째 돌의 x좌표 , y좌표가 저장되어야 합니다.
int  x[1], y[1] : 자신이 둘 두 번 째 돌의 x좌표 , y좌표가 저장되어야 합니다.
void domymove(int x[], int y[], cnt) : 둘 돌들의 좌표를 저장해서 출력
//int board[BOARD_SIZE][BOARD_SIZE]; 바둑판 현재상황 담고 있어 바로사용 가능함. 단, 원본데이터로 수정 절대금지
// 놓을수 없는 위치에 바둑돌을 놓으면 실격패 처리.
boolean ifFree(int x, int y) : 현재 [x,y]좌표에 바둑돌이 있는지 확인하는 함수 (없으면 true, 있으면 false)
int showBoard(int x, int y) : [x, y] 좌표에 무슨 돌이 존재하는지 보여주는 함수 (1 = 자신의 돌, 2 = 상대의 돌, 3 = 블럭킹)
<-------AI를 작성하실 때, 같은 이름의 함수 및 변수 사용을 권장하지 않습니다----->
*/

#include <stdio.h>
//#include "Windows.h"
#include <time.h>
//#include "Connect6Algo.h"
#include <string>
#include <vector> 
#include <cstdlib>

#define BOARDSIZE 19
#define BOARDLINE 112
#define CASESIZE 361
/**************************************/
/**************************************/
/**                                  **/
/**     structure, class 선언         **/
/**                                  **/
/**************************************/
/**************************************/
float Infinity = 0.1;
struct Evaluation{
	int pattern[9];
	int pattern_num; // 패턴의 개수를 의미함
	int value; // evaluation score을 의미함.
};

struct Locations{
	int size;
	int loc[10][2];
	Locations() :size(0){}
	void push(int x, int y){
		loc[size][0] = x;
		loc[size][1] = y;
		size++;
	}

	void pop(int a){
		size -=a;
	}
};

struct AlphaNode{
	Locations locations;
	float value;
	AlphaNode() :value(0.1){}
};

struct Board{
	int board[BOARDSIZE][BOARDSIZE];
};

struct Boardline{
	int size;
	int boardline[19];
	Boardline() :size(0){}
	void push(int a){
		boardline[size] = a;
		size++;
	}
};

struct Boardlines{
	int size;
	Boardline boardlines[BOARDLINE];
	Boardlines() :size(0){}
	void push(int a){
		boardlines[size].push(a);
	}
	void size_up(){
		size++;
	}
};

struct sBoard{
	int shareboard[BOARDSIZE][BOARDSIZE][2];
	sBoard(){
		for (int i = 0; i<BOARDSIZE; i++){
			for (int ii = 0; ii<BOARDSIZE; ii++){
				shareboard[i][ii][0] = 0;
				shareboard[i][ii][1] = 0;
			}
		}
	}
};

struct Cases{
	int size;
	int cases[CASESIZE][2];
	Cases() :size(0){}
	void push(int x, int y){
		cases[size][0] = x;
		cases[size][1] = y;
		size++;
	}
};

struct ARRAY{
	int size;
	int array[BOARDSIZE];
	ARRAY() :size(0){}
};

struct Rocks{
	int size;
	int xy[10][2];

	Rocks() :size(0){}
	int get_x(int i) { return xy[i][0]; }
	int get_y(int i) { return xy[i][1]; }
	int get_cnt() { return size; }
	bool isEmpty() { return size == 0 ? true : false; }
	void push(int a, int b) {
		xy[size][0] = a;
		xy[size][1] = b;
		size++;
	}
};


/**************************************/
/**************************************/
/**                                  **/
/**        함수들 미리 선언           **/
/**                                  **/
/**************************************/
/**************************************/
bool check_connect7_3(int modi_board[BOARDSIZE][BOARDSIZE], int x, int y, int player);
bool check_connect7_6(int modi_board[BOARDSIZE][BOARDSIZE], int x, int y, int player);
bool check_connect7_4h(int modi_board[BOARDSIZE][BOARDSIZE], int x, int y, int player);
bool check_connect7_1h(int modi_board[BOARDSIZE][BOARDSIZE], int x, int y, int player);
int win_pattern(int a, int b, int c, int d, int e, int f, int player);
int dir_num(int dir, int xy);
Rocks* must_do_a(int x, int y, int pattern, int dir, Rocks* xy);
Rocks * must_win(Rocks* xy, int player);
bool isInf(AlphaNode a);
void getPi(Evaluation line, int* pi);
Boardlines getBoardline(Board board);
ARRAY kmp(int s[], Evaluation line, int pi[]);
Board get_board();
bool isOut(int a);
Board remove_block(Board board, int player);
Board put_rock(Locations locations, int player, Board board);
Boardlines get_Boardline_around_location(Locations new_locations, Board board);
Boardlines getBoardline(Board board);
float evaluate_score(Board board);
float evaluate_score_BNA(Board board, Locations locations, int player, float pre_totalscore);
sBoard get_share_board(Board board);
Cases get_cases(sBoard share_board);
AlphaNode minimax(Board board, sBoard share_board, float totalscore, int depth, bool isMaximizingPlayer, AlphaNode alpha, AlphaNode beta, AlphaNode alphanode);




/**************************************/
/**************************************/
/**                                  **/
/**          전역 함수 모임           **/
/**                                  **/
/**************************************/
/**************************************/



bool first = true;
// "샘플코드[C]"  -> 자신의 팀명 (수정)
// "AI부서[C]"  -> 자신의 소속 (수정)
// 제출시 실행파일은 반드시 팀명으로 제출!
char info[] = { "TeamName:AIOAI,Department:UNIST" };
int BOARD[BOARDSIZE][BOARDSIZE];
int DEPTH = 2;
const Evaluation score[45] = { { { 1, 1, 1, 1, 1, 1, 4, 4, 4 }, 6, 100000000 },
{ { 0, 1, 1, 1, 1, 0, 4, 4, 4 }, 6, 471125 },
{ { 0, 1, 0, 1, 1, 1, 0, 1, 0 }, 9, 178408 },
{ { 0, 1, 0, 1, 1, 1, 0, 0, 1 }, 9, 178408 },
{ { 1, 0, 0, 1, 1, 1, 0, 1, 0 }, 9, 968445 },
{ { 1, 0, 0, 1, 1, 1, 0, 0, 1 }, 9, 136975 },
{ { 0, 0, 1, 1, 1, 1, 0, 0, 4 }, 8, 958631 },
{ { 1, 1, 1, 1, 1, 0, 4, 4, 4 }, 6, 224804 },
{ { 0, 1, 1, 1, 1, 1, 4, 4, 4 }, 6, 224804 },
{ { 1, 0, 1, 1, 1, 1, 4, 4, 4 }, 6, 745729 },
{ { 1, 1, 1, 1, 0, 1, 4, 4, 4 }, 6, 745729 },
{ { 1, 1, 0, 1, 1, 1, 4, 4, 4 }, 6, 146434 },
{ { 1, 1, 1, 0, 1, 1, 4, 4, 4 }, 6, 146434 },
{ { 1, 0, 0, 1, 1, 1, 1, 4, 4 }, 7, 653420 },
{ { 1, 1, 1, 1, 0, 0, 1, 4, 4 }, 7, 653420 },
{ { 1, 1, 0, 0, 1, 1, 1, 4, 4 }, 7, 406632 },
{ { 1, 1, 1, 0, 0, 1, 1, 4, 4 }, 7, 406632 },
{ { 1, 0, 1, 0, 1, 1, 1, 4, 4 }, 7, 752847 },
{ { 1, 1, 1, 0, 1, 0, 1, 4, 4 }, 7, 752847 },
{ { 1, 1, 0, 1, 1, 0, 1, 4, 4 }, 7, 835834 },
{ { 1, 0, 1, 1, 0, 1, 1, 4, 4 }, 7, 835834 },
{ { 1, 0, 1, 1, 1, 0, 1, 4, 4 }, 7, 180915 },
{ { 1, 1, 0, 1, 0, 1, 1, 4, 4 }, 7, 170667 },
{ { 1, 1, 1, 1, 0, 0, 4, 4, 4 }, 6, 965425 },
{ { 0, 0, 1, 1, 1, 1, 4, 4, 4 }, 6, 965425 },
{ { 1, 1, 1, 0, 0, 1, 4, 4, 4 }, 6, 294247 },
{ { 1, 0, 0, 1, 1, 1, 4, 4, 4 }, 6, 294247 },
{ { 1, 1, 0, 0, 1, 1, 4, 4, 4 }, 6, 308961 },
{ { 1, 0, 1, 0, 1, 1, 4, 4, 4 }, 6, 296802 },
{ { 1, 1, 0, 1, 0, 1, 4, 4, 4 }, 6, 296802 },
{ { 1, 0, 1, 1, 0, 1, 4, 4, 4 }, 6, 919855 },
{ { 0, 0, 1, 1, 1, 0, 0, 0, 4 }, 8, 916206 },
{ { 0, 0, 0, 1, 1, 1, 0, 0, 4 }, 8, 916206 },
{ { 0, 0, 1, 1, 0, 1, 0, 0, 4 }, 8, 378982 },
{ { 0, 0, 1, 0, 1, 1, 0, 0, 4 }, 8, 378982 },
{ { 0, 0, 0, 0, 1, 1, 0, 1, 0 }, 9, 262479 },
{ { 0, 1, 0, 1, 1, 0, 0, 0, 0 }, 9, 262479 },
{ { 0, 0, 0, 0, 1, 1, 0, 0, 1 }, 9, 771643 },
{ { 1, 0, 0, 1, 1, 0, 0, 0, 0 }, 9, 771643 },
{ { 0, 0, 0, 1, 1, 0, 0, 1, 0 }, 9, 28720 },
{ { 0, 1, 0, 0, 1, 1, 0, 0, 0 }, 9, 28720 },
{ { 0, 0, 0, 1, 1, 0, 0, 1, 0 }, 9, 437413 },
{ { 0, 1, 0, 1, 0, 1, 0, 0, 0 }, 9, 437413 },
{ { 1, 1, 1, 0, 0, 0, 4, 4, 4 }, 6, 625778 },
{ { 0, 0, 0, 1, 1, 1, 4, 4, 4 }, 6, 625778 } };

Evaluation score2[45] = { { { 2, 2, 2, 2, 2, 2, 4, 4, 4 }, 6, 100000000 },
{ { 0, 2, 2, 2, 2, 0, 4, 4, 4 }, 6, 471125 },
{ { 0, 2, 0, 2, 2, 2, 0, 2, 0 }, 9, 178408 },
{ { 0, 2, 0, 2, 2, 2, 0, 0, 2 }, 9, 178408 },
{ { 2, 0, 0, 2, 2, 2, 0, 2, 0 }, 9, 968445 },
{ { 2, 0, 0, 2, 2, 2, 0, 0, 2 }, 9, 136975 },
{ { 0, 0, 2, 2, 2, 2, 0, 0, 4 }, 8, 958631 },
{ { 2, 2, 2, 2, 2, 0, 4, 4, 4 }, 6, 224804 },
{ { 0, 2, 2, 2, 2, 2, 4, 4, 4 }, 6, 224804 },
{ { 2, 0, 2, 2, 2, 2, 4, 4, 4 }, 6, 745729 },
{ { 2, 2, 2, 2, 0, 2, 4, 4, 4 }, 6, 745729 },
{ { 2, 2, 0, 2, 2, 2, 4, 4, 4 }, 6, 146434 },
{ { 2, 2, 2, 0, 2, 2, 4, 4, 4 }, 6, 146434 },
{ { 2, 0, 0, 2, 2, 2, 2, 4, 4 }, 7, 653420 },
{ { 2, 2, 2, 2, 0, 0, 2, 4, 4 }, 7, 653420 },
{ { 2, 2, 0, 0, 2, 2, 2, 4, 4 }, 7, 406632 },
{ { 2, 2, 2, 0, 0, 2, 2, 4, 4 }, 7, 406632 },
{ { 2, 0, 2, 0, 2, 2, 2, 4, 4 }, 7, 752847 },
{ { 2, 2, 2, 0, 2, 0, 2, 4, 4 }, 7, 752847 },
{ { 2, 2, 0, 2, 2, 0, 2, 4, 4 }, 7, 835834 },
{ { 2, 0, 2, 2, 0, 2, 2, 4, 4 }, 7, 835834 },
{ { 2, 0, 2, 2, 2, 0, 2, 4, 4 }, 7, 180915 },
{ { 2, 2, 0, 2, 0, 2, 2, 4, 4 }, 7, 170667 },
{ { 2, 2, 2, 2, 0, 0, 4, 4, 4 }, 6, 965425 },
{ { 0, 0, 2, 2, 2, 2, 4, 4, 4 }, 6, 965425 },
{ { 2, 2, 2, 0, 0, 2, 4, 4, 4 }, 6, 294247 },
{ { 2, 0, 0, 2, 2, 2, 4, 4, 4 }, 6, 294247 },
{ { 2, 2, 0, 0, 2, 2, 4, 4, 4 }, 6, 308961 },
{ { 2, 0, 2, 0, 2, 2, 4, 4, 4 }, 6, 296802 },
{ { 2, 2, 0, 2, 0, 2, 4, 4, 4 }, 6, 296802 },
{ { 2, 0, 2, 2, 0, 2, 4, 4, 4 }, 6, 919855 },
{ { 0, 0, 2, 2, 2, 0, 0, 0, 4 }, 8, 916206 },
{ { 0, 0, 0, 2, 2, 2, 0, 0, 4 }, 8, 916206 },
{ { 0, 0, 2, 2, 0, 2, 0, 0, 4 }, 8, 378982 },
{ { 0, 0, 2, 0, 2, 2, 0, 0, 4 }, 8, 378982 },
{ { 0, 0, 0, 0, 2, 2, 0, 2, 0 }, 9, 262479 },
{ { 0, 2, 0, 2, 2, 0, 0, 0, 0 }, 9, 262479 },
{ { 0, 0, 0, 0, 2, 2, 0, 0, 2 }, 9, 771643 },
{ { 2, 0, 0, 2, 2, 0, 0, 0, 0 }, 9, 771643 },
{ { 0, 0, 0, 2, 2, 0, 0, 2, 0 }, 9, 28720 },
{ { 0, 2, 0, 0, 2, 2, 0, 0, 0 }, 9, 28720 },
{ { 0, 0, 0, 2, 2, 0, 0, 2, 0 }, 9, 437413 },
{ { 0, 2, 0, 2, 0, 2, 0, 0, 0 }, 9, 437413 },
{ { 2, 2, 2, 0, 0, 0, 4, 4, 4 }, 6, 625778 },
{ { 0, 0, 0, 2, 2, 2, 4, 4, 4 }, 6, 625778 } };


/**************************************/
/**************************************/
/**                                  **/
/**          실제 함수 시작           **/
/**                                  **/
/**************************************/
/**************************************/

/**************************************/
/*                                    */
/*          check_connect7            */
/*                                    */
/**************************************/
// 함수를 하나로 합치고 싶은데..흠
// 가로방향 (3시방향) 칠목체크
bool check_connect7_3(int modi_board[BOARDSIZE][BOARDSIZE], int x, int y, int player){
	if (isOut(x - 1) && modi_board[x + 6][y] != player) return true;
	else if (isOut(x + 6) && modi_board[x - 1][y] != player)	return true;
	else if (modi_board[x - 1][y] != player&&modi_board[x + 6][y] != player)	return true;
	else return false;
}
// 세로방향 (6시방향) 칠목체크
bool check_connect7_6(int modi_board[BOARDSIZE][BOARDSIZE], int x, int y, int player){
	if (isOut(x - 1) && modi_board[x][y + 6] != player) return true;
	else if (isOut(y + 6) && modi_board[x][y - 1] != player)	return true;
	else if (modi_board[x][y - 1] != player&&modi_board[x][y + 6] != player)	return true;
	else return false;
}
// 우하향방향 (4시반방향) 칠목체크
bool check_connect7_4h(int modi_board[BOARDSIZE][BOARDSIZE], int x, int y, int player){
	if (isOut(x - 1) && isOut(y - 1) && modi_board[x + 6][y + 6] != player) return true;
	else if (isOut(x + 6) && isOut(y + 6) && modi_board[x - 1][y - 1] != player)	return true;
	else if (modi_board[x - 1][y - 1] != player&&modi_board[x + 6][y + 6] != player)	return true;
	else return false;
}
// 우상향방향 (2시반방향) 칠목체크
bool check_connect7_1h(int modi_board[BOARDSIZE][BOARDSIZE], int x, int y, int player){
	if (isOut(x - 1) && isOut(y + 1) && modi_board[x + 6][y - 6] != player) return true;
	else if (isOut(x + 6) && isOut(y - 6) && modi_board[x - 1][y + 1] != player)	return true;
	else if (modi_board[x - 1][y + 1] != player&&modi_board[x + 6][y - 6] != player)	return true;
	else return false;
}

/*
필승 수 넘버링
1:  001111
2:  010111
3:  011011
4:  011101
5:  011110
6:  011111
7:  100111
8:  101011
9:  101101
10: 101110
11: 101111
12: 110011
13: 110101
14: 110110
15: 110111
16: 111001
17: 111010
18: 111011
19: 111100
20: 111101
21: 111110
*/

/**************************************/
/*                                    */
/*            win_pattern             */
/*                                    */
/**************************************/
int win_pattern(int a, int b, int c, int d, int e, int f, int player){
	int candi[6] = { a, b, c, d, e, f };
	if (candi[0] == 0){
		if (candi[1] == 0){
			if (candi[2] == player&&candi[3] == player&&candi[4] == player&&candi[5] == player){
				return 1;
			}
		}
		else if (candi[1] == player){
			if (candi[2] == 0 && candi[3] == player&&candi[4] == player&&candi[5] == player){
				return 2;
			}
			else if (candi[2] == player){
				if (candi[3] == 0 && candi[4] == player&&candi[5] == player){
					return 3;
				}
				else if (candi[3] == player&&candi[4] == 0 && candi[5] == player){
					return 4;
				}
				else if (candi[3] == player&&candi[4] == player&&candi[5] == 0){
					return 5;
				}
				else if (candi[3] == player&&candi[4] == player&&candi[5] == player){
					return 6;
				}
			}
		}
	}
	else if (candi[0] == player){
		if (candi[1] == 0){
			if (candi[2] == 0 && candi[3] == player&&candi[4] == player&&candi[5] == player){
				return 7;
			}
			else if (candi[2] == player){
				if (candi[3] == 0 && candi[4] == player&&candi[5] == player){
					return 8;
				}
				else if (candi[3] == player&&candi[4] == 0 && candi[5] == player){
					return 9;
				}
				else if (candi[3] == player&&candi[4] == player&&candi[5] == 0){
					return 10;
				}
				else if (candi[3] == player&&candi[4] == player&&candi[5] == player){
					return 11;
				}
			}
		}
		else if (candi[1] == player){
			if (candi[2] == 0){
				if (candi[3] == 0 && candi[4] == player&&candi[5] == player){
					return 12;
				}
				else if (candi[3] == player&&candi[4] == 0 && candi[5] == player){
					return 13;
				}
				else if (candi[3] == player&&candi[4] == player&&candi[5] == 0){
					return 14;
				}
				else if (candi[3] == player&&candi[4] == player&&candi[5] == player){
					return 15;
				}
			}
			else if (candi[2] == 1){
				if (candi[3] == 0 && candi[4] == 0 && candi[5] == player){
					return 16;
				}
				else if (candi[3] == 0 && candi[4] == player&&candi[5] == 0){
					return 17;
				}
				else if (candi[3] == 0 && candi[4] == player&&candi[5] == player){
					return 18;
				}
				else if (candi[3] == player&&candi[4] == 0 && candi[5] == 0){
					return 19;
				}
				else if (candi[3] == player&&candi[4] == 0 && candi[5] == player){
					return 20;
				}
				else if (candi[3] == player&&candi[4] == player&&candi[5] == 0){
					return 21;
				}
			}
		}
	}
	return 0;
}

/**************************************/
/*                                    */
/*              dir_num               */
/*                                    */
/**************************************/
// 방향에 따라서 xy에 더하고 빼는 값을 반환하는 함수
// xy에 들어가는건 x:1, y:0
int dir_num(int dir, int xy){
	if (xy){
		switch (dir){
		case 3: return 1;
		case 6: return 0;
		case 4: return 1;
		case 1: return 1;
		}
	}
	else{
		switch (dir){
		case 3: return 0;
		case 6: return 1;
		case 4: return -1;
		case 1: return 1;
		}
	}
}

/**************************************/
/*                                    */
/*            must_do_a               */
/*                                    */
/**************************************/
Rocks* must_do_a(int x, int y, int pattern, int dir, Rocks* xy){
	int cnt = 2;
	switch (pattern){
	case 1: // 001111
		xy->push(x, y); xy->push(x + dir_num(dir, x), y + dir_num(dir, y)); cnt -= 2; break;
	case 2: // 010111
		xy->push(x, y); xy->push(x + 2 * dir_num(dir, x), y + 2 * dir_num(dir, y)); cnt -= 2; break;
	case 3: // 011011
		xy->push(x, y); xy->push(x + 3 * dir_num(dir, x), y + 3 * dir_num(dir, y)); cnt -= 2; break;
	case 4: // 011101
		xy->push(x, y); xy->push(x + 4 * dir_num(dir, x), y + 4 * dir_num(dir, y)); cnt -= 2; break;
	case 5: // 011110
		xy->push(x, y); xy->push(x + 5 * dir_num(dir, x), y + 5 * dir_num(dir, y)); cnt -= 2; break;
	case 6: // 011111
		xy->push(x, y); cnt -= 1; break;
	case 7: // 100111
		xy->push(x + dir_num(dir, x), y + dir_num(dir, y)); xy->push(x + 2 * dir_num(dir, x), y + 2 * dir_num(dir, y)); cnt -= 2; break;
	case 8: // 101011
		xy->push(x + dir_num(dir, x), y + dir_num(dir, y)); xy->push(x + 3 * dir_num(dir, x), y + 3 * dir_num(dir, y)); cnt -= 2; break;
	case 9: // 101101
		xy->push(x + dir_num(dir, x), y + dir_num(dir, y)); xy->push(x + 4 * dir_num(dir, x), y + 4 * dir_num(dir, y)); cnt -= 2; break;
	case 10: // 101110
		xy->push(x + dir_num(dir, x), y + dir_num(dir, y)); xy->push(x + 5 * dir_num(dir, x), y + 5 * dir_num(dir, y)); cnt -= 2; break;
	case 11: // 101111
		xy->push(x + dir_num(dir, x), y + dir_num(dir, y)); cnt -= 1; break;
	case 12: // 110011
		xy->push(x + 2 * dir_num(dir, x), y + 2 * dir_num(dir, y)); xy->push(x + 3 * dir_num(dir, x), y + 3 * dir_num(dir, y)); cnt -= 2; break;
	case 13: // 110101
		xy->push(x + 2 * dir_num(dir, x), y + 2 * dir_num(dir, y)); xy->push(x + 4 * dir_num(dir, x), y + 4 * dir_num(dir, y)); cnt -= 2; break;
	case 14: // 110110
		xy->push(x + 2 * dir_num(dir, x), y + 2 * dir_num(dir, y)); xy->push(x + 5 * dir_num(dir, x), y + 5 * dir_num(dir, y)); cnt -= 2; break;
	case 15: // 110111
		xy->push(x + 2 * dir_num(dir, x), y + 2 * dir_num(dir, y)); cnt -= 1; break;
	case 16: // 111001
		xy->push(x + 3 * dir_num(dir, x), y + 3 * dir_num(dir, y)); xy->push(x + 4 * dir_num(dir, x), y + 4 * dir_num(dir, y)); cnt -= 2; break;
	case 17: // 111010
		xy->push(x + 3 * dir_num(dir, x), y + 3 * dir_num(dir, y)); xy->push(x + 5 * dir_num(dir, x), y + 5 * dir_num(dir, y)); cnt -= 2; break;
	case 18: // 111011
		xy->push(x + 3 * dir_num(dir, x), y + 3 * dir_num(dir, y)); cnt -= 1; break;
	case 19: // 111100
		xy->push(x + 4 * dir_num(dir, x), y + 4 * dir_num(dir, y)); xy->push(x + 5 * dir_num(dir, x), y + 5 * dir_num(dir, y)); cnt -= 2; break;
	case 20: // 111101
		xy->push(x + 4 * dir_num(dir, x), y + 4 * dir_num(dir, y)); cnt -= 1; break;
	case 21: // 111110
		xy->push(x + 5 * dir_num(dir, x), y + 5 * dir_num(dir, y)); cnt -= 1; break;
	}
	return xy;
}

/**************************************/
/*                                    */
/*             must_win               */
/*                                    */
/**************************************/
// 필승 수 확인
// player는 1이 나 2가 상대방
Rocks * must_win(Rocks* xy, int player){
	int op = player == 1 ? 2 : 1;

	int modi_board[BOARDSIZE][BOARDSIZE];
	for (int i = 0; i<BOARDSIZE; i++){
		for (int j = 0; j<BOARDSIZE; j++){
			if (BOARD[j][i] == 3)	modi_board[j][i] = player;
			else modi_board[j][i] = BOARD[j][i];
		}
	}
	int pattern = 0;

	for (int y = 0; y<BOARDSIZE - 5; y++){
		for (int x = 0; x<BOARDSIZE - 5; x++){
			if (modi_board[x][y] == op) continue;
			else{
				// 우
				if (pattern = win_pattern(modi_board[x][y], modi_board[x + 1][y], modi_board[x + 2][y], modi_board[x + 3][y], modi_board[x + 4][y], modi_board[x + 5][y], player)){
					if (player == 1){
						if (check_connect7_3(modi_board, x, y, player)) return must_do_a(x, y, pattern, 3, xy);
					}
					else{
						//must_do_d(x, y, pattern, 3, xy);
					}
				}
				// 하
				else if (pattern = win_pattern(modi_board[x][y], modi_board[x][y + 1], modi_board[x][y + 2], modi_board[x][y + 3], modi_board[x][y + 4], modi_board[x][y + 5], player)){
					if (player == 1){
						if (check_connect7_6(modi_board, x, y, player)) return must_do_a(x, y, pattern, 6, xy);
					}
					else{
						//must_do_d(x, y, pattern, 6, xy);
					}
				}
				// 우하
				else if (pattern = win_pattern(modi_board[x][y], modi_board[x + 1][y + 1], modi_board[x + 2][y + 2], modi_board[x + 3][y + 3], modi_board[x + 4][y + 4], modi_board[x + 5][y + 5], player)){
					if (player == 1){
						if (check_connect7_4h(modi_board, x, y, player)) return must_do_a(x, y, pattern, 4, xy);
					}
					else{
						//must_do_d(x, y, pattern, 4, xy);
					}
				}
				// 우상
				else if (pattern = win_pattern(modi_board[x][y], modi_board[x + 1][y - 1], modi_board[x + 2][y - 2], modi_board[x + 3][y - 3], modi_board[x + 4][y - 4], modi_board[x + 5][y - 5], player)){
					if (player == 1){
						if (check_connect7_1h(modi_board, x, y, player)) return must_do_a(x, y, pattern, 1, xy);
					}
					else{
						//must_do_d(x, y, pattern, 1, xy);
					}
				}
			}
		}
	}
	return xy;
}

/**************************************/
/*                                    */
/*               isInf                */
/*                                    */
/**************************************/
bool isInf(AlphaNode a){
	return (a.value == Infinity) ? true : false;
}

/**************************************/
/*                                    */
/*               getPi                */
/*                                    */
/**************************************/
void getPi(Evaluation line, int* pi){
	int j = 0;
	int length = (int)(line.pattern_num);
	for (int i = 1; i< length; i++){
		while (j > 0 && line.pattern[i] != line.pattern[j]){
			j = pi[j - 1];
		}
		if (line.pattern[i] == line.pattern[j])
			pi[i] = ++j;
	}
	// return pi; 
}

/**************************************/
/*                                    */
/*                kmp                 */
/*                                    */
/***************s***********************/
ARRAY kmp(Boardline s, Evaluation line, int pi[]){   //kmp 알고리즘  -> 긴 나열속에서 주어진 패턴을 찾는것.
	ARRAY ans;
	ans.size = 0;
	if (s.size<line.pattern_num) return ans;

	// pi = getPi(line,pi); 
	int n = s.size;
	int m = line.pattern_num;
	int j = 0;
	for (int i = 0; i < n; i++){
		while (j>0 && s.boardline[i] != line.pattern[j])
			j = pi[j - 1];
		if (s.boardline[i] == line.pattern[j]){
			if (j == m - 1){
				ans.array[ans.size] = (i - m + 1);
				ans.size++;
				j = pi[j];
			}
			else{
				j++;
			}
		}
	}
	return ans;  // 이 answer에 대해서는 딱히 위치가 중요하진 않기 때문에 변수타입을 내가 필요한 것만 넣을수있도록 바꾸어 주자.
}

/**************************************/
/*                                    */
/*             get_board              */
/*                                    */
/**************************************/
Board get_board(){
	Board board;
	for (int i = 0; i<BOARDSIZE; i++){ // 블로킹 돌을 내돌로 인식하도록 함.
		for (int j = 0; j<BOARDSIZE; j++){
			board.board[j][i] = BOARD[j][i];
		}
	}
	return board;
}


// 해당 좌표가 판 안에 있는지 확인
// 넘어기면 true반환, 안에 있으면 false 반환.
bool isOut(int a){
	return (a>18 || a<0) ? true : false;
}

void print_board(){
	for (int i = 0; i<BOARDSIZE; i++){
		for (int j = 0; j<BOARDSIZE; j++){
			std::string text;
			text = std::to_string(showBoard(j, i));
			const char* hi1 = text.c_str();
			char* hi2 = (char*)(hi1);
			writeLog(hi2);
		}
		writeLog("\n");
	}
}

void print_board(float score){

	std::string text;
	text = std::to_string(score);
	const char* hi1 = text.c_str();
	char* hi2 = (char*)(hi1);
	writeLog(hi2);

	writeLog("\n");
}

/**************************************/
/*                                    */
/*           remove_block             */
/*                                    */
/**************************************/
//board의 3을 1로 표시
Board remove_block(Board board, int player){
	for (int i = 0; i<BOARDSIZE; i++){ // 블로킹 돌을 내돌로 인식하도록 함.
		for (int j = 0; j<BOARDSIZE; j++){
			if (board.board[j][i] == 3){
				board.board[j][i] = player;
			}
		}
	}
	return board;
}

/**************************************/
/*                                    */
/*       put_rock - 2D vector         */
/*                                    */
/**************************************/
// 임시로 돌을 놓는다. //player : 1은 나 2는 상대
Board put_rock(Locations locations, int player, Board board){
	// x,y위치에 돌을 놓는다.  player로 어떤 돌을 놓을지 결정한다.
	//놓으려는 x y위치가 free한 위치인지 확인한다.
	for (int i = 0; i<locations.size; i++){
		if (isFree(locations.loc[i][0], locations.loc[i][1])){
			board.board[locations.loc[i][0]][locations.loc[i][1]] = player;
		}
	}
	return board;
}

/**************************************/
/*                                    */
/*    get_Boardline_around_location   */
/*                                    */
/**************************************/
//돌을 놓기 이전의 boardline을 넘겨줌. board는 보드 판을 넘겨주고, new_location은 새로 놓을 돌을 놓는다. 형식은 {{x,y},{x,y}} , 새로 넣을 좌표의 개수는 상관없음. 
//put rock 하기 전의 board를 parameter에 넣으면 이전판의 boardline을 리턴할 것이고, put rock 한 이후의 board를 parameter에 넣으면 이후판의 boardline을 return할것이다.
Boardlines get_Boardline_around_location(Locations new_locations, Board board){
	Boardlines boardline;
	print_board((float)new_locations.size);
	for (int location_num = 0; location_num<new_locations.size; location_num++){
		print_board((float)location_num);
		
		writeLog("aabb1\n");
		bool InSameLine = false;
		for (int previous_location_num = 0; previous_location_num<location_num; previous_location_num++){
			if (new_locations.loc[previous_location_num][0] == new_locations.loc[location_num][0]) InSameLine = true;  // 세로부분을 발췌할때 이전에 발췌한 세로부분과 동일한 부분인지 확인을 한다.
			print_board((float)previous_location_num);
		}
		writeLog("aabbcc1\n");
		if (!InSameLine)	{
			// 세로부분 발췌 ↓
			writeLog("aabbcc2\n");
			for (int i = 0; i<BOARDSIZE; i++){
				print_board((float)board.board[new_locations.loc[location_num][0]][i]);
				boardline.push(board.board[new_locations.loc[location_num][0]][i]);
				print_board((float)boardline.size);
				writeLog("aabbcc3\n");
			}
			print_board((float)boardline.size);
			boardline.size_up();
			print_board((float)boardline.size);
		}
		writeLog("aabb2\n");
		InSameLine = false;
		for (int previous_location_num = 0; previous_location_num<location_num; previous_location_num++){
			if (new_locations.loc[previous_location_num][1] == new_locations.loc[location_num][1]) InSameLine = true;  // 가로부분을 발췌할때 이전에 발췌한 가로부분과 동일한 부분인지 확인을 한다.
		}
		if (!InSameLine){
			//가로부분 발췌 →
			for (int i = 0; i<BOARDSIZE; i++){
				boardline.push(board.board[i][new_locations.loc[location_num][1]]);
			}
			boardline.size_up();
		}
		writeLog("aabb3\n");
		InSameLine = false;
		for (int previous_location_num = 0; previous_location_num<location_num; previous_location_num++){
			if (new_locations.loc[location_num][1] - new_locations.loc[previous_location_num][1] == new_locations.loc[location_num][0] - new_locations.loc[previous_location_num][0]) InSameLine = true;  // ↘부분을 발췌할때 이전에 발췌한 ↘부분과 동일한 부분인지 확인을 한다.
		}
		if (!InSameLine){
			int start_x = new_locations.loc[location_num][0], start_y = new_locations.loc[location_num][1], end_x = new_locations.loc[location_num][0], end_y = new_locations.loc[location_num][1];
			int min = (start_x < start_y) ? start_x : start_y;
			start_x -= min;		start_y -= min;
			min = ((BOARDSIZE - end_x) < BOARDSIZE - end_y) ? end_x : end_y;
			end_x += min;		end_y += min;

			for (int i = start_x, j = start_y; i<end_x, j<end_y; i++, j++){
				boardline.push(board.board[i][j]);
			}
			boardline.size_up();   // ↘부분을 발췌
		}
		writeLog("aabb4\n");
		InSameLine = false;
		for (int previous_location_num = 0; previous_location_num<location_num; previous_location_num++){
			if (new_locations.loc[location_num][1] - new_locations.loc[previous_location_num][1] == -(new_locations.loc[location_num][0] - new_locations.loc[previous_location_num][0])) InSameLine = true;  // ↗부분을 발췌할때 이전에 발췌한 ↗부분과 동일한 부분인지 확인을 한다.
		}
		if (!InSameLine){
			int start_x = new_locations.loc[location_num][0], start_y = new_locations.loc[location_num][1], end_x = new_locations.loc[location_num][0], end_y = new_locations.loc[location_num][1];
			int min = (start_x < start_y) ? start_x : start_y;
			start_x -= min;		start_y += min;
			min = ((BOARDSIZE - end_x) < BOARDSIZE - end_y) ? end_x : end_y;
			end_x += min;		end_y -= min;

			for (int i = start_x, j = start_y; i<end_x, j>end_y; i++, j--){
				boardline.push(board.board[i][j]);
			}
			boardline.size_up();   // ↗부분을 발췌
		}
	}writeLog("aabb5\n");
	return boardline;
}

/**************************************/
/*                                    */
/*            get_Boardline           */
/*                                    */
/**************************************/
// board[x][y] 기준으로 작성됨
// 전체 boardline을 발췌함
Boardlines getBoardline(Board board){  // 보드에 있는 
	Boardlines boardline;

	for (int i = 0; i<BOARDSIZE; i++){ //가로부분 발췌 →
		for (int j = 0; j<BOARDSIZE; j++){
			boardline.push(board.board[j][i]);
		}
		boardline.size_up();
	}
	for (int i = 0; i<BOARDSIZE; i++){ //세로부분 발췌 ↓
		for (int j = 0; j<BOARDSIZE; j++){
			boardline.push(board.board[i][j]);
		}
		boardline.size_up();
	}
	for (int i = 0; i<BOARDSIZE; i++){ // 대각선 남서부분 발췌 ↘
		for (int j = i, k = 0; j<BOARDSIZE; j++, k++){
			boardline.push(board.board[k][j]);
		}
		boardline.size_up();
	}
	for (int i = 0; i<BOARDSIZE; i++){ // 대각선 남동부분 발췌 ↙
		std::vector<int> temp;
		for (int j = i, k = BOARDSIZE - 1; j<BOARDSIZE; j++, k--){
			boardline.push(board.board[k][j]);
		}
		boardline.size_up();
	}
	for (int i = 1; i<BOARDSIZE; i++){ // 대각선 북동부분 발췌 ↘
		std::vector<int> temp;
		for (int j = 0, k = i; j<BOARDSIZE - i; j++, k++){
			boardline.push(board.board[k][j]);
		}
		boardline.size_up();
	}
	for (int i = 0; i<BOARDSIZE - 1; i++){ // 대각선 북서부분 발췌 ↗
		std::vector<int> temp;
		for (int j = i, k = 0; j >= 0; j--, k++){
			boardline.push(board.board[k][j]);
		}
		boardline.size_up();
	}
	return boardline;
}

/**************************************/
/*                                    */
/*           evaluate_score           */
/*                                    */
/**************************************/
float evaluate_score(Board board){   // 여기서 블로킹 처리가 필요함.
	Boardlines boardline;
	boardline = getBoardline(board);
	float totalscore = 0;
	int *pi;
	for (int a = 0; a<45; a++){
		int sum = 0;
		pi = new int[score[a].pattern_num];
		for (int b = 0; b<score[a].pattern_num; b++){
			pi[b] = 0;
		}
		getPi(score[a], pi);
		for (int c = 0; c<boardline.size; c++){
			ARRAY temp = kmp(boardline.boardlines[c], score[a], pi);
			ARRAY temp2 = kmp(boardline.boardlines[c], score2[a], pi);
			sum += temp.size;
			sum -= temp2.size;
		}
		totalscore += sum * score[a].value;
		delete[] pi;
	}
	return totalscore;
}

/**************************************/
/*                                    */
/*        evaluate_score_BNA          */
/*                                    */
/**************************************/
float evaluate_score_BNA(Board board, Locations locations, int player, float pre_totalscore){
	Boardlines boardline_B; // 돌을 놓기 전 돌이 있는 라인들의 패턴
	Boardlines boardline_A; // 돌을 놓은 후 돌이 있는 라인들의 패턴
	Board board_A; // 돌을 놓은 후 board
	float cur_totalscore = pre_totalscore;
	writeLog("aa\n");
	boardline_B = get_Boardline_around_location(locations, board);
	writeLog("aabb\n");
	board_A = put_rock(locations, player, board);
	writeLog("bb\n");
	boardline_A = get_Boardline_around_location(locations, board_A);
	writeLog("cc\n");
	int* pi;
	for (int a = 0; a<45; a++){
		int sum = 0;
		pi = new int[score[a].pattern_num];
		for (int b = 0; b<score[a].pattern_num; b++){
			pi[b] = 0;
		}
		getPi(score[a], pi);
		for (int c = 0; c<boardline_B.size; c++){
			ARRAY temp1 = kmp(boardline_B.boardlines[c], score[a], pi); // before_my
			ARRAY temp2 = kmp(boardline_B.boardlines[c], score[a], pi); // before_op
			sum -= temp1.size;
			sum += temp2.size;
		}
		for (int c = 0; c<boardline_A.size; c++){
			ARRAY temp1 = kmp(boardline_A.boardlines[c], score2[a], pi); // after_my
			ARRAY temp2 = kmp(boardline_A.boardlines[c], score2[a], pi); // after_op
			sum += temp1.size;
			sum -= temp2.size;
		}
		cur_totalscore += sum * score[a].value;
		delete[] pi;
	}
	writeLog("dd\n");
	return cur_totalscore;
}

/**************************************/
/*                                    */
/*          get_share_board           */
/*                                    */
/**************************************/
sBoard get_share_board(Board board){   //놓을 수 있는 경우의 수를 찾아서 리턴함.
	sBoard share_board;
	for (int player = 1; player <= 2; player++){
		int other_player;
		if (player == 1) other_player = 2;
		else other_player = 1;
		for (int y = 0; y<BOARDSIZE; y++){
			for (int x = 0; x<BOARDSIZE; x++){
				if (board.board[x][y] == player || board.board[x][y] == 3){
					int start_x, end_x, start_y, end_y;
					start_x = x - 3;
					end_x = x + 3;
					start_y = y - 3;
					end_y = y + 3;
					if (isOut(start_x)){
						start_x = 0;
					}
					if (isOut(start_y)){
						start_y = 0;
					}
					if (isOut(end_x)){
						end_x = BOARDSIZE - 1;
					}
					if (isOut(end_y)){
						end_y = BOARDSIZE - 1;
					}
					for (int num = x - 1; num >= start_x; num--){ //가로
						if (board.board[num][y] == other_player) break;
						if (board.board[num][y] == 0){
							share_board.shareboard[num][y][player - 1] += 1;
						}
					}
					for (int num = x + 1; num <= end_x; num++){ //가로
						if (board.board[num][y] == other_player) break;
						if (board.board[num][y] == 0){
							share_board.shareboard[num][y][player - 1] += 1;
						}
					}
					for (int num = y - 1; num >= start_y; num--){ //세로
						if (board.board[x][num] == other_player) break;
						if (board.board[x][num] == 0){
							share_board.shareboard[x][num][player - 1] += 1;
						}
					}
					for (int num = y + 1; num <= end_y; num++){ //세로
						if (board.board[x][num] == other_player) break;
						if (board.board[x][num] == 0){
							share_board.shareboard[x][num][player - 1] += 1;
						}
					}
					int start_RU_x, start_RU_y, end_RU_x, end_RU_y; // 우상향
					int num_x, num_y;
					if ((x - start_x) >= (end_y - y)){
						start_RU_x = x - (end_y - y);
						start_RU_y = end_y;
					}
					else{
						start_RU_y = y + (x - start_x);
						start_RU_x = start_x;
					}
					if ((end_x - x) >= (y - start_y)){
						end_RU_x = x + (y - start_y);
						end_RU_y = start_y;
					}
					else{
						end_RU_y = y - (end_x - x);
						end_RU_x = end_x;
					}
					num_x = x - 1;
					num_y = y + 1;
					while (num_x >= start_RU_x&&num_y <= start_RU_y){ // 우상향
						if (board.board[num_x][num_y] == other_player) break;
						if (board.board[num_x][num_y] == 0){
							share_board.shareboard[num_x][num_y][player - 1] += 1;
						}
						num_x--;
						num_y++;
					}
					num_x = x + 1;
					num_y = y - 1;
					while (num_x <= end_RU_x&&num_y >= end_RU_y){ // 우상향
						if (board.board[num_x][num_y] == other_player) break;
						if (board.board[num_x][num_y] == 0){
							share_board.shareboard[num_x][num_y][player - 1] += 1;
						}
						num_x++;
						num_y--;
					}
					int start_RD_x, start_RD_y, end_RD_x, end_RD_y; //우하향
					if ((x - start_x) >= (y - start_y)){
						start_RD_x = x - (y - start_y);
						start_RD_y = start_y;
					}
					else{
						start_RD_y = y - (x - start_x);
						start_RD_x = start_x;
					}
					if ((end_x - x) >= (end_y - y)){
						end_RD_x = x + (end_y - y);
						end_RD_y = end_y;
					}
					else{
						end_RD_y = y + (end_x - x);
						end_RD_x = end_x;
					}
					num_x = x - 1;
					num_y = y - 1;
					// cout << start_RD_x << " " << start_RD_y << " " << end_RD_x << " " << end_RD_y << " " << start_RU_x << " " << start_RU_y << " " << end_RU_x << " " << end_RU_y << " " << endl;
					while (num_x >= start_RD_x&&num_y >= start_RD_y){ // 우하향
						if (board.board[num_x][num_y] == other_player) break;
						if (board.board[num_x][num_y] == 0){
							share_board.shareboard[num_x][num_y][player - 1] += 1;
						}
						num_x--;
						num_y--;
					}
					num_x = x + 1;
					num_y = y + 1;
					while (num_x <= end_RD_x&&num_y <= end_RD_y){ // 우하향
						if (board.board[num_x][num_y] == other_player) break;
						if (board.board[num_x][num_y] == 0){
							share_board.shareboard[num_x][num_y][player - 1] += 1;
						}
						num_x++;
						num_y++;
					}
				}
			}
		}
	}
	return share_board;
}

/**************************************/
/*                                    */
/*             get_cases              */
/*                                    */
/**************************************/
Cases get_cases(sBoard share_board){
	Cases cases;
	for (int y = 0; y<BOARDSIZE; y++){
		for (int x = 0; x<BOARDSIZE; x++){
			if (share_board.shareboard[x][y][0] >= 2 || share_board.shareboard[x][y][1] >= 2){
				cases.push(x, y);
			}
		}
	}
	return cases;
}

Cases get_cases_by_board(Board board){
	Cases cases;
	sBoard share_board = get_share_board(board);
	for (int y = 0; y<BOARDSIZE; y++){
		for (int x = 0; x<BOARDSIZE; x++){
			if (share_board.shareboard[x][y][0] >= 2 || share_board.shareboard[x][y][1] >= 2){
				cases.push(x, y);
			}
		}
	}
	return cases;
}
int count_break1 = 0;
int count_break2 = 0;
/**************************************/
/*                                    */
/*            minimax_root            */
/*                                    */
/**************************************/
// must_num : 0/1/2 무조건 둬야 하는 돌의 개수(pair의 갯수가 아님)
AlphaNode minimax_root(Board board, Cases cases, int must_num){
	AlphaNode a_node;
	AlphaNode bestVal, alpha, beta, alphanode;
	alpha.value = -10000000000;
	beta.value = 10000000000;
	int depth = 0;

	sBoard share_board = get_share_board(board);
	float new_totalscore = evaluate_score(board);
	print_board(new_totalscore);
	writeLog("1");
	if (must_num == 0){
		for (int i = 0; i<cases.size; i++){
			for (int j = i + 1; j<cases.size; j++){
				writeLog("4");
				alphanode.locations.push(cases.cases[i][0], cases.cases[i][1]);
				alphanode.locations.push(cases.cases[j][0], cases.cases[j][1]);

				a_node = minimax(board, share_board, new_totalscore, depth + 1, false, alpha, beta, alphanode);

				alphanode.locations.pop(2);

				bestVal = (bestVal.value < a_node.value) ? a_node : bestVal;
				alpha = (bestVal.value < alpha.value) ? alpha : bestVal;
				if (beta.value <= alpha.value) {
					count_break1++; break;
				}
			}
			if (beta.value <= alpha.value) break;
		}
		writeLog("7");
	}
	else if (must_num == 1){
		writeLog("2");
	}
	writeLog("3");
	return bestVal;

}

int count = 0;

/**************************************/
/*                                    */
/*              minimax               */
/*                                    */
/**************************************/
AlphaNode minimax(Board board, sBoard share_board, float totalscore, int depth, bool isMaximizingPlayer, AlphaNode alpha, AlphaNode beta, AlphaNode alphanode){
	int player = isMaximizingPlayer ? 1 : 2;

	if (depth == DEPTH){
		Locations locations;
		locations.push(alphanode.locations.loc[(depth - 1) * 2][0], alphanode.locations.loc[(depth - 1) * 2 ][1]);
		locations.push(alphanode.locations.loc[(depth - 1) * 2+1][0], alphanode.locations.loc[(depth - 1) * 2 + 1][1]);
			
		alphanode.value = evaluate_score_BNA(board, locations, player, totalscore);
		count++;
		return alphanode;
	}

	if (isMaximizingPlayer){
		AlphaNode a_node;
		AlphaNode bestVal;
		bestVal.value = -10000000000; //무한대로 초기화

		Locations locations;
		locations.push(alphanode.locations.loc[(depth - 1) * 2][0], alphanode.locations.loc[(depth - 1) * 2][1]);
		locations.push(alphanode.locations.loc[(depth - 1) * 2 + 1][0], alphanode.locations.loc[(depth - 1) * 2 + 1][1]);

		Board new_board = put_rock(locations, player, board);;
		sBoard new_share_board = get_share_board(new_board);
		Cases new_cases = get_cases(new_share_board);
		float new_totalscore = evaluate_score_BNA(board, locations, player, totalscore);

		for (int i = 0; i<new_cases.size; i++){
			for (int j = i + 1; j<new_cases.size; j++){
				alphanode.locations.push(new_cases.cases[i][0], new_cases.cases[i][1]);
				alphanode.locations.push(new_cases.cases[j][0], new_cases.cases[j][1]);
				
				a_node = minimax(new_board, new_share_board, new_totalscore, depth + 1, false, alpha, beta, alphanode);

				alphanode.locations.pop(2);

				bestVal = (bestVal.value <= a_node.value) ? a_node : bestVal;
				alpha = (bestVal.value <= alpha.value) ? alpha : bestVal;
				if (beta.value <= alpha.value) {
					count_break2++; break;
				}
			}
			if (beta.value <= alpha.value) break;
		}

		return bestVal;
	}
	else{
		AlphaNode a_node;
		AlphaNode bestVal;
		bestVal.value = 10000000000;

		Locations locations;
		locations.push(alphanode.locations.loc[(depth - 1) * 2][0], alphanode.locations.loc[(depth - 1) * 2][1]);
		locations.push(alphanode.locations.loc[(depth - 1) * 2 + 1][0], alphanode.locations.loc[(depth - 1) * 2 + 1][1]);

		Board new_board = put_rock(locations, player, board);
		sBoard new_share_board = get_share_board(new_board);
		Cases new_cases = get_cases(new_share_board);
		float new_totalscore = evaluate_score_BNA(board, locations, player, totalscore);

		for (int i = 0; i<new_cases.size; i++){
			for (int j = i + 1; j<new_cases.size; j++){
				alphanode.locations.push(new_cases.cases[i][0], new_cases.cases[i][1]);
				alphanode.locations.push(new_cases.cases[j][0], new_cases.cases[j][1]);

				a_node = minimax(new_board, new_share_board, new_totalscore, depth + 1, true, alpha, beta, alphanode);

				alphanode.locations.pop(2);


				bestVal = (bestVal.value >= a_node.value) ? a_node : bestVal;
				beta = (bestVal.value >= beta.value) ? beta : bestVal;
				if (beta.value <= alpha.value) {
					count_break2++; break;
				}
			}
			if (beta.value <= alpha.value) break;
		}

		return bestVal;
	}
}

/**************************************/
/*                                    */
/*               myturn               */
/*                                    */
/**************************************/
void myturn(int cnt) {
	int x[2], y[2];
	int temp = 0, new_cnt = 0, new_x[2] = { 0, 0 }, new_y[2] = { 0, 0 };
	Rocks * xy = new Rocks;
	int left_cnt = cnt;
	if (first){
		first = false;
		for (int i = 0; i < BOARDSIZE; i++) {
			for (int j = 0; j < BOARDSIZE; j++) {
				BOARD[j][i] = showBoard(j, i);
			}
		}
	}
	else{
		for (int i = 0; i < BOARDSIZE; i++) {
			for (int j = 0; j < BOARDSIZE; j++) {

				temp = showBoard(j, i);
				if (BOARD[j][i] != temp){
					new_x[new_cnt] = j;
					new_y[new_cnt] = i;
					BOARD[j][i] = temp;
					new_cnt++;
				}
				if (new_cnt >= 2) break;
			}
			if (new_cnt >= 2) break;
		}
	}
	// 이 부분에서 알고리즘 프로그램(AI)을 작성하십시오. 기본 제공된 코드를 수정 또는 삭제하고 본인이 코드를 사용하시면 됩니다.
	// 현재 Sample code의 AI는 Random으로 돌을 놓는 Algorithm이 작성되어 있습니다.
	/*
	for (int i = 0; i < cnt; i++) {
	do {
	x[i] = rand() % width;
	y[i] = rand() % height;
	if (terminateAI) return;
	} while (!isFree(x[i], y[i]));

	if (x[1] == x[0] && y[1] == y[0]) i--;
	}*/
	
	xy = must_win(xy, 1);
	for (int i = 0; i<xy->get_cnt(); i++){
		writeLog("end\n");
		x[i] = xy->get_x(i);
		y[i] = xy->get_y(i);
	}
	left_cnt -= xy->get_cnt();
	print_board((float)left_cnt);
	
	if (left_cnt>0){
		Board board = get_board();
		Cases cases = get_cases_by_board(board);
		AlphaNode choice = minimax_root(board, cases, 2-left_cnt);
		if (choice.locations.size == 0){
			writeLog("rand\n");
			for (int i = 0; i < left_cnt; i++) {
				do {
					x[i] = rand() % width;
					y[i] = rand() % height;
					if (terminateAI) return;
				} while (!isFree(x[i], y[i]));

				if (x[1] == x[0] && y[1] == y[0]) i--;
			}
		}
		else{
			writeLog("score\n");
			for (int i = left_cnt; i<2; i++){
				x[i] = choice.locations.loc[i][0];
				y[i] = choice.locations.loc[i][1];
			}
		}

	}
	
	print_board((float)count);
	print_board((float)count_break1);
	print_board((float)count_break2);
	// 이 부분에서 자신이 놓을 돌을 출력하십시오.
	// 필수 함수 : domymove(x배열,y배열,배열크기)
	// 여기서 배열크기(cnt)는 myturn()의 파라미터 cnt를 그대로 넣어야합니다.
	domymove(x, y, cnt);
}


int main(){
	srand (time(NULL));
	clock_t begin, end;
	begin = clock();
	ios_base::sync_with_stdio(false);
	cin.tie(0);cout.tie(0);
	for(int i=0;i<BOARDSIZE;i++){
		for(int j=0;j<BOARDSIZE;j++){
			if(i<13&&j<10&&i>5&&j>5) BOARD[i][j]=rand()%3;
			else BOARD[i][j] = 0;
		}
	}
	int number = (rand()%5)*2;
	for(int i=0;i<number;i++){
		int x = rand()%BOARDSIZE;
		int y = rand()%BOARDSIZE;
		BOARD[x][y]=3;
	}

	cout <<  evaluate_score(board) << endl ;
	end = clock();
	cout << "Operation time: " << ((end-begin)/CLOCKS_PER_SEC)<< "\n";
}
