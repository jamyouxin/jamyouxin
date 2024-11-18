/*
这是一个用于计算跑步计划执行进度和预估未来计划的进度的命令行程序。
本程序在visual studio 2022环境中编写，在其他环境中可能会出错，比如"localtime_s"。
日期获取和计算部分是ChatGPT-4o生成的代码，我也不知道这部分是怎么运行起来的。
部分参数已经在程序中固定，如这些参数不符，请根据个人计划自行修改。
*/
#include <stdio.h>      //用于输出输入
#include <time.h>       //用于计算时间
#include <stdlib.h>     //用于exit

int main() {
	float Target = 70;		// 定义目标里程(km)
    float DistanceLeast = 2;        // 定义单次跑步最少距离(km)
    float DistanceMost = 10;        //定义单次跑步最多距离(km)

    // 获取当前时间
    time_t now = time(NULL);
    struct tm CurrentTime;

    // 使用 localtime_s 代替 localtime
    localtime_s(&CurrentTime, &now);

    // 设置目标日期：2024年12月13日
    struct tm TargetDate = { 0 };
    TargetDate.tm_year = 2024 - 1900; // 年份从 1900 开始
    TargetDate.tm_mon = 11;       // 月份从 0 开始，实际月份需要减一
    TargetDate.tm_mday = 13;
    TargetDate.tm_hour = 0;
    TargetDate.tm_min = 0;
    TargetDate.tm_sec = 0;

    // 将目标日期转换为 time_t
    time_t TargetTime = mktime(&TargetDate);

    // 格式化并输出目标日期
    char FormattedDate[20];
    strftime(FormattedDate, sizeof(FormattedDate), "%Y年%m月%d日", &TargetDate);

    char CurrentDate[20];
    strftime(CurrentDate, sizeof(CurrentDate), "%Y年%m月%d日", &CurrentTime);

    // 计算天数差：DaysDifference
    double SecondsDifference = difftime(TargetTime, now);
    int DaysDifference = (int)(SecondsDifference / (60 * 60 * 24));

    // 日期判断
    if (DaysDifference < 0) {
        printf("你完蛋啦！乐跑已经结束了。\n当然也不能排除没有更新代码中的日期的可能性。");
        return 1;
    }     

    //已知量
    printf("=========================\n");
    printf("当前日期：%s\n", CurrentDate);
    printf("截止日期：%s\n", FormattedDate);
    printf("目标里程：%4.2fkm\n", Target);
    printf("单次跑步最少距离：%4.2fkm\n", DistanceLeast);
    printf("单次跑步最多距离：%4.2fkm\n", DistanceMost);
    printf("=========================\n\n");

        //交互
    float Distance = 0;
    int validInput = 0;

    while (!validInput) {
        printf("请输入已跑里程(km)：");
        if (scanf_s("%f", &Distance) == 1 && Distance <= Target && Distance >= 0) {
            validInput = 1;  // 输入有效
        }
        else {
            printf("输入无效，请输入一个大于0小于 %.2f 的数字。\n", Target);
            while (getchar() != '\n');  // 清除缓冲区
        }
    }
	
    // 计算
    float RemainingAmount = Target - Distance;
    float Average = RemainingAmount / DaysDifference;
    int TimesMost = (int)((RemainingAmount + 1) / DistanceLeast);      // 每次跑步至少2公里
    int TimesLeast = (int)((RemainingAmount + 9) / DistanceMost);      // 每次跑步至多10公里
    float DaysRemainingLeast = (TimesMost > 0) ? (float)DaysDifference / TimesMost : 0;     // 确保 Times 不为零，以避免除零错误
    float DaysRemainingMost = (TimesLeast > 0) ? (float)DaysDifference / TimesLeast : 0;     // 确保 Times 不为零，以避免除零错误

    // 输出
    printf("\n=========================\n");
    //计算结果
    printf("剩余天数：%2d天\n", DaysDifference);
    printf("剩余里程：%4.2fkm\n", RemainingAmount);

    // 计算进度百分比
    float progress = (Distance / Target) * 100;
    printf("当前进度：%4.2f%%\n", progress);

    // 显示进度条
    int barWidth = 25;      //进度条长度
    printf("[");
    int pos =(int)( barWidth * (progress / 100));
    for (int i = 0; i < barWidth; ++i) {
        if (i < pos) printf("=");
        else if (i == pos) printf(">");
        else printf(" ");
    }
    printf("] %4.2f%%\n", progress);
    printf("未来平均每日需要里程：%4.2fkm\n", Average);
    printf("剩余跑步次数：%2d ~ %2d 次\n", TimesLeast, TimesMost);
    printf("未来跑步间隔天数：%4.2f ~ %4.2f 天\n", DaysRemainingLeast, DaysRemainingMost);
    printf("=========================\n");

    // 防止程序退出
    printf("按回车键退出...");
    int ch;
    ch = getchar(); // 清除上次输入的换行符
    ch = getchar(); // 等待用户输入

    // 主动退出
    exit(0);
}