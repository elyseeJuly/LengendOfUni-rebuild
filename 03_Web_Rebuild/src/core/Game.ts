import { EpochType, EventEffect, FriendshipType, TecTreeType } from "../types/enums";
import { StarManager } from "./StarManager";
import { PersonManager } from "./PersonManager";
import { WeaponManager } from "./WeaponManager";
import { GameEventManager } from "./GameEventManager";
import { EarthCivilization } from "./EarthCivilization";
import { AlienCiviManager, AlienCivilization } from "./AlienCivilization";
import { TecTreeManager } from "./TecTreeManager";
import { TecTree } from "./TecTree";
import { GameEventPayload } from "../types/narrative";
import { createFleet } from "./Fleet";

export class Game {
  public year: number = 0;
  public epoch: EpochType = EpochType.CRISIS;
  public historyLogs: string[] = [];
  
  public starManager: StarManager;
  public personManager: PersonManager;
  public weaponManager: WeaponManager;
  public eventManager: GameEventManager;
  
  public earthCivi: EarthCivilization;
  public alienCiviManager: AlienCiviManager;

  // 叙事系统状态
  public currentEvent: GameEventPayload | null = null;
  public eventQueue: GameEventPayload[] = [];
  public isGameOver: boolean = false;
  public gameOverReason: string = "";
  public isProcessing: boolean = false;

  constructor() {
    this.starManager = new StarManager();
    this.personManager = new PersonManager();
    this.weaponManager = new WeaponManager();
    this.eventManager = new GameEventManager();
    
    this.earthCivi = new EarthCivilization();
    this.alienCiviManager = new AlienCiviManager();
  }

  public getYear(): number {
    return this.year;
  }

  public getEpoch(): EpochType {
    return this.epoch;
  }

  public addHistory(log: string): void {
    const epochNames = ["危机纪元", "威慑纪元", "广播纪元", "掩体纪元", "银河纪元"];
    const prefix = `${epochNames[this.epoch]} ${this.year} 年 - `;
    this.historyLogs.push(prefix + log);
    console.log("[History]", prefix + log);
  }

  public isSophonBlocked(): boolean {
    if (this.year < 10) return false;
    const sanTi = this.alienCiviManager.aliens.get("三体");
    if (sanTi && !sanTi.isDieOut() && sanTi.friendshipType < FriendshipType.FRIEND) {
      const tecMgr = this.earthCivi.tecTreeManager;
      if (tecMgr.isTecFinished(TecTreeType.INFORMATION, "550W量子计算机") || 
          tecMgr.isTecFinished(TecTreeType.PHYSICS, "智子工程")) {
        return false;
      }
      return true;
    }
    return false;
  }

  public runARound(): void {
    if (this.isGameOver) return;
    
    // 如果当前有事件未处理，禁止推进回合
    if (this.currentEvent || this.eventQueue.length > 0) {
      this.addHistory("提示：请先处理当前的剧情事件。");
      return;
    }

    if (this.isProcessing) {
      console.warn("Turn blocked by processing lock");
      return;
    }

    this.isProcessing = true;

    try {
      this.earthCivi.runARound();
      this.alienCiviManager.runARound();
      
      const triggeredEvents = this.eventManager.checkEvents(this.year);
      
      // 同时也检查随机事件
      const randomEvent = this.eventManager.checkRandomEvents();
      if (randomEvent) {
        triggeredEvents.push(randomEvent);
      }

      triggeredEvents.forEach(e => {
        this.addHistory(`触发事件: ${e.name}`);
        
        const payload: GameEventPayload = {
          id: e.id || `event_${this.year}_${e.name}`,
          title: e.name,
          dialogQueue: e.dialogNodes.length > 0 ? e.dialogNodes : [{
            speakerName: "系统",
            content: e.tip
          }],
          choices: e.choices && e.choices.length > 0 ? e.choices.map(c => ({
            label: c.label,
            action: () => {
              if (c.effects) this.applyNewEffects(c.effects);
              this.applyEventEffect(e.effect);
            }
          })) : [
            {
              label: "确认",
              action: () => this.applyEventEffect(e.effect)
            }
          ]
        };

        this.eventQueue.push(payload);
      });

      this.year++;
      
      // 纪元转换检查
      if (this.epoch === EpochType.CRISIS && this.earthCivi.deterrenceValue >= 100) {
        this.epoch = EpochType.DETERRENCE;
        this.addHistory("【纪元更替】进入威慑纪元！");
      }

      // 终局检查
      this.checkGameOverConditions();

      // 触发第一个事件
      this.processNextEvent();
      this.addHistory(`回合推进完成：${this.year - 1} -> ${this.year}`);
    } catch (err) {
      console.error("Critical error in runARound:", err);
      this.addHistory("【错误】回合结算过程中发生异常，系统已尝试紧急恢复。");
    } finally {
      this.isProcessing = false;
    }
  }

  public checkGameOverConditions(): void {
    if (this.earthCivi.population <= 0) {
      this.isGameOver = true;
      this.gameOverReason = "文明灭绝: 地球已成为一颗死寂的星球。";
    } else if (this.year > 400 && this.epoch < EpochType.GALAXY) {
      this.isGameOver = true;
      this.gameOverReason = "太阳氦闪: 漫长的等待终结于刺眼的白光，地球未能逃离。";
    }
    
    if (this.isGameOver) {
      window.dispatchEvent(new CustomEvent('game-over'));
    }
  }

  public processNextEvent(): void {
    if (this.eventQueue.length > 0 && !this.currentEvent) {
      this.currentEvent = this.eventQueue.shift() || null;
      window.dispatchEvent(new CustomEvent('game-event-triggered'));
    }
  }

  public applyEventEffect(effect: EventEffect): void {
    switch (effect) {
      case EventEffect.ADDECONEMY: this.earthCivi.economy += 50; break;
      case EventEffect.ADDCULTURE: this.earthCivi.culture += 30; break;
      case EventEffect.ADDPOP: this.earthCivi.population += 20; break;
      case EventEffect.MOON_CRISIS:
        const moon = this.starManager.getStar(0);
        if (moon) { moon.totalResource = 0; moon.currentResource = 0; }
        break;
      case EventEffect.WANDERING_EARTH:
        if (this.earthCivi.tecTreeManager.isTecFinished(TecTreeType.AEROSPACE, "行星发动机Ⅲ型")) {
          this.addHistory("流浪地球计划启动！");
        } else {
          this.addHistory("缺少行星发动机技术，无法启动流浪地球计划！");
        }
        break;
    }
    this.currentEvent = null;
    
    // 强制触发一次同步，确保 UI 知道事件已结束
    window.dispatchEvent(new CustomEvent('game-event-triggered'));
    
    this.processNextEvent();
    window.dispatchEvent(new CustomEvent('game-turn-complete'));
  }

  public applyNewEffects(effects: any[]): void {
    if (!effects) return;
    effects.forEach(eff => {
      if (eff.type === 'resource') {
        const val = Number(eff.value);
        switch (eff.target) {
          case 'military': 
            for (let i = 0; i < val; i++) {
              this.earthCivi.fleets.push(createFleet(`第${this.earthCivi.fleets.length + 1}舰队`, "地球", 4, 4, 0));
            }
            break;
          case 'economy': this.earthCivi.economy += val; break;
          case 'population': this.earthCivi.population += val; break;
          case 'culture': this.earthCivi.culture += val; break;
          case 'prestige': this.earthCivi.deterrenceValue += val; break;
        }
      } else if (eff.type === 'flag') {
        // Flag logic can be added here
        this.addHistory(`[因果标记] 已激活: ${eff.target}`);
      }
    });
  }
}

// 全局单例管理器
export class GameInstance {
  private static instance: Game | null = null;

  public static get(): Game {
    if (!this.instance) {
      this.instance = new Game();
    }
    return this.instance;
  }

  public static reset(): void {
    this.instance = new Game();
  }

  public static saveGame(): void {
    if (!this.instance) return;
    this.instance.addHistory("游戏已保存到本地存储。");
    const data = JSON.stringify(this.instance, this.replacer);
    localStorage.setItem("LegendOfUni_Save", data);
  }

  public static loadGame(): boolean {
    const dataStr = localStorage.getItem("LegendOfUni_Save");
    if (!dataStr) return false;

    try {
      const parsedData = JSON.parse(dataStr, this.reviver);
      this.instance = new Game();
      
      // Deep assign properties
      Object.assign(this.instance, parsedData);
      
      // Patch critical prototypes that have methods called on them
      Object.setPrototypeOf(this.instance.earthCivi, EarthCivilization.prototype);
      Object.setPrototypeOf(this.instance.alienCiviManager, AlienCiviManager.prototype);
      
      if (this.instance.alienCiviManager && this.instance.alienCiviManager.aliens) {
        for (const alien of this.instance.alienCiviManager.aliens.values()) {
           Object.setPrototypeOf(alien, AlienCivilization.prototype);
        }
      }

      // BUG-08 Fix: Restore nested prototypes
      Object.setPrototypeOf(this.instance.earthCivi.tecTreeManager, TecTreeManager.prototype);
      for (const tree of this.instance.earthCivi.tecTreeManager.trees.values()) {
        Object.setPrototypeOf(tree, TecTree.prototype);
      }
      Object.setPrototypeOf(this.instance.starManager, StarManager.prototype);
      Object.setPrototypeOf(this.instance.personManager, PersonManager.prototype);
      Object.setPrototypeOf(this.instance.eventManager, GameEventManager.prototype);
      // BUG-FIX: Re-initialize event pool because JSON parsed arrays were not saved
      this.instance.eventManager.init();

      this.instance.isProcessing = false;
      this.instance.addHistory("【系统】游戏读取成功。");
      
      window.dispatchEvent(new CustomEvent('game-loaded'));
      return true;
    } catch (e) {
      console.error("Failed to load game:", e);
      return false;
    }
  }

  private static replacer(_key: string, value: any) {
    if (value instanceof Map) {
      return { dataType: 'Map', value: Array.from(value.entries()) };
    } else if (value instanceof Set) {
      return { dataType: 'Set', value: Array.from(value) };
    }
    return value;
  }

  private static reviver(_key: string, value: any) {
    if (typeof value === 'object' && value !== null) {
      if (value.dataType === 'Map') {
        return new Map(value.value);
      }
      if (value.dataType === 'Set') {
        return new Set(value.value);
      }
    }
    return value;
  }
}
