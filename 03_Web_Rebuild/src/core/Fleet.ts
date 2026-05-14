import { WeaponInstance } from "./Weapon";

export interface Fleet {
  id: string;
  name: string;
  belongToCivi: string;
  leaderName: string | null;
  weapons: WeaponInstance[];
  
  sourceStarIndex: number;
  targetStarIndex: number;
  
  totalEta: number; // 总航程时间 (回合)
  eta: number;      // 剩余航程时间 (回合)
}

export function createFleet(
  name: string,
  belongToCivi: string,
  source: number,
  target: number,
  eta: number
): Fleet {
  return {
    id: `fleet_${Date.now()}_${Math.floor(Math.random() * 1000)}`,
    name,
    belongToCivi,
    leaderName: null,
    weapons: [],
    sourceStarIndex: source,
    targetStarIndex: target,
    totalEta: eta,
    eta: eta
  };
}
