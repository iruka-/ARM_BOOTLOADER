//
//	�Ƃ肠�����S���_�~�[�֐�.
//


//
//	STM32�̃y���t�F�����֐��Ŋ��ɗp�ӂ���Ă������.
//
void	FLASH_ProgramWord(int adr,int data){}	// 1word�P�ʂ�FLASH��������.
void	FLASH_ErasePage(int adr){}				//  page�P�ʂ�FLASH����.
void 	FLASH_Lock(void){}						// flash��������lock���|����.
void 	FLASH_Unlock(void){}					// flash��������lock���O��.

//
//	LPC1343/LPC2388 �ȂǂŎg�p����y�[�W�P�ʂ̏�������.
//
void	FLASH_ProgramPage(int adr){}			//  page�P�ʂ�FLASH��������/����.

