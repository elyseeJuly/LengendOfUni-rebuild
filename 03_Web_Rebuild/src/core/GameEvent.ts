import { EventEffect, EventType, EpochType } from "../types/enums";
import { DialogNode } from "../types/narrative";

export interface GameEventChoice {
  label: string;
  effects?: any[]; // 对应 JSON 中的 effects
  action?: () => void;
}

export interface TriggerCondition {
  epoch?: string | EpochType;
  probability?: number;
  reqTech?: string | null;
}

export interface GameEvent {
  id?: string;
  name: string;
  type: EventType;
  inYear: number;
  tip: string;
  effect: EventEffect;
  hasTriggered: boolean;
  dialogNodes: DialogNode[];
  triggerCondition?: TriggerCondition;
  choices?: GameEventChoice[];
}

export function createGameEvent(
  name: string,
  type: EventType,
  inYear: number,
  tip: string,
  effect: EventEffect,
  dialogNodes: DialogNode[] = [],
  id?: string,
  triggerCondition?: TriggerCondition,
  choices?: GameEventChoice[]
): GameEvent {
  return {
    id,
    name,
    type,
    inYear,
    tip,
    effect,
    hasTriggered: false,
    dialogNodes,
    triggerCondition,
    choices
  };
}


