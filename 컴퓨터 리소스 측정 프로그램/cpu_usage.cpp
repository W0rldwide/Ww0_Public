// ConsoleApplication1.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <windows.h>
#include <stdio.h>



float try_cpu_usage(const FILETIME* pre_idle, const FILETIME* pre_usertime, const FILETIME* pre_kerneltime, const FILETIME* aft_idle, const FILETIME* aft_usertime, const FILETIME* aft_kerneltime) {
	ULARGE_INTEGER stru_fir_idle, stru_fir_usertime, stru_fir_kerneltime;
	ULARGE_INTEGER stru_sec_idle, stru_sec_usertime, stru_sec_kerneltime;

	//반환값을 Low/High로 나누어 구조체에 저장
	stru_fir_idle.LowPart = pre_idle->dwLowDateTime; 
	stru_fir_idle.HighPart = pre_idle->dwHighDateTime;
	stru_fir_usertime.LowPart = pre_usertime->dwLowDateTime;
	stru_fir_usertime.HighPart = pre_usertime->dwHighDateTime;
	stru_fir_kerneltime.LowPart = pre_kerneltime->dwLowDateTime;
	stru_fir_kerneltime.HighPart = pre_kerneltime->dwHighDateTime;

	stru_sec_idle.LowPart = aft_idle->dwLowDateTime;
	stru_sec_idle.HighPart = aft_idle->dwHighDateTime;
	stru_sec_usertime.LowPart = aft_usertime->dwLowDateTime;
	stru_sec_usertime.HighPart = aft_usertime->dwHighDateTime;
	stru_sec_kerneltime.LowPart = aft_kerneltime->dwLowDateTime;
	stru_sec_kerneltime.HighPart = aft_kerneltime->dwHighDateTime;

	//최종 반환값 계산 (CPU 사용시간인 user+kerneltime 대비 유휴시간 idle의 퍼센트비율 계산)
	ULONGLONG calcule_idle = stru_sec_idle.QuadPart - stru_fir_idle.QuadPart;
	ULONGLONG calcule_usertime = stru_sec_usertime.QuadPart - stru_fir_usertime.QuadPart;
	ULONGLONG calcule_kerneltime = stru_sec_kerneltime.QuadPart - stru_fir_kerneltime.QuadPart;
	ULONGLONG result_usingcpu = calcule_usertime + calcule_kerneltime;

	float cpu_usage = (float)(result_usingcpu - calcule_idle) * 100.0f / result_usingcpu;

	return cpu_usage;
}



int main() {
	FILETIME pre_idle, pre_usertime, pre_kerneltime;
	FILETIME aft_idle, aft_usertime, aft_kerneltime;

	//첫번째 시스템시간 체크
	GetSystemTimes(&pre_idle, &pre_usertime, &pre_kerneltime);

	//(1초쉬고)두번째 시스템시간 체크
	while (1) {
		Sleep(1000);
		GetSystemTimes(&aft_idle, &aft_usertime, &aft_kerneltime);

		//최종 시스템 사용량 계산결과 출력
		float cpu_usage = try_cpu_usage(&pre_idle, &pre_usertime, &pre_kerneltime, &aft_idle, &aft_usertime, &aft_kerneltime);
		printf("CPU_USAGE: %.2f%%\n", cpu_usage);
		
		//자원 초기화
		pre_idle = aft_idle;
		pre_usertime = aft_usertime;
		pre_kerneltime = aft_kerneltime;
	}
	return 0;
}