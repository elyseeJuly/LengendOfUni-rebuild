export interface Star {
  index: number;
  name: string;
  isPlanet: boolean;
  migrationLevel: number;
  totalResource: number;
  currentResource: number;
  exist: boolean;
  belongToCivi: string;
  populationLimit: number;
  currentPopulation: number;
  found: boolean;

  // 扁平化数据：不再直接持有对象指针，而是记录其建筑状态
  // 为了后续序列化，只要这些字段存在，就说明有该建筑
  hasStope: boolean;
  hasFactory: boolean;
  hasCity: boolean;
  barbackId: string | null;  // 军营 ID，若为 null 则说明没有军营

  departmentName: string | null; // 代替原 CStar 的 m_pDepartment
}

export function createEmptyStar(index: number): Star {
  return {
    index,
    name: `Star ${index}`,
    isPlanet: true,
    migrationLevel: 0,
    totalResource: 0,
    currentResource: 0,
    exist: true,
    belongToCivi: "",
    populationLimit: 0,
    currentPopulation: 0,
    found: false,
    hasStope: false,
    hasFactory: false,
    hasCity: false,
    barbackId: null,
    departmentName: null,
  };
}
