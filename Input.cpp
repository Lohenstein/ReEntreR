 #include "Main.h"

int		key[256];
int		pad_b[18];

short	stick_lx, stick_ly;
float	stick_rad;
int		trigger_l, trigger_r;

// キーボード入力を取得
void	input_key() {

	static char buf[256];
	GetHitKeyStateAll(buf);

	for (int i = 0; i < 256; i++) {
		if (buf[i]) {
			if (key[i] == 0) key[i] = 1;
			else if (key[i] == 1) key[i] = 2;
		}
		else key[i] = 0;
	}
}

// ゲームパッド入力を取得
void	 input_pad() {

	XINPUT_STATE input_pad;
	GetJoypadXInputState(DX_INPUT_PAD1, &input_pad);

	for (int i = 0; i < 16; i++) {
		if (input_pad.Buttons[i]) {
			if (pad_b[i] == 0) pad_b[i] = 1;
			else if (pad_b[i] == 1) pad_b[i] = 2;
		}
		else pad_b[i] = 0;
	}

	stick_lx = input_pad.ThumbLX / 100;
	stick_ly = input_pad.ThumbLY / 100;

	trigger_l = input_pad.LeftTrigger;
	trigger_r = input_pad.RightTrigger;

	stick_rad = atan2f(stick_ly, stick_lx);


	if (stick_ly > 200) {
		if (pad_b[STICK_UP] == 0) pad_b[STICK_UP] = 1;
		else if (pad_b[STICK_UP] == 1) pad_b[STICK_UP] = 2;
	}
	else pad_b[STICK_UP] = 0;

	if (stick_ly < -200) {
		if (pad_b[STICK_DOWN] == 0) pad_b[STICK_DOWN] = 1;
		else if (pad_b[STICK_DOWN] == 1) pad_b[STICK_DOWN] = 2;
	}
	else pad_b[STICK_DOWN] = 0;

}

// 両方とも取得(グローバル)
void	input() {
	input_key();
	input_pad();
}
