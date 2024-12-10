/*
*  display.c:
* ȭ�鿡 ���� ������ ���
* ��, Ŀ��, �ý��� �޽���, ����â, �ڿ� ���� ���
* io.c�� �ִ� �Լ����� �����
*/

#include "display.h"
#include "io.h"

// ����� ������� �»��(topleft) ��ǥ
const POSITION resource_pos = { 0, 0 };
const POSITION map_pos = { 1, 0 };


char backbuf[MAP_HEIGHT][MAP_WIDTH] = { 0 };
char frontbuf[MAP_HEIGHT][MAP_WIDTH] = { 0 };
//�ý��� �޽���,����â, ���â
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
				case 'S': color = COLOR_SPICE; break; //�����̽�
				case 'H': color = COLOR_PLAYER; break; // �Ϻ�����
				case 'B': color = COLOR_PLAYER; break; //���̽�(�÷��̾�)
				case 'I': ch = 'H'; color = COLOR_ENEMY; break; //�Ϻ�����(AI)
				case 'A': ch = 'B'; color = COLOR_ENEMY; break; //���̽�(AI)
				case 'R': color = COLOR_ROCK; break; //����
				case 'W': color = COLOR_WORM; break; //�����
				case 'P': color = COLOR_PLATE; break; //����
				default: color = 112;  break; //�⺻ ����
				}
				printc(padd(map_pos, pos), ch, color);
			}

			frontbuf[i][j] = backbuf[i][j]; //frontbuf ������Ʈ
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
// frontbuf[][]���� Ŀ�� ��ġ�� ���ڸ� ���� �ٲ㼭 �״�� �ٽ� ���
void display_cursor(CURSOR cursor) {
	POSITION prev = cursor.previous;
	POSITION curr = cursor.current;

	//���� ��ġ�� ���ڿ� ���� ����
	char prev_char = frontbuf[prev.row][prev.column];
	int prev_color;
	
	//���� ��ġ�� ���� ����
	switch (prev_char) {
	case 'S': prev_color = COLOR_SPICE; break; //�����̽�
	case 'H': prev_color = COLOR_PLAYER; break; // �Ϻ�����
	case 'B': prev_color = COLOR_PLAYER; break; //���̽�(�÷��̾�)
	case 'I': prev_char = 'H'; prev_color = COLOR_ENEMY; break; //�Ϻ�����(AI)
	case 'A': prev_char = 'B'; prev_color = COLOR_ENEMY; break; //���̽�(AI)
	case 'R': prev_color = COLOR_ROCK; break; //����
	case 'W': prev_color = COLOR_WORM; break; //�����
	case 'P': prev_color = COLOR_PLATE; break; //����
	default: prev_color = 112;  break; //�⺻ ����
	}
	//���� ��ġ�� ���� �������� ���
	printc(padd(map_pos, prev), prev_char, prev_color);

	//���� Ŀ�� ��ġ�� ����
	prev_char = frontbuf[curr.row][curr.column];

	//������ġ�� ���ڸ� �״�� �����ϸ鼭 Ŀ�� �������� ���
	printc(padd(map_pos, curr), prev_char, COLOR_CURSOR); //Ŀ�� �������� ���
}

BUILDING_INFO building_infos[MAX_BUILDINGS] = {
	{"����(Base)", "�ǹ� ���� ���� ����", 0, 50, "H. �Ϻ����� ����"},
	{"����(Plate)", "�ǹ� ���� ���� ���", 1, 0, "����"},
	{"����(Dormitory)", "�α� �ִ�ġ ����(10)", 2, 10, "����"},
	{"â��(Garage)", "�����̽� ���� �ִ�ġ ����(10)", 4, 20, "����"},
	{"����(Barracks)", "���� ����", 4, 20, "���� ����: Soldier"},
	{"����ó(Shelter)", "Ư�� ���� ����", 5, 30, "������ ����: Fremen"},
	{"������(Arena)", "���� ����", 3, 15, "���� ����: Fighter"},
	{"����(Factory)", "Ư�� ���� ����", 5, 30, "������ ����: Heavy Tank"},
};

UNIT_INFO unit_infos[MAX_UNITS] = {
	{"�Ϻ�����(Harvester)", 5, 5, 2000, 0, 0, 70, 0, "H: Harvest, M: Move"},
	{"������(Fremen)", 5, 2, 400, 15, 200, 25, 8, "M: �̵�, P: ����"},
	{"����(Soldier)", 1, 1, 1000, 5, 800, 15, 1, "M: �̵�, P: ����"},
	{"����(Fighter)", 1, 1, 1200, 6, 600, 10, 1, "M: �̵�, P: ����"},
	{"������(Heavy Tank)", 12, 5, 3000, 40, 4000, 60, 4, "M: �̵�, P: ����"},
	{"�����(Sandworm)", 0, 0, 2500, 0, 10000, 0, 0, "���Ѵ�"},
};