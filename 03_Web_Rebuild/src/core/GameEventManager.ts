import { GameEvent, createGameEvent } from "./GameEvent";
import eventsData from "../data/events.json";
import randomEventsData from "../data/randomevents.json";
import { DialogNode, FilteredEventPayload } from "../types/narrative";
import { GameInstance } from "./Game";

export class GameEventManager {
  public events: GameEvent[] = [];
  public randomEvents: GameEvent[] = [];
  public filteredEvents: FilteredEventPayload[] = [];
  public triggeredFilteredIds: Set<string> = new Set();

  constructor() {
    this.init();
  }

  private mapAvatar(bmpName: string): string {
    if (!bmpName || bmpName === "default") return "/images/character_default.png";

    // Standardize input
    let name = bmpName.toLowerCase();
    name = name.replace("/images/", "");
    name = name.replace("character_", "");
    name = name.replace("unified_", "");
    name = name.replace(".png", "");
    name = name.replace(".bmp", "");
    name = name.split("_")[0]; // handle timestamp suffixes like _1778...

    const mapping: Record<string, string> = {
      // 8 main characters (unified)
      "dashi": "/images/unified_dashi_1778921331273.png",
      "shiqiang": "/images/unified_dashi_1778921331273.png",
      "sophon": "/images/unified_sophon_1778921509458.png",
      "zhizi": "/images/unified_sophon_1778921509458.png",
      "luoji": "/images/unified_luoji_1778921262534.png",
      "beihai": "/images/unified_beihai_1778921366897.png",
      "zhangbeihai": "/images/unified_beihai_1778921366897.png",
      "chengxin": "/images/unified_chengxin_1778921400346.png",
      "yewenjie": "/images/unified_yewenjie_1778921299091.png",
      "wade": "/images/unified_wade_1778921437022.png",
      "weide": "/images/unified_wade_1778921437022.png",
      "tianming": "/images/unified_tianming_1778921470963.png",
      "yuntianming": "/images/unified_tianming_1778921470963.png",

      // Other characters
      "wangmiao": "/images/character_wangmiao_1778724166873.png",
      "hines": "/images/character_hines_1778724207245.png",
      "xieenshi": "/images/character_hines_1778724207245.png",
      "reydiaz": "/images/character_reydiaz_1778724231986.png",
      "leidiyaizi": "/images/character_reydiaz_1778724231986.png",
      "tyler": "/images/character_tyler_1778724253558.png",
      "taile": "/images/character_tyler_1778724253558.png",
      "aa": "/images/character_aiaa_1778724300313.png",
      "aiaa": "/images/character_aiaa_1778724300313.png",
      "guanyifan": "/images/character_guanyifan_1778724448368.png",
      "linyun": "/images/character_linyun_1778724276166.png",
      "dingyi": "/images/character_dingyi_1778724123469.png",
      "evans": "/images/character_evans_1778724472738.png",
      "yiwensi": "/images/character_evans_1778724472738.png",
      "yangdong": "/images/character_yangdong_1778724413422.png",
      "huahua": "/images/character_huahua_1778818926539.png",
      "yiyi": "/images/character_yiyi_1778724524669.png",
      "shenyuan": "/images/character_shenyuan_1778726061895.png",
      "hawking": "/images/character_hawking_1778726088806.png",
      "huoking": "/images/character_hawking_1778726088806.png",
      "changweisi": "/images/character_changweisi_1778724189193.png",
      "zhuangyan": "/images/character_zhuangyan_1778724322851.png",
      "shuiwa": "/images/character_shuiwa_1778726120500.png",
      "leizhicheng": "/images/character_leizhicheng_1778818873520.png",
      "yangweining": "/images/character_yangweining_1778818900159.png",
      "yanjing": "/images/character_yanjing_1778819395854.png",
      "baibing": "/images/character_baibing_1778819424975.png",
      "miaofuquan": "/images/character_miaofuquan_1778818954566.png",
      "huatang": "/images/character_huatang_1778819276066.png",
      "zhuhanyang": "/images/character_zhuhanyang_1778833149488.png",
      "liucixin": "/images/character_liucixin_1778819370180.png",
      "keiko": "/images/character_keiko_1778724347302.png",
      "shanshanhuizi": "/images/character_keiko_1778724347302.png"
    };

    if (mapping[name]) return mapping[name];

    // If it's already a full path or looks like one, return it, otherwise fallback
    if (bmpName.startsWith("/images/") || bmpName.startsWith("character_") || bmpName.startsWith("unified_") || bmpName.endsWith(".png")) {
      return bmpName.startsWith("/") ? bmpName : `/images/${bmpName}`;
    }

    return "/images/character_default.png";
  }

  public init(): void {
    this.events = this.parseEventData(eventsData);
    this.randomEvents = this.parseEventData(randomEventsData);
    this.seedFilteredEvents();

    if (this.events.length === 0) {
      console.warn("Event data empty, adding fallback welcome event.");
      this.events.push(createGameEvent(
        "系统初始化完成",
        0, 0, "模拟器叙事系统已就绪。",
        0,
        [{ speakerName: "系统 AI", content: "欢迎来到《LegendOfUni》模拟器。当前叙事引擎已由于数据缺失进入紧急备用模式。", avatarUrl: "/images/character_default.png" }]
      ));
    }
  }

  private seedFilteredEvents(): void {
    this.filteredEvents = [
      {
        id: "wallfacer_election",
        title: "面壁者选拔",
        tip: "联合国行星防御理事会(PCD)正在选拔面壁者。",
        dialogQueue: [
          { speakerName: "联合国秘书长", content: "女士们先生们，现在是人类文明存亡的危急关头。我们正式启动面壁计划。", avatarUrl: this.mapAvatar("default") },
          { speakerName: "萨伊", content: "四位面壁者将获得人类文明的全部资源支持。", avatarUrl: this.mapAvatar("default") }
        ],
        condition: { minYear: 10, maxYear: 50, epoch: "CRISIS", minCulture: 30 },
        choices: [
          { label: "全力支持面壁计划", effects: [{ type: "flag", target: "wallfacer_project", value: 1 }, { type: "resource", target: "culture", value: 20 }] },
          { label: "谨慎观望", effects: [{ type: "flag", target: "wallfacer_cautious", value: 1 }, { type: "resource", target: "military", value: 2 }] }
        ],
        cooldownYears: 10
      },
      {
        id: "deterrence_establishment",
        title: "建立威慑体系",
        tip: "罗辑博士提出了黑暗森林威慑理论。",
        dialogQueue: [
          { speakerName: "罗辑", content: "我找到了对抗三体文明的终极方案。", avatarUrl: this.mapAvatar("luoji") },
          { speakerName: "大史", content: "罗老弟，你确定这能行？", avatarUrl: this.mapAvatar("shiqiang") }
        ],
        condition: { minYear: 50, epoch: "CRISIS", reqTech: "黑暗森林威慑", minDeterrence: 50 },
        choices: [
          { label: "任命罗辑为执剑人", effects: [{ type: "flag", target: "swordholder_appointed", value: 1 }, { type: "resource", target: "prestige", value: 50 }] },
          { label: "暂缓威慑体系建设", effects: [{ type: "flag", target: "deterrence_delayed", value: 1 }, { type: "resource", target: "treachery", value: 10 }] }
        ]
      },
      {
        id: "sophon_blockade",
        title: "智子封锁生效",
        tip: "三体文明正式启动了智子对地球科技的全方位封锁。",
        dialogQueue: [
          { speakerName: "智子", content: "你们是虫子。", avatarUrl: this.mapAvatar("sophon") },
          { speakerName: "丁仪", content: "我们的基础物理学...被锁死了。", avatarUrl: this.mapAvatar("dingyi") }
        ],
        condition: { minYear: 10, maxYear: 200, epoch: "CRISIS", reqNotFlag: "sophon_broken" },
        choices: [
          { label: "加速研发量子计算机", effects: [{ type: "flag", target: "quantum_focus", value: 1 }, { type: "resource", target: "economy", value: -20 }] },
          { label: "放弃基础物理，主攻应用技术", effects: [{ type: "flag", target: "applied_tech_focus", value: 1 }, { type: "resource", target: "economy", value: 30 }] }
        ],
        cooldownYears: 5
      },
      {
        id: "wandering_earth_decision",
        title: "流浪地球大辩论",
        tip: "面对即将到来的太阳氦闪，人类必须在多个方案中做出选择。",
        dialogQueue: [
          { speakerName: "联合政府发言人", content: "经过充分论证，流浪地球计划是人类唯一的生路。", avatarUrl: this.mapAvatar("default") },
          { speakerName: "反对派", content: "这是拿全人类的生命在赌博！我们需要数字方舟方案！", avatarUrl: this.mapAvatar("default") }
        ],
        condition: { minYear: 100, epoch: "CRISIS", reqTech: "行星发动机基础" },
        choices: [
          { label: "启动流浪地球计划", effects: [{ type: "flag", target: "wandering_chosen", value: 1 }, { type: "resource", target: "economy", value: -100 }, { type: "resource", target: "prestige", value: 30 }] },
          { label: "转向数字方舟方案", effects: [{ type: "flag", target: "digital_ark_chosen", value: 1 }, { type: "resource", target: "culture", value: 50 }] }
        ],
        cooldownYears: 40
      },
      {
        id: "alien_first_contact",
        title: "地外文明初接触",
        tip: "我们的深空探测阵列发现了一个新的地外文明信号。",
        dialogQueue: [
          { speakerName: "林云", content: "长官，我们接收到一个极其复杂的数学信号。", avatarUrl: this.mapAvatar("lin_yun") },
          { speakerName: "维德", content: "不回应。不要暴露我们的坐标。", avatarUrl: this.mapAvatar("weide") }
        ],
        condition: { minYear: 80, epoch: "CRISIS", reqTech: "50光年远镜" },
        choices: [
          { label: "保持静默", effects: [{ type: "flag", target: "silent_contact", value: 1 }, { type: "resource", target: "prestige", value: 10 }] },
          { label: "发送友好信号", effects: [{ type: "flag", target: "friendly_broadcast", value: 1 }, { type: "resource", target: "culture", value: -20 }] }
        ],
        cooldownYears: 30
      },
      {
        id: "rebellion_crisis",
        title: "逃亡主义叛乱",
        tip: "逃亡主义的蔓延正在撕裂人类社会的根基。",
        dialogQueue: [
          { speakerName: "褚岩", content: "我们有权离开！我们有权活下去！", avatarUrl: this.mapAvatar("default") },
          { speakerName: "联合政府发言人", content: "请保持冷静，逃亡即是背叛全人类。", avatarUrl: this.mapAvatar("default") }
        ],
        condition: { minYear: 60, maxTreachery: 30, epoch: "CRISIS" },
        choices: [
          { label: "严厉打击逃亡主义", effects: [{ type: "resource", target: "treachery", value: -15 }, { type: "resource", target: "military", value: 3 }] },
          { label: "疏导安抚民心", effects: [{ type: "resource", target: "treachery", value: -5 }, { type: "resource", target: "culture", value: 15 }] }
        ],
        cooldownYears: 15
      },
      {
        id: "sophon_countermeasure",
        title: "智子反制突破",
        tip: "550W量子计算机研制成功，智子科技封锁出现破口。",
        dialogQueue: [
          { speakerName: "罗辑", content: "智子的监视盲区被我们找到了。", avatarUrl: this.mapAvatar("luoji") },
          { speakerName: "面壁者", content: "从这一刻开始，真正的面壁计划正式开始。", avatarUrl: this.mapAvatar("default") }
        ],
        condition: { minYear: 30, reqTech: "550W量子计算机", reqNotFlag: "sophon_countermeasure_activated" },
        choices: [
          { label: "秘密启动面壁会议室", effects: [{ type: "flag", target: "sophon_countermeasure_activated", value: 1 }, { type: "flag", target: "sophon_broken", value: 1 }, { type: "resource", target: "prestige", value: 40 }] }
        ],
        cooldownYears: 5
      },
    ];
  }

  public getFilteredEventsForTurn(): FilteredEventPayload[] {
    const game = GameInstance.get();
    const result: FilteredEventPayload[] = [];

    for (const fev of this.filteredEvents) {
      if (this.triggeredFilteredIds.has(fev.id)) {
        if (fev.cooldownYears && fev.lastTriggeredYear) {
          if (game.year - fev.lastTriggeredYear < fev.cooldownYears) continue;
        } else continue;
      }

      if (!this.checkFilterConditions(fev.condition)) continue;

      result.push(fev);
    }
    return result;
  }

  private checkFilterConditions(cond: any): boolean {
    const game = GameInstance.get();
    const e = game.earthCivi;
    const epochNames = ["CRISIS", "DETERRENCE", "BROADCAST", "BUNKER", "GALAXY"];
    const currentEpoch = epochNames[game.epoch];

    if (cond.minYear !== undefined && game.year < cond.minYear) return false;
    if (cond.maxYear !== undefined && game.year > cond.maxYear) return false;
    if (cond.epoch && cond.epoch !== "ANY" && cond.epoch !== "WANDERING" && cond.epoch !== "SHELTER") {
      if (cond.epoch !== currentEpoch) return false;
    }
    if (cond.reqTech && !this.isTecFinishedInAnyTree(cond.reqTech)) return false;
    if (cond.reqFlag && !game.hasFlag(cond.reqFlag)) return false;
    if (cond.reqNotFlag && game.hasFlag(cond.reqNotFlag)) return false;
    if (cond.minEconomy !== undefined && e.economy < cond.minEconomy) return false;
    if (cond.minPopulation !== undefined && e.population < cond.minPopulation) return false;
    if (cond.minCulture !== undefined && e.culture < cond.minCulture) return false;
    if (cond.minDeterrence !== undefined && e.deterrenceValue < cond.minDeterrence) return false;
    if (cond.maxTreachery !== undefined && e.treachery > cond.maxTreachery) return false;

    if (cond.probability && Math.random() > cond.probability) return false;

    return true;
  }

  private isTecFinishedInAnyTree(name: string): boolean {
    const game = GameInstance.get();
    return game.earthCivi.tecTreeManager.isTecFinishedAnywhere(name);
  }

  public markFilteredEventTriggered(id: string, year: number): void {
    this.triggeredFilteredIds.add(id);
    const fev = this.filteredEvents.find(f => f.id === id);
    if (fev) fev.lastTriggeredYear = year;
  }

  private parseEventData(dataList: any): GameEvent[] {
    const events: GameEvent[] = [];
    if (!dataList || !Array.isArray(dataList)) return events;

    dataList.forEach((data: any) => {
      let dialogNodes: DialogNode[] = [];

      if (data.dialogQueue) {
        dialogNodes = data.dialogQueue.map((node: any) => ({
          speakerName: node.speakerName,
          content: node.content,
          avatarUrl: this.mapAvatar(node.avatarUrl)
        }));
      } else {
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

      let inYear = data.inYear ?? 0;
      if (inYear === 0 && typeof data.name === 'number') {
        inYear = data.name;
      }

      const e = createGameEvent(
        data.title || (typeof data.name === 'string' ? data.name : `纪元大事记_${inYear}`),
        data.eventtype ?? 0,
        inYear,
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
    const currentEpoch = game.epoch;
    const epochNames = ["CRISIS", "DETERRENCE", "BROADCAST", "BUNKER", "GALAXY"];
    let epochName = epochNames[currentEpoch];

    const pool = [...this.randomEvents].sort(() => Math.random() - 0.5);

    for (const e of pool) {
      const prob = e.triggerCondition?.probability ?? 0.4;
      if (Math.random() > prob) continue;

      if (e.triggerCondition?.epoch && e.triggerCondition.epoch !== "ANY") {
        const targetEpoch = e.triggerCondition.epoch;
        if (targetEpoch === "WANDERING" && (epochName === "CRISIS" || epochName === "DETERRENCE")) {
        } else if (targetEpoch === "SHELTER" && epochName === "BUNKER") {
        } else if (targetEpoch !== epochName) {
          continue;
        }
      }

      if (e.triggerCondition?.reqTech) {
        if (!game.earthCivi.tecTreeManager.isTecFinishedAnywhere(e.triggerCondition.reqTech)) {
          continue;
        }
      }

      return e;
    }

    return null;
  }
}


