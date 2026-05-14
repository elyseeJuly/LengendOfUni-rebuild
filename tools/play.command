#!/bin/zsh
# 宇宙群英传 Web 重制版 - 极速启动器

# 强制加载用户环境以确保 npm 可用
[ -s "$HOME/.zshrc" ] && source "$HOME/.zshrc"
[ -s "$HOME/.bash_profile" ] && source "$HOME/.bash_profile"

# 获取脚本所在目录
DIR="$( cd "$( dirname "${0}" )" && pwd )"
cd "$DIR/../03_Web_Rebuild"

echo "================================================="
echo "🚀 《宇宙群英传》Web 重制版 - 启动中..."
echo "================================================="

# 检查 Node.js 是否安装
if ! command -v node &> /dev/null; then
    echo "❌ 错误: 未检测到 Node.js 环境！"
    echo "请先从 https://nodejs.org/ 安装 Node.js。"
    echo "按下回车键退出..."
    read
    exit 1
fi

# 检查依赖
if [ ! -d "node_modules" ]; then
    echo "📦 正在安装项目依赖，请稍候..."
    npm install
fi

# 检查并清理旧进程
echo "🧹 正在检查并清理旧的服务器进程..."
lsof -ti :5173 | xargs kill -9 2>/dev/null
lsof -ti :5174 | xargs kill -9 2>/dev/null
lsof -ti :5175 | xargs kill -9 2>/dev/null

# 启动服务器
echo "🌐 正在建立本地纠缠节点..."
echo "🚀 正在为您打开星图浏览器..."
echo "-------------------------------------------------"

# 使用 Vite 默认行为打开浏览器，或者延迟打开
(sleep 3 && open "http://localhost:5173") &

npm run dev || {
    echo "❌ 服务器启动失败！"
    echo "请检查环境依赖或端口占用。"
    echo "按下回车键退出..."
    read
}
