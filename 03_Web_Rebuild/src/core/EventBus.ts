import { EventLane, EpochType } from "../types/enums";

export type EventHandler = (...args: any[]) => void;

export class EventBus {
  private handlers: Map<string, Set<EventHandler>> = new Map();

  /** 监听事件 */
  on(event: string, handler: EventHandler): void {
    if (!this.handlers.has(event)) this.handlers.set(event, new Set());
    this.handlers.get(event)!.add(handler);
  }

  /** 取消监听 */
  off(event: string, handler: EventHandler): void {
    this.handlers.get(event)?.delete(handler);
  }

  /** 触发事件 */
  emit(event: string, ...args: any[]): void {
    this.handlers.get(event)?.forEach(handler => {
      try { handler(...args); } catch (e) { console.error(`EventBus handler error for ${event}:`, e); }
    });
  }

  /** 触发并派发到 Window（用于 React 组件监听） */
  emitToWindow(event: string, detail?: any): void {
    window.dispatchEvent(new CustomEvent(event, { detail }));
    this.emit(event, detail);
  }

  /** 清空所有监听 */
  clear(): void {
    this.handlers.clear();
  }

  /** 获取指定事件的监听器数量 */
  listenerCount(event: string): number {
    return this.handlers.get(event)?.size || 0;
  }

  toJSON(): object {
    return { listenerCounts: Array.from(this.handlers.entries()).map(([k, v]) => [k, v.size]) };
  }
}

// ===== 标准化事件名称 =====
export const GameEvents = {
  TURN_START: 'game:turn:start',
  TURN_COMPLETE: 'game:turn:complete',
  EPOCH_CHANGED: 'game:epoch:changed',
  EVENT_TRIGGERED: 'game:event:triggered',
  TAG_APPLIED: 'game:tag:applied',
  TAG_REMOVED: 'game:tag:removed',
  ATMOSPHERE_CHANGED: 'game:atmosphere:changed',
  BATTLE_START: 'game:battle:start',
  BATTLE_END: 'game:battle:end',
  PERSON_UNLOCKED: 'game:person:unlocked',
  SAVE_COMPLETED: 'game:save:completed',
  LOAD_COMPLETED: 'game:load:completed',
  GAME_OVER: 'game:over',
  AUDIO_PLAY: 'audio:play',
  AUDIO_STOP: 'audio:stop',
  RESOURCE_CHANGED: 'game:resource:changed',
  FLEET_MOVED: 'game:fleet:moved',
  TECH_COMPLETED: 'game:tech:completed',
} as const;