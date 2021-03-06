
#include "Main.h"

// -----計算に必要な変数類---------------------------------------------
double	camera_time_x, camera_time_y;
bool	camera_flag_x, camera_flag_y;
bool	camera_flag_half_x, camera_flag_half_y;

float camera_px;
float camera_py;

void cCamera::Update(VECTOR focus) {

	window.left   = (int)focus.x - WINDOW_SIZE_X / 2;
	window.right  = (int)focus.x + WINDOW_SIZE_X / 2;
	window.top    = (int)focus.y - WINDOW_SIZE_Y / 2;
	window.bottom = (int)focus.y + WINDOW_SIZE_Y / 2;

	// -----カメライージング横軸-------------------------------------------
	if (FocusPos.x == px) {
		camera_time_x = 0.0;
		camera_flag_x = false;
		camera_flag_half_x = false;
	}
	else if (camera_flag_x == false) {
		camera_flag_x = true;
	}
	else if (camera_flag_x) {
		if (camera_time_x >= 0.5) {
			camera_flag_half_x = true;
		}
		if (camera_flag_half_x) {
			if (FocusPos.x == FocusOld.x) {
				camera_time_x += 0.01;
				if (camera_time_x >= 1.0) {
					camera_flag_x = false;
					camera_flag_half_x = false;
					px = FocusPos.x;
				}
			}
			else {
				camera_time_x = 0.5;
			}
		}
		else {
			camera_time_x += 0.01;
		}
	}
	// -----カメライージング縦軸-------------------------------------------
	if (FocusPos.y == py) {
		camera_time_y = 0.0;
		camera_flag_y = false;
		camera_flag_half_y = false;
	}
	else if (camera_flag_y == false) {
		camera_flag_y = true;
	}
	else if (camera_flag_y) {
		if (camera_time_y >= 0.5) {
			camera_flag_half_y = true;
		}
		if (camera_flag_half_y) {
			if (FocusPos.y == FocusOld.y) {
				camera_time_y += 0.01;
				if (camera_time_y >= 1.0) {
					camera_flag_y = false;
					camera_flag_half_y = false;
					py = FocusPos.y;
				}
			}
			else {
				camera_time_y = 0.5;
			}
		}
		else {
			camera_time_y += 0.01;
		}
	}
	// ----イージング-------------------------------------------
	px = (float)Easing::InOutQuad(camera_time_x, 1.0, (double)FocusPos.x, (double)px);
	py = (float)Easing::InOutQuad(camera_time_y, 1.0, (double)FocusPos.y, (double)py);

	FocusCam.x = px;
	FocusCam.y = py;
	MouseAdd.x = px;
	MouseAdd.y = py;

	ox = camera_px;
	oy = camera_py;

	camera_px = px - WINDOW_SIZE_X / 2.f;
	camera_py = py - WINDOW_SIZE_Y / 2.f;

	if (camera_px < 0.f) camera_px = 0.f;
	if (camera_px > sx * bsize - WINDOW_SIZE_X) camera_px = sx * bsize - WINDOW_SIZE_X;
	if (camera_py < 0.f) camera_py = 0.f;
	if (camera_py > sy * bsize - WINDOW_SIZE_Y) camera_py = sy * bsize - WINDOW_SIZE_Y;
	for (int i = 0; i < 2; i++) cloud[i] -= 0.2f;
	if (camera_py > 3000.f) camera_py = 3000.f;
}

void cCamera::AutoScrol(VECTOR focus) {
	window.left = (int)focus.x - WINDOW_SIZE_X / 2;
	window.right = (int)focus.x + WINDOW_SIZE_X / 2;
	window.top = (int)focus.y - WINDOW_SIZE_Y / 2;
	window.bottom = (int)focus.y + WINDOW_SIZE_Y / 2;
	if (move != 3) {
		if (ans <= 0) {
			move++;
			mokutekiX = DestinationX[move + 1] - DestinationX[move];
			mokutekiY = DestinationY[move + 1] - DestinationY[move];
			ans = (float)sqrt((mokutekiX * mokutekiX) + (mokutekiY * mokutekiY));
		}

		float rad = atan2f(DestinationY[move + 1] - DestinationY[move], DestinationX[move + 1] - DestinationX[move]);	
		// ans -= 10;
		// ans -= 100.f;
		if (DestinationX[move + 1] != 0 && DestinationY[move + 1] != 0) {
			if (ans >= 0) {
				camera_px = camera_px - (cosf(rad + PI) * speed);
				camera_py = camera_py - (sinf(rad + PI) * speed);
				focus.x += -(cosf(rad + PI) * speed);
				focus.y += -(sinf(rad + PI) * speed);
				FocusCam.x += -(cosf(rad + PI) * speed);
				FocusCam.y += -(sinf(rad + PI) * speed);
				ans -= (float)sqrt(((cosf(rad + PI) * speed)*(cosf(rad + PI) * speed) + (sinf(rad + PI) * speed)*(sinf(rad + PI) * speed)));
			}
		}
		else { move = 4; }
	}
}

void cCamera::Render(int handle,int getx,int gety) {
	int imageid;
	// 背景：空
	imageid = 2;
	for (int i = 0; i < 2; i++) {
		float scrollx = bg->bghandle_pos_09[imageid][i].x - camera_px / 4.f + cloud[i];		// スクロールと合わせて動くのでスクロール分を足している
		float scrolly = bg->bghandle_pos_09[imageid][i].y - camera_py / 8.f;				// 
		if (cloud[i] <= -4096.f) cloud[i] = 4090.f;
		DrawGraph(scrollx, scrolly, bg->bghandle_09[imageid], true);
	}
	// 背景：平原
	imageid = 3;
	for (int i = 0; i < 2; i++) {
		/*bg->bghandle_pos_09[imageid][i].x = -camera_px / 2.f + (i * 2880.f);		// 横ループしている
		bg->bghandle_pos_09[imageid][i].y = -camera_py / 4.f;
		DrawExtendGraph(bg->bghandle_pos_09[imageid][i].x, bg->bghandle_pos_09[imageid][i].y, bg->bghandle_pos_09[imageid][i].x + 2880, bg->bghandle_pos_09[imageid][i].y + 720, bg->bghandle_09[imageid], true);*/
		bg->bghandle_pos_09[imageid][i].x = -camera_px / 2.f + (i * 2880.f);		// 横ループしている
		bg->bghandle_pos_09[imageid][i].y = -camera_py / 4.f + 600.f;
		//DrawGraph(bg->bghandle_pos_09[imageid][i].x, bg->bghandle_pos_09[imageid][i].y, bg->bghandle_09[imageid], true);
		DrawExtendGraph(bg->bghandle_pos_09[imageid][i].x, bg->bghandle_pos_09[imageid][i].y, bg->bghandle_pos_09[imageid][i].x + 2880, bg->bghandle_pos_09[imageid][i].y + 1000, bg->bghandle_09[imageid], true);
		
	}
	DrawRectGraph(0, 0, (int)camera_px, (int)camera_py, WINDOW_SIZE_X, WINDOW_SIZE_Y, handle, true, false);
	//DrawRectRotaGraph(WINDOW_SIZE_X / 2, WINDOW_SIZE_Y / 2, (int)camera_px - 320, (int)camera_py - 180, WINDOW_SIZE_X + 320, WINDOW_SIZE_Y + 180, 1.0, 0.0, handle, true, false); //
	DrawFormatString(FocusPos.x, FocusPos.y, 0xFFFFFF, "%f", count);
}

void cCamera::AutoScrolConfig(string name)
{
	
	string scrol = name + "auto.csv";
	ifstream ifs; ifs.open(scrol.c_str());
	string line;
	
	vector<string> str = split(line, ',');


	while (getline(ifs, line)) {
		
		vector<string> str = split(line, ',');
		
		DestinationX[count] = stof(str.at(0));
		
		DestinationY[count] = stof(str.at(1));

		count++;

	}
}