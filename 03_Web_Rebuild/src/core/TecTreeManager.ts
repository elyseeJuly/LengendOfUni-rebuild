import { TecTree } from "./TecTree";
import { TecTreeType } from "../types/enums";

export class TecTreeManager {
  public trees: Map<TecTreeType, TecTree> = new Map();

  constructor() {
    this.init();
  }

  public init(): void {
    // 翻译原来的 CTecTreeManager::Init
    const physics = new TecTree(TecTreeType.PHYSICS);
    physics.addNode("", "粒子对撞实验", false, 80, 20, "基础物理学。");
    physics.addNode("粒子对撞实验", "质子3维展开", false, 150, 50, "可智子盲区屏蔽。");
    physics.addNode("质子3维展开", "智子工程", false, 300, 100, "解锁智子建造。");
    physics.addNode("粒子对撞实验", "50光年远镜", false, 100, 20, "观测范围50光年。");
    physics.addNode("50光年远镜", "1万光年远镜", false, 200, 40, "观测范围1万光年。");
    physics.addNode("1万光年远镜", "银河系远镜", false, 400, 80, "观测范围银河系。");
    // ... 实际移植需要把所有节点翻译，这里给出结构示范
    this.trees.set(TecTreeType.PHYSICS, physics);

    const aerospace = new TecTree(TecTreeType.AEROSPACE);
    aerospace.addNode("", "行星发动机Ⅰ型", false, 100, 20, "增加工厂产出。");
    aerospace.addNode("行星发动机Ⅰ型", "行星发动机Ⅱ型", false, 200, 40, "允许流浪。");
    aerospace.addNode("行星发动机Ⅱ型", "行星发动机Ⅲ型", false, 400, 80, "重核聚变，满足流浪条件。");
    this.trees.set(TecTreeType.AEROSPACE, aerospace);

    const military = new TecTree(TecTreeType.MILITARY);
    military.addNode("", "引力波探测", false, 120, 30, "及早发现异星入侵舰队。");
    military.addNode("引力波探测", "曲率驱动研究", false, 400, 100, "解锁高速恒星级战舰。");
    this.trees.set(TecTreeType.MILITARY, military);

    const info = new TecTree(TecTreeType.INFORMATION);
    info.addNode("", "意识上传", false, 100, 30, "保留人口火种。");
    info.addNode("意识上传", "550W量子计算机", false, 200, 50, "极强算力，可对抗智子封锁。");
    info.addNode("550W量子计算机", "数字方舟", false, 300, 80, "满足数字生命胜利。");
    this.trees.set(TecTreeType.INFORMATION, info);

    const interstellar = new TecTree(TecTreeType.INTERSTELLAR);
    interstellar.addNode("", "新家园选址", false, 200, 50, "流浪地球的前置。");
    this.trees.set(TecTreeType.INTERSTELLAR, interstellar);
  }

  public isTecFinished(type: TecTreeType, name: string): boolean {
    const tree = this.trees.get(type);
    if (!tree) return false;
    return tree.isFinished(name);
  }
}
