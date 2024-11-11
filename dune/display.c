/*
*  display.c:
* 화면에 게임 정보를 출력
* 맵, 커서, 시스템 메시지, 정보창, 자원 상태 등등
* io.c에 있는 함수들을 사용함
*/

#include "display.h"
#include "io.h"

// 출력할 내용들의 좌상단(topleft) 좌표
const POSITION resource_pos = { 0, 0 };
const POSITION map_pos = { 1, 0 };


char backbuf[MAP_HEIGHT][MAP_WIDTH] = { 0 };
char frontbuf[MAP_HEIGHT][MAP_WIDTH] = { 0 };
//시스템 메시지,상태창, 명령창
char object_info_backbuf[OBJECT_INFO_HEIGHT][OBJECT_INFO_WIDTH] = { 0 };
char object_info_frontbuf[OBJECT_INFO_HEIGHT][OBJECT_INFO_WIDTH] = { 0 };
char sys_message_backbuf[SYS_MESSAGE_HEIGHT][SYS_MESSAGE_WIDTH] = { 0 };
char sys_message_frontbuf[SYS_MESSAGE_HEIGHT][SYS_MESSAGE_WIDTH] = { 0 };
char command_backbuf[COMMAND_HEIGHT][COMMAND_WIDTH] = { 0 };
char command_frontbuf[COMMAND_HEIGHT][COMMAND_WIDTH] = { 0 };


void project(char src[N_LAYER][MAP_HEIGHT][MAP_WIDTH], char dest[MAP_HEIGHT][MAP_WIDTH]);
void display_resource(RESOURCE resource);
void display_map(char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH]);


void object_info_project(char src[OBJECT_INFO_HEIGHT][OBJECT_INFO_WIDTH], char dest[OBJECT_INFO_HEIGHT][OBJECT_INFO_WIDTH]);
void display_object_info(char object_info[OBJECT_INFO_HEIGHT][OBJECT_INFO_WIDTH]);
void system_message_project(char src[SYS_MESSAGE_HEIGHT][SYS_MESSAGE_WIDTH], char dest[SYS_MESSAGE_HEIGHT][SYS_MESSAGE_WIDTH]);
void display_system_message(char system_message[SYS_MESSAGE_HEIGHT][SYS_MESSAGE_WIDTH]);
void command_project(char src[COMMAND_HEIGHT][COMMAND_WIDTH], char dest[COMMAND_HEIGHT][COMMAND_WIDTH]);
void display_command(char command[COMMAND_HEIGHT][COMMAND_WIDTH]);


void display(
	RESOURCE resource,
	char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH],
	char object_info[OBJECT_INFO_HEIGHT][OBJECT_INFO_WIDTH],
	char system_message[SYS_MESSAGE_HEIGHT][SYS_MESSAGE_WIDTH],
	char command[COMMAND_HEIGHT][COMMAND_WIDTH],
	CURSOR cursor)
{
	display_resource(resource);
	display_map(map);
	display_cursor(cursor);
	display_object_info(object_info);
	display_system_message(system_message);
	display_command(command);
	// ...
}

void display_resource(RESOURCE resource) {
	set_color(COLOR_RESOURCE);
	gotoxy(resource_pos);
	printf("spice = %d/%d, population=%d/%d\n",
		resource.spice, resource.spice_max,
		resource.population, resource.population_max
	);
}

// subfunction of draw_map()
void project(char src[N_LAYER][MAP_HEIGHT][MAP_WIDTH], char dest[MAP_HEIGHT][MAP_WIDTH]) {
	for (int i = 0; i < MAP_HEIGHT; i++) {
		for (int j = 0; j < MAP_WIDTH; j++) {
			for (int k = 0; k < N_LAYER; k++) {
				if (src[k][i][j] >= 0) {
					dest[i][j] = src[k][i][j];
				}
			}
		}
	}
}

void display_map(char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH]) {
	project(map, backbuf);

	for (int i = 0; i < MAP_HEIGHT; i++) {
		for (int j = 0; j < MAP_WIDTH; j++) {
			if (frontbuf[i][j] != backbuf[i][j]) {
				char ch = backbuf[i][j];
				int color;
				POSITION pos = { i, j };
				switch (ch) {
				case 'S': color = COLOR_SPICE; break; //스파이스
				case 'H': color = COLOR_PLAYER; break; // 하베스터
				case 'B': color = COLOR_PLAYER; break; //베이스(플레이어)
				case 'I': color = COLOR_ENEMY; break; //하베스터(AI)
				case 'A': color = COLOR_ENEMY; break; //베이스(AI)
				case 'R': color = COLOR_ROCK; break; //바위
				case 'W': color = COLOR_WORM; break; //샌드웜
				case 'P': color = COLOR_PLATE; break; //장판
				default: color = 112;  break; //기본 색상
				}
				printc(padd(map_pos, pos), backbuf[i][j], color);
			}

			frontbuf[i][j] = backbuf[i][j]; //frontbuf 업데이트
		}
	}
}

void object_info_project(char src[OBJECT_INFO_HEIGHT][OBJECT_INFO_WIDTH], char dest[OBJECT_INFO_HEIGHT][OBJECT_INFO_WIDTH]) {
	for (int i = 0; i < OBJECT_INFO_HEIGHT; i++) {
		for (int j = 0; j < OBJECT_INFO_WIDTH; j++) {
			if (src[i][j] >= 0) {
				dest[i][j] = src[i][j];
			}
		}
	}
}
void display_object_info(char object_info[OBJECT_INFO_HEIGHT][OBJECT_INFO_WIDTH]) {
	object_info_project(object_info, object_info_backbuf);

	for (int i = 0; i < OBJECT_INFO_HEIGHT; i++) {
		//POSITION pos = object_info_pos;
		//pos.row += i;
		for (int j = 0; j < OBJECT_INFO_WIDTH; j++) {
			if (object_info_frontbuf[i][j] != object_info_backbuf[i][j]) {
				POSITION pos = { i, j+MAP_WIDTH };
				printc(padd(map_pos, pos), object_info_backbuf[i][j], COLOR_DEFAULT);
			}
			object_info_frontbuf[i][j] = object_info_backbuf[i][j];
		}
	}
}
void system_message_project(char src[SYS_MESSAGE_HEIGHT][SYS_MESSAGE_WIDTH], char dest[SYS_MESSAGE_HEIGHT][SYS_MESSAGE_WIDTH]) {
	for (int i = 0; i < SYS_MESSAGE_HEIGHT; i++) {
		for (int j = 0; j < SYS_MESSAGE_WIDTH; j++) {
			if (src[i][j] >= 0) {
				dest[i][j] = src[i][j];
			}
		}
	}
}
void display_system_message(char system_message[SYS_MESSAGE_HEIGHT][SYS_MESSAGE_WIDTH]) {
	system_message_project(system_message, sys_message_backbuf);

	for (int i = 0; i < SYS_MESSAGE_HEIGHT; i++) {
		//POSITION pos = sys_message_pos;
		//pos.row += i;
		for (int j = 0; j < SYS_MESSAGE_WIDTH; j++) {
			if (sys_message_frontbuf[i][j] != sys_message_backbuf[i][j]) {
				POSITION pos = { i+MAP_HEIGHT, j };
				printc(padd(map_pos, pos), sys_message_backbuf[i][j], COLOR_DEFAULT);
			}
			sys_message_frontbuf[i][j] = sys_message_backbuf[i][j];
		}
	}
}

void command_project(char src[COMMAND_HEIGHT][COMMAND_WIDTH], char dest[COMMAND_HEIGHT][COMMAND_WIDTH]) {
	for (int i = 0; i < COMMAND_HEIGHT; i++) {
		for (int j = 0; j < COMMAND_WIDTH; j++) {
			if (src[i][j] >= 0) {
				dest[i][j] = src[i][j];
			}
		}
	}
}
void display_command(char command[COMMAND_HEIGHT][COMMAND_WIDTH]) {
	command_project(command, command_backbuf);

	for (int i = 0; i < COMMAND_HEIGHT; i++) {
		//POSITION pos = command_pos;
		//pos.row += i;
		for (int j = 0; j < COMMAND_WIDTH; j++) {
			if (command_frontbuf[i][j] != command_backbuf[i][j]) {
				POSITION pos = { i+MAP_HEIGHT, j+MAP_WIDTH };
				printc(padd(map_pos, pos), command_backbuf[i][j], COLOR_DEFAULT);
			}
			command_frontbuf[i][j] = command_backbuf[i][j];
		}
	}
}
// frontbuf[][]에서 커서 위치의 문자를 색만 바꿔서 그대로 다시 출력
void display_cursor(CURSOR cursor) {
	POSITION prev = cursor.previous;
	POSITION curr = cursor.current;

	//이전 위치의 문자와 색상 복원
	char prev_char = frontbuf[prev.row][prev.column];
	int prev_color;
	
	//이전 위치의 색상 설정
	switch (prev_char) {
	case 'S': prev_color = COLOR_SPICE; break; //스파이스
	case 'H': prev_color = COLOR_PLAYER; break; // 하베스터
	case 'B': prev_color = COLOR_PLAYER; break; //베이스(플레이어)
	case 'I': prev_color = COLOR_ENEMY; break; //하베스터(AI)
	case 'A': prev_color = COLOR_ENEMY; break; //베이스(AI)
	case 'R': prev_color = COLOR_ROCK; break; //바위
	case 'W': prev_color = COLOR_WORM; break; //샌드웜
	case 'P': prev_color = COLOR_PLATE; break; //장판
	default: prev_color = 112;  break; //기본 색상
	}
	//이전 위치를 원래 색상으로 출력
	printc(padd(map_pos, prev), prev_char, prev_color);

	//현재 커서 위치의 문자
	prev_char = frontbuf[curr.row][curr.column];

	//현재위치의 문자를 그대로 유지하면서 커서 색상으로 출력
	printc(padd(map_pos, curr), prev_char, COLOR_CURSOR); //커서 색상으로 출력
}