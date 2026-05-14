import { Civilization } from "./Civilization";
import { Department, createDepartment } from "./Department";
import { DepartmentType, TecTreeType } from "../types/enums";
import { GameInstance } from "./Game";
import { CombatEngine } from "./CombatEngine";
import { createBarback } from "./Barback";

export class EarthCivilization extends Civilization {
  public idlePopulation: number = 0;
  public levelString: string = "普通文明";
  public deterrenceValue: number = 0;

  
  public wallfacers: Set<string> = new Set();
  public swordholder: string | null = null;
  
  public departments: Map<DepartmentType, Department> = new Map();

  constructor() {
    super("地球");
    
    // 初始化 11 个部门
    const deptNames = [
      "经济部", "军事部", "文化部", "人力资源部", "宇宙社会学", 
      "核技术", "航天技术", "质子技术", "天体物理", "文化研究所", "经济研究所"
    ];
    for (let i = 0; i < 11; i++) {
      const d = createDepartment(i as DepartmentType, deptNames[i]);
      this.departments.set(i as DepartmentType, d);
    }

    // BUG-06 Fix: Initialize Earth
    this.starIndices.add(4); // Earth index=4
    this.population = 100;
  }

  public addWallfacer(name: string): void {
    this.wallfacers.add(name);
  }

  public removeWallfacer(name: string): void {
    this.wallfacers.delete(name);
  }

  public isWallfacer(name: string): boolean {
    return this.wallfacers.has(name);
  }

  public runARound(): void {
    const game = GameInstance.get();

    // 计算部门加成 (以经济部为例)
    let economyBonus = 0;
    const ecoDept = this.departments.get(DepartmentType.ECONOMY);
    if (ecoDept && ecoDept.leaderName) {
      const leader = game.personManager.getPerson(ecoDept.leaderName);
      if (leader) economyBonus = leader.economy * 0.1; // 每 1 点经济属性增加 10% 产出
    }

    // 基础地球属性成长计算
    this.economy += 10 * (1 + economyBonus);
    this.culture += 5;
    
    // 人口增长（受拥有的星系数量和人口上限影响）
    const popGrowthRate = 0.02; // 2% per turn
    const totalPopLimit = Array.from(this.starIndices).reduce((sum, idx) => {
      const s = game.starManager.getStar(idx);
      return sum + (s ? s.populationLimit : 0);
    }, 0);
    if (this.population < totalPopLimit) {
      this.population += Math.max(1, Math.floor(this.population * popGrowthRate));
      if (this.population > totalPopLimit) this.population = totalPopLimit;
    }

    // BUG-B2 Fix: 同步星球人口显示
    for (const idx of this.starIndices) {
      const star = game.starManager.getStar(idx);
      if (star) star.currentPopulation = this.population;
    }

    // 面壁计划加成 (隐性增加威慑值)
    for (const wName of this.wallfacers) {
      const p = game.personManager.getPerson(wName);
      if (p) {
        // 领袖能力与艺术(欺骗)能力提升威慑建立速度
        this.deterrenceValue += (p.leadership + p.art) * 0.5;
      }
    }

    // 科技研发进度推进 (以物理部为例计算加成)
    let physicsBonus = 0;
    const physDept = this.departments.get(DepartmentType.ASTROPHYSICS);
    if (physDept && physDept.leaderName) {
      const leader = game.personManager.getPerson(physDept.leaderName);
      if (leader) physicsBonus = leader.science * 0.2; // 每 1 点科学属性增加 20% 效率
    }

    for (const tree of this.tecTreeManager.trees.values()) {
      let treeBonus = 0;
      if (tree.type === TecTreeType.PHYSICS) treeBonus = physicsBonus;
      // 其他系可以接其他部的 bonus

      for (const node of tree.nodes.values()) {
        if (node.inResearch && !node.finished) {
          const baseWork = 10;
          node.currentWorkload += baseWork * (1 + treeBonus); 
          if (node.currentWorkload >= node.totalWorkload) {
            node.currentWorkload = node.totalWorkload;
            node.finished = true;
            node.inResearch = false;
            game.addHistory(`科技研发完成: ${node.name}`);
          }
        }
      }
    }

    // 舰队飞行与战斗逻辑
    for (let i = this.fleets.length - 1; i >= 0; i--) {
      const fleet = this.fleets[i];
      if (fleet.eta > 0) {
        fleet.eta--;
        if (fleet.eta === 0) {
          game.addHistory(`舰队 [${fleet.name}] 已抵达目的地星系 ${fleet.targetStarIndex}！`);
          
          // 判定目标星系归属
          const targetStar = game.starManager.getStar(fleet.targetStarIndex);
          if (targetStar && targetStar.belongToCivi !== this.name) {
            // 在实际逻辑中，应该查询异星文明的 barback
            // 这里我们现场造一个临时的用于防守演示
            const tempDef = createBarback("temp_def", fleet.targetStarIndex);
            tempDef.soldierCount = 100; // 假想敌
            
            const win = CombatEngine.resolveFleetVsBarback(fleet, tempDef);
            if (win) {
              targetStar.belongToCivi = this.name;
              game.addHistory(`【胜利】成功占领星系 ${targetStar.name}！`);
            } else {
              // 舰队被消灭
              this.fleets.splice(i, 1);
            }
          }
        }
      }
    }
  }
}
