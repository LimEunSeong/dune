/*
*  display.c:
* 화면에 게임 정보를 출력
* 맵, 커서, 시스템 메시지, 정보창, 자원 상태 등등
* io.c에 있는 함수들을 사용함
*/

#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#include "common.h"

// 표시할 색상 정의. 대충 맞춰 뒀는데, 취향껏 추가하거나 변경하기
#define COLOR_DEFAULT	112
#define COLOR_CURSOR	13
#define COLOR_RESOURCE  112
//색상 추가 정의
#define COLOR_SPICE 100 //황토색
#define COLOR_PLAYER 31 //파란색
#define COLOR_WORM	111 //주황색
#define COLOR_ROCK 143 // 회색
#define COLOR_ENEMY 79 //빨간색
#define COLOR_PLATE 15 //검은색



// 지금은 자원, 맵, 커서만 표시
// 앞으로 화면에 표시할 내용들 여기에 추가하기
void display(
	RESOURCE resource,
	char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH],
	char object_info[OBJECT_INFO_HEIGHT][OBJECT_INFO_WIDTH],
	char system_message[SYS_MESSAGE_HEIGHT][SYS_MESSAGE_WIDTH],
	char command[COMMAND_HEIGHT][COMMAND_WIDTH],
	CURSOR cursor
);
void display_cursor(CURSOR cursor);
#endif
