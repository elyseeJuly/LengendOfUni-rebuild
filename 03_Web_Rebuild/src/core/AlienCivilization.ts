import { Civilization } from "./Civilization";
import { AiPersonality, FriendshipType } from "../types/enums";
import { GameInstance } from "./Game";
import { createFleet } from "./Fleet";
import { CombatEngine } from "./CombatEngine";
import { createBarback } from "./Barback";
import aliensData from "../data/aliens.json";

export class AlienCivilization extends Civilization {
  public typeIndex: number;
  public personality: AiPersonality;
  public friendshipType: FriendshipType;
  
  constructor(name: string, typeIndex: number) {
    super(name);
    this.typeIndex = typeIndex;
    this.personality = AiPersonality.HUNTER;
    this.friendshipType = FriendshipType.NORMAL;
  }

  public runARound(): void {
    const game = GameInstance.get();
    
    // 面壁威慑评估
    let deterrenceRate = 0;
    const swordholderName = game.earthCivi.swordholder;
    if (swordholderName) {
      const sh = game.personManager.getPerson(swordholderName);
      if (sh) {
        // 简单计算执剑人威慑力 (采用领导力作为威慑力)
        deterrenceRate = sh.leadership;
      }
    }

    if (deterrenceRate > 80) {
      // 威慑纪元：异星不敢轻举妄动
      if (Math.random() < 0.05) {
        game.addHistory(`【情报】受到执剑人 ${swordholderName} 的威慑，${this.name} 舰队停止集结。`);
      }
    } else {
      // 威慑度不足或无执剑人，必定进攻
      if (Math.random() < 0.08) {
        game.addHistory(`【警告】探测到 ${this.name} 正在向太阳系派遣远征舰队！`);
        const fleet = createFleet(`${this.name} 远征军`, this.name, -1, 0, 5);
        fleet.weapons.push({ weaponName: "水滴", currentBuild: 100 });
        this.fleets.push(fleet);
      }
    }

    // 异星舰队飞行与结算
    for (let i = this.fleets.length - 1; i >= 0; i--) {
      const fleet = this.fleets[i];
      if (fleet.eta > 0) {
        fleet.eta--;
        if (fleet.eta === 0) {
          game.addHistory(`【警报】${fleet.name} 抵达太阳系！人类文明面临毁灭打击！`);
          
          const earthTarget = game.starManager.getStar(fleet.targetStarIndex);
          if (earthTarget && earthTarget.belongToCivi === "地球") {
            // 简单防守
            let defBarback = createBarback("earth_def", 0);
            defBarback.soldierCount = 500;
            const win = CombatEngine.resolveFleetVsBarback(fleet, defBarback);
            if (win) {
              earthTarget.belongToCivi = this.name;
              game.addHistory(`【战败】地球被 ${this.name} 占领，人类文明面临灭绝！`);
              // 由 Game.checkGameOverConditions 统一处理状态
            } else {
              game.addHistory(`【奇迹】地球守军成功击退了 ${fleet.name}！`);
              this.fleets.splice(i, 1);
            }
          }
        }
      }
    }
  }
}

export class AlienCiviManager {
  public aliens: Map<string, AlienCivilization> = new Map();

  constructor() {
    this.init();
  }

  public init(): void {
    aliensData.forEach((data: any) => {
      const alien = new AlienCivilization(data.name || data.Name, data.personality || 0); 
      // BUG-A3 Fix: 给每个异星一个虚拟星系索引，防止其初始即灭亡
      alien.starIndices.add(1000 + this.aliens.size);
      this.aliens.set(alien.name, alien);
    });
  }

  public isAllCiviConquered(): boolean {
    for (const alien of this.aliens.values()) {
      if (!alien.isDieOut()) {
        return false;
      }
    }
    return true;
  }

  public runARound(): void {
    for (const alien of this.aliens.values()) {
      alien.runARound();
    }
  }
}
