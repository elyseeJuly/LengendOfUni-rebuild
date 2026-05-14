import { GameEvent, createGameEvent } from "./GameEvent";
import eventsData from "../data/events.json";
import randomEventsData from "../data/randomevents.json";
import { DialogNode } from "../types/narrative";

export class GameEventManager {
  public events: GameEvent[] = [];
  public randomEvents: GameEvent[] = [];

  constructor() {
    this.init();
  }

  private mapAvatar(bmpName: string): string {
    if (!bmpName || bmpName === "default") return "/images/character_default.png";
    
    // 如果已经是 png/webp 路径，则直接返回
    if (bmpName.startsWith("/images/character_") || bmpName.startsWith("character_")) {
      return bmpName.startsWith("/") ? bmpName : `/images/${bmpName}`;
    }

    const name = bmpName.toLowerCase().replace(".bmp", "");
    const mapping: Record<string, string> = {
      "shiqiang": "/images/character_dashi.png",
      "sophon": "/images/character_sophon.png",
      "luoji": "/images/character_luoji.png",
      "wangmiao": "/images/character_wangmiao_1778724166873.png",
      "beihai": "/images/character_beihai.png",
      "chengxin": "/images/character_chengxin.png",
      "yewenjie": "/images/character_yewenjie.png",
      "wade": "/images/character_wade.png",
      "weide": "/images/character_wade.png",
      "tianming": "/images/character_tianming.png",
      "hines": "/images/character_hines_1778724207245.png",
      "reydiaz": "/images/character_reydiaz_1778724231986.png",
      "tyler": "/images/character_tyler_1778724253558.png",
      "aa": "/images/character_aiaa_1778724300313.png",
      "guan_yifan": "/images/character_guanyifan_1778724448368.png",
      "lin_yun": "/images/character_linyun_1778724276166.png"
    };
    return mapping[name] || "/images/character_default.png";
  }

  public init(): void {
    // 加载固定年份事件
    this.events = this.parseEventData(eventsData);
    // 加载随机事件
    this.randomEvents = this.parseEventData(randomEventsData);
  }

  private parseEventData(dataList: any[]): GameEvent[] {
    const events: GameEvent[] = [];
    dataList.forEach((data: any) => {
      let dialogNodes: DialogNode[] = [];
      
      // 新版格式支持
      if (data.dialogQueue) {
        dialogNodes = data.dialogQueue.map((node: any) => ({
          speakerName: node.speakerName,
          content: node.content,
          avatarUrl: this.mapAvatar(node.avatarUrl)
        }));
      } else {
        // 旧版格式解析
        const talkCount = data.talkcount || 1;
        for (let i = 0; i < talkCount; i++) {
          const speaker = data[`talk${i}_talker`];
          const content = data[`talk${i}_content`];
          const pic = data[`talk${i}_pic`];
          
          if (speaker && content) {
            dialogNodes.push({
              speakerName: speaker,
              content: content,
              avatarUrl: this.mapAvatar(pic)
            });
          }
        }
      }

      const e = createGameEvent(
        data.title || data.name || `事件_${data.id || ""}`, 
        data.eventtype ?? 0,
        data.inYear ?? 0,
        data.tip || (dialogNodes.length > 0 ? dialogNodes[0].content : ""),
        data.eventeffect ?? 0,
        dialogNodes,
        data.id,
        data.triggerCondition,
        data.choices
      );
      events.push(e);
    });
    return events;
  }


  public checkEvents(currentYear: number): GameEvent[] {
    const triggered: GameEvent[] = [];
    this.events.forEach(e => {
      if (!e.hasTriggered && e.inYear === currentYear) {
        e.hasTriggered = true;
        triggered.push(e);
      }
    });
    return triggered;
  }

  public checkRandomEvents(): GameEvent | null {
    const game = GameInstance.get();
    const currentEpoch = game.epoch; // EpochType enum (0, 1, 2, 3, 4)
    const epochNames = ["CRISIS", "DETERRENCE", "BROADCAST", "SHELTER", "GALAXY"];
    const epochName = epochNames[currentEpoch];

    // 打乱顺序进行检查，增加随机性
    const pool = [...this.randomEvents].sort(() => Math.random() - 0.5);
    
    for (const e of pool) {
      // 1. 基础触发概率检查
      const prob = e.triggerCondition?.probability ?? 0.15;
      if (Math.random() > prob) continue;

      // 2. 纪元匹配检查
      if (e.triggerCondition?.epoch && e.triggerCondition.epoch !== "ANY") {
        if (e.triggerCondition.epoch !== epochName) continue;
      }

      // 3. 科技要求检查 (后续可完善)
      
      return e;
    }

    return null;
  }
}


