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
				case 'I': ch = 'H'; color = COLOR_ENEMY; break; //하베스터(AI)
				case 'A': ch = 'B'; color = COLOR_ENEMY; break; //베이스(AI)
				case 'R': color = COLOR_ROCK; break; //바위
				case 'W': color = COLOR_WORM; break; //샌드웜
				case 'P': color = COLOR_PLATE; break; //장판
				default: color = 112;  break; //기본 색상
				}
				printc(padd(map_pos, pos), ch, color);
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
	case 'I': prev_char = 'H'; prev_color = COLOR_ENEMY; break; //하베스터(AI)
	case 'A': prev_char = 'B'; prev_color = COLOR_ENEMY; break; //베이스(AI)
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

BUILDING_INFO building_infos[MAX_BUILDINGS] = {
	{"본진(Base)", "건물 지기 전에 짓기", 0, 50, "H. 하베스터 생산"},
	{"장판(Plate)", "건물 짓기 전에 깔기", 1, 0, "없음"},
	{"숙소(Dormitory)", "인구 최대치 증가(10)", 2, 10, "없음"},
	{"창고(Garage)", "스파이스 보관 최대치 증가(10)", 4, 20, "없음"},
	{"병영(Barracks)", "보병 생산", 4, 20, "보병 생산: Soldier"},
	{"은신처(Shelter)", "특수 유닛 생산", 5, 30, "프레멘 생산: Fremen"},
	{"투기장(Arena)", "투사 생산", 3, 15, "투사 생산: Fighter"},
	{"공장(Factory)", "특수 유닛 생산", 5, 30, "충전자 생산: Heavy Tank"},
};

UNIT_INFO unit_infos[MAX_UNITS] = {
	{"하베스터(Harvester)", 5, 5, 2000, 0, 0, 70, 0, "H: Harvest, M: Move"},
	{"프레멘(Fremen)", 5, 2, 400, 15, 200, 25, 8, "M: 이동, P: 순찰"},
	{"보병(Soldier)", 1, 1, 1000, 5, 800, 15, 1, "M: 이동, P: 순찰"},
	{"투사(Fighter)", 1, 1, 1200, 6, 600, 10, 1, "M: 이동, P: 순찰"},
	{"중전차(Heavy Tank)", 12, 5, 3000, 40, 4000, 60, 4, "M: 이동, P: 순찰"},
	{"샌드웜(Sandworm)", 0, 0, 2500, 0, 10000, 0, 0, "무한대"},
};