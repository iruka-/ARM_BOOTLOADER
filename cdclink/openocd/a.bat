rem 
rem �J�����gDIR�ɂ��� main-0000.hex��STM8S-D��STM32CPU�ɏ�������Ŏ����I�ɏI������.
rem
rem openocd.exe -f blaster.cfg -f stm32.cfg -f batch.cfg

openocd.exe -f cdc_if.cfg -f stm32f1x.cfg -f batch.cfg 
rem pause