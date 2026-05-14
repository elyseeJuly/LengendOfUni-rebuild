import { TecTreeManager } from "./TecTreeManager";
import { Fleet } from "./Fleet";

export class Civilization {
  public name: string = "";
  public population: number = 0;
  public culture: number = 0;
  public economy: number = 0;
  public resource: number = 0;
  public army: number = 0;
  public treachery: number = 0;
  public civiLevel: number = 0;
  
  public starIndices: Set<number> = new Set();
  public barbackIds: Set<string> = new Set();
  public fleets: Fleet[] = [];
  
  public tecTreeManager: TecTreeManager;

  constructor(name: string) {
    this.name = name;
    this.tecTreeManager = new TecTreeManager();
  }

  public isDieOut(): boolean {
    return this.starIndices.size === 0;
  }
}
