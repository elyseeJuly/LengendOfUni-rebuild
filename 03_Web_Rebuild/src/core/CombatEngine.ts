import { Fleet } from "./Fleet";
import { Barback } from "./Barback";
import { GameInstance } from "./Game";

export class CombatEngine {
  
  // 返回 true 代表攻击方胜利，false 代表防守方胜利/击退
  public static resolveFleetVsBarback(atkFleet: Fleet, defBarback: Barback): boolean {
    const game = GameInstance.get();
    
    // 极简战力计算：
    // 1. 累加武器基础威力
    // 2. 将领/驻防长官属性加成
    let atkPower = this.calculateFleetPower(atkFleet);
    let defPower = this.calculateBarbackPower(defBarback);
    
    game.addHistory(`战斗爆发！星系[${defBarback.planetIndex}] 遭到【${atkFleet.belongToCivi}】舰队袭击！`);
    game.addHistory(`>> 攻击方战力评级: ${atkPower}`);
    game.addHistory(`>> 防守方战力评级: ${defPower}`);

    if (atkPower > defPower) {
      game.addHistory(`【战报】守军全军覆没，星系易主！`);
      return true;
    } else {
      game.addHistory(`【战报】进攻舰队被击溃，守军获得胜利！`);
      return false;
    }
  }

  private static calculateFleetPower(fleet: Fleet): number {
    let base = 0;
    fleet.weapons.forEach(w => {
      // 简单地把所有完成的数量当做战斗力
      base += w.currentBuild * 10; 
    });

    // 统帅加成
    if (fleet.leaderName) {
      const game = GameInstance.get();
      const leader = game.personManager.getPerson(fleet.leaderName);
      if (leader) {
        base *= (1 + leader.army * 0.1);
      }
    }
    return Math.floor(base);
  }

  private static calculateBarbackPower(barback: Barback): number {
    let base = barback.soldierCount * 1;
    barback.weapons.forEach(w => {
      base += w.currentBuild * 10;
    });

    if (barback.departmentLeaderName) {
      const game = GameInstance.get();
      const leader = game.personManager.getPerson(barback.departmentLeaderName);
      if (leader) {
        base *= (1 + leader.army * 0.1);
      }
    }
    
    // 如果没有防守方（空星），给个象征性抵抗 0
    return Math.floor(base);
  }
}
