void ResetControls(int consoleCtrl, int wiiCtrl)
{
	int i;
	/*** Gamecube controller Padmap ***/
	if(consoleCtrl == -1 || (consoleCtrl == CTRL_PAD && wiiCtrl == CTRLR_GCPAD))
	{
		i=0;
		btnmap[CTRL_PAD][CTRLR_GCPAD][i++] = PAD_BUTTON_A;
		btnmap[CTRL_PAD][CTRLR_GCPAD][i++] = PAD_BUTTON_B;
		btnmap[CTRL_PAD][CTRLR_GCPAD][i++] = PAD_BUTTON_X;
		btnmap[CTRL_PAD][CTRLR_GCPAD][i++] = PAD_BUTTON_Y;
		btnmap[CTRL_PAD][CTRLR_GCPAD][i++] = PAD_TRIGGER_L;
		btnmap[CTRL_PAD][CTRLR_GCPAD][i++] = PAD_TRIGGER_R;
		btnmap[CTRL_PAD][CTRLR_GCPAD][i++] = PAD_BUTTON_START;
		btnmap[CTRL_PAD][CTRLR_GCPAD][i++] = PAD_TRIGGER_Z;
		btnmap[CTRL_PAD][CTRLR_GCPAD][i++] = PAD_BUTTON_UP;
		btnmap[CTRL_PAD][CTRLR_GCPAD][i++] = PAD_BUTTON_DOWN;
		btnmap[CTRL_PAD][CTRLR_GCPAD][i++] = PAD_BUTTON_LEFT;
		btnmap[CTRL_PAD][CTRLR_GCPAD][i++] = PAD_BUTTON_RIGHT;
	}

	/*** Wiimote Padmap ***/
	if(consoleCtrl == -1 || (consoleCtrl == CTRL_PAD && wiiCtrl == CTRLR_WIIMOTE))
	{
		i=0;
		btnmap[CTRL_PAD][CTRLR_WIIMOTE][i++] = WPAD_BUTTON_B;
		btnmap[CTRL_PAD][CTRLR_WIIMOTE][i++] = WPAD_BUTTON_2;
		btnmap[CTRL_PAD][CTRLR_WIIMOTE][i++] = WPAD_BUTTON_1;
		btnmap[CTRL_PAD][CTRLR_WIIMOTE][i++] = WPAD_BUTTON_A;
		btnmap[CTRL_PAD][CTRLR_WIIMOTE][i++] = 0x0000;
		btnmap[CTRL_PAD][CTRLR_WIIMOTE][i++] = 0x0000;
		btnmap[CTRL_PAD][CTRLR_WIIMOTE][i++] = WPAD_BUTTON_PLUS;
		btnmap[CTRL_PAD][CTRLR_WIIMOTE][i++] = WPAD_BUTTON_MINUS;
		btnmap[CTRL_PAD][CTRLR_WIIMOTE][i++] = WPAD_BUTTON_RIGHT;
		btnmap[CTRL_PAD][CTRLR_WIIMOTE][i++] = WPAD_BUTTON_LEFT;
		btnmap[CTRL_PAD][CTRLR_WIIMOTE][i++] = WPAD_BUTTON_UP;
		btnmap[CTRL_PAD][CTRLR_WIIMOTE][i++] = WPAD_BUTTON_DOWN;
	}

	/*** Classic Controller Padmap ***/
	if(consoleCtrl == -1 || (consoleCtrl == CTRL_PAD && wiiCtrl == CTRLR_CLASSIC))
	{
		i=0;
		btnmap[CTRL_PAD][CTRLR_CLASSIC][i++] = WPAD_CLASSIC_BUTTON_A;
		btnmap[CTRL_PAD][CTRLR_CLASSIC][i++] = WPAD_CLASSIC_BUTTON_B;
		btnmap[CTRL_PAD][CTRLR_CLASSIC][i++] = WPAD_CLASSIC_BUTTON_X;
		btnmap[CTRL_PAD][CTRLR_CLASSIC][i++] = WPAD_CLASSIC_BUTTON_Y;
		btnmap[CTRL_PAD][CTRLR_CLASSIC][i++] = WPAD_CLASSIC_BUTTON_FULL_L;
		btnmap[CTRL_PAD][CTRLR_CLASSIC][i++] = WPAD_CLASSIC_BUTTON_FULL_R;
		btnmap[CTRL_PAD][CTRLR_CLASSIC][i++] = WPAD_CLASSIC_BUTTON_PLUS;
		btnmap[CTRL_PAD][CTRLR_CLASSIC][i++] = WPAD_CLASSIC_BUTTON_MINUS;
		btnmap[CTRL_PAD][CTRLR_CLASSIC][i++] = WPAD_CLASSIC_BUTTON_UP;
		btnmap[CTRL_PAD][CTRLR_CLASSIC][i++] = WPAD_CLASSIC_BUTTON_DOWN;
		btnmap[CTRL_PAD][CTRLR_CLASSIC][i++] = WPAD_CLASSIC_BUTTON_LEFT;
		btnmap[CTRL_PAD][CTRLR_CLASSIC][i++] = WPAD_CLASSIC_BUTTON_RIGHT;
	}

	/*** Wii U Pro Padmap ***/
	if(consoleCtrl == -1 || (consoleCtrl == CTRL_PAD && wiiCtrl == CTRLR_WUPC))
	{
		i=0;
		btnmap[CTRL_PAD][CTRLR_WUPC][i++] = WPAD_CLASSIC_BUTTON_A;
		btnmap[CTRL_PAD][CTRLR_WUPC][i++] = WPAD_CLASSIC_BUTTON_B;
		btnmap[CTRL_PAD][CTRLR_WUPC][i++] = WPAD_CLASSIC_BUTTON_X;
		btnmap[CTRL_PAD][CTRLR_WUPC][i++] = WPAD_CLASSIC_BUTTON_Y;
		btnmap[CTRL_PAD][CTRLR_WUPC][i++] = WPAD_CLASSIC_BUTTON_FULL_L;
		btnmap[CTRL_PAD][CTRLR_WUPC][i++] = WPAD_CLASSIC_BUTTON_FULL_R;
		btnmap[CTRL_PAD][CTRLR_WUPC][i++] = WPAD_CLASSIC_BUTTON_PLUS;
		btnmap[CTRL_PAD][CTRLR_WUPC][i++] = WPAD_CLASSIC_BUTTON_MINUS;
		btnmap[CTRL_PAD][CTRLR_WUPC][i++] = WPAD_CLASSIC_BUTTON_UP;
		btnmap[CTRL_PAD][CTRLR_WUPC][i++] = WPAD_CLASSIC_BUTTON_DOWN;
		btnmap[CTRL_PAD][CTRLR_WUPC][i++] = WPAD_CLASSIC_BUTTON_LEFT;
		btnmap[CTRL_PAD][CTRLR_WUPC][i++] = WPAD_CLASSIC_BUTTON_RIGHT;
	}

	/*** Wii U Gamepad Padmap ***/
	if(consoleCtrl == -1 || (consoleCtrl == CTRL_PAD && wiiCtrl == CTRLR_WIIDRC))
	{
		i=0;
		btnmap[CTRL_PAD][CTRLR_WIIDRC][i++] = WIIDRC_BUTTON_A;
		btnmap[CTRL_PAD][CTRLR_WIIDRC][i++] = WIIDRC_BUTTON_B;
		btnmap[CTRL_PAD][CTRLR_WIIDRC][i++] = WIIDRC_BUTTON_X;
		btnmap[CTRL_PAD][CTRLR_WIIDRC][i++] = WIIDRC_BUTTON_Y;
		btnmap[CTRL_PAD][CTRLR_WIIDRC][i++] = WIIDRC_BUTTON_L;
		btnmap[CTRL_PAD][CTRLR_WIIDRC][i++] = WIIDRC_BUTTON_R;
		btnmap[CTRL_PAD][CTRLR_WIIDRC][i++] = WIIDRC_BUTTON_PLUS;
		btnmap[CTRL_PAD][CTRLR_WIIDRC][i++] = WIIDRC_BUTTON_MINUS;
		btnmap[CTRL_PAD][CTRLR_WIIDRC][i++] = WIIDRC_BUTTON_UP;
		btnmap[CTRL_PAD][CTRLR_WIIDRC][i++] = WIIDRC_BUTTON_DOWN;
		btnmap[CTRL_PAD][CTRLR_WIIDRC][i++] = WIIDRC_BUTTON_LEFT;
		btnmap[CTRL_PAD][CTRLR_WIIDRC][i++] = WIIDRC_BUTTON_RIGHT;
	}
		
	/*** Nunchuk + wiimote Padmap ***/
	if(consoleCtrl == -1 || (consoleCtrl == CTRL_PAD && wiiCtrl == CTRLR_NUNCHUK))
	{
		i=0;
		btnmap[CTRL_PAD][CTRLR_NUNCHUK][i++] = WPAD_BUTTON_A;
		btnmap[CTRL_PAD][CTRLR_NUNCHUK][i++] = WPAD_BUTTON_B;
		btnmap[CTRL_PAD][CTRLR_NUNCHUK][i++] = WPAD_NUNCHUK_BUTTON_C;
		btnmap[CTRL_PAD][CTRLR_NUNCHUK][i++] = WPAD_NUNCHUK_BUTTON_Z;
		btnmap[CTRL_PAD][CTRLR_NUNCHUK][i++] = WPAD_BUTTON_2;
		btnmap[CTRL_PAD][CTRLR_NUNCHUK][i++] = WPAD_BUTTON_1;
		btnmap[CTRL_PAD][CTRLR_NUNCHUK][i++] = WPAD_BUTTON_PLUS;
		btnmap[CTRL_PAD][CTRLR_NUNCHUK][i++] = WPAD_BUTTON_MINUS;
		btnmap[CTRL_PAD][CTRLR_NUNCHUK][i++] = WPAD_BUTTON_UP;
		btnmap[CTRL_PAD][CTRLR_NUNCHUK][i++] = WPAD_BUTTON_DOWN;
		btnmap[CTRL_PAD][CTRLR_NUNCHUK][i++] = WPAD_BUTTON_LEFT;
		btnmap[CTRL_PAD][CTRLR_NUNCHUK][i++] = WPAD_BUTTON_RIGHT;
	}

	/*** Superscope : GC controller button mapping ***/
	if(consoleCtrl == -1 || (consoleCtrl == CTRL_SCOPE && wiiCtrl == CTRLR_GCPAD))
	{
		i=0;
		btnmap[CTRL_SCOPE][CTRLR_GCPAD][i++] = PAD_BUTTON_A;
		btnmap[CTRL_SCOPE][CTRLR_GCPAD][i++] = PAD_BUTTON_B;
		btnmap[CTRL_SCOPE][CTRLR_GCPAD][i++] = PAD_TRIGGER_Z;
		btnmap[CTRL_SCOPE][CTRLR_GCPAD][i++] = PAD_BUTTON_Y;
		btnmap[CTRL_SCOPE][CTRLR_GCPAD][i++] = PAD_BUTTON_X;
		btnmap[CTRL_SCOPE][CTRLR_GCPAD][i++] = PAD_BUTTON_START;
	}

	/*** Superscope : wiimote button mapping ***/
	if(consoleCtrl == -1 || (consoleCtrl == CTRL_SCOPE && wiiCtrl == CTRLR_WIIMOTE))
	{
		i=0;
		btnmap[CTRL_SCOPE][CTRLR_WIIMOTE][i++] = WPAD_BUTTON_B;
		btnmap[CTRL_SCOPE][CTRLR_WIIMOTE][i++] = WPAD_BUTTON_A;
		btnmap[CTRL_SCOPE][CTRLR_WIIMOTE][i++] = WPAD_BUTTON_MINUS;
		btnmap[CTRL_SCOPE][CTRLR_WIIMOTE][i++] = WPAD_BUTTON_UP;
		btnmap[CTRL_SCOPE][CTRLR_WIIMOTE][i++] = WPAD_BUTTON_DOWN;
		btnmap[CTRL_SCOPE][CTRLR_WIIMOTE][i++] = WPAD_BUTTON_PLUS;
	}

	/*** Superscope : Classic Controller button mapping ***/
	if(consoleCtrl == -1 || (consoleCtrl == CTRL_SCOPE && wiiCtrl == CTRLR_CLASSIC))
	{
		i=0;
		btnmap[CTRL_SCOPE][CTRLR_CLASSIC][i++] = WPAD_CLASSIC_BUTTON_B;
		btnmap[CTRL_SCOPE][CTRLR_CLASSIC][i++] = WPAD_CLASSIC_BUTTON_A;
		btnmap[CTRL_SCOPE][CTRLR_CLASSIC][i++] = WPAD_CLASSIC_BUTTON_MINUS;
		btnmap[CTRL_SCOPE][CTRLR_CLASSIC][i++] = WPAD_CLASSIC_BUTTON_Y;
		btnmap[CTRL_SCOPE][CTRLR_CLASSIC][i++] = WPAD_CLASSIC_BUTTON_X;
		btnmap[CTRL_SCOPE][CTRLR_CLASSIC][i++] = WPAD_CLASSIC_BUTTON_PLUS;
	}

	/*** Superscope : Wii U Pro Controller button mapping ***/
	if(consoleCtrl == -1 || (consoleCtrl == CTRL_SCOPE && wiiCtrl == CTRLR_WUPC))
	{
		i=0;
		btnmap[CTRL_SCOPE][CTRLR_WUPC][i++] = WPAD_CLASSIC_BUTTON_B;
		btnmap[CTRL_SCOPE][CTRLR_WUPC][i++] = WPAD_CLASSIC_BUTTON_A;
		btnmap[CTRL_SCOPE][CTRLR_WUPC][i++] = WPAD_CLASSIC_BUTTON_MINUS;
		btnmap[CTRL_SCOPE][CTRLR_WUPC][i++] = WPAD_CLASSIC_BUTTON_Y;
		btnmap[CTRL_SCOPE][CTRLR_WUPC][i++] = WPAD_CLASSIC_BUTTON_X;
		btnmap[CTRL_SCOPE][CTRLR_WUPC][i++] = WPAD_CLASSIC_BUTTON_PLUS;
	}

	/*** Superscope : Wii U Gamepad button mapping ***/
	if(consoleCtrl == -1 || (consoleCtrl == CTRL_SCOPE && wiiCtrl == CTRLR_WIIDRC))
	{
		i=0;
		btnmap[CTRL_SCOPE][CTRLR_WIIDRC][i++] = WIIDRC_BUTTON_B;
		btnmap[CTRL_SCOPE][CTRLR_WIIDRC][i++] = WIIDRC_BUTTON_A;
		btnmap[CTRL_SCOPE][CTRLR_WIIDRC][i++] = WIIDRC_BUTTON_MINUS;
		btnmap[CTRL_SCOPE][CTRLR_WIIDRC][i++] = WIIDRC_BUTTON_Y;
		btnmap[CTRL_SCOPE][CTRLR_WIIDRC][i++] = WIIDRC_BUTTON_X;
		btnmap[CTRL_SCOPE][CTRLR_WIIDRC][i++] = WIIDRC_BUTTON_PLUS;
	}

	/*** Mouse : GC controller button mapping ***/
	if(consoleCtrl == -1 || (consoleCtrl == CTRL_MOUSE && wiiCtrl == CTRLR_GCPAD))
	{
		i=0;
		btnmap[CTRL_MOUSE][CTRLR_GCPAD][i++] = PAD_BUTTON_A;
		btnmap[CTRL_MOUSE][CTRLR_GCPAD][i++] = PAD_BUTTON_B;
	}

	/*** Mouse : wiimote button mapping ***/
	if(consoleCtrl == -1 || (consoleCtrl == CTRL_MOUSE && wiiCtrl == CTRLR_WIIMOTE))
	{
		i=0;
		btnmap[CTRL_MOUSE][CTRLR_WIIMOTE][i++] = WPAD_BUTTON_A;
		btnmap[CTRL_MOUSE][CTRLR_WIIMOTE][i++] = WPAD_BUTTON_B;
	}

	/*** Mouse : Classic Controller button mapping ***/
	if(consoleCtrl == -1 || (consoleCtrl == CTRL_MOUSE && wiiCtrl == CTRLR_CLASSIC))
	{
		i=0;
		btnmap[CTRL_MOUSE][CTRLR_CLASSIC][i++] = WPAD_CLASSIC_BUTTON_A;
		btnmap[CTRL_MOUSE][CTRLR_CLASSIC][i++] = WPAD_CLASSIC_BUTTON_B;
	}

	/*** Mouse : Wii U Pro Controller button mapping ***/
	if(consoleCtrl == -1 || (consoleCtrl == CTRL_MOUSE && wiiCtrl == CTRLR_WUPC))
	{
		i=0;
		btnmap[CTRL_MOUSE][CTRLR_WUPC][i++] = WPAD_CLASSIC_BUTTON_A;
		btnmap[CTRL_MOUSE][CTRLR_WUPC][i++] = WPAD_CLASSIC_BUTTON_B;
	}

	/*** Mouse : Wii U Gamepad button mapping ***/
	if(consoleCtrl == -1 || (consoleCtrl == CTRL_MOUSE && wiiCtrl == CTRLR_WIIDRC))
	{
		i=0;
		btnmap[CTRL_MOUSE][CTRLR_WIIDRC][i++] = WIIDRC_BUTTON_A;
		btnmap[CTRL_MOUSE][CTRLR_WIIDRC][i++] = WIIDRC_BUTTON_B;
	}

	/*** Justifier : GC controller button mapping ***/
	if(consoleCtrl == -1 || (consoleCtrl == CTRL_JUST && wiiCtrl == CTRLR_GCPAD))
	{
		i=0;
		btnmap[CTRL_JUST][CTRLR_GCPAD][i++] = PAD_BUTTON_B;
		btnmap[CTRL_JUST][CTRLR_GCPAD][i++] = PAD_BUTTON_A;
		btnmap[CTRL_JUST][CTRLR_GCPAD][i++] = PAD_BUTTON_START;
	}

	/*** Justifier : wiimote button mapping ***/
	if(consoleCtrl == -1 || (consoleCtrl == CTRL_JUST && wiiCtrl == CTRLR_WIIMOTE))
	{
		i=0;
		btnmap[CTRL_JUST][CTRLR_WIIMOTE][i++] = WPAD_BUTTON_B;
		btnmap[CTRL_JUST][CTRLR_WIIMOTE][i++] = WPAD_BUTTON_A;
		btnmap[CTRL_JUST][CTRLR_WIIMOTE][i++] = WPAD_BUTTON_PLUS;
	}

	/*** Justifier : Classic Controller button mapping ***/
	if(consoleCtrl == -1 || (consoleCtrl == CTRL_JUST && wiiCtrl == CTRLR_CLASSIC))
	{
		i=0;
		btnmap[CTRL_JUST][CTRLR_CLASSIC][i++] = WPAD_CLASSIC_BUTTON_B;
		btnmap[CTRL_JUST][CTRLR_CLASSIC][i++] = WPAD_CLASSIC_BUTTON_A;
		btnmap[CTRL_JUST][CTRLR_CLASSIC][i++] = WPAD_CLASSIC_BUTTON_PLUS;
	}

	/*** Justifier : Wii U Pro Controller button mapping ***/
	if(consoleCtrl == -1 || (consoleCtrl == CTRL_JUST && wiiCtrl == CTRLR_WUPC))
	{
		i=0;
		btnmap[CTRL_JUST][CTRLR_WUPC][i++] = WPAD_CLASSIC_BUTTON_B;
		btnmap[CTRL_JUST][CTRLR_WUPC][i++] = WPAD_CLASSIC_BUTTON_A;
		btnmap[CTRL_JUST][CTRLR_WUPC][i++] = WPAD_CLASSIC_BUTTON_PLUS;
	}

	/*** Justifier : Wii U Gamepad button mapping ***/
	if(consoleCtrl == -1 || (consoleCtrl == CTRL_JUST && wiiCtrl == CTRLR_WIIDRC))
	{
		i=0;
		btnmap[CTRL_JUST][CTRLR_WIIDRC][i++] = WIIDRC_BUTTON_B;
		btnmap[CTRL_JUST][CTRLR_WIIDRC][i++] = WIIDRC_BUTTON_A;
		btnmap[CTRL_JUST][CTRLR_WIIDRC][i++] = WIIDRC_BUTTON_PLUS;
	}
}
