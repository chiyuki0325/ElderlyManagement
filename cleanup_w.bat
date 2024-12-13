@echo off
echo 活力长者社区管理系统——清除数据
rd /s /q "%AppData%/ElderlyManagement"
echo 数据清除完成。按任意键退出。
pause >nul