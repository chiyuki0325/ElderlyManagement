#!/bin/sh
echo "活力长者社区管理系统——清除数据"
rm -rf "${XDG_CONFIG_HOME:-$HOME/.config}/ElderlyManagement"
echo "数据清除完成。"