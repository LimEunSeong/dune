/*
*  display.c:
* ȭ�鿡 ���� ������ ���
* ��, Ŀ��, �ý��� �޽���, ����â, �ڿ� ���� ���
* io.c�� �ִ� �Լ����� �����
*/

#ifndef _DISPLAY_H_
#define _DISPLAY_H_

#include "common.h"

// ǥ���� ���� ����. ���� ���� �״µ�, ���ⲯ �߰��ϰų� �����ϱ�
#define COLOR_DEFAULT	112
#define COLOR_CURSOR	13
#define COLOR_RESOURCE  112
//���� �߰� ����
#define COLOR_SPICE 100 //Ȳ���
#define COLOR_PLAYER 31 //�Ķ���
#define COLOR_WORM	111 //��Ȳ��
#define COLOR_ROCK 143 // ȸ��
#define COLOR_ENEMY 79 //������
#define COLOR_PLATE 15 //������



// ������ �ڿ�, ��, Ŀ���� ǥ��
// ������ ȭ�鿡 ǥ���� ����� ���⿡ �߰��ϱ�
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
