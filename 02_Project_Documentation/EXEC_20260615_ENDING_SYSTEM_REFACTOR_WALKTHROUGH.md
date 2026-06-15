# 游戏结局系统重构、跨周目持久化及 Metagame UI 开发执行报告

> **文档编号**: EXEC_20260615_ENDING_SYSTEM_REFACTOR_WALKTHROUGH  
> **完成日期**: 2026-06-15  
> **分类前缀**: `EXEC_` (执行报告)  
> **执行人**: Antigravity  

---

## 📖 一、 概述与修改目的

为了实现高标准的结局玩法体验与稳健的底层逻辑，我们针对《三体：宇宙群英传》的结局触发系统和周边元游戏（Metagame）系统进行了彻底的重构和开发。

主要目的如下：
1. **防重叠与结局互斥**：确保在一局游戏中，玩家只能以最高优先级且唯一的路径触发胜负结局，避免触发标志重叠导致多个结局同时弹出的系统漏洞。
2. **跨周目遗迹持久化**：使玩家在旧周目遭遇失败（如灭绝、氦闪）时的游戏历史（年份、文化、科技数）能保存到 LocalStorage 中，并在下周目随机生成专属遭遇遗迹，提高游戏的可重玩性与 NG+ 体验。
3. **视觉性能表现自适应**：结局过场动画包含大量 WebGL/Canvas 粒子动效，为避免低端配置发生卡顿，设计了粒子数量自适应下降策略。
4. **画廊与信息回溯集成**：实现主界面“岁月史书”独立画廊、胜负结局走向预警面板以及关键决策的时间线回溯面板。

---

## 🛠️ 二、 执行内容与修改详情

### 1. 核心判定重构与防绕过拦截
- **逻辑重构**：修改了 [Game.ts](file:///Users/quantumrose/Documents/Emberois/LengendOfUni-rebuild/03_Web_Rebuild/src/core/Game.ts) 中的 `checkVictoryConditions()`。将各胜利条件设计为显式的前置互斥。例如：
  - 数字永生胜利排斥流浪计划完成、黑域决策与征服宣战。
  - 威慑胜利排斥征服宣战、流浪地球完成、数字永生与黑域决策。
- **防绕过收口**：将所有导致直接 Game Over 的行为（如人口归零、二向箔打击、坐标广播）均改为仅在内部设置触发状态（如 `dimensionStrikeTriggered`），并将判定归口到回合结束时统一在 `checkVictoryConditions()` 内处理，从而修复了绕过胜利结算直接退出的漏洞。

### 2. 跨周目 LocalStorage 遗迹持久化
- **API 开发**：在 [SaveManager.ts](file:///Users/quantumrose/Documents/Emberois/LengendOfUni-rebuild/03_Web_Rebuild/src/core/SaveManager.ts) 中新增了 `saveRuinRecord`、`getRuinHistory` 和 `clearRuinHistory`，最多存储 5 条最近的失败记录，采用 FIFO 淘汰。
- **事件绑定**：在 [Game.ts](file:///Users/quantumrose/Documents/Emberois/LengendOfUni-rebuild/03_Web_Rebuild/src/core/Game.ts) 的第 50 回合，检查 LocalStorage 中的遗迹，随机在某星系坐标触发“深空异常遗迹”事件，提供文化或资源上的逆向研究成长。

### 3. Metagame UI 与性能自适应
- **性能评估**：新建了 [particlePerformance.ts](file:///Users/quantumrose/Documents/Emberois/LengendOfUni-rebuild/03_Web_Rebuild/src/components/ending/particlePerformance.ts)，基于 CPU 逻辑核心数、移动端环境和帧率自动定级（High / Medium / Low）。
- **动效集成**：在 [EndingCinematic.tsx](file:///Users/quantumrose/Documents/Emberois/LengendOfUni-rebuild/03_Web_Rebuild/src/components/ending/EndingCinematic.tsx) 的 Canvas 渲染中调用该评级，动态限制最大粒子数并开闭 Glow 发光特效。
- **画廊与回溯**：
  - 新建 [EndingForecastPanel.tsx](file:///Users/quantumrose/Documents/Emberois/LengendOfUni-rebuild/03_Web_Rebuild/src/components/ending/EndingForecastPanel.tsx)（已挂载至 [RightInspector.tsx](file:///Users/quantumrose/Documents/Emberois/LengendOfUni-rebuild/03_Web_Rebuild/src/components/RightInspector.tsx)），对结局倾向性进行预警。
  - 新建 [KeyDecisionRetrospective.tsx](file:///Users/quantumrose/Documents/Emberois/LengendOfUni-rebuild/03_Web_Rebuild/src/components/ending/KeyDecisionRetrospective.tsx)，展现关键决策的时间线。
  - 新建 [MuseumGallery.tsx](file:///Users/quantumrose/Documents/Emberois/LengendOfUni-rebuild/03_Web_Rebuild/src/components/MuseumGallery.tsx)（已挂载至 [App.tsx](file:///Users/quantumrose/Documents/Emberois/LengendOfUni-rebuild/03_Web_Rebuild/src/App.tsx)），常驻展现岁月史书结局图鉴。

### 4. 解决 TypeScript 编译与集成报错
- **`hasAnyAtWar` 未定义修复**：在 [AlienCivilization.ts](file:///Users/quantumrose/Documents/Emberois/LengendOfUni-rebuild/03_Web_Rebuild/src/core/AlienCivilization.ts) 的 `AlienCiviManager` 中新增了 `hasAnyAtWar(): boolean`，遍历异星文明判定是否存在 `FriendshipType.VERYANGRY` 敌对实体。
- **清除未使用的类型声明**：移除了测试用例文件 `Game.bypassPrevention.test.ts` and `Game.defeatConditions.test.ts` 中多余的 enum 导入，消除未读取报错。

---

## 🧪 三、 测试与构建验证

1. **自动化测试**：
   - 运行 `npx vitest run`。
   - **502 个测试用例全部通过**，通过率达 100%。新编写的 `Game.victoryConditions.test.ts` 和 `Game.defeatConditions.test.ts` 完美通过。
2. **生产构建验证**：
   - 运行 `npm run build`。
   - TypeScript 编译 (`tsc`) 与 Vite 打包无任何错误，顺利输出生产 dist 文件：
     - `dist/assets/index-G-S-oA1k.js` (846.09 kB)
     - `dist/assets/index-BPBnntax.css` (115.37 kB)

---

## 🚀 四、 归档更新与推送说明

- **归档路径**：`02_Project_Documentation/EXEC_20260615_ENDING_SYSTEM_REFACTOR_WALKTHROUGH.md`
- **代码库推送**：已将所有本地修改（26个文件）通过 Git 提交并推送至 `elyseeJuly/LengendOfUni-rebuild` 的 `main` 分支。最新的 Commit 标识为 `1f0aca9`。

---

## 🌌 五、 纪元系统审计与“星屑纪元”扩展建议

### 1. 现有纪元统计
目前游戏中共设计了 **5** 个纪元。定义位于 [enums.ts](file:///Users/quantumrose/Documents/Emberois/LengendOfUni-rebuild/03_Web_Rebuild/src/types/enums.ts) 的 `EpochType` 并且在 [epochs.json](file:///Users/quantumrose/Documents/Emberois/LengendOfUni-rebuild/03_Web_Rebuild/src/data/epochs.json) 中进行数据配置。列表如下：

| 纪元索引 | 纪元名称 | 切换所需文化度区间 (Culture) | 原作对应背景描述 |
| :---: | :---: | :---: | :--- |
| **0** | **危机纪元 (CRISIS)** | `[0, 199]` | 三体危机爆发，人类建造太空军并实施面壁计划。 |
| **1** | **威慑纪元 (DETERRENCE)** | `[200, 499]` | 罗辑成功建立黑暗森林威慑，人类与三体世界达成冷战和平。 |
| **2** | **广播纪元 (BROADCAST)** | `[500, 799]` | 威慑失效，太阳系与三体世界坐标广播暴露。三体舰队撤离。 |
| **3** | **掩体纪元 (BUNKER)** | `[800, 1199]` | 人类利用木星、土星、天王星等掩体规避光粒打击。 |
| **4** | **银河纪元 (GALAXY)** | `[1200, 999999]` | 太阳系被二向箔降维打击毁灭。人类火种乘光速飞船逃往银河系深空。 |

### 2. “星屑纪元 (Stardust Epoch)” 的定位与放置建议
“星屑纪元”在科幻语境中指代星系与文明被更强力的法则武器（如多向折叠、归零等）彻底粉碎，文明流浪流落为茫茫宇宙中的“尘埃与尘沙”，或者是降维后文明以碎片化形态生存的历史。

若要在游戏中加入该纪元，我们建议将其放置在 **银河纪元之后，作为第 6 个纪元（索引 5）**：

#### 放置理由：
1. **科学史诗叙事顺位**：危机 $\rightarrow$ 威慑 $\rightarrow$ 广播 $\rightarrow$ 掩体 $\rightarrow$ 银河（逃亡银河系） $\rightarrow$ **星屑（小宇宙或宇宙坍缩重置前的尘埃生存期）**。星屑纪元最适合表现银河系流亡中后期、大范围降维发生、宇宙各流亡分支由于引力波广播和黑暗森林伏击被打击成小碎片（星屑）的苍凉纪元。
2. **触发条件设置**：
   - 方式一：设定极高的文化阈值条件（如 `minCulture: 2500`），作为超后期的高峰展示。
   - 方式二（推荐）：作为隐藏/触发式纪元。当玩家完成特定的高级结局任务（例如：成功触发 `galaxy_exodus_seen` 银河外逃且游戏轮数超过 400 轮，或触发了与归零者接头 `zero_homer_contacted` 且玩家选择将自身小宇宙的物质归还给大宇宙时），动态将 `epoch` 切换为 `EpochType.STARDUST`。
3. **实现修改路径**：
   - 在 `enums.ts` 的 `EpochType` 中增加 `STARDUST = 5`，并将 `COUNT` 设为 `6`。
   - 在 `epochs.json` 中将银河纪元的 `maxCulture` 限制修改为 `2499`，并新增一行：
     `{ "epoch": 5, "name": "星屑纪元", "minCulture": 2500, "maxCulture": 999999 }`。
   - 在 `Game.ts` 的 `updateEpoch()` 中为 `epochNames` 数组增加 `"星屑纪元"` 标签，并在 `epochTagMap` 增加 `'stardust_era_deep'` 标识绑定。
