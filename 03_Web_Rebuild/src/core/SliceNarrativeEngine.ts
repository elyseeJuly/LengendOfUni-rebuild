/**
 * SliceNarrativeEngine - 切片叙事引擎 (UEE Layer 7)
 *
 * 将宏观事件通过"小人物"视角具象化，提升沉浸感。
 * 每个宏观事件可以关联多个切片叙事，展示不同视角。
 *
 * 设计原则：
 * - 切片叙事不改变游戏状态（纯叙事内容）
 * - 每个切片的角色为虚构小人物（不占用角色解锁资源）
 * - 可基于 Tag 自动生成切片
 */

export interface SliceNarrative {
  eventId: string;          // 关联的宏观事件ID
  characterName: string;    // 虚构小人物姓名
  characterRole: string;    // 社会角色（"地下城厨师"、"星舰工程师"等）
  scene: string;            // 场景描述
  innerMonologue: string;   // 内心独白
  impact: string;           // 事件对小人物生活的影响
}

// 小人物名字池（虚构，不占角色解锁资源）
const NPC_NAMES = [
  "张伟", "王芳", "李强", "赵丽", "陈明",
  "刘洋", "周杰", "吴敏", "孙浩", "林静",
];

const NPC_ROLES = [
  "地下城配给管理员", "星舰维修工程师", "生态循环站技术员",
  "行星发动机操作员", "信息监测站观察员", "物资调配中心调度员",
  "社会秩序维护员", "文化档案管理员", "能源站运维工程师",
  "通讯中继站值班员",
];

const SCENE_TEMPLATES = [
  "地下城{area}区的昏暗走廊里，{role}正在{action}。",
  "{role}站在{location}的控制台前，屏幕上的数据{data_state}。",
  "在{location}的休息区，{role}与同事低声讨论着最近的{event_theme}。",
  "{role}的个人日志上记录着这样一段文字：『{monologue}』",
];

const AREAS = ["A-7", "B-12", "C-3", "D-9", "E-5", "F-1", "G-8"];

export class SliceNarrativeEngine {
  private slices: Map<string, SliceNarrative[]> = new Map();
  private usedNpcNames: Set<string> = new Set();

  /** 注册切片叙事 */
  registerSlice(eventId: string, slice: SliceNarrative): void {
    const existing = this.slices.get(eventId) || [];
    existing.push(slice);
    this.slices.set(eventId, existing);
  }

  /** 批量注册切片叙事 */
  registerSlices(eventId: string, slices: SliceNarrative[]): void {
    const existing = this.slices.get(eventId) || [];
    this.slices.set(eventId, [...existing, ...slices]);
  }

  /** 获取事件的切片叙事 */
  getSlices(eventId: string): SliceNarrative[] {
    return this.slices.get(eventId) || [];
  }

  /** 自动生成切片叙事 */
  generateSlice(eventId: string, eventTitle: string, _tagManager: any): SliceNarrative {
    const name = this.getNextNpcName();
    const role = NPC_ROLES[Math.floor(Math.random() * NPC_ROLES.length)];
    const area = AREAS[Math.floor(Math.random() * AREAS.length)];

    const scene = SCENE_TEMPLATES[Math.floor(Math.random() * SCENE_TEMPLATES.length)]
      .replace('{area}', area)
      .replace('{role}', role)
      .replace('{action}', '检查着日常设备运行状态')
      .replace('{location}', '地下城')
      .replace('{data_state}', '显示着不寻常的波动')
      .replace('{event_theme}', eventTitle)
      .replace('{monologue}', `听说${eventTitle}的消息后，我意识到这个世界正在发生变化……`);

    const slice: SliceNarrative = {
      eventId,
      characterName: name,
      characterRole: role,
      scene,
      innerMonologue: `「${eventTitle}……这会对我们的生活造成什么影响呢？」`,
      impact: `作为一名${role}，${name}的工作节奏因为${eventTitle}而被打乱。`,
    };

    this.registerSlice(eventId, slice);
    return slice;
  }

  /** 获取下一个可用的 NPC 名字 */
  private getNextNpcName(): string {
    const available = NPC_NAMES.filter(n => !this.usedNpcNames.has(n));
    if (available.length === 0) {
      this.usedNpcNames.clear(); // 循环使用
      return NPC_NAMES[Math.floor(Math.random() * NPC_NAMES.length)];
    }
    const name = available[Math.floor(Math.random() * available.length)];
    this.usedNpcNames.add(name);
    return name;
  }

  /** 检查事件是否有切片叙事 */
  hasSlices(eventId: string): boolean {
    return (this.slices.get(eventId)?.length ?? 0) > 0;
  }

  toJSON(): object {
    return {
      slices: Array.from(this.slices.entries()),
      usedNpcNames: Array.from(this.usedNpcNames),
    };
  }

  static fromJSON(data: any): SliceNarrativeEngine {
    const sne = new SliceNarrativeEngine();
    if (data?.slices) {
      for (const [k, v] of data.slices) {
        sne.slices.set(k, v);
      }
    }
    if (data?.usedNpcNames) {
      sne.usedNpcNames = new Set(data.usedNpcNames);
    }
    return sne;
  }

  reset(): void {
    this.slices.clear();
    this.usedNpcNames.clear();
  }
}