#pragma once

extern float camera_px;
extern float camera_py;

using namespace std;

typedef struct {
	int top;
	int bottom;
	int left;
	int right;
}WINDOWRECT;

class cBackground {
protected:
public:
	int bghandle_09[4];					// �摜������z��
	VECTOR bghandle_pos_09[4][2];
	cBackground() {						// �z��̍��W������
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 2; j++) {
				bghandle_pos_09[i][j].x = (float)j;
				bghandle_pos_09[i][j].y = -WINDOW_SIZE_Y / 4;
			}
		}
		LoadDivGraph("data/img/bg/Mobile_Backgrounds_09.png", 4, 1, 4, 4096, 1024, bghandle_09);	// �؂̔w�i�Ɖ_�̔w�i������@����09�͂��̔ԍ��������Ă���
	}
};

class cCamera {
protected:
	WINDOWRECT window;
	float px, py;
	int	  ox, oy;
	int	  sx, sy;
	float cloud[2] = {0.f, 4090.f};
	cBackground *bg;
	float speed = 2.f;
	int count = 0;
public:
	int	  move = -1;
	float DestinationX[10] = {0,0,0,0,0,0,0,0,0,0 }, DestinationY[10] = { 0,0,0,0,0,0,0,0,0,0 };

	cCamera(string name) {
		AutoScrolConfig(name);
		bg = new cBackground();
	};
	~cCamera() {
		delete bg;
		bg = nullptr;
	};
	void	Update(VECTOR focus);
	void	Render(int handle,int getx,int gety);
	void	SetStageSize(int x, int y) {sx = x, sy = y;}
	void	AutoScrol(VECTOR focus);
	// csv����I�[�g�X�N���[������ꏊ������o��
	void	AutoScrolConfig(string name);

	float mokutekiX,mokutekiY;
	float ans = -50.f;
};