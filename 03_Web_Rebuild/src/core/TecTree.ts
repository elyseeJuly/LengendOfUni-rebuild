import { TecTreeType } from "../types/enums";

export interface TecTreeNode {
  name: string;
  finished: boolean;
  inResearch: boolean;
  totalWorkload: number;
  currentWorkload: number;
  cost: number;
  tip: string;
  parentName: string | null;
  childrenNames: string[];
}

export function createTecTreeNode(
  name: string,
  totalWorkload: number,
  cost: number,
  tip: string,
  parentName: string | null = null
): TecTreeNode {
  return {
    name,
    finished: false,
    inResearch: false,
    totalWorkload,
    currentWorkload: 0,
    cost,
    tip,
    parentName,
    childrenNames: [],
  };
}

export class TecTree {
  public type: TecTreeType;
  public nodes: Map<string, TecTreeNode> = new Map();
  public rootNodeName: string | null = null;
  public researchNodeName: string | null = null;

  constructor(type: TecTreeType) {
    this.type = type;
  }

  public addNode(
    parentName: string | null,
    name: string,
    finished: boolean,
    totalWorkload: number,
    cost: number,
    tip: string
  ): void {
    const node = createTecTreeNode(name, totalWorkload, cost, tip, parentName);
    node.finished = finished;
    
    this.nodes.set(name, node);

    if (parentName === null || parentName === "") {
      this.rootNodeName = name;
    } else {
      const parent = this.nodes.get(parentName);
      if (parent) {
        parent.childrenNames.push(name);
      }
    }
  }

  public getNode(name: string): TecTreeNode | undefined {
    return this.nodes.get(name);
  }

  public isFinished(name: string): boolean {
    const node = this.getNode(name);
    return node ? node.finished : false;
  }
}
