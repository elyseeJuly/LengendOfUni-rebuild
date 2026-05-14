import { WeaponPrototype } from "./Weapon";
import weaponsData from "../data/weapons.json";

export class WeaponManager {
  public prototypes: Map<string, WeaponPrototype> = new Map();

  constructor() {
    this.init();
  }

  public init(): void {
    weaponsData.forEach((data: any) => {
      const proto: WeaponPrototype = {
        name: data.name || data.Name,
        type: data.type || 0,
        dependTecType: data.dependTecType || 0,
        dependTecName: data.dependTecName || "",
        attack: data.attack || 0,
        hp: data.hp || 0,
        totalBuild: data.totalBuild || 100,
        buildPerRound: data.buildPerRound || 10,
        cost: data.cost || 0,
        priority: data.priority || 0,
        needCiviLevel: data.needCiviLevel || 0
      };
      this.prototypes.set(proto.name, proto);
    });
  }

  public getPrototype(name: string): WeaponPrototype | undefined {
    return this.prototypes.get(name);
  }
}
