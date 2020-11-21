#include <stdio.h>
#include <wiringSerial.h>

/* 외부 코드의 파이 카메라 캡처 요청 함수 */
extern int capture_and_save_bmp(void);

int main(void)
{
	int buf[2];
	int i = 0, speed;

	int fd = serialOpen("/dev/ttyACM0", 115200);
	if (fd < 0) {
		printf("Unable to open serial device\n");
		return -1;
	}

	while (1) {
		if (serialDataAvail(fd)) {
			buf[i] = (int)(serialGetchar(fd)-48);
			i++;
			//printf("%c\n", serialGetchar(fd));	// \n을 넣어주어야 콘솔에 출력된다
		}
		if (i == 2) {
			speed = buf[0]*10 + buf[1];
			printf("%d\n", speed);
			fflush(stdout);
			if (speed > 50) {
				printf("촬영을 시작합니다...\n");
				if (capture_and_save_bmp() < 0) {
					printf("촬영실패\n");
				} else {
					printf("촬영성공\n");
				}
			}
			i = 0;
		}
	}

	return 0;
}
