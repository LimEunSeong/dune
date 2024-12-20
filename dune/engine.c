﻿#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include "common.h"
#include "io.h"
#include "display.h"

void init(void);
void intro(void);
void outro(void);
void cursor_move(DIRECTION dir);
void sample_obj_move(void);
POSITION sample_obj_next_position(void);

#define DOUBLE_MOVE_TIME 300 //300 ms 이내에 두번 누르면

//전역 변수 추가
DIRECTION last_direction = d_stay;//마지막 방향
clock_t last_move_time = 0; //마지막 이동 시간
bool can_double_move = false; //추가 이동 가능 여부
bool clear_object_info = false; // 상태창 비우기 변수 정의

/* ================= control =================== */
int sys_clock = 0;		// system-wide clock(ms)
CURSOR cursor = { { 1, 1 }, {1, 1} };


/* ================= game data =================== */
char map[N_LAYER][MAP_HEIGHT][MAP_WIDTH] = { 0 };
//변수 추가 정의
char object_info[OBJECT_INFO_HEIGHT][OBJECT_INFO_WIDTH] = { 0 };
char system_message[SYS_MESSAGE_HEIGHT][SYS_MESSAGE_WIDTH] = { 0 };
char command[COMMAND_HEIGHT][COMMAND_WIDTH] = { 0 };

RESOURCE resource = { 
	.spice = 0,
	.spice_max = 0,
	.population = 0,
	.population_max = 0
};

OBJECT_SAMPLE obj = {
	.pos = {1, 1},
	.dest = {MAP_HEIGHT - 2, MAP_WIDTH - 2},
	.repr = 'o',
	.speed = 300,
	.next_move_time = 300
};

/* ================= main() =================== */
int main(void) {
	srand((unsigned int)time(NULL));

	init();
	intro();
	display(resource, map, object_info, system_message,command, cursor);

	while (1) {
		// loop 돌 때마다(즉, TICK==10ms마다) 키 입력 확인
		KEY key = get_key();

		// 키 입력이 있으면 처리
		if (is_arrow_key(key)) {
			cursor_move(ktod(key));
		}
		else if (key == k_space) {  // 스페이스바 처리
			POSITION selected_pos = cursor.current;
			char selected_char = map[1][selected_pos.row][selected_pos.column]; // 선택된 위치의 문자

			// 선택된 문자가 오브젝트인지 확인
			int index = -1;
			if (selected_char == 'H') index = 0; // 하베스터
			else if (selected_char == 'B') index = 1; // 본진
			else if (selected_char == 'P') index = 2; // 장판
			else if (selected_char == 'D') index = 3; // 숙소
			else if (selected_char == 'G') index = 4; // 창고
			else if (selected_char == 'R') index = 5; // 병영
			else if (selected_char == 'S') index = 6; // 은신처
			else if (selected_char == 'A') index = 7; // 투기장
			else if (selected_char == 'F') index = 8; // 공장
			else if (selected_char == 'W') index = 9; // 샌드웜
			else if (selected_char == 'F') index = 10; // 프레멘
			else if (selected_char == 'S') index = 11; // 보병
			else if (selected_char == 'F') index = 12; // 투사
			else if (selected_char == 'T') index = 13; // 중전차

			// 오브젝트 정보를 상태창에 표시
			if (index >= 0 && index < MAX_BUILDINGS) {
				snprintf(object_info[1], OBJECT_INFO_WIDTH, "이름: %s", building_infos[index].name);
				snprintf(object_info[2], OBJECT_INFO_WIDTH, "설명: %s", building_infos[index].description);
				snprintf(object_info[3], OBJECT_INFO_WIDTH, "건설비용: %d", building_infos[index].cost);
				snprintf(object_info[4], OBJECT_INFO_WIDTH, "내구도: %d", building_infos[index].capacity);
			}
			else if (index >= 0 && index < 10 + MAX_UNITS) {
				index -= 10; // 유닛 인덱스 조정
				// 유닛 정보 출력
				snprintf(object_info[1], OBJECT_INFO_WIDTH, "이름:%s", unit_infos[index].name);
				snprintf(object_info[2], OBJECT_INFO_WIDTH, "생산 비용: %d", unit_infos[index].cost);
				snprintf(object_info[3], OBJECT_INFO_WIDTH, "인구수: %d", unit_infos[index].population);
				snprintf(object_info[4], OBJECT_INFO_WIDTH, "이동주기: %d", unit_infos[index].speed);
				snprintf(object_info[5], OBJECT_INFO_WIDTH, "공격력: %d", unit_infos[index].attack);
				snprintf(object_info[6], OBJECT_INFO_WIDTH, "공격주기: %d", unit_infos[index].attack_period);
				snprintf(object_info[7], OBJECT_INFO_WIDTH, "체력: %d", unit_infos[index].health);
				snprintf(object_info[8], OBJECT_INFO_WIDTH, "시야: %d", unit_infos[index].view);
			}
			else {
				// 빈 지형일 경우
				snprintf(object_info[1], OBJECT_INFO_WIDTH, "지형: 사막지형");
			}
		}
		else if (key == k_esc) {  // ESC 키 처리
			for (int i = 1; i < OBJECT_INFO_HEIGHT - 1; i++) {
				for (int j = 1; j < OBJECT_INFO_WIDTH - 1; j++) {
					object_info[i][j] = ' '; // 상태창 내부 내용을 비우기
				}
			}
		}
		else {
			// 방향키 외의 입력
			switch (key) {
			case k_quit: outro();
			case k_none:
			case k_undef:
			default: break;
			}
		}

		// 샘플 오브젝트 동작
		sample_obj_move();

		// 화면 출력
		display(resource, map, object_info, system_message, command, cursor);
		Sleep(TICK);
		sys_clock += 10;
	}
}

/* ================= subfunctions =================== */
void intro(void) {
	printf("DUNE 1.5\n");		
	Sleep(2000);
	system("cls");
}

void outro(void) {
	system("cls");
	printf("exiting...\n");
	exit(0);
}

void init(void) {
	// layer 0(map[0])에 지형 생성
	for (int j = 0; j < MAP_WIDTH; j++) {
		map[0][0][j] = '#';
		map[0][MAP_HEIGHT - 1][j] = '#';
	}

	for (int i = 1; i < MAP_HEIGHT - 1; i++) {
		map[0][i][0] = '#';
		map[0][i][MAP_WIDTH - 1] = '#';
		for (int j = 1; j < MAP_WIDTH-1; j++) {
			map[0][i][j] = ' ';
		}
	}

	// 상태창
	for (int j = 0; j < OBJECT_INFO_WIDTH; j++) {
		object_info[0][j] = '#';
		object_info[OBJECT_INFO_HEIGHT - 1][j] = '#';
	}

	for (int i = 1; i < OBJECT_INFO_HEIGHT - 1; i++) {
		object_info[i][0] = '#';
		object_info[i][OBJECT_INFO_WIDTH - 1] = '#';
		for (int j = 1; j < OBJECT_INFO_WIDTH - 1; j++) {
			object_info[i][j] = ' ';
		}
	}
	//시스템 메시지
	for (int j = 0; j < SYS_MESSAGE_WIDTH; j++) {
		system_message[0][j] = '#';
		system_message[SYS_MESSAGE_HEIGHT - 1][j] = '#';
	}

	for (int i = 1; i < SYS_MESSAGE_HEIGHT - 1; i++) {
		system_message[i][0] = '#';
		system_message[i][SYS_MESSAGE_WIDTH - 1] = '#';
		for (int j = 1; j < SYS_MESSAGE_WIDTH - 1; j++) {
			system_message[i][j] = ' ';
		}
	}
	//명령창
	for (int j = 0; j < COMMAND_WIDTH; j++) {
		command[0][j] = '#';
		command[COMMAND_HEIGHT - 1][j] = '#';
	}

	for (int i = 1; i < COMMAND_HEIGHT - 1; i++) {
		command[i][0] = '#';
		command[i][COMMAND_WIDTH - 1] = '#';
		for (int j = 1; j < COMMAND_WIDTH - 1; j++) {
			command[i][j] = ' ';
		}
	}



	// layer 1(map[1])은 비워 두기(-1로 채움)
	for (int i = 0; i < MAP_HEIGHT; i++) {
		for (int j = 0; j < MAP_WIDTH; j++) {
			map[1][i][j] = -1;
		}
	}

	//좌하단 본부(2x2) - 아트레이디스(푸른색)
	map[0][15][1] = 'B';  // 본부 (15, 1)
	map[0][15][2] = 'B';  // 본부 (15, 2)
	map[0][16][1] = 'B';  // 본부 (16, 1)
	map[0][16][2] = 'B';  // 본부 (16, 2)
	//좌하단 장판(2x2) - 검은색
	map[0][15][3] = 'P'; //장판(15,3)
	map[0][15][4] = 'P'; //장판(15,4)
	map[0][16][3] = 'P'; //장판(16,3)
	map[0][16][4] = 'P'; //장판(16,4)

	// 좌하단 하베스터 (H) - 아트레이디스 (푸른색)
	map[1][14][1] = 'H';  // 하베스터 (14, 1)

	// 좌하단 스파이스 (S) - 주황색
	map[0][12][1] = 'S';  // 스파이스 (12, 1)

	// 지형(바위) - 회색
	map[0][4][49] = 'R';  // 바위 (4, 49)
	map[0][4][50] = 'R';  // 바위 (4, 50)
	map[0][5][49] = 'R';  // 바위 (5, 49)
	map[0][5][50] = 'R';  // 바위 (5, 50)

	map[0][5][10] = 'R'; //바위(5,10)

	map[0][10][11] = 'R'; //바위(10,11)

	map[0][8][25] = 'R'; //바위(8,25)
	map[0][8][26] = 'R'; //바위(8,26)
	map[0][9][25] = 'R'; //바위(9,25)
	map[0][9][26] = 'R'; //바위(9,26)

	// 샌드웜 (2) - 황토색
	map[1][4][2] = 'W';  // 샌드웜 (4, 2)
	map[1][10][54] = 'W';
	

	// 하코넨 (AI) - 빨간색
	map[0][1][58] = 'A';  // 2x2 바위 (1, 56)
	map[0][1][57] = 'A';  // 2x2 바위 (1, 57)
	map[0][2][58] = 'A';  // 2x2 바위 (2, 56)
	map[0][2][57] = 'A';  // 2x2 바위 (2, 57)
	
	// 우상단 장판(2x2) - 검은색
	map[0][1][56] = 'P'; //장판(1,56)
	map[0][1][55] = 'P'; //장판(1,55)
	map[0][2][56] = 'P'; //장판(2,56)
	map[0][2][55] = 'P'; //장판(2,55)

	// 우상단 하베스터 (H) - 하코넨 (빨간색)
	map[1][3][58] = 'I';  // 하베스터 (14, 1)

	// 우상단 스파이스 (S) - 주황색
	map[0][5][58] = 'S';  // 스파이스 (12, 1)
	// object sample
	map[1][obj.pos.row][obj.pos.column] = 'o';
}

// (가능하다면) 지정한 방향으로 커서 이동
void cursor_move(DIRECTION dir) {
	POSITION curr = cursor.current;

	//현재 시간
	clock_t current_time = clock();

	//방향이 같고, DOUBLE_MOVE_TIME 이내에 눌렀다면 추가 이동
	if (dir == last_direction && (current_time - last_move_time) < DOUBLE_MOVE_TIME) {
		can_double_move = true;//추가 이동 가능
	}
	else {
		can_double_move = false; //추가 이동 불가능
	}
	//기본 1칸 이동
	POSITION new_pos = pmove(curr, dir);
	// validation check
	if (1 <= new_pos.row && new_pos.row <= MAP_HEIGHT - 2 &&
		1 <= new_pos.column && new_pos.column <= MAP_WIDTH - 2) {
		cursor.previous = cursor.current;
		cursor.current = new_pos; //정상 이동
		display_cursor(cursor);
	}
	//추가 이동
	if (can_double_move) {
		new_pos = pmove(cursor.current, dir);
		if (1 <= new_pos.row && new_pos.row <= MAP_HEIGHT - 2 &&
			1 <= new_pos.column && new_pos.column <= MAP_WIDTH - 2) {
			cursor.previous = cursor.current;
			cursor.current = new_pos;
			display_cursor(cursor);
		}
	}
	//현재 방향과 시간 기록
	last_direction = dir;
	last_move_time = current_time;//현재 시간을 기록
}

/* ================= sample object movement =================== */
POSITION sample_obj_next_position(void) {
	// 현재 위치와 목적지를 비교해서 이동 방향 결정	
	POSITION diff = psub(obj.dest, obj.pos);
	DIRECTION dir;

	// 목적지 도착. 지금은 단순히 원래 자리로 왕복
	if (diff.row == 0 && diff.column == 0) {
		if (obj.dest.row == 1 && obj.dest.column == 1) {
			// topleft --> bottomright로 목적지 설정
			POSITION new_dest = { MAP_HEIGHT - 2, MAP_WIDTH - 2 };
			obj.dest = new_dest;
		}
		else {
			// bottomright --> topleft로 목적지 설정
			POSITION new_dest = { 1, 1 };
			obj.dest = new_dest;
		}
		return obj.pos;
	}
	
	// 가로축, 세로축 거리를 비교해서 더 먼 쪽 축으로 이동
	if (abs(diff.row) >= abs(diff.column)) {
		dir = (diff.row >= 0) ? d_down : d_up;
	}
	else {
		dir = (diff.column >= 0) ? d_right : d_left;
	}
	
	// validation check
	// next_pos가 맵을 벗어나지 않고, (지금은 없지만)장애물에 부딪히지 않으면 다음 위치로 이동
	// 지금은 충돌 시 아무것도 안 하는데, 나중에는 장애물을 피해가거나 적과 전투를 하거나... 등등
	POSITION next_pos = pmove(obj.pos, dir);
	if (1 <= next_pos.row && next_pos.row <= MAP_HEIGHT - 2 && \
		1 <= next_pos.column && next_pos.column <= MAP_WIDTH - 2 && \
		map[1][next_pos.row][next_pos.column] < 0) {
		
		return next_pos;
	}
	else {
		return obj.pos;  // 제자리
	}
}

void sample_obj_move(void) {
	if (sys_clock <= obj.next_move_time) {
		// 아직 시간이 안 됐음
		return;
	}

	// 오브젝트(건물, 유닛 등)은 layer1(map[1])에 저장
	map[1][obj.pos.row][obj.pos.column] = -1;
	obj.pos = sample_obj_next_position();
	map[1][obj.pos.row][obj.pos.column] = obj.repr;

	obj.next_move_time = sys_clock + obj.speed;
}