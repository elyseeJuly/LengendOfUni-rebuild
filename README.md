# Legend of Uni: Three-Body Rebuild | 宇宙群英传：三体重构

[![Web Version](https://img.shields.io/badge/Web_Version-Alpha-orange.svg)](#03-web-reconstruction-web重构版)
[![Windows Version](https://img.shields.io/badge/Windows_Version-Legacy_Finished-blue.svg)](#01-windows-legacy-source-windows版源码)
[![Tech Stack](https://img.shields.io/badge/Stack-React_19_%7C_Vite_%7C_TS-61dafb.svg)](#)
[![License](https://img.shields.io/badge/License-MIT-green.svg)](#-开源协议-license)

这是一个跨时代的“三体”主题 4X 太空策略模拟游戏项目。项目包含 2009 年的原始 Windows MFC 源代码以及正在进行的现代 Web 重构版。

---

## 📂 项目结构

项目分为三个核心部分，旨在清晰区分历史遗产与现代重构：

1.  **[01_Windows_Source](./01_Windows_Source)**: 原始 Windows MFC C++ 源代码（**重构修复版**）。解决了原始代码中的编码乱码问题，并补全了部分 AI 逻辑。
2.  **[02_Project_Documentation](./02_Project_Documentation)**: 包含开发日志、技术规范、审计报告及原型设计资产。
3.  **[03_Web_Rebuild](./03_Web_Rebuild)**: 基于 React + TypeScript + Vite 的现代化 Web 重构版。

---

## 🏛️ 01. Windows Legacy Source (Windows版源码)

### 开发进度：**已完成 (Legacy Masterpiece)**
这是项目的起点，一个基于 **C++/MFC/DirectX 9** 的完整单机策略游戏。

-   **核心架构**: 传统的 Windows 消息循环与 `CDialog` 驱动的 UI 体系。
-   **版本说明**: 本目录下存放的是 **本地重构修复版**。相比于 GitHub 初始提交的版本，该版本解决了关键的 GBK 编码乱码问题（如外星文明名称、战斗文本等），并补全了 `pAlienCivi->SetPersonality` 等 AI 逻辑，是目前最完整的 Windows 版参考源码。
-   **功能模块**: 包含 3D 预渲染片头 (`3DPrelude`)、完整的游戏逻辑 (`LengendOfUni`) 及 音乐播放器 (`MusicPlayer`)。
-   **技术特性**: 使用 `CArchive` 进行二进制数据序列化，支持 GBK 编码的 INI 配置系统。
-   **历史地位**: 2009 年的国产策略佳作，为重构版提供了完整且严谨的逻辑参考。

---

## 🚀 03. Web Reconstruction (Web重构版)

### 开发进度：**Alpha 阶段 (核心功能已迁移)**
旨在将经典的 Windows 体验带入现代浏览器，实现全平台可玩性。

-   **技术栈**: React 19 + TypeScript + Vite 8 + Tailwind CSS 4。
-   **重构亮点**:
    -   **叙事引擎 (M8)**: 全新的事件驱动系统，支持打字机效果、多分支剧情选择。
    -   **星图系统**: 使用 Canvas 2D 重制的太阳系动态轨道模型，支持平滑的交互与缩放。
    -   **数据架构**: 全面由二进制/INI 转向现代 JSON 配置，支持 `localStorage` 存档与逻辑持久化。
    -   **审美重塑**: 引入“工笔赛博 (Gongbi Cyberpunk)”美术风格，重绘核心角色立绘。

### 🔄 Windows vs Web 主要区别

| 特性 | Windows 原始版 (2009) | Web 重构版 (2026) |
| :--- | :--- | :--- |
| **底层语言** | C++ (MFC) | TypeScript (React) |
| **图形库** | DirectX 9 / GDI | HTML5 Canvas / CSS3 |
| **存储格式** | 二进制 (.sav) / INI | JSON / localStorage |
| **跨平台** | 仅限 Windows | 全平台 (浏览器即开即玩) |
| **叙事体验** | 固定文本对话 | 动态分支、事件队列 |
| **配置编码** | GBK (中文易乱码) | UTF-8 (标准国际化) |

---

## 🛠️ 工具与脚本

项目在 `tools/` 目录下提供了一系列辅助工具：
-   `convert_ini_to_json.py`: 自动化将 GBK 编码的原始 INI 数据转换为 Web 标准的 JSON。
-   `play.command`: 快捷启动脚本。

---

## 🌸 致谢与版权声明

本项目能够重启并重构，离不开原作打下的坚实基础。

**特别鸣谢**：
-   **源作者：Tormoo**。感谢您在 2009 年创作了如此出色的《宇宙群英传》，您的代码是这个宇宙的基石。
-   **OurShow 社区**：感谢当年对该项目的支持。

> “失去人性，失去很多；失去兽性，失去一切。” —— 我们在数字世界重建这个宇宙，不仅是为了情怀，更是为了探索文明的无限可能。

---

---

## 📜 开源协议 (License)

### Web 重构版 (Web Rebuild)
本项目 Web 重构版部分遵循 **MIT License** 开源协议。你可以自由地使用、修改和分发该部分的源代码，但请保留原始的版权声明。

### Windows 原始版 (Windows Legacy)
Windows 原始版源代码（`01_Windows_Source`）的版权归原作者 **Tormoo** 所有。本仓库仅作历史归档与重构参考使用。在使用、研究该部分代码时，请务必尊重原作者的劳动成果与开发初衷。

---

> “失去人性，失去很多；失去兽性，失去一切。” —— 我们在数字世界重建这个宇宙，不仅是为了情怀，更是为了探索文明的无限可能。
