import { TecTreeType, WeaponType } from "../types/enums";

export interface WeaponPrototype {
  name: string;
  type: WeaponType;
  dependTecType: TecTreeType;
  dependTecName: string;
  attack: number;
  hp: number;
  totalBuild: number;
  buildPerRound: number;
  cost: number;
  priority: number;
  needCiviLevel: number;
}

export interface WeaponInstance {
  weaponName: string;   // 引用 Prototype 的名称
  currentBuild: number;
}

export function isWeaponFinished(weapon: WeaponInstance, prototype: WeaponPrototype): boolean {
  return weapon.currentBuild >= prototype.totalBuild;
}

export function runWeaponRound(weapon: WeaponInstance, prototype: WeaponPrototype): void {
  if (!isWeaponFinished(weapon, prototype)) {
    weapon.currentBuild += prototype.buildPerRound;
    if (weapon.currentBuild > prototype.totalBuild) {
      weapon.currentBuild = prototype.totalBuild;
    }
  }
}
