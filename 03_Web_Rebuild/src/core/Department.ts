import { DepartmentType } from "../types/enums";

export interface Department {
  type: DepartmentType;
  name: string;
  leaderName: string | null; // 引用 Person 的 name
  workEfficiency: number;
}

export function createDepartment(type: DepartmentType, name: string): Department {
  return {
    type,
    name,
    leaderName: null,
    workEfficiency: 0,
  };
}

export function calculateDepartmentEfficiency(personScience: number, personLeadership: number): number {
  // 原 MFC 中根据不同部门有不同的计算公式，这里提供一个统一的基础实现
  // 可以在实际业务中覆盖
  return Math.floor((personScience + personLeadership) / 2);
}
