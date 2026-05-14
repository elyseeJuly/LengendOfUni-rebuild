import { EventEffect } from "./enums";

export interface DialogNode {
  speakerName: string; // 讲话者姓名，如 "史强", "系统警告"
  avatarUrl?: string;  // 角色头像路径，如 "/images/shiqiang.bmp"
  content: string;     // 对话内容
}

export interface GameEventPayload {
  id: string;
  title: string;       // 事件大标题
  dialogQueue: DialogNode[]; // 多段对话序列
  choices?: { 
    label: string; 
    action: () => void;
    effect?: EventEffect;
    value?: number;
  }[]; // 玩家决策
}
